#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <keypad.h>
#include <stdio.h>
#include <io.c>

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
    unsigned long int c;
    while(1){
        c = a%b;
        if(c==0){return b;}
        a = b;
		b = c;
    }
    return 0;
}
//--------End find GCD function ----------------------------------------------

//global varibles
unsigned char outputKeypad = 'X';

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
    /*Tasks should have members that include: state, period,
        a measurement of elapsed time, and a function pointer.*/
    unsigned char state; //Task's current state
    unsigned long int period; //Task period
    unsigned long int elapsedTime; //Time elapsed since last task tick
    int (*TickFct)(int); //Task tick function
} task;
enum KeypadStates {KPStart, KPInit, KPWait, KPHold, KPRelease};
unsigned char TickKeypad(unsigned char state) {
	unsigned char inputKP = GetKeypadKey();
	static unsigned char tempKP;
	switch (state){
		case KPStart:
		state = KPInit;
		break;
		
		case KPInit:
		state = KPWait;
		break;
		
		case KPWait:
		if(inputKP != '\0') {
			state = KPHold;
		}
		break;
		
		case KPHold:
		if(inputKP == '\0') {
			state = KPRelease;
		}
		break;
		
		case KPRelease:
		state = KPWait;
		break;
		
		default:
		state = KPStart;
		break;
	}
	
	switch (state){
		case KPInit:
		tempKP = '0';
		break;
		
		case KPHold:
		tempKP = inputKP;
		break;
		
		case KPRelease:
		outputKeypad = tempKP;
		break;
			
		default:
		break;
	}
	return state;
}

enum LEDStates {LEDStart, LEDOutput};
unsigned char TickOutputLED(unsigned char state) {
	switch (state){
		case LEDStart:
		state = LEDOutput;
		break;
		
		case LEDOutput:
		break;
		
		default:
		state = LEDStart;
		break;
	}
	switch (state){
		case LEDOutput:
		LCD_WriteData(outputKeypad);
		LCD_Cursor(1);
		break;
		
		default:
		break;
	}
	return state;
}
// --------END User defined FSMs-----------------------------------------------


// Implement scheduler code from PES.
int main(){
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRC = 0xF0; PORTC = 0x0F;
	
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	// Period for the tasks
	unsigned long int SMTick1_calc = 50;
	unsigned long int SMTick2_calc = 50;;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;

	//Declare an array of tasks 
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &TickKeypad;//Function pointer for the tick.

	// Task 2
	task2.state = -1;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &TickOutputLED;//Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}


