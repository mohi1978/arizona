#ifndef __HC11_H
#define __HC11_H	1

/* 
 * base address of register block for 68HC11F1.
 * Adpated by : mohi1978
 */
#ifndef _IO_BASE
#define _IO_BASE	0x1000
#endif

#define PORTA	(*(unsigned char volatile *)(_IO_BASE + 0x00))	/* Port A register */
#define DDRA	(*(unsigned char volatile *)(_IO_BASE + 0x01))	/* Data direction register for port A */
#define PORTG	(*(unsigned char volatile *)(_IO_BASE + 0x02))	/* Port G register */
#define DDRG	(*(unsigned char volatile *)(_IO_BASE + 0x03))	/* Data direction register for port G */
#define PORTB	(*(unsigned char volatile *)(_IO_BASE + 0x04))	/* Port B register */
#define PORTF	(*(unsigned char volatile *)(_IO_BASE + 0x05))	/* Port F register */
#define PORTC	(*(unsigned char volatile *)(_IO_BASE + 0x06))	/* Port C register */
#define DDRC	(*(unsigned char volatile *)(_IO_BASE + 0x07))	/* Data direction register for port C */
#define PORTD	(*(unsigned char volatile *)(_IO_BASE + 0x08))	/* Port D register */
#define DDRD	(*(unsigned char volatile *)(_IO_BASE + 0x09))	/* Data direction register for port D */
#define PORTE	(*(unsigned char volatile *)(_IO_BASE + 0x0A))	/* Port E input register */
#define CFORC	(*(unsigned char volatile *)(_IO_BASE + 0x0B))	/* Compare Force Register */
#define OC1M	(*(unsigned char volatile *)(_IO_BASE + 0x0C))	/* OC1 Action Mask register */
#define OC1D	(*(unsigned char volatile *)(_IO_BASE + 0x0D)) 	/* OC1 Action Data register */
#define TCNT	(*(unsigned short volatile *)(_IO_BASE + 0x0E)) /* Timer Counter Register */
#define TIC1	(*(unsigned short volatile *)(_IO_BASE + 0x10))
#define TIC2	(*(unsigned short volatile *)(_IO_BASE + 0x12))
#define TIC3	(*(unsigned short volatile *)(_IO_BASE + 0x14))
#define TOC1	(*(unsigned short volatile *)(_IO_BASE + 0x16))
#define TOC2	(*(unsigned short volatile *)(_IO_BASE + 0x18))
#define TOC3	(*(unsigned short volatile *)(_IO_BASE + 0x1A))
#define TOC4	(*(unsigned short volatile *)(_IO_BASE + 0x1C))
#define TOC5	(*(unsigned short volatile *)(_IO_BASE + 0x1E))
#define TCTL1	(*(unsigned char volatile *)(_IO_BASE + 0x20))
#define TCTL2	(*(unsigned char volatile *)(_IO_BASE + 0x21))
#define TMSK1	(*(unsigned char volatile *)(_IO_BASE + 0x22))
#define TFLG1	(*(unsigned char volatile *)(_IO_BASE + 0x23))
#define TMSK2	(*(unsigned char volatile *)(_IO_BASE + 0x24))
#define TFLG2	(*(unsigned char volatile *)(_IO_BASE + 0x25))
#define PACTL	(*(unsigned char volatile *)(_IO_BASE + 0x26))
#define PACNT	(*(unsigned char volatile *)(_IO_BASE + 0x27))
#define SPCR	(*(unsigned char volatile *)(_IO_BASE + 0x28))
#define SPSR	(*(unsigned char volatile *)(_IO_BASE + 0x29))
#define SPDR	(*(unsigned char volatile *)(_IO_BASE + 0x2A))
#define BAUD	(*(unsigned char volatile *)(_IO_BASE + 0x2B))
#define SCCR1	(*(unsigned char volatile *)(_IO_BASE + 0x2C))
#define SCCR2	(*(unsigned char volatile *)(_IO_BASE + 0x2D))
#define SCSR	(*(unsigned char volatile *)(_IO_BASE + 0x2E))
#define SCDR	(*(unsigned char volatile *)(_IO_BASE + 0x2F))
#define ADCTL	(*(unsigned char volatile *)(_IO_BASE + 0x30))
#define ADR1	(*(unsigned char volatile *)(_IO_BASE + 0x31))
#define ADR2	(*(unsigned char volatile *)(_IO_BASE + 0x32))
#define ADR3	(*(unsigned char volatile *)(_IO_BASE + 0x33))
#define ADR4	(*(unsigned char volatile *)(_IO_BASE + 0x34))
#define OPTION	(*(unsigned char volatile *)(_IO_BASE + 0x39))
#define COPRST	(*(unsigned char volatile *)(_IO_BASE + 0x3A))
#define PPROG	(*(unsigned char volatile *)(_IO_BASE + 0x3B))
#define HPRIO	(*(unsigned char volatile *)(_IO_BASE + 0x3C))
#define INIT	(*(unsigned char volatile *)(_IO_BASE + 0x3D))
#define TEST1	(*(unsigned char volatile *)(_IO_BASE + 0x3E))
#define CONFIG	(*(unsigned char volatile *)(_IO_BASE + 0x3F))

#endif
