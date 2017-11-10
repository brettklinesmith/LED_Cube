/*
* Cube_V_2.c
*
* Created: 10/06/2017
* Author : Brett Smith

With significant influence from Asher Glick's charliecube:
https://aglick.com/charliecube.html

*******************************************************************************
pin assignments:
PORTC:
PC0		PC1		PC2		PC3
pin6	pin7	pin8	pin9

PORTD:
PD0		PD1		PD2		PD3		PD4
pin1	pin2	pin3	pin4	pin5
*/

/***************************** INCLUDE LIBRARIES *****************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "cube_init.h"
#include "animation_functions.h"
#include "AVRTimer.h"

/****************************** DECLARE MACROS  ******************************/

#define PIXEL_TIME 2048							//pixel max ON time in instruction cycles
#define FRAME_TIME 28								//number of timer1 overflows per frame
#define NUM_OF_ANIMATIONS 7					//number of animation sequences

/************************* DECLARE GLOBAL VARIABLES  *************************/

struct	 _LED CUBE[64];			        //array of _LEDs to define cube
struct 	 _LED *currentLED;	        //pointer to current LED

int 			currentAnimTime;					//current animation sequence running time

//ISR helpers
volatile char nextPixel 			= 0;  //move to next LED (functionally a bool)
volatile char frameStep 			= 0;  //count animation frame (functionally a bool)
volatile int frameCounter 		= 0;	//frame counter for current animation

/********************* DEFINE INTERRUPT SERVICE ROUTINES *********************/

/************************** TIMER1_COMPA_vect  **************************
* TIMER1 Output Compare A Match Interrupt. Should trigger every 				*
* PIXEL_TIME timer counts change currentLED point to next LED.					*
************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	nextPixel = 1;										//indicate ISR was triggered
}

/************************** TIMER1_COMPB_vect  **************************
* TIMER1 Output Compare B Match Interrupt. Should trigger after					*
* intensity based amount of time to allow for 4-bit dimming. Just turns *
* all outputs off and assumes the details will be handled by next LED. 	*
************************************************************************/
ISR(TIMER1_COMPB_vect)
{
	PORTD = 0;												//pins 1-5 off
	PORTC = 0;												//pins 6-9 off
}

/*************************** TIMER1_OVF_vect  ***************************
* TIMER1 overflow interrupt. Increments counter to keep track of        *
* overflows for animation timing purposes.															*
* Might move to bool type behavior and handle animation dwell time      *
* separately in each animation. Should probably do that...							*
************************************************************************/
ISR(TIMER1_OVF_vect)
{
	frameStep = 1;
}


/************************** DEFINE HELPER FUNCTIONS **************************/
void INITIALIZE_CUBE();

/*************************** INITIALIZE_CUBE  ***************************
* Function to initialize the cube. Will dedicate memory and define      *
* pointers to enable the cascade of LED animations.                     *
************************************************************************/
void INITIALIZE_CUBE(){
	CLKPR = _BV(CLKPCE);							//allow writing to set system clock prescaler
	CLKPR = 0;												//set system clock prescaler to 1
	
	for (int i =0; i<64; i++) {
		CUBE[i].level = 0;
		
		// init contains cathode and anode pin information and is defined in header file
		CUBE[i].anode = init[i*2];
		CUBE[i].cathode = init[i*2+1];
		
		if(i != 63){
			CUBE[i].next = &CUBE[i+1];
			} else {
			CUBE[i].next = &CUBE[0];			//for last LED set next to the first LED
		}
	}
	
	currentLED = &CUBE[0];						//after loop point to first LED
}

/******************************** MAIN FUNCTION ******************************/

