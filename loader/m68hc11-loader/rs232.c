/* All examples have been derived from miniterm.c                          */
/* Don’t forget to give the appropriate serial ports the right permissions */
/* (e. g.: chmod a+rw /dev/ttyS0)                                         */

#include "rs232.h"

#define RAWMODE 0

extern struct termios oldtio, newtio;

int init_rs232 ()
{
	int fd;

	debug_print("RS232 Initialization start\n");
	fd = open (HC11DEVICE, O_RDWR | O_NOCTTY);
  if (fd < 0){
      perror (HC11DEVICE);
      exit (-1);
  }
  debug_print("RS232 : Device %s opened \n", HC11DEVICE);
  tcgetattr (fd, &oldtio);					/* save current serial port settings */
  bzero (&newtio, sizeof (newtio));	/* clear struct for new port settings */

	if (RAWMODE == 1)
	{
		newtio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
		newtio.c_oflag &= ~OPOST;
		newtio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
		newtio.c_cflag &= ~(CSIZE|PARENB);
		newtio.c_cflag |= CS8;
	}
	else
	{
		/* tcflag_t c_cflag : control modes */
		newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;	/* _no_ CRTSCTS */
		//newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CRTSCTS ; /* _with_ CRTSCTS */

		debug_log ("BAUDRATE=%d | CS8 | CLOCAL | CREAD and _no_ CRTSCTS\n", BAUDRATE);
		
		/* tcflag_t c_iflag : input modes */
		newtio.c_iflag = IGNPAR | ICRNL | IXON;
		
		/* tcflag_t c_oflag : output modes */
		newtio.c_oflag = ONOCR | ONLRET | OLCUC;

		/* tcflag_t c_lflag : local modes */ 
		//newtio.c_lflag = ICANON;
	}
	/* cc_t     c_cc[NCCS] : special characters */
	// newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
	// newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	// newtio.c_cc[VERASE]   = 0;     /* del */
	// newtio.c_cc[VKILL]    = 0;     /* @ */
	//newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
	newtio.c_cc[VTIME] 		= 0;	/* inter-character timer unused */
	newtio.c_cc[VMIN] 		= 1;	/* blocking read until 1 character arrives */
	// newtio.c_cc[VSWTC]    = 0;     /* ’\0’ */
	// newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
	// newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	// newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	// newtio.c_cc[VEOL]     = 0;     /* ’\0’ */
	// newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	// newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	// newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	// newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	// newtio.c_cc[VEOL2]    = 0;     /* ’\0’ */

	/* Set the baud rates to 1200... */
	//cfsetispeed(&options, B1200);
	//cfsetospeed(&options, B1200);

  tcflush (fd, TCIFLUSH);
  tcsetattr (fd, TCSANOW, &newtio);
	debug_print("RS232 Initialization done\n\n");
  return (fd);
}


int 
init_serial_rs232 (char *device, speed_t baudrate, int rawmode )
{
	int fd;
	debug_print("RS232 Initialization start\n");
	fd = open (device, O_RDWR | O_NOCTTY);
  if (fd < 0){
      perror (device);
      exit (-1);
  }
  debug_print("RS232 : Device %s opened \n", device);
  tcgetattr (fd, &oldtio);					/* save current serial port settings */
  bzero (&newtio, sizeof (newtio));	/* clear struct for new port settings */

	if (rawmode == 1)
	{
		debug_print("RS232 : Device %s opened in RAW mode \n", device);
		newtio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
		newtio.c_oflag &= ~OPOST;
		newtio.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
		newtio.c_cflag &= ~(CSIZE|PARENB);
		newtio.c_cflag |= CS8;
	}
	else
	{
		/* tcflag_t c_cflag : control modes */
		newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;	/* _no_ CRTSCTS */
		//newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CRTSCTS ; /* _with_ CRTSCTS */

		debug_log ("BAUDRATE=%d | CS8 | CLOCAL | CREAD and _no_ CRTSCTS\n", BAUDRATE);
		
		/* tcflag_t c_iflag : input modes */
		newtio.c_iflag = IGNPAR | ICRNL | IXON;
		
		/* tcflag_t c_oflag : output modes */
		newtio.c_oflag = ONOCR | ONLRET | OLCUC;

		/* tcflag_t c_lflag : local modes */ 
		//newtio.c_lflag = ICANON;
	}
	
	/* Set the baud rates to 1200... */
	cfsetispeed(&newtio, baudrate);
	cfsetospeed(&newtio, baudrate);

  tcflush (fd, TCIFLUSH);
  tcsetattr (fd, TCSANOW, &newtio);
  
	debug_print("RS232 Initialization done\n\n");
  return (fd);
}


void sendcmd (int fd, char *buf)
{
  unsigned int i, j;
  if ((write (fd, buf, strlen (buf))) < strlen (buf))
    {
      printf ("\n No connection...\n");
      exit (-1);
    }
  
  for (j = 0; j < 5; j++)
    for (i = 0; i < 3993768; i++)
      {
      }
	/* usleep(attente); */
}

/* restore the old port settings */
void free_rs232 (int fd)
{
  tcsetattr (fd, TCSANOW, &oldtio);
  close (fd);
  debug_print("RS232 : Device %s closed \n", HC11DEVICE);
}				

/* RAW Mode */
/*
	termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                           | INLCR | IGNCR | ICRNL | IXON);
	termios_p->c_oflag &= ~OPOST;
	termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	termios_p->c_cflag &= ~(CSIZE | PARENB);
	termios_p->c_cflag |= CS8;
*/



/*
 * Set the speed on the serial port
 */

void
rs232_speed(int tty_fd, speed_t speed)
{
	struct termios tp;

	tcgetattr(tty_fd, &tp);
	cfsetispeed(&tp, speed);
	cfsetospeed(&tp, speed);
	tcsetattr(tty_fd, TCSADRAIN, &tp);
	printf ("Speed : %d\n", speed);

	return;
}

void  
rs232_cfmakeraw(struct termios *t)
{
	t->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	t->c_cflag &= ~(CSIZE|PARENB);
	t->c_cflag |= CS8;
	/* XXX set MIN/TIME */
}
