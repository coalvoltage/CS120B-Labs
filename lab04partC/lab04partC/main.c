/*
 * lab04partC.c
 *
 * Created: 4/11/2019 10:42:41 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	PORTC = 0x00;
	DDRC = 0xFF;
	char unsigned output = 0;
	char unsigned input = 0;
	char unsigned inside = 0;
	char unsigned stateOutput = 0;
	enum state{n0,n1,n2,n3,n4} state;
		state = 0;
    while (1) 
    {
		input = PORTA & 0x07;
		inside = PORTA & 0x80;
		switch(state){
			case n0:
				state = n1;
				stateOutput = 0;
				output = 0;
				break;
			case n1:
				stateOutput = 1;
				if (input == 4 && inside == 0){
					state = n2;
				}
				break;
			case n2:
			stateOutput = 2;
				if(input == 0 && inside == 0) {
					state = n3;
				}
				break;
			case n3:
				stateOutput = 3;
				if(input == 2 && inside == 0) {
					state = n4;
					output = 1;
				}
				break;
			case n4:
				stateOutput = 4;
				output = 1;
				if(input == 0 && inside == 8) {
					state = n1;
					output = 0;
				}
				break;
				
		}
		PORTB = output;
		PORTC = stateOutput;
    }
}

