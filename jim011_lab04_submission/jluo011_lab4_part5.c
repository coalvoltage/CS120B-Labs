/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 4 Exercise 5
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#include <avr/io.h>

enum state {n0, n1, n2, n3, n4} state;
state = n0;
unsigned char arr[] = {0x04, 0x01, 0x02, 0x01};
unsigned short pos = 0;
char unsigned output = 0;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; output = 0; 
	DDRC = 0xFF; PORTC = 0x00; 
	while (1)
	{
		switch(state) {
		case n0:
			state = n1;
			break;
		case n1:
			if (PINA & 0x80) {
				state = n4;
				output = 0;
			} else if (PINA == arr[pos]) {
				if (pos == 0x03) {
					if (PORTB == 0x00) {
						state = n3;
						output = 1;
					} else {
						state = n4;
						output = 0;
					}
				} else {
					state = n2;
				}
			} else {
				state = n1;
			}
			break;
		case n2:
			if (PINA & 0x80) {
				state = n4;
				output = 0;
			} else if (PINA == 0x00) {
				pos++;
				state = n1;
			} else if (PINA == arr[pos]) {
				state = n2;
			} else {
				state = n1;
			}
			break;
		case n3:
			if (PINA & 0x80) {
				state = n4;
				output = 0;
			} else {
				state = n1;
			}
			pos = 0;
			break;
		case n4:
			if (PINA & 0x80) {
				state = n4;
				output = 0;
			} else {
				state = n1;
			}
			pos = 0;
			break;
		default:
			state = n0;
			break;
	}
	PORTB = output;
	PORTC = state << 4;
	PORTC = (PORTC & 0xF0) | pos;
	}
}
