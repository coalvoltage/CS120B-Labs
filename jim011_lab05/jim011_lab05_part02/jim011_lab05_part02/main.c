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
	DDRB = 0xFF;
	PORTB = 0x00;
	char unsigned a = 0;
	char unsigned b = 0;
    /* Replace with your application code */
	enum state {start,init,maintance,reset,sub, subRel,add, addRel} state;
	state = start;
    while (1) 
    {
		a = (~PINA) & 0x03;
		//Transition?
		switch(state){
		case start:
			state = init;
			break;
		case init:
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
			if(a == 0) {
				state = maintance;
			}
			break;
		case sub:
			if(a == 0) {
				state = subRel;
			}
			else if(a == 3) {
				state = reset;
			}
			break;
		case add:
			if(a == 0) {
				state = addRel;
			}
			else if(a == 3) {
				state = reset;
			}
			break;
		case subRel:
			state = maintance;
			break;
		case addRel:
			state = maintance;
			break;
		default:
			state = init;
			break;
		}
		//State Action
		switch(state){
			case init:
				b = 7;
				break;
					
			case reset:
				b= 0;
				break;
					
			case sub:
					
				break;
					
			case add:
					
				break;
					
			case subRel:
				if(b > 0) {
					b--;
				}
			break;
					
			case addRel:
				if(b < 9) {
					b++;
				}
				break;
					
			default:
				break;
		}
		PORTB = b;
    }
}
