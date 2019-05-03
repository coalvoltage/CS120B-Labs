#define NOTE_B0  31.0
#define NOTE_C1  33.0
#define NOTE_CS1 35.0
#define NOTE_D1  37.0
#define NOTE_DS1 39.0
#define NOTE_E1  41.0
#define NOTE_F1  44.0
#define NOTE_FS1 46.0
#define NOTE_G1  49.0
#define NOTE_GS1 52.0
#define NOTE_A1  55.0
#define NOTE_AS1 58.0
#define NOTE_B1  62.0
#define NOTE_C2  65.0
#define NOTE_CS2 69.0
#define NOTE_D2  73.0
#define NOTE_DS2 78.0
#define NOTE_E2  82.0
#define NOTE_F2  87.0
#define NOTE_FS2 93.0
#define NOTE_G2  98.0
#define NOTE_GS2 104.0
#define NOTE_A2  110.0
#define NOTE_AS2 117.0
#define NOTE_B2  123.0
#define NOTE_C3  131.0
#define NOTE_CS3 139.0
#define NOTE_D3  147.0
#define NOTE_DS3 156.0
#define NOTE_E3  165.0
#define NOTE_F3  175.0
#define NOTE_FS3 185.0
#define NOTE_G3  196.0
#define NOTE_GS3 208.0
#define NOTE_A3  220.0
#define NOTE_AS3 233.0
#define NOTE_B3  247.0
#define NOTE_C4  262.0
#define NOTE_CS4 277.0
#define NOTE_D4  294.0
#define NOTE_DS4 311.0
#define NOTE_E4  330.0
#define NOTE_F4  349.0
#define NOTE_FS4 370.0
#define NOTE_G4  392.0
#define NOTE_GS4 415.0
#define NOTE_A4  440.0
#define NOTE_AS4 466.0
#define NOTE_B4  494.0
#define NOTE_C5  523.0
#define NOTE_CS5 554.0
#define NOTE_D5  587.0
#define NOTE_DS5 622.0
#define NOTE_E5  659.0
#define NOTE_F5  698.0
#define NOTE_FS5 740.0
#define NOTE_G5  784.0
#define NOTE_GS5 831.0
#define NOTE_A5  880.0
#define NOTE_AS5 932.0
#define NOTE_B5  988.0
#define NOTE_C6  1047.0
#define NOTE_CS6 1109.0
#define NOTE_D6  1175.0
#define NOTE_DS6 1245.0
#define NOTE_E6  1319.0
#define NOTE_F6  1397.0
#define NOTE_FS6 1480.0
#define NOTE_G6  1568.0
#define NOTE_GS6 1661.0
#define NOTE_A6  1760.0
#define NOTE_AS6 1865.0
#define NOTE_B6  1976.0
#define NOTE_C7  2093.0
#define NOTE_CS7 2217.0
#define NOTE_D7  2349.0
#define NOTE_DS7 2489.0
#define NOTE_E7  2637.0
#define NOTE_F7  2794.0
#define NOTE_FS7 2960.0
#define NOTE_G7  3136.0
#define NOTE_GS7 3322.0
#define NOTE_A7  3520.0
#define NOTE_AS7 3729.0
#define NOTE_B7  3951.0
#define NOTE_C8  4186.0
#define NOTE_CS8 4435.0
#define NOTE_D8  4699.0
#define NOTE_DS8 4978.0

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
const unsigned char MAXINDEX = 78;
//Mario main theme melody
double melody[78] = {
	NOTE_E7, NOTE_E7, 0, NOTE_E7,
	0, NOTE_C7, NOTE_E7, 0,
	NOTE_G7, 0, 0,  0,
	NOTE_G6, 0, 0, 0,

	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,

	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0,

	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,

	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[78] = {
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,

	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,

	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,

	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,

	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
};


enum States {start, init, wait, play} state;
unsigned char tempA = 0x00;
unsigned char button1 = 0x00;
unsigned short count = 0;
const unsigned short PERIOD = 200;
unsigned char noteIndex;
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
	//Transition
	switch(state) {
		case start:
		state = init;
		break;
		
		case init:
		state = wait;
		break;
		
		case wait:
		if(button1){
			state = play;
		}
		break;
		
		case play:
		if(!button1 && noteIndex >= MAXINDEX - 1) {
			state = wait;
		}
		break;
		
		default:
		break;
	}
	//Action
	switch(state) {
		case init:
		noteIndex = 0;
		count = 0;
		break;
		
		case wait:
		noteIndex = 0;
		freqOut = 0;
		count = 0;
		break;
		
		case play:
		if(tempo[noteIndex] >= count) {
			if(noteIndex < MAXINDEX - 1) {
				noteIndex++;
				count = 0;
			}
		}
		freqOut = melody[noteIndex];
		count++;
		break;
		
		default:
		break;
	}
	set_PWM(freqOut);
}



