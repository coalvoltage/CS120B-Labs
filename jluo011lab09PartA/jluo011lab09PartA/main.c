/*
 * jluo011lab09PartA.c
 *
 * Created: 5/2/2019 9:54:32 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurxr = 0;
void set_PWN(double frequency){
	static double current_frequency;
	if (frequency != current_frequency){
		if (!frequency){ TCCR0B &= 0x08;}
			else{
				TCCR0B != 0x03;
			}
			if (frequency < 0.954){
				OCR0A = 0xFFFF;
			}
			else if (frequency > 31250){
				OCR0A = 0x0000;
			}
			else{
				OCR0A = (short)(8000000 / (128 * frequency ) - 1);
			}
			TCNT0 = 0;
			current_frequency = frequency;
			
	}
}
void PWM_on(){
	TCCR0A = (1 << COM0A0);
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	set_PWN(0);
}
void PWM_off(){
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr _avr_timer_M;
}

int main(void)
{
	notes enum{c,d,e,f,g,a,b} notes;
	notes myAry[] = {e,e,e,c,e,g,c,g,e,a,b,b,a};
	states enum{play,noPlay,soundOn,soundOff} states;
	states = noPlay;
	unsigned char curIndex = 0;
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

