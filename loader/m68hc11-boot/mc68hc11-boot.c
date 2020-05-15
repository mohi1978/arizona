/*
 *
 *    File:         mc68hc11-boot.c
 *    Rev:          $Id: mc68hc11-boot.c,v 1.7 2002/07/01 19:42:13 lvd Exp $
 *    Created:      Sun Jun 30 18:27:08 CEST 2002
 *    Author:       Luuk van Dijk
 *    mail:         lvd@mndmttr.nl
 *    Description:  upload up to 512 bytes from first
 *                  non-empty .text section in elf object
 *                  over 1200 baud serial link to a mc68hc11
 *                  cf. p.3-15, section 3.5.4 of the 
 *                  Motorola M68HC11 Reference Manual, 
 *                  "Special Bootstrap Mode"
 *
 *    Compile with: cc -lbfd mc68hc11-boot.c -o mc68hc11-boot
 *                  gcc -Wall mc68hc11-boot.c -o mc68hc11-boot -lbfd
 *
 *   Copyright (C) 2002 Luuk van Dijk/Mind over Matter
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

//#include "bfd.h"
#include <bfd.h>

#include <fcntl.h>
#include <termios.h>

/*
 * currently, defining DEBUG just hexdumps the object
 */

#undef DEBUG


/* 
 * usage() and crash(): ui stuff
 */

int verbose=0;
const char* argv0;
int report_bfd_errors=1;

void usage(void){
  fprintf( stderr, "usage: %s [-v][-s /dev/ttySx] binary.o\n", argv0);
  exit(1);
}


void crash(const char* msg){

  fprintf( stderr, "\n" );

  if(msg) fprintf( stderr, "%s: %s\n", argv0, msg);

  if( report_bfd_errors && bfd_get_error() ){ 
    // fprintf( stderr, "%s: BFD error:\n", argv0);
    bfd_perror(argv0);
  }
  else
    if(errno) 
      perror(argv0);

  fprintf( stderr, "%s: Quitting.\n", argv0);

  exit(3);

}




/*
 *  get_data(): getting the data from bfd
 */ 


void get_data(const char* path_to_obj, bfd_byte** data, bfd_size_type* datasize ){

  bfd* abfd;
  asection* section;
  
  *data = NULL;
  *datasize = 0;

  bfd_init(); 
    
  abfd = bfd_openr( path_to_obj, NULL ); 
  
  if( !abfd ) 
    crash( "error opening object file." );

  if( !bfd_check_format_matches(abfd, bfd_object, NULL) ) 
    crash( "error checking object type." );
  
  if(verbose) 
    fprintf(stderr, "    file: %s\n  format: %s\n", bfd_get_filename(abfd), abfd->xvec->name);
  
  /* look for first non empty section named .text */

  for (section = abfd->sections; section != NULL; section = section->next){
    if(  ( strcmp( ".text", section->name )== 0 )
	 && ( section->flags & SEC_HAS_CONTENTS ) 
	 && (bfd_section_size(abfd, section) > 0 ) ){
      break;
    }
  }

  if( section == NULL )
    crash( "error locating a non-empty .text section." );
    

  if(verbose) 
    fprintf(stderr, " lma/vma: 0x%lx/0x%lx\n", section->lma, section->vma );

  *datasize = bfd_section_size(abfd, section);

  if(verbose) 
    fprintf(stderr, "datasize: %ld bytes\n", *datasize);
  
  *data = (bfd_byte*) malloc( (size_t) *datasize );

  if( ! *data )
    crash( "error allocating buffer." );

  if( !bfd_get_section_contents(abfd, section, (PTR) *data, 0, *datasize) )
    crash( "error obtaining section contents." );

  bfd_close(abfd);

  

  return;

} /* get_data */


/*
 * setup_serial(): open & setup the serial port
 */ 


int setup_serial(const char* path_to_dev){
  
  int serial_fd;
  struct termios serial_tiosettings;

  if(verbose) 
    fprintf(stderr, "%s: initializing serial port %s...", argv0, path_to_dev);
  
  if( ( serial_fd = open( path_to_dev, O_RDWR   ) ) == -1 ) 
    crash( "error opening serial port." );
  
  if( tcgetattr(serial_fd, &serial_tiosettings)  == -1 ) 
    crash( "error getting current serial attributes." );

  cfmakeraw( &serial_tiosettings );    /* a.o. this clears BRKINT, meaning that BRK will 
					  be received in-band as 0x00 */

  serial_tiosettings.c_cc[VMIN]  = 128;  /* return from  read(2) after reading at least n bytes from the fifo */
  serial_tiosettings.c_cc[VTIME] = 1;    /* timeout */

  cfsetispeed( &serial_tiosettings, B0 );         /* ispeed == 0 means ispeed equal to ospeed */
  cfsetospeed( &serial_tiosettings, B1200 );      /* 1200 would always work, according to the manual*/
  
  if( tcsetattr(serial_fd, TCSANOW, &serial_tiosettings)  == -1 )
    crash( "error setting serial attributes." );

  if(verbose)
    fprintf( stderr, "ok.\n" );

  return serial_fd;

} /* setup_serial */


