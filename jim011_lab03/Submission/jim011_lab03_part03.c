/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 3  Exercise 3
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA;
	unsigned char seatbeltAndIgn;
	unsigned char tempC;
    while (1) 
    {
		tempA = PINA & 0x0F;
		seatbeltAndIgn = PINA;
		if(tempA > 12) {
			tempC = 0x3F;
		}
		else if(tempA > 9) {
			tempC = 0x3E;
		}
		else if(tempA > 6) {
			tempC = 0x3B;
		}
		else if(tempA > 4) {
			tempC = 0x38;
		}
		else if(tempA > 2) {
			tempC = 0x30;
		}
		else if(tempA > 0) {
			tempC = 0x20;
		}
		else {
			tempC = 0x00;
		}
		if (tempA <= 4){
			tempC = tempC | 0x40;
		}
		
		if(~GetBit(seatbeltAndIgn, 6) & GetBit(seatbeltAndIgn, 5) & GetBit(seatbeltAndIgn, 4)) {
			tempC = tempC | 0x80;
		}
		
		PORTC = tempC;
		
    }
}

