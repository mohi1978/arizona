#include <hc11-f1.h>

typedef unsigned int uint16_t;

void	wait(uint16_t cnt);

int
main (void)
{
	uint16_t tempo;
	DDRG=0xFF;
	PORTG=0x00;
	tempo=30000;
	for(;;){
		PORTG = ~0x01;
		wait(tempo);
		PORTG = ~0x00;
		wait(tempo);
		}
	return;
}

void	wait(uint16_t cnt)
{
	for (;cnt>0;cnt--);
}
