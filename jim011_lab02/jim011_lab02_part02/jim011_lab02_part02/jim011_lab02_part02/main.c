/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 2  Exercise 2
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
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
		PORTC = 4 - cntavail;
	}
	return 0;
}
