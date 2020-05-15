#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>		/* declaration of bzero() */
#include <fcntl.h>
#include <termios.h>

int init_rs232 ();
int init_serial_rs232 (char *, speed_t , int );
void free_rs232 ();
void sendcmd (int, char *);
void cfmakeraw_rs232(struct termios *t);

struct termios oldtio, newtio;

#define BAUDRATE B1200
//#define BAUDRATE B2400
// #define BAUDRATE B4800 B9600 B19200
// #define BAUDRATE B9600
// #define BAUDRATE B19200

//#define HC11DEVICE "/dev/ttyUSB0"
#define HC11DEVICE "/dev/ttyS0"

#define DEBUG 	1

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define debug_print(fmt, ...) \
					do { if (DEBUG_TEST) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)
         
#define debug_log(fmt, ...) \
        do { if (DEBUG_TEST) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                              __LINE__, __func__, ##__VA_ARGS__); } while (0)

