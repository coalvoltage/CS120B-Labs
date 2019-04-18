/*
 * jim011_lab05_part03.c
 *
 * Created: 4/18/2019 9:24:24 AM
 * Author : Coal
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char sequence[5];
	sequence[0] = 0x2A;
	sequence[1] = 0x15;
	sequence[2] = 0x3F;
	sequence[3] = 0x0C;
	sequence[4] = 0x33;
	
    enum state {start, init, loop, buttonPres, buttonRel} state;
	state = start;
	
	unsigned char inputA = 0;
	unsigned char outputB = 0;
	unsigned char index = 0;
	const unsigned char ARRAY_LIMIT = 4;
	
    while (1) 
    {
		//read input
		inputA = ~PINA & 0x01;
		//transition
		switch(state) {
			case start:
			state = init;
			break;
			
			case init:
			state = loop;
			break;
			
			case loop:
			if(inputA) {
				state = buttonPres;
			}
			break;
			
			case buttonPres:
			if(!inputA) {
				state = buttonRel;
			}
			break;
			
			case buttonRel:
			state = loop;
			break;
			
			default:
			state = start;
			break;
		};
		
		switch(state) {
			case start:
			break;
			
			case init:
			index = 0;
			outputB = 0x00;
			break;
			
			case loop:
			outputB = sequence[index];
			break;
			
			case buttonRel:
			if(index < ARRAY_LIMIT) {
				index++;
			}
			else {
				index = 0;
			}
			break;
			
			case buttonPres:
			break;
			
			default:
			break;
		};
		PORTB = outputB;
    }
}

