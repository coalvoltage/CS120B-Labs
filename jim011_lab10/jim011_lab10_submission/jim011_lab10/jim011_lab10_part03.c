/*Jimmy Im : jim011@ucr.edu
 *James Luo : jluo011@ucr.edu
 *Lab Section: 024
 *Assignment: Lab 10 Exercise 3
 *
 *I acknowledge all content contained herein, excluding template or example
 *code, is my own original work.
 */

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

void ThreeLED();
void BlinkingLED();
void SoundButton();
void CombineLED();

enum StatesThreeLED {start3, init3, on3} state3LED;
enum StatesBlinkingLED {startB, onB, offB} statebLED;
enum StatesToggleSound {startS, waitS, pressedOnS,pressedOffS} stateSound;
enum StateOutputLED {start, output} stateOutput;
const unsigned short PERIOD = 2;
const unsigned char threeLEDPatterns[] = {0x01, 0x02, 0x04};
const unsigned char BLINKINGLEDPIN = 0x08;
const unsigned char SOUNDPIN = 0x10;
const unsigned char BUTTONPIN = 0x04;
unsigned long blinkingLEDsElapsedTime = 0;
unsigned long threeLEDsElapsedTime = 0;
unsigned long soundElapsedTime = 0;
const unsigned long blinkingLEDsPERIOD = 1000;
const unsigned long threeLEDsPERIOD = 300;
const unsigned long soundPERIOD = 2;
unsigned char tempA = 0x00;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLEDs = 0x00;
unsigned char soundOutputs = 0x00;
unsigned char outputLEDs = 0x00;


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(PERIOD);
	TimerOn();
	
	while (1)
	{
		tempA = ~PINA;
		if(blinkingLEDsElapsedTime >= blinkingLEDsPERIOD) {
			BlinkingLED();
			blinkingLEDsElapsedTime = 0;
		}
		if(threeLEDsElapsedTime >= threeLEDsPERIOD) {
			ThreeLED();
			threeLEDsElapsedTime = 0;
		}
		if(soundElapsedTime >= soundPERIOD) {
			SoundButton();
			soundElapsedTime = 0;
		}
		CombineLED();
		while(!TimerFlag){}
		TimerFlag = 0;
		threeLEDsElapsedTime += PERIOD;
		blinkingLEDsElapsedTime += PERIOD;
		soundElapsedTime += PERIOD;
	}
}

void ThreeLED(){
	//STATIC VARS
	unsigned static char currentPattern;
	//LOGIC
	switch(state3LED) {
		case start3:
		state3LED = init3;
		break;
		case init3:
		state3LED = on3;
		break;
		case on3:
		//state3LED = off3;
		break;
		default:
		state3LED = start3;
		break;
	}
	//OUTPUT
	switch(state3LED) {
		case init3:
		currentPattern = 0;
		break;
		case on3:
		threeLEDs = threeLEDPatterns[currentPattern];
		if(currentPattern >=2) {
			currentPattern = 0;
		}
		else {
			currentPattern++;
		}
		break;
		default:
		state3LED = start3;
	}
}
void BlinkingLED(){
	//LOGIC
	switch(statebLED) {
		case startB:
		statebLED = onB;
		break;
		case onB:
		statebLED = offB;
		break;
		case offB:
		statebLED = onB;
		break;
		default:
		statebLED = startB;
		break;
	}
	//OUTPUT
	switch(statebLED) {
		case onB:
		blinkingLEDs = BLINKINGLEDPIN;
		break;
		
		case offB:
		blinkingLEDs = 0x00;
		break;
		
		default:
		break;
	}
}

void SoundButton(){
	unsigned char inputButton = tempA & BUTTONPIN;
	switch(stateSound) {
		case startS:
			stateSound = waitS;
			break;
		case waitS:
			if(inputButton == BUTTONPIN) {
				stateSound = pressedOnS;
			}
			break;
		case pressedOnS:
			if(inputButton == BUTTONPIN) {
				stateSound = pressedOffS;
			}
			else {
				stateSound = waitS;
			}
			break;
		case pressedOffS:
			if(inputButton) {
				stateSound = pressedOnS;
			}
			else {
				stateSound = waitS;
			}
			break;
		default:
			stateSound = startS;
			break;
	}
	switch(stateSound) {
		case waitS:
			soundOutputs = 0x00;
			break;
		case pressedOnS:
			soundOutputs = SOUNDPIN;
			break;
		case pressedOffS:
			soundOutputs = 0x00;
			break;
		default:
			soundOutputs = 0x00;
			break;
	}
}

void CombineLED(){
	//OUTPUT
	switch(stateOutput) {
		case start:
		stateOutput = output;
		break;
		case output:
		break;
		default:
		stateOutput = start;
		break;
	}
	switch(stateOutput) {
		case output:
		outputLEDs = threeLEDs | blinkingLEDs | soundOutputs;
		PORTB = outputLEDs;
		break;
		default:
		break;
	}
}