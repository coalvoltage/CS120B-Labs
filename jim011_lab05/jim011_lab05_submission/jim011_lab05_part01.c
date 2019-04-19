/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 5 Exercise 1
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */
#include <avr/io.h>

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tempA;
	unsigned char tempB;
    while (1) 
    {
		tempA = ~PINA & 0x0F;
		if(tempA > 12) {
			tempB = 0x3F;
		}
		else if(tempA > 9) {
			tempB = 0x3E;
		}
		else if(tempA > 6) {
			tempB = 0x3C;
		}
		else if(tempA > 4) {
			tempB = 0x38;
		}
		else if(tempA > 2) {
			tempB = 0x30;
		}
		else if(tempA > 0) {
			tempB = 0x20;
		}
		else {
			tempB = 0x00;
		}
		if (tempA <= 4){
			tempB = tempB | 0x40;
		}
		
		PORTB = tempB;
		
    }
}
