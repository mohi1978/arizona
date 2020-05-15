/* Le logiciel de programmation du 68HC11 hc11 linux.c */

#include "rs232.h"

#define BUFFER_SIZE 256
	
void
vrfy_buf (unsigned char *buf, int i)	/* verify buffer’s content */
{
  unsigned int j, k;
  for (j = 0; j < i; j++)
    {
      k = (unsigned int) buf[j];
      if (k < 0)
				k += 256;
      /*if(!(j%0x10) && j != 0)
      	printf ("%02X \n", k);
      else
      	printf ("%02X ", k);*/
      if(((j+1)%0x10))
      	printf ("%02X ", k);
      else
      	printf ("%02X \n", k);
      	
    }
  printf ("\n");
}

void
send_hc11_bootloader (FILE * f, int fd)
{
  unsigned char buf[BUFFER_SIZE];
	int i = 0, status, ret, k;
  int j;
  
  /* start with ’FF’ */
  buf[0] = 255; 
  ret=write(fd, buf, 1);
  if(ret < 0)		
  	debug_print("Write error");
	else
		debug_print("start with 0x%02x\n",buf[0]);
	
	/* Read in 256 8-bit numbers into the buffer */
	size_t bytes_read = 0;
	bytes_read = fread(buf, sizeof(unsigned char), BUFFER_SIZE, f);
	// Note: sizeof(unsigned char) is for emphasis	
	debug_print("Size of FILE : %d\n",bytes_read);
  for(k=0;k<BUFFER_SIZE;k++)
      	debug_print("buf[%d]=0x%02x\n",k, buf[k]);
  
  if (i < BUFFER_SIZE)
				i--;
	vrfy_buf (buf, bytes_read);

	/* Send Buffer to device */
	ret=write (fd, buf, bytes_read ); 
  if(ret < 0)           
  	printf("Write error");
  else
    printf("%d bytes sent\n", ret/*bytes_read*/);
}

void
send_hc11 (FILE * f, int fd)
{
  //unsigned char buf[255];
  unsigned char buf[BUFFER_SIZE];
	int i = 0, status, ret, k;
  int j;
  
  buf[0] = 255; /* start with ’FF’ */
  ret=write(fd, buf, 1);
  if(ret < 0)		/* start with ’FF’ */
  	debug_print("Write error");
	else
		debug_print("start with 0x%02x\n",buf[0]);
	
	/* Read in 256 8-bit numbers into the buffer */
	size_t bytes_read = 0;
	bytes_read = fread(buf, sizeof(unsigned char), BUFFER_SIZE, f);
	// Note: sizeof(unsigned char) is for emphasis	
	debug_print("Size of FILE : %d\n",bytes_read);
  for(k=0;k<BUFFER_SIZE;k++)
      	debug_print("buf[%d]=0x%02x\n",k, buf[k]);
  	
  // Fill buffer
  do
  {
      // read while !EOF && less than 255 chars
      /**/
      do
      {
	status = fscanf (f, "%02x", &j);
	buf[i] = (unsigned char) j;
	i++;
	}			
      while ((status != EOF) && (i < 255));	
	/**/
      
      
      debug_print("Size of FILE : %d\n",i);
      for(k=0;k<BUFFER_SIZE;k++)
      	debug_print("buf[%d]=0x%02x\n",k, buf[k]);
      
      if (i < 255)
				i--;
      vrfy_buf (buf, i);
  
      ret=write (fd, buf, i); 
      if(ret < 0)           
         printf("Write error");
      else
        printf("%d bytes sent\n", i);
      i = 0;
  }
  while (status != EOF);

}

int
main (int argc, char **argv)
{
	int fd;
	char c;
  FILE *f;
  printf ("Firmware loader for 68HC11 - hc11_linux - [2020-05-09]\n");
  printf ("Author : Ch. De Figueiredo\n");
  printf ("Date:	2020-05-09\n");
  printf ("Inspired by J-M Friedt\n");
  printf ("Usage : ./hc11_linux Talkeram.bin Firmware.s19\n\n");
  
  if (argc < 2)
    printf ("%s filename\n", argv[0]);
  else
	{
    	if (argc == 3)
    	{
    		printf ("Bootloader : %s\n",argv[1]);
  			printf ("Firmware s19 : %s\n\n",argv[2]);
  		}
  		else
     		printf ("Bootloader : %s\n\n",argv[1]);
  			
		    fd = init_rs232();
		    debug_print ("File to send to HC11 : %s\n", argv[1]);
		    f = fopen (argv[1], "r");
		    if(f < 0)
		    	printf("Error when opening file !! \n");
		    	
				send_hc11_bootloader (f, fd);
				
				/* Waiting for Echo message */
				debug_print ("Waiting for Echo message\n");
				/**/
				while ( 1 ) {
					read(fd,&c,1);
					if ( c == 0x03 )		//Ctrl-C
						break;
					//printf("%03u %02x %c\n",c&0xff,c&0xff,c);
					printf("%c",c);
					if ((c&0xff) == 0xff)
						printf("%02x received as echo \n",c&0xff);
					if ((c&0xff) == 0x8e){
						printf("%02x received for breaking\n",c&0xff);
						break;
					}
				printf ("S19 Firmware : %s\n\n",argv[2]);
 				/* Setting new baudrate */
 				rs232_speed(fd, B9600);
 				
			}
			/**/
			free_rs232(fd); 
	}
  return 0;
}
