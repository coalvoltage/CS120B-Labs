/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 3  Exercise 1
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA;
	unsigned char tempB;
	unsigned char bitCounter = 0;
    while (1) 
    {
		//Read input
		tempA = PINA;
		tempB = PINB;
		bitCounter = 0;
		for(unsigned char i = 0; i < 8;i++) {
			if((tempA & 0x01) == 0x01) {
				bitCounter++;
			}
			if((tempB & 0x01) == 0x01) {
				bitCounter++;
			}
			tempA = tempA >> 0x01;
			tempB = tempB >> 0x01;
		}
		PORTC = bitCounter;
		
    }
}

