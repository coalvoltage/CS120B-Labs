/*
 * jim011_lab03_part04.c
 *
 * Created: 4/9/2019 10:10:33 AM
 * Author : Coal
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

