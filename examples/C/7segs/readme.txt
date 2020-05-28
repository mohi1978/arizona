This is a simple program that prints on a 7-segment display mounted
arizonaF1 development board using the 68HC11F1 processor.

* caractère,seg. A,seg. B,seg. C,seg. D,seg. E,seg. F,seg. G,seg. DP

* ******************************************************************
*   _A__
* F|    |B
*  |_G__|
* E|    |C 
*  |____| 
*   D    *DP
*

PORTA[0]=>A
PORTA[1]=>B
PORTA[2]=>C
PORTA[3]=>D
PORTA[4]=>E
PORTA[5]=>F
PORTA[6]=>G
PORTA[7]=>*DP

PORTD[3] => CS

 