int main (){
	uint16_t 	anode,cathode,inOut;		//pixel select helpers
	uint16_t 	currentTime;						//current time for compare ISR setting
	uint16_t 	animationNum = 0;				//current animation sequence
	char 			blankPixelCount = 0;		//number of consecutive LEDs that are off
	
	INITIALIZE_CUBE();
	
	// Initialize timer1
	SET_TIMER1_PRESCALER(1);
	ENABLE_TIMER1_COMPARE_A();
	ENABLE_TIMER1_COMPARE_B();
	ENABLE_TIMER1_OVERFLOW();
	SET_TIMER1_VALUE(0);
	
	sei();														//global interrupt enable
	
	// Give timer compare A a value so it will trigger
	SET_TIMER1_OUTPUT_COMPARE_A(PIXEL_TIME);
	
	// Main loop
	while(1)
	{
		if (nextPixel > 0){  						//if it's time to move to next LED
			DDRD = 0;											//turn off pins 1-5
			DDRC = 0;											//turn off pins 6-9
			currentLED = currentLED->next; //direct pointer to next LED address
			blankPixelCount = 0;					//reset the number of LEDs that are off
			
			// Step thought each LED in the cube looking for one where level > 0
			while (currentLED->level == 0){
				currentLED = currentLED->next; //if current LED is off move to next one
				
				blankPixelCount++;					//increment number of LED that are off
				
				// If all 64 LEDs are currently off disable compare ISRs until next frame
				if (blankPixelCount > 63){
					DISABLE_TIMER0_COMPARE_A();
					DISABLE_TIMER0_COMPARE_B();
					break;
				}
			}
			
			if(currentLED->level > 0){
				// Set anode and cathode pins as outputs, all others as inputs (high-Z)
				anode = 1<<(currentLED->anode - 1);					//place a 1 for the anode pin
				cathode = 1<<(currentLED->cathode - 1);			//place a 1 for the cathode pin
				inOut = anode | cathode;										//or anode and cathode 1s together
				DDRD = inOut & 0b11111;											//set pins 1-5 to appropriate I/O state
				DDRC = (inOut>>5) & 0b1111;									//set pins 6-9 to appropriate I/O state
				
				// Set anode pin to output HIGH and all other pins (including cathode) to LOW
				// High-Z pins are unaffected by this assignment
				PORTD = anode & 0b11111;
				PORTC = (anode>>5) & 0b1111;
				
				/*set ISR routines compare values*/
				currentTime = GET_TIMER1_VALUE();
				
				// Turn off current LED at time of (current time + PIXEL_TIME * ratio of (level)/16).
				// Integer divide could cause very small timing error that isn't a concern.
				// Intensity of 1 is not possible. 1-15 = 2-16
				SET_TIMER1_OUTPUT_COMPARE_B(0xffff & (currentTime + ((currentLED->level))*PIXEL_TIME / 16 ));
				
				// Change to next LED after PIXEL_TIME
				SET_TIMER1_OUTPUT_COMPARE_A(0xffff & (currentTime + PIXEL_TIME));
			}
			
			nextPixel = 0;								//reset bool behavior to wait for ISR
		}
		
		// When the frame is done being displayed
		if (frameStep == 1){
			switch(animationNum){
				case 0:
					if(frameCounter%5 == 0) SNAKE((frameCounter/5)%434);
					break;
				case 1:
					if(frameCounter%20 == 0) FADE_TEST((frameCounter/20)%29);
					break;
				case 2:
					LINE_RIDER(frameCounter%133);
					break;
				case 3:
					if(frameCounter%40 == 0) SHIFT_TEST((frameCounter/40)%13);
					break;
				case 4:
					if(frameCounter%4 == 0) FIREWORKS((frameCounter/4)%151);
					break;
				case 5:
					if(frameCounter%3 == 0) H_A_D_SHIFT((frameCounter/3)%113);
					break;
				case 6:
					LIGHTNING(frameCounter%282);
					break;
				case 7:
					if(frameCounter%90 == 0) SHAPES((frameCounter/90)%7);
					break;
				default:
					animationNum = 0;
					break;
			}
			frameCounter++;								//set index to next frame
			
			if (frameCounter > currentAnimTime){
				frameCounter = 0;
				animationNum++;
				if (animationNum >= NUM_OF_ANIMATIONS){
					animationNum = 0;
				}
				ALL_OFF();
			}
			
			frameStep = 0;
		}
	}
}
