/*
 * S19-analyzer.c
 * 
 * Copyright 2020 mohi1978 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/*
 * Function prototypes
 */

void usage(void);
int gethex(char *, unsigned char **, int *);

/*
 * Set up a few defaults
 */
#define BUFFER_SIZE 255

//char port[SERIAL_PORT_LENGTH];
int lineno;			/* Line number of current file */
int serial_debug = 0;		/* If true, print out serial debugging info */

/*
 * This is the default bootstrap program used for the serial download.  This
 * program gets loaded into RAM starting at $0000, and then gets executed.
 * It does the actual work of burning the program into EEPROM.  It is important
 * that each S-record is newline delimited.
 */

char bootstring[] =
"S12300008E00FF4FB71035CE10001F2E80FC1D282086B0A72B860CA72D8D2C20FC7F00EC4A\n"
"S123002020058601B700EC8D5F167D00EC27048D102006183C8D5F183808180926E93918FB\n"
"S12300408FE700A600202D863E8D298D3B817327F6368D2C8F8D29188F32817227E58170E3\n"
"S12300602732815727BC815027B381422741817727CD863F3CCE10001F2E80FCA72F3839B7\n"
"S12300808D06368D031632393CCE10001F2E20FCA62F3839188FE10027A9A60043F700ED94\n"
"S12300A0B400ED270486168D2486028D2020948606B7103BB7103F8A01B7103B183CBD0008\n"
"S12300C0DD18387F103B188F8602CE103FB7103BE7008A01B7103B8D0A7F103B3918CE6F74\n"
"S10F00E09A200418CE0B29180926FC39BC\n"
"S9030000FC\n";

int
main(int argc, char *argv[])
{
	int opt, bytes, total_bytes = 0, address=1;
	char *b = bootstring;
	unsigned char *buffer;
	
	printf("S19 Analyzer file for 68HC11 by "
	"mohi1978\n");
	printf("Date : 2020/05/17 10:14:00\n");
	printf("Compiled the : %s by %s\n", __DATE__, __TIME__);
	
	printf("S19 frame = %sType%sCount%sAddress%sData%sChecksum\n",KYEL,KRED,KWHT,KGRN,KMAG);
	printf("Example:\n");
	printf("%sS1%s23%s0000%s8E00FF4FB71035CE10001F2E80FC1D282086B0A72B860CA72D8D2C20FC7F00EC%s4A\n",KYEL,KRED,KWHT,KGRN,KMAG);
	printf("--------------------------------------------------------------------------\n");											
	while((opt = getopt(argc, argv, "c:db")) != -1)
		switch (opt) {
		case 'c':
			// function
			break;
		case 'd':
			serial_debug++;
			break;
		case 'h':
		case '?':
		default:
			usage();
		}

	/*
	 * analyze
	 */

	while(1) {
		if ((bytes = gethex(b, &buffer, &address)) == 0)
			goto nextbootstrap;

		total_bytes+=bytes;
	
nextbootstrap:
		lineno++;

		if ((b = index(b, '\n')) == NULL)
			break;
		b++;
		
		if (*b == '\0')
			break;
	}

	printf("\n");
	
	printf("\nAddress : %d (0x%04X)\n", address, address);
	printf("\nTotal Bytes : %d (0x%04X)\n", total_bytes, total_bytes);

	printf("\nUser program downloaded successfully\n");

	exit(0);
}

/*
 * Take one line of an S-record and convert it into a hex string
 */

int
gethex(char *line, unsigned char **retval, int *address)
{
	int i, checksum = 0, addr;
	unsigned int bytes, byte;
	static unsigned char hexbuf[255];

	if (line[0] != 'S') {
		fprintf(stderr, "Line number %d is not an S-record; "
		"aborting\n", lineno);
		exit(1);
	}
	else
		printf("\n%s%c",KYEL,line[0]);
   
	if (line[1] != '1' && line[1] != '9') {
		fprintf(stderr, "Line number %d is a type %c S-record.\n"
		"DLM only handles types 1 and 9.  Aborting\n", lineno,
		line[1]);
		exit(1);
	}
	else if (line[1] == '9')
		printf("%s%c",KBLU,line[1]);
  else if (line[1] == '1')
		printf("%s%c",KYEL,line[1]);
  else
  	printf("%s%c",KCYN,line[1]);
  
	if (sscanf(&line[2], "%2x", &bytes) != 1) {
		fprintf(stderr, "Non-hex digit in S-record on line %d, "
			"aborting\n", lineno);
		exit(1);
	}
	// Nb of bytes
	printf("%s%02X",KRED,bytes);
  
	// Checksum : making up the count
	checksum += bytes;

	line += 4;

	if (sscanf(line, "%4x", &addr) != 1) {
		fprintf(stderr, "Non-hex digit in S-record on line %d, "
			"aborting\n", lineno);
		exit(1);
	}
	printf("%s%04X",KWHT,addr);
	
  // Checksum : making up the count, the address 
	checksum += (addr & 0xff) + ((addr >> 8) & 0xff);
	line += 4;

	if (address)
		*address = addr;

	for(i = 0; i < bytes - 3; i++) {
		if (sscanf(line, "%2x", &byte) != 1) {
			fprintf(stderr, "Non-hex digit in S-record on line "
				"%d, aborting\n", lineno);
			exit(1);
		}
		hexbuf[i] = byte;
		// Checksum : making up the count, the address and the data fields.
		checksum += byte;
		line += 2;
		printf("%s%02X", KGRN, hexbuf[i]);
	}

	if (sscanf(line, "%2x", &byte) != 1) {
		fprintf(stderr, "Non-hex digit in S-record on line "
			"%d, aborting\n", lineno);
		exit(1);
	}

	// Checksum : These characters when paired and interpreted as a 
	// hexadecimal value display  the least significant bytes of the ones
	// complement of the sum of the byte values represented by the pairs 
	// of characters making up the count, the address and the data fields.
	if (byte != (~checksum & 0xff)) {
		fprintf(stderr, "Warning: invalid checksum on line %d "
			"(given: %02X, computed: %02X)\n", lineno, byte,
			~checksum & 0xff);
			printf("%s%02X",KRED,byte);
	}
	else
		printf("%s%02X",KMAG,byte);
  	
	*retval = hexbuf;
	return bytes - 3;
}

/*
 * Get a character from the serial port, but do not block getting it
 */

void
usage()
{
	printf("Usage: mohi1978 [-h] [-c ] [S19filename]\n\n");
	printf("\t-c S19 Analyzer tool\n"
		"\t\t\t(default: bootstring)\n");
	printf("\t-h		This message\n");

	exit(2);
}