/*
 * sigalarm handler
 */

void handle_sigalrm(int arg){ 
  errno=0; 
  crash("timeout receiving data."); 
}


/*
 * sending the data over the serial port
 */

void send_data(int serial_fd, bfd_byte* data, bfd_size_type datasize ){

  int rslt;

  unsigned char buf;
  int data_i=0;
  int bytes_remaining=datasize;

  unsigned char* verify_buf;   /* buffer to receive the verification */
  int verify_i=0;
  int verify_bytes_remaining=0;
  int i=0;
    
  verify_buf = (unsigned char*) malloc( (size_t) datasize );  
  if(!verify_buf) crash("error allocating verification buffer.");

  /*
   * the following is cf. p.3-15, section 3.5.4 of the 
   * Motorola M68HC11 Reference Manual, "Special Bootstrap Mode"
   *
   * we wait for the 'brk' that will appear as 0x00 on input, then
   * send the 0xFF at 1200 baud, followed by the datasection of the ELF file
   */

  
  fprintf(stderr, "Waiting for handshake (reset the mc68hc1x now)...");

  rslt = read( serial_fd, &buf, 1 );

  if( rslt < 0 ) 
    crash( "error reading handshake." );
 


  if( buf != 0x00 ){ 
    fprintf(stderr, "%s: expected 0x00 but got: 0x%02hhX.", argv0, buf);
    crash(NULL);
  }
  
  fprintf(stderr, "got it.\nsending data");
  
  buf = 0xff;
    
  if( write( serial_fd, &buf, 1 ) != 1 ) 
    crash( "error replying to handshake." );
  
  fprintf(stderr, ".");
  
  tcflush(serial_fd, TCIFLUSH); 

  while( bytes_remaining || verify_bytes_remaining ){

    int rslt;  
    int verify_rslt;
    int timeout;

    rslt = write(serial_fd, data+data_i, bytes_remaining );
    
    if( rslt < 0 ) crash( "error sending data." );
    
    fprintf(stderr, ".");

    data_i          += rslt;

    if( rslt && (bytes_remaining == rslt) )  /* only true after last batch */
      fprintf( stderr, "done.\nverifying" );

    bytes_remaining -= rslt;


    verify_bytes_remaining += rslt;

    /* now try to read as many bytes back again */

    timeout = 2* verify_bytes_remaining * 10 / 1200; 
    alarm( timeout ? timeout : 1 ); /* approx 2x the time needed to receive this */

    verify_rslt = read( serial_fd, verify_buf+verify_i, verify_bytes_remaining ); 
    
    if( verify_rslt < 0 ) crash( "error receiving data." );

    alarm(0);

    verify_i += verify_rslt;
    verify_bytes_remaining -= verify_rslt; 

    for( /*nix*/ ; i < verify_i; ++i )
      if( data[i] != verify_buf[i] ){
	fprintf(stderr, "%s: verification failed at %3i: 0x%02hhX != 0x%02hhX\n", 
		argv0, i, data[i], verify_buf[i]);
	crash( NULL );
      }

  }

  fprintf(stderr, "done (%ld bytes).\n", datasize);

  free(verify_buf);

} /* send_data */


/*
 * main
 */


int main(int argc, char* argv[]){
  
  int flag;

  const char* path_to_serial = strdup("/dev/ttyS0");
  const char* path_to_obj = NULL;

  bfd_size_type datasize;
  bfd_byte*     data;

  int serial_fd;


  /*  parse parameters  */

  argv0 = argv[0];

  while( (flag=getopt(argc, argv,  "s:hv")) != EOF ){

    switch(flag){
    case 's': path_to_serial = strdup(optarg); break;
    case 'v': verbose++; break;
    case 'h': 
    default:
      usage();
    }

  }

  if( optind != argc-1 ) usage();

  path_to_obj = strdup( argv[optind] );


  /* read section from object file */

  get_data(path_to_obj, &data, &datasize);

  report_bfd_errors=0;

#ifdef DEBUG
  {
    int i;
    for( i=0; i<datasize; ++i ) 
      printf( "%02hhX %s", data[i], ( ((i+1)%16)?"":"\n") );  
    printf("\n");
  }
#endif

  /* open serial port */

  serial_fd = setup_serial(path_to_serial);


  if( signal(SIGALRM, handle_sigalrm) == SIG_ERR ) crash("error setting signal handler");

  /* send the data */

  send_data( serial_fd, data, datasize );

  /* cleanup */

  close(serial_fd);

  free((char*)path_to_obj);
  free((char*)path_to_serial);
  free(data);

  return 0;

} /* main */

