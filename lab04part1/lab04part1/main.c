/*
 * lab04part1.c
 *
 * Created: 4/11/2019 9:56:14 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
	PORTB = 0X00;
    /* Replace with your application code */
	char unsigned A = 0;
	char unsigned B = 0;
	enum state {start, b0On, b0Toggle, b1On, B1Toggle} state;
	state = start;
    while (1) 
    {
		A = PINA;
		A = A & 0x01;
		switch (state){
		case start:
			state = b0On;
			break;
		case b0On:
		//A = A & 0x01;
		if (A == 1){
			B = 0x02;
			state = b0Toggle;
		}
		else{
			B = 0x01;
		}
		break;
		case b0Toggle:
			//A = A & 0x01;
			if (A == 1){
				B = 0x02;
			}
			else{
				state = b1On;
				B = 0x02;
			}
			break;
		case b1On:
			if (A == 1){
				B = 0x01;
				state = B1Toggle;	
			}
			else{
				B = 0x02;
			}
			break;
		case B1Toggle:
			if (A==1){
				B = 0x01;
			}
			else{
				state = b0On;
				B = 0x01;
			}
			break;
		default:
			state = start;
			break;
		}
	
		PORTB = B;
    }
}

