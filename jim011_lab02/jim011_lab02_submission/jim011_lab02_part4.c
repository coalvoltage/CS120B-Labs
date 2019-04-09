
/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 2  Challenge 1
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */
#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char aWeight = 0x00;
	unsigned char bWeight = 0x00;
	unsigned char cWeight = 0x00;
	unsigned char tempD = 0x00;
	signed short signedAWeight = 0;
	signed short signedCWeight = 0;
	signed short difBetweenAC = 0;
	unsigned short weightTotal = 0;
	unsigned char shiftedWeightTotal = 0;
	while (1)
	{
		aWeight = PINA;
		bWeight = PINB;
		cWeight = PINC;
		
		signedAWeight = 0 | aWeight;
		signedCWeight = 0 | cWeight;
		
		difBetweenAC = signedAWeight - signedCWeight;
		weightTotal = aWeight + bWeight + cWeight;
		
		if(weightTotal >= 140) {
			tempD = (tempD & 0xFE) | 0x01;
		}
		else {
			tempD = (tempD & 0xFE) | 0x00;
		}
		if((difBetweenAC >= 80) || (difBetweenAC <= -80)) {
			tempD = (tempD & 0xFD) | 0x02;
		}
		else {
			tempD = (tempD & 0xFD) | 0x00;
		}
		
		shiftedWeightTotal = weightTotal >> 4; //converting weightTotal to have less bits and accuracy (being unable to detect 16 pounds)
		shiftedWeightTotal = shiftedWeightTotal << 2; //moving right in preparation with anding with remaining PORTD bits
		
		tempD = (tempD & 0x03) | shiftedWeightTotal;
		
		PORTD = tempD;
		
		
	}
	return 0;
}
