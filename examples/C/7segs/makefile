OC = m6811-elf-objcopy
CC = m6811-elf-gcc
AS = m6811-elf-as

RM = rm

CFLAGS  = -m68hc11 -O3 -fno-ident -fno-common -fomit-frame-pointer -mshort
LDFLAGS = -Wl,-u,-mm68hc11elfb
OCFLAGS = -O srec
LOCALDIR =/home/c-defigueiredo/Bureau/68HC11F1-Linux/arizona
INCDIR = $(LOCALDIR)/include
LIBDIR = $(LOCALDIR)/lib

PROG=7segs
CSRCS=$(PROG).c

OBJS=$(CSRCS:.c=.o)

all:	$(PROG)-hc11.elf $(PROG)-hc11.s19

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $<

$(PROG)-hc11.elf:	$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -L$(LIBDIR) -o $(PROG)-hc11.elf $(OBJS)

$(PROG)-hc11.s19:	$(PROG)-hc11.elf
	$(OC) $(OCFLAGS) $(PROG)-hc11.elf $(PROG)-hc11.s19

clean:
	$(RM) -f $(PROG)-hc11.elf
	$(RM) -f $(PROG)-hc11.s19
	$(RM) -f $(PROG)-hc11.dmp
	$(RM) -f $(OBJS)
