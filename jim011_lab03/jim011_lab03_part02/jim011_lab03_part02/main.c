/*
 * jim011_lab03_part02.c
 *
 * Created: 4/9/2019 8:19:34 AM
 * Author : Coal
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
		
		PORTC = tempC;
		
    }
}

