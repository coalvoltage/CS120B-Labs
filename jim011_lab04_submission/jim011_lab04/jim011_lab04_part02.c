/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 4 Exercise 2
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRC = 0xFF;
	PORTC = 0x00;
	char unsigned a = 0;
	char unsigned c = 0;
    /* Replace with your application code */
	enum state {init,maintance,reset,sub,add} state;
		state = init;
    while (1) 
    {
		a = PINA & 0x03;
		switch(state){
		case init:
			c = 7;
			state = maintance;
			break;
		case maintance:
			if (a==1){
				state = add;
			}
			else if (a==2){
				state = sub;
			}
			else if (a==3){
				state = reset;
			}
			break;
		case reset:
			c = 0;
			state = maintance;
			break;
		case sub:
			c -= 1;
			state = maintance;
			break;
		case add:
			c++;
			state = maintance;
			break;
		default:
			state = init;
			break;
		}
		PORTC = c;
    }
}

