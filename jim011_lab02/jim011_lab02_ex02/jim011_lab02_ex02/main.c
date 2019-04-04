/*
 * jim011_lab02_ex02.c
 *
 * Created: 4/4/2019 1:48:52 AM
 * Author : Coal
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char cntavail;
	unsigned char tempA = 0x00;
    while (1) 
    {
		tempA = PINA & 0x0F;
		cntavail = 0;
		for(char i = 0; i < 4;i++) {
			if((tempA & 0x01) == 0x01) {
				cntavail++;
			}
			tempA = tempA >> 0x01;
		}
		PORTC = cntavail;
    }
	return 0;
}

