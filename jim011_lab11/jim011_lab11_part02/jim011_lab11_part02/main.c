#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <io.c>
#include <timer.h>
#include <keypad.h>
#include <stdio.h>

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

void LCD_CustomStringPrint(unsigned char stringSize, const char stringData[]) {
	LCD_ClearScreen();
	unsigned char c = 1;
	unsigned char d = 0;
	while(c < stringSize) {
		LCD_Cursor(c++);
		LCD_WriteData(stringData[d]);
		d++;
	}
};

char tempArray[16] = " ";

void moveArray(char arrayMessage[], unsigned char index, unsigned char outputSize, unsigned char maxSizeMessage) {
	
	for(unsigned char i = 0; i < outputSize; i++) {
        tempArray[i] = ' ';
	}
	unsigned char tempPosition = outputSize - 1;
	if(index < outputSize){
		for(unsigned char i = 0; i < index; i++) {
			tempArray[tempPosition - (index - 1 - i)] = arrayMessage[i];
		}
	}
	/*else if(index >= maxSizeMessage) {
		unsigned char difIndex = index - maxSizeMessage;

		for(unsigned char i = 0; i < (outputSize - difIndex); i++) {
            tempArray[outputSize - i - 1] = arrayMessage[maxSizeMessage -1 -i];
		}
	}*/
	else {

		for(unsigned char i = 0; i < outputSize; i++) {
			tempArray[outputSize - i - 1] = arrayMessage[index - i];
		}
	}
}
//--------End find GCD function ----------------------------------------------

//global varibles
char message[37] = "CS120B is Legend... wait for it DARY!";
unsigned short messageSize = 37;
const unsigned char MAX_LCD_SIZE = 16;

unsigned char count;
const unsigned char countMAX = 53;
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

// --------END User defined FSMs-----------------------------------------------
enum DISStates {DISStart, DISReset, DISDisplay};
	
unsigned char DISTick(unsigned char state) {
	switch(state) {
		case DISStart:
			state = DISReset;
			break;
		case DISReset:
			state = DISDisplay;
			break;
		case DISDisplay:
			if(count >= countMAX){
				state = DISReset;
			}
			break;
		default:
			state = DISStart;
			break;
	}
	switch(state) {
		case DISReset:
		count = 0;
		case DISDisplay:
		moveArray(message,count,MAX_LCD_SIZE,messageSize);
		LCD_CustomStringPrint(MAX_LCD_SIZE,tempArray);
		count++;
		break;
		
		default:
		break;
	}
	return state;
}

// Implement scheduler code from PES.
int main(){
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// . . . etc

	// Period for the tasks
	unsigned long int SMTick1_calc = 200;
	unsigned long int GCD = SMTick1_calc;
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;

	//Declare an array of tasks 
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &DISTick;//Function pointer for the tick.

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


