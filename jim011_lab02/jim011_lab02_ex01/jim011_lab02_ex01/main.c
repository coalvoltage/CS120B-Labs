/*
 * jim011_lab02_ex01.c
 *
 * Created: 4/4/2019 12:48:33 AM
 * Author : Coal
 */ 

#include <avr/io.h>

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	while(1)
	{
			// 1) Read input
			tmpA = PINA & 0x03;
			// 2) Perform computation
			// if PA0 is 1, set PB0 = 1, else = 0
			if (tmpA == 0x01) { // True if PA0 is 1
				tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbbb1
			}
			else {
				tmpB = (tmpB & 0xFC) | 0x00; // Sets tmpB to bbbbbbb0
			}    
			// 3) Write output
-		PORTB = tmpB;
	}
    return 0;
}



