

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

enum States {start, init, flicker, nextPattern,gameoverPress,gameover,resetPress} state;
unsigned char tempB = 0x00;
unsigned char tempA = 0x00;
unsigned char outputIndex = 0;
unsigned short count = 0;
const unsigned short PERIOD = 100;
const unsigned short MSEC3 = 3;
unsigned char ledPattern[3] = {0x01, 0x02, 0x04};

void Tick();

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(PERIOD);
	TimerOn();
	
	while (1)
	{
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
		
	}
}

void Tick() {
	//Input
	tempA = ~PINA & 0x01;
	//Transition
	switch(state) {
		case start:
		state = init;
		break;
		
		case init:
		state = flicker;
		break;
		
		case flicker:
		if(count >= MSEC3) {
			state = nextPattern;
		}
		if (tempA == 0x01){
			state = gameoverPress;
		}
		break;
		
		case gameoverPress:
			if (tempA == 0x00){
				state = gameover;
			}
			break;
			
		case gameover:
			if (tempA == 0x01){
			state = resetPress;
			}
			break;
			
		case resetPress:
			if (tempA == 0x00){
				state = flicker;
			}
			break;
		
		case nextPattern:
		state = flicker;
		break;
		
		default:
		break;
	}
	//Action
	switch(state) {
		case start:
		break;
		
		case init:
			count = 0;
			outputIndex = 0;
			break;
		
		case flicker:
			tempB = ledPattern[outputIndex];
			count++;
			break;
		
		case nextPattern:
		if(outputIndex > 1) {
			outputIndex = 0;
		}
		else {
			outputIndex++;
		}
		count = 0;
		break;
		
		case gameoverPress:
		count = 0;
		tempB = ledPattern[outputIndex];
		break;
		
		case gameover:
		tempB = ledPattern[outputIndex];
		break;
		
		case resetPress:
		tempB = ledPattern[outputIndex];
		break;
		
		default:
		break;
	}
	PORTB = tempB;
}

