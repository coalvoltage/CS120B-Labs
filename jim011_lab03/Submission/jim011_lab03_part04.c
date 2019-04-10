/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 3 Exercise 4
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA;
	unsigned char tempB;
	unsigned char tempC;
    while (1) 
    {
		tempA = PINA;
		
		tempB = tempA & 0xF0;
		tempB = tempB >> 4;
		tempC = tempA & 0x0F;
		tempC = tempC << 4;
		PORTB = tempB;
		PORTC = tempC;
    }
}

