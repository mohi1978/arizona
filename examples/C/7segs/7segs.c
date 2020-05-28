/* *******************************************************/
// Author (C) 2020 mohi1978
// Date : 2020/05/28 10:59:41
// Routine to drive a 7 segment display (Common Anode)
/* *******************************************************/
#include <hc11-f1.h>
#include "tsmtypes.h"

/*
********************************************************************
Caractère,seg. A,seg. B,seg. C,seg. D,seg. E,seg. F,seg. G,seg. DP
* ******************************************************************
*   _A__
* F|    |B
*  |_G__|
* E|    |C 
*  |____| 
*   D    *DP
* 
Y,A,B,C,D,E,F,G,DP
0,x,x,x,x,x,x, , , // 0x3F 
1, ,x,x, , , , , , // 0x06
2,x,x, ,x,x, ,x, , // 
3,x,x,x,x, , ,x, ,
4, ,x,x, , ,x,x, ,
5,x, ,x,x, ,x,x, ,
6,x, ,x,x,x,x,x, ,
7,x,x,x, , , , , , 
8,x,x,x,x,x,x,x, ,
9,x,x,x,x, ,x,x, , // 0x6F
A,x,x,x, ,x,x,x, , // 0x77
b, , ,x,x,x,x,x, , // 0x7C
C,x, , ,x,x,x, , , // 0x39 
d, ,x,x,x,x,x, , , // 0x3E
E,x, , ,x,x,x,x, , // 0x79
F,x, , , ,x,x,x, , // 0x71

H, ,x,x, ,x,x,x, , // 0x76
I, ,x,x, , , , , , // 0x06
J, ,x,x,x,x, , , , // 0xE1
L, , , ,x,x,x, , , // 0x83
o, , ,x,x,x, ,x, , // 0xC5
P,x,x, , ,x,x,x, , // 0x37
S,x, ,x,x,x, ,x, , // 0xD5
t, , , , ,x,x,x, , // 0x07
U, ,x,x,x,x,x, , , // 0xE3
u, ,x,x,x,x, ,x, , // 0xE5
°,x,x, , , ,x,x, , // 0x36
*
*/

/* Declaration of fonction */ 
void init_setup(void);
void init_portA(void);
void init_portD(void);
void mdelay(void);
void wait(uint16_t cnt);

void digit_unit();
void digit_7segs_display( short index);


void init_setup()
{
	init_portA(); // Data
	init_portD(); // Control
}

void mdelay()
{
  unsigned short int i,j;
  unsigned short int speed = 0xffff;

  for (i = 0; i < speed; i++)
  {
     for (j=0; j< 10; j++){
   	/* nothing */
  	}
  }
}

void init_portA()
{
	/* Configure the Port A in OUTPUT  */
	DDRA 	= 0xFF; // For DDRx bits, 0 = input and 1 = output.
	PORTA = ~0x3F; // Init with all PortA @FF
}

void init_portD()
{
	/* Configure the Port C in OUTPUT  */
	DDRD	= 0x3C; // 0b00111100;  // For DDRx bits, 0 = input and 1 = output.
	PORTD	= 0x38; // 0b00110000;  // Init with all PortA @FF
}

void digit_7segs_display( short index)
{
	//char hexadecimal[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x76,0x06,0xE1,0x83,0xC5,0x37,0xD5,0x07,0xE3,0xE5,0x36,'\0'};
	//char alphabet[29]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x7D,0x76,0x06,0xE1,0x83,0xC5,0x37,0xD5,0x07,0xE3,0xE5,0x36,'\0'};

	char number[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,'\0'};
	
	PORTD = 0x04;
	PORTA = ~number[index & 0xFF] & 0xFF;
	mdelay();
}

void digit_dizaine()
{
   PORTD = 0x04; //DIGIT_UNIT;
}         

void digit_unit()
{
   PORTD = 0x08; //DIGIT_UNIT;
}         

int
main (void)
{
	uint16_t tempo=0x1FFF;
	uint8_t cpt;
	
	init_setup();
	
	for(;;){
		
		/* Print numbers from 0 to 9*/
		for (cpt=0;cpt<10;cpt++ ){
			
			digit_7segs_display(cpt);
			wait(tempo);
		}
	}
	return;
}

void	wait(uint16_t cnt)
{
	for (;cnt>0;cnt--);
}
