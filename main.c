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
//#include <string.h>
#include "cube_init.h"
#include "AVRTimer.h"

/****************************** DECLARE MACROS  ******************************/

#define PIXEL_TIME 2048	//pixel time in program cycles
#define FRAME_TIME 28	//number of timer1 overflows per frame

/***************************** DEFINE STRUCTURE  *****************************/

/***************************** struct _LED  *****************************
* Function to initialize the cube. Will dedicate memory and define      *
* pointers to enable the cascade of LEDs                                *
* Parameters:                                                           *
*     int level						intensity of led: 0-15 (4 bits)						  	*
*     int anode						pin number of anode: 0-9 (4 bits)							*
*     int cathode					pin number of cathode: 0-9 (4 bits)						*
*     struct _LED *next		pointer to next led in cube										*
************************************************************************/
struct _LED {
	int level : 4;
	int anode : 4;
	int cathode : 4;
	struct _LED *next;
};

/************************* DECLARE GLOBAL VARIABLES  *************************/

struct _LED CUBE[64];			//array of _LEDs to define cube

struct _LED *currentLED;	//pointer to current LED

//volatile uint8_t pointIndex = 0;
volatile char nextPixel = 0;  //ISR helpers
volatile char overflowCounter = 0;

/********************* DEFINE INTERRUPT SERVICE ROUTINES *********************/

/************************** TIMER1_COMPA_vect  **************************
* TIMER1 Output Compare A Match Interrupt Enable                        *
* Parameters:                                                           *
*		                                                                    *
************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	nextPixel = 1;
}

/************************** TIMER1_COMPB_vect  **************************
* TIMER1 Output Compare B Match Interrupt Enable                        *
* Parameters:                                                           *
*		                                                                    *
************************************************************************/
ISR(TIMER1_COMPB_vect)
{
	PORTD = 0;
	PORTC = 0;
}

//////////////////////////////////////////////////////////////////////////
// TODO: handle interrupt A and B occur simultaneously
//////////////////////////////////////////////////////////////////////////

/*************************** TIMER1_OVF_vect  ***************************
* TIMER1 overflow interrupt service routine                             *
* Parameters:                                                           *
*		                                                                    *
************************************************************************/
ISR(TIMER1_OVF_vect)
{
	overflowCounter++;
	//CUBE[pointIndex].level = 0;
	//pointIndex = (pointIndex<63) ? pointIndex+1 : 0;
	//CUBE[pointIndex].level = 15;
}


/************************** DEFINE HELPER FUNCTIONS **************************/
void INITIALIZE_CUBE();
void LEFT_SHIFT();
void RIGHT_SHIFT();

/*************************** INITIALIZE_CUBE  ***************************
* Function to initialize the cube. Will dedicate memory and define      *
* pointers to enable the cascade of LEDs                                *
* Parameters:                                                           *
*		                                                                    *
************************************************************************/
void INITIALIZE_CUBE(){
	CLKPR = _BV(CLKPCE);
	CLKPR = 0;
	
	for (int i =0; i<64; i++) {
		CUBE[i].level = (i == 8) ? 15 : 0;
		CUBE[i].anode = init[i*2];			//cathode and anode pin defined in header file
		CUBE[i].cathode = init[i*2+1];
		if(i != 63){
			CUBE[i].next = &CUBE[i+1];
			} else {
			CUBE[i].next = &CUBE[0];
		}
	}
	currentLED = &CUBE[0];
}

/****************************** LEFT_SHIFT ******************************
* Function to initialize the cube. Will dedicate memory and define      *
* pointers to enable the cascade of LEDs                                *
* Parameters:                                                           *
*		                                                                    *
*		NOT CURRENTLY WORKING                                                                    
************************************************************************/
void LEFT_SHIFT(){
	//Far left plane
	CUBE[3].level = CUBE[2].level;
	CUBE[7].level = CUBE[6].level;
	CUBE[11].level = CUBE[10].level;
	CUBE[15].level = CUBE[14].level;
	CUBE[19].level = CUBE[18].level;
	CUBE[23].level = CUBE[22].level;
	CUBE[27].level = CUBE[26].level;
	CUBE[31].level = CUBE[30].level;
	CUBE[35].level = CUBE[34].level;
	CUBE[39].level = CUBE[38].level;
	CUBE[43].level = CUBE[42].level;
	CUBE[47].level = CUBE[46].level;
	CUBE[51].level = CUBE[50].level;
	CUBE[55].level = CUBE[54].level;
	CUBE[59].level = CUBE[58].level;
	CUBE[63].level = CUBE[62].level;
	
	//Next plane from left
	CUBE[2].level = CUBE[1].level;
	CUBE[6].level = CUBE[5].level;
	CUBE[10].level = CUBE[9].level;
	CUBE[14].level = CUBE[13].level;
	CUBE[18].level = CUBE[17].level;
	CUBE[22].level = CUBE[21].level;
	CUBE[26].level = CUBE[25].level;
	CUBE[30].level = CUBE[29].level;
	CUBE[34].level = CUBE[33].level;
	CUBE[38].level = CUBE[37].level;
	CUBE[42].level = CUBE[41].level;
	CUBE[46].level = CUBE[45].level;
	CUBE[50].level = CUBE[49].level;
	CUBE[54].level = CUBE[53].level;
	CUBE[58].level = CUBE[57].level;
	CUBE[62].level = CUBE[61].level;
	
	//Next plane from left
	CUBE[1].level = CUBE[0].level;
	CUBE[5].level = CUBE[4].level;
	CUBE[9].level = CUBE[8].level;
	CUBE[13].level = CUBE[12].level;
	CUBE[17].level = CUBE[16].level;
	CUBE[21].level = CUBE[20].level;
	CUBE[25].level = CUBE[24].level;
	CUBE[29].level = CUBE[28].level;
	CUBE[33].level = CUBE[32].level;
	CUBE[37].level = CUBE[36].level;
	CUBE[41].level = CUBE[40].level;
	CUBE[45].level = CUBE[44].level;
	CUBE[49].level = CUBE[48].level;
	CUBE[53].level = CUBE[52].level;
	CUBE[57].level = CUBE[56].level;
	CUBE[61].level = CUBE[60].level;
	
	//Far right plane
	CUBE[0].level = 0;
	CUBE[4].level = 0;
	CUBE[8].level = 0;
	CUBE[12].level = 0;
	CUBE[16].level = 0;
	CUBE[20].level = 0;
	CUBE[24].level = 0;
	CUBE[28].level = 0;
	CUBE[32].level = 0;
	CUBE[36].level = 0;
	CUBE[40].level = 0;
	CUBE[44].level = 0;
	CUBE[48].level = 0;
	CUBE[52].level = 0;
	CUBE[56].level = 0;
	CUBE[60].level = 0;
}


/******************************** MAIN FUNCTION ******************************/

/****************************** ASSIGN_PIN ******************************
* Function to assign state of a pin.                                    *
* Parameters:                                                           *
*		ledPin: The pin number to be changed, refer to pin assignments at   *
*						top of page                                                 *
*    value:	Indicated the state to change the given pin to              *
*            0  - pin is an output LOW (0V)                             *
*            1  - pin is an output HIGH (5V)                            *
*            99 - pin in an input (HIGH-Z)                              *
************************************************************************/
int main (){	
	uint16_t anode,cathode,inOut;		//pixel select helpers
	uint16_t currentTime;						//Current time for compare ISR setting
	uint16_t blankPixelCount = 0;		//Current time for compare ISR setting
	
	INITIALIZE_CUBE();
	
	//initialize timer1
	SET_TIMER1_PRESCALER(1);
	ENABLE_TIMER1_COMPARE_A();
	ENABLE_TIMER1_COMPARE_B();
	ENABLE_TIMER1_OVERFLOW();
	SET_TIMER1_VALUE(0);
	
	sei();											// global interrupt enable
	
	SET_TIMER1_OUTPUT_COMPARE_A(PIXEL_TIME);
	
	
	while(1)
	{
		if (nextPixel > 0){
			DDRD = 0;
			DDRC = 0;
			currentLED = currentLED->next;
			while (currentLED->level == 0){
				currentLED = currentLED->next;
				
				blankPixelCount++;
				if (blankPixelCount > 63){
					DISABLE_TIMER0_COMPARE_A();
					DISABLE_TIMER0_COMPARE_B();
					blankPixelCount = 0;
					PORTD = 0;
					PORTC = 0;
					DDRD  = 0b11111;
					DDRC  = 0b1111;
					break;
				}
			}
			nextPixel = 0;
			
			anode = 1<<(currentLED->anode - 1);
			cathode = 1<<(currentLED->cathode - 1);
			PORTD = anode & 0b11111;
			PORTC = (anode>>5) & 0b1111;
			inOut = anode | cathode;
			DDRD = inOut & 0b11111;
			DDRC = (inOut>>5) & 0b1111;
			currentTime = GET_TIMER1_VALUE();
			SET_TIMER1_OUTPUT_COMPARE_B(0xffff & (currentTime + ((currentLED->level)+1)*PIXEL_TIME / 16 ));
			SET_TIMER1_OUTPUT_COMPARE_A(0xffff & (currentTime + PIXEL_TIME - 1));
		}
		
		if (overflowCounter > FRAME_TIME){
			CUBE[8].level = (CUBE[8].level == 0) ? 15 : 0;
			overflowCounter = 0;
			//LEFT_SHIFT();
			//ENABLE_TIMER1_COMPARE_A();
			//ENABLE_TIMER1_COMPARE_B();
			//if(CUBE[3].level != 0){
				//for (int i=0 ; i<64 ; i=i+4){
					//CUBE[3].level = 15;
				//}
			//}
		}
	}
}

/*Fade Effect
 *
 if (overflowCounter > FRAME_TIME){
	 if (CUBE[0].level  < 15){
		 for (int i=0;i<64;i++){
			 CUBE[i].level++;
		 }
	 }
	 else{
		 for (int i=0;i<64;i++){
			 CUBE[i].level = 1;
		 }
	 }

	 overflowCounter = 0;
 }
 */
