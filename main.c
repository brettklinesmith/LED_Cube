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

/************************* DECLARE GLOBAL VARIABLES  *************************/

struct	 _LED CUBE[64];			        //array of _LEDs to define cube
struct 	 _LED *currentLED;	        //pointer to current LED

//ISR helpers
volatile char nextPixel 			= 0;  //move to next LED (functionally a bool)         
volatile char overflowCounter = 0;	//number of overflows since last reset
volatile char frameCounter 		= 0;	//frame counter for current animation

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
* TIMER1 overflow interrupt. Incriments counter to keep track of        *
* overflows for animation timing purposes.															*
* Might move to bool type behaviour and handle animation dwell time     *
* seperately in each animation. Should probably do that...							*
************************************************************************/
ISR(TIMER1_OVF_vect)
{
	overflowCounter++;
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

/*********************** DEFINE ANIMATIONS FUNCTIONS *************************/
void SHIFT_TEST();
void FADE_TEST();
void DRAW_X();

void SHIFT_TEST(int frameNum){
	switch(frameNum){
		case 1:
			LEFT_SHIFT();
			break;
		case 2:
			LEFT_SHIFT();
			break;
		case 3:
			LEFT_SHIFT();
			break;
		case 4:
			LEFT_SHIFT();
			break;
		case 5:
			//for (int i =0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
			ALL_ON(15);
			break;
		case 6:
			RIGHT_SHIFT();
			break;
		case 7:
			RIGHT_SHIFT();
			break;
		case 8:
			RIGHT_SHIFT();
			break;
		case 9:
			RIGHT_SHIFT();
			break;
		case 10:
			//for (int i =0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
			ALL_ON(15);
			break;
		case 11:
			UP_SHIFT();
			break;
		case 12:
			UP_SHIFT();
			break;
		case 13:
			UP_SHIFT();
			break;
		case 14:
			UP_SHIFT();
			break;
		case 15:
			ALL_ON(15);
			break;
		case 16:
			DOWN_SHIFT();
			break;
		case 17:
			DOWN_SHIFT();
			break;
		case 18:
			DOWN_SHIFT();
			break;
		case 19:
			DOWN_SHIFT();
			break;
		case 20:
			ALL_ON(15);
			break;
		case 21:
			FORWARD_SHIFT();
			break;
		case 22:
			FORWARD_SHIFT();
			break;
		case 23:
			FORWARD_SHIFT();
			break;
		case 24:
			FORWARD_SHIFT();
			break;
		case 25:
			ALL_ON(15);
			break;
		case 26:
			BACK_SHIFT();
			break;
		case 27:
			BACK_SHIFT();
			break;
		case 28:
			BACK_SHIFT();
			break;
		case 29:
			BACK_SHIFT();
			break;
		default:
			frameCounter = 0;
			ALL_ON(15);
			break;
	}
}

void FADE_TEST(int frameNum){
	switch(frameNum){
		case 1:
			ALL_ON(1);
			break;
		case 2:
			ALL_ON(2);
			break;
		case 3:
			ALL_ON(3);
			break;
		case 4:
			ALL_ON(4);
			break;
		case 5:
			ALL_ON(5);
			break;
		case 6:
			ALL_ON(6);
			break;
		case 7:
			ALL_ON(7);
			break;
		case 8:
			ALL_ON(8);
			break;
		case 9:
			ALL_ON(9);
			break;
		case 10:
			ALL_ON(10);
			break;
		case 11:
			ALL_ON(11);
			break;
		case 12:
			ALL_ON(12);
			break;
		case 13:
			ALL_ON(13);
			break;
		case 14:
			ALL_ON(14);
			break;
		case 15:
			ALL_ON(15);
			break;
		case 16:
			ALL_ON(14);
			break;
		case 17:
			ALL_ON(13);
			break;
		case 18:
			ALL_ON(12);
			break;
		case 19:
			ALL_ON(11);
			break;
		case 20:
			ALL_ON(10);
			break;
		case 21:
			ALL_ON(9);
			break;
		case 22:
			ALL_ON(8);
			break;
		case 23:
			ALL_ON(7);
			break;
		case 24:
			ALL_ON(6);
			break;
		case 25:
			ALL_ON(5);
			break;
		case 26:
			ALL_ON(4);
			break;
		case 27:
			ALL_ON(3);
			break;
		case 28:
			ALL_ON(2);
			break;
		case 29:
			ALL_ON(1);
			break;
		default:
			frameCounter = 0;
			ALL_OFF();
			break;
	}
}

void DRAW_X(int offsetPlane,int brightness){
	LED_ON(0+(4*offsetPlane),brightness);
	LED_ON(3+(4*offsetPlane),brightness);
	LED_ON(17+(4*offsetPlane),brightness);
	LED_ON(18+(4*offsetPlane),brightness);
	LED_ON(33+(4*offsetPlane),brightness);
	LED_ON(34+(4*offsetPlane),brightness);
	LED_ON(48+(4*offsetPlane),brightness);
	LED_ON(51+(4*offsetPlane),brightness);
}

/******************************** MAIN FUNCTION ******************************/

int main (){
	uint16_t 	anode,cathode,inOut;		//pixel select helpers
	uint16_t 	currentTime;						//current time for compare ISR setting
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
			
			// Step throught each LED in the cube looking for one where level > 0
			while (currentLED->level == 0){		
				currentLED = currentLED->next; //if current LED is off move to next one
				
				blankPixelCount++;					//increment number of LED that are off
				
				// If all 64 LEDs are currently off disable compare ISRs until next frame
				if (blankPixelCount > 63){
					DISABLE_TIMER0_COMPARE_A();
					DISABLE_TIMER0_COMPARE_B();
					
					// Turning everything off again try this shouldn't be necessary
					PORTD = 0;
					PORTC = 0;
					DDRD  = 0b11111;
					DDRC  = 0b1111;
					break;
				}
			}
			
			// Try (currentLED->level > 0)
			if(blankPixelCount<64){
				// Set anode and cathode pins as outputs, all others as inputs (high-Z)
				anode = 1<<(currentLED->anode - 1);					//place a 1 for the anode pin
				cathode = 1<<(currentLED->cathode - 1);			//place a 1 for the cathode pin
				inOut = anode | cathode;										//or anode and cathode 1s together
				DDRD = inOut & 0b11111;											//set pins 1-5 to appropriate I/O state
				DDRC = (inOut>>5) & 0b1111;									//set pins 6-9 to appropriate I/O state
				
				// Set anode pin to output HIGH and all other pins (including cathode) to LOW
				// High-Z pins are uneffected by this assignment
				PORTD = anode & 0b11111;
				PORTC = (anode>>5) & 0b1111;
				
				/*set ISR routines compare values*/
				currentTime = GET_TIMER1_VALUE();

				/////////////////////////////////////////////////////////////////////////
				// TODO: try (level)/16) so COMP_B triggers always
				/////////////////////////////////////////////////////////////////////////
				
				// Turn off current LED at time of (current time + PIXEL_TIME * ratio of (level+1)/16).
				// Integer divide could cause very small timing error. No intensity of 1 possible only 2-16
				SET_TIMER1_OUTPUT_COMPARE_B(0xffff & (currentTime + ((currentLED->level)+1)*PIXEL_TIME / 16 ));
				
				// Change to next LED after PIXEL_TIME
				// Try removing - 1
				SET_TIMER1_OUTPUT_COMPARE_A(0xffff & (currentTime + PIXEL_TIME - 1));
			}
			
			nextPixel = 0;								//reset bool behaviour to wait for ISR
		}
		
		// When the frame is done being displayed
		if (overflowCounter == FRAME_TIME){
			overflowCounter = 0;					//reset overflow counter
			frameCounter++;								//set index to next frame
			//SHIFT_TEST(frameCounter);
			//FADE_TEST(frameCounter);
			DRAW_X(0,15);
			
		}
	}
}
