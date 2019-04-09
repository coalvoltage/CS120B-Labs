/*
 * jim011_lab03_part05.c
 *
 * Created: 4/9/2019 10:21:54 AM
 * Author : Coal
 */ 

#include <avr/io.h>

int main(void)
{
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;
	unsigned char tempD;
	unsigned char inputB;
	unsigned char outputB;
	unsigned short fullInputWeight;
    while (1) 
    {
		tempD = PIND;
		fullInputWeight = tempD << 1;
		inputB = PINB & 0x01;
		fullInputWeight = fullInputWeight | inputB;
		
		if(fullInputWeight < 5) {
			outputB = 0x00;
		}
		else if (fullInputWeight < 70) {
			outputB = 0x02;
		}
		else {
			outputB = 0x04;
		}
		outputB = outputB | inputB;
		PORTB = outputB;
    }
}

