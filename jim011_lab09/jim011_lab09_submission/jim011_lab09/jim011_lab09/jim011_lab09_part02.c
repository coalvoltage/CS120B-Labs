/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 9 Exercise 2
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

#define NOTEC4 261.63
#define NOTED4 293.66
#define NOTEE4 329.63
#define NOTEF4 349.23
#define NOTEG4 392.00
#define NOTEA4 440.00
#define NOTEB4 493.88
#define NOTEC5 523.25

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;    // Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}


void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64                    // 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}



enum States {start, init, wait, toggle, toggleRelease, upScale, upScaleRelease, downScale, downScaleRelease} state;
unsigned char tempA = 0x00;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
//unsigned short count = 0;
const unsigned short PERIOD = 100;

const char NOTEARRAYMAX = 7;
double NOTEARRAY[8] = {NOTEC4,NOTED4,NOTEE4,NOTEF4,NOTEG4,NOTEA4,NOTEB4,NOTEC5};
unsigned char noteIndex;
unsigned char isOn;
double freqOut = 0.00;
void Tick();

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x08; PORTB = 0x00;
	TimerSet(PERIOD);
	TimerOn();
	PWM_on();
	
	while (1)
	{
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
		
	}
}

void Tick() {
	//Input
	tempA = (~PINA) & 0x07;
	
	button1 = tempA & 0x01;
	button2 = (tempA & 0x02) >> 1;
	button3 = (tempA & 0x04) >> 2;
	//Transition
	switch(state) {
		case start:
		state = init;
		break;
		
		case init:
		state = wait;
		break;
		
		case wait:
		if(button2 && !button1 && !button3) {
			state = toggle;
		}
		else if(!button2 && button1 && !button3) {
			state = upScale;
		}
		else if(!button2 && !button1 && button3) {
			state = downScale;
		}
		break;
		
		case toggle:
		if(!button2 && !button1 && !button3) {
			state = toggleRelease;
		}
		break;
		
		case toggleRelease:
		state = wait;
		break;
		
		case upScale:
		if(!button2 && !button1 && !button3) {
			state = upScaleRelease;
		}
		break;
		
		case upScaleRelease:
		state = wait;
		break;
		
		case downScale:
		if(!button2 && !button1 && button3) {
			state = downScaleRelease;
		}
		break;
		
		case downScaleRelease:
		state = wait;
		break;
		
		default:
		break;
	}
	//Action
	switch(state) {
		case init:

		noteIndex = 0;
		freqOut = NOTEARRAY[0];
		isOn = 0;
		break;
		
		case wait:
		break;
		
		case toggleRelease:
		if(isOn) {
			PWM_off();
		}
		else {
			PWM_on();
		}
		isOn = !isOn;
		break;
		
		case upScaleRelease:
		if(noteIndex < NOTEARRAYMAX) {
			noteIndex++;
		}
		freqOut = NOTEARRAY[noteIndex];
		break;
		
		case downScaleRelease:
		if(noteIndex > 0) {
			noteIndex--;
		}
		freqOut = NOTEARRAY[noteIndex];
		break;
		
		default:
		break;
	}
	set_PWM(freqOut);
}