/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 4 Exercise 4
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
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
	enum state{n0,n1,n2,n3,n4,n5,n6} state;
		state = 0;
    while (1) 
    {
		input = PINA & 0x07;
		inside = PINA & 0x80;
		inside = inside >> 7;
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
				else if(input != 0 && inside == 0) {
					state = n1;
				}
				break;
			case n4:
				stateOutput = 4;
				output = 1;
				if(input == 0 && inside == 1) {
					state = n1;
					output = 0;
				}
				else if(input == 0 && inside == 0) {
					state = n5;
				}
				break;
			case n5:
				if(input == 0 && inside == 0) {
					state = n3;
				}
				break;
			case n6:
				stateOutput = 3;
				if(input == 2 && inside == 0) {
					state = n4;
					output = 0;
				}
				else if(input != 0 && inside == 0) {
					state = n4;
				}
				break;
			default:
				state = n0;
				break;
		}
		PORTB = output;
		PORTC = stateOutput;
    }
}


