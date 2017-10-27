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

#define _LEFTSHIFT(A) CUBE[A+1].level = CUBE[A].level;
#define _RIGHTSHIFT(A) CUBE[A-1].level = CUBE[A].level;
#define _UPSHIFT(A) CUBE[A+16].level = CUBE[A].level;
#define _DOWNSHIFT(A) CUBE[A-16].level = CUBE[A].level;
#define _FORWARDSHIFT(A) CUBE[A-4].level = CUBE[A].level;
#define _BACKSHIFT(A) CUBE[A+4].level = CUBE[A].level;

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
volatile char overflowCounter = 0, frameCounter = 0;

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
		//CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
		CUBE[i].level = 0;
		//CUBE[i].level = (i == 8) ? 15 : 0;		//For testing
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

/******************** DEFINE ANIMATION HELPER FUNCTIONS **********************/
void LED_ON();
void LED_OFF();
void ALL_ON();
void ALL_OFF();

void LEFT_SHIFT();
void RIGHT_SHIFT();
void UP_SHIFT();
void DOWN_SHIFT();
void FORWARD_SHIFT();
void BACK_SHIFT();

void LEFT_TWIST();
void RIGHT_TWIST();

/******************************** LED_ON ********************************
* Function to turn a single LED on at a given intensity.								*
************************************************************************/
void LED_ON(int LEDNum,int brightness){
	CUBE[LEDNum].level = brightness;
}

/******************************* LED_OFF  *******************************
* Function to turn a single LED off.																		*
************************************************************************/
void LED_OFF(int LEDNum){
	CUBE[LEDNum].level = 0;
}

/******************************** ALL_ON ********************************
* Function to turn all of the LEDs on at a given intensity.							*
************************************************************************/
void ALL_ON(int brightness){
	CUBE[0].level = brightness;
	CUBE[1].level = brightness;
	CUBE[2].level = brightness;
	CUBE[3].level = brightness;
	CUBE[4].level = brightness;
	CUBE[5].level = brightness;
	CUBE[6].level = brightness;
	CUBE[7].level = brightness;
	CUBE[8].level = brightness;
	CUBE[9].level = brightness;
	CUBE[10].level = brightness;
	CUBE[11].level = brightness;
	CUBE[12].level = brightness;
	CUBE[13].level = brightness;
	CUBE[14].level = brightness;
	CUBE[15].level = brightness;
	CUBE[16].level = brightness;
	CUBE[17].level = brightness;
	CUBE[18].level = brightness;
	CUBE[19].level = brightness;
	CUBE[20].level = brightness;
	CUBE[21].level = brightness;
	CUBE[22].level = brightness;
	CUBE[23].level = brightness;
	CUBE[24].level = brightness;
	CUBE[25].level = brightness;
	CUBE[26].level = brightness;
	CUBE[27].level = brightness;
	CUBE[28].level = brightness;
	CUBE[29].level = brightness;
	CUBE[30].level = brightness;
	CUBE[31].level = brightness;
	CUBE[32].level = brightness;
	CUBE[33].level = brightness;
	CUBE[34].level = brightness;
	CUBE[35].level = brightness;
	CUBE[36].level = brightness;
	CUBE[37].level = brightness;
	CUBE[38].level = brightness;
	CUBE[39].level = brightness;
	CUBE[40].level = brightness;
	CUBE[41].level = brightness;
	CUBE[42].level = brightness;
	CUBE[43].level = brightness;
	CUBE[44].level = brightness;
	CUBE[45].level = brightness;
	CUBE[46].level = brightness;
	CUBE[47].level = brightness;
	CUBE[48].level = brightness;
	CUBE[49].level = brightness;
	CUBE[50].level = brightness;
	CUBE[51].level = brightness;
	CUBE[52].level = brightness;
	CUBE[53].level = brightness;
	CUBE[54].level = brightness;
	CUBE[55].level = brightness;
	CUBE[56].level = brightness;
	CUBE[57].level = brightness;
	CUBE[58].level = brightness;
	CUBE[59].level = brightness;
	CUBE[60].level = brightness;
	CUBE[61].level = brightness;
	CUBE[62].level = brightness;
	CUBE[63].level = brightness;
}

/******************************* ALL_OFF  *******************************
* Function to turn all of the LEDs off.																	*
************************************************************************/
void ALL_OFF(){
	CUBE[0].level = 0;
	CUBE[1].level = 0;
	CUBE[2].level = 0;
	CUBE[3].level = 0;
	CUBE[4].level = 0;
	CUBE[5].level = 0;
	CUBE[6].level = 0;
	CUBE[7].level = 0;
	CUBE[8].level = 0;
	CUBE[9].level = 0;
	CUBE[10].level = 0;
	CUBE[11].level = 0;
	CUBE[12].level = 0;
	CUBE[13].level = 0;
	CUBE[14].level = 0;
	CUBE[15].level = 0;
	CUBE[16].level = 0;
	CUBE[17].level = 0;
	CUBE[18].level = 0;
	CUBE[19].level = 0;
	CUBE[20].level = 0;
	CUBE[21].level = 0;
	CUBE[22].level = 0;
	CUBE[23].level = 0;
	CUBE[24].level = 0;
	CUBE[25].level = 0;
	CUBE[26].level = 0;
	CUBE[27].level = 0;
	CUBE[28].level = 0;
	CUBE[29].level = 0;
	CUBE[30].level = 0;
	CUBE[31].level = 0;
	CUBE[32].level = 0;
	CUBE[33].level = 0;
	CUBE[34].level = 0;
	CUBE[35].level = 0;
	CUBE[36].level = 0;
	CUBE[37].level = 0;
	CUBE[38].level = 0;
	CUBE[39].level = 0;
	CUBE[40].level = 0;
	CUBE[41].level = 0;
	CUBE[42].level = 0;
	CUBE[43].level = 0;
	CUBE[44].level = 0;
	CUBE[45].level = 0;
	CUBE[46].level = 0;
	CUBE[47].level = 0;
	CUBE[48].level = 0;
	CUBE[49].level = 0;
	CUBE[50].level = 0;
	CUBE[51].level = 0;
	CUBE[52].level = 0;
	CUBE[53].level = 0;
	CUBE[54].level = 0;
	CUBE[55].level = 0;
	CUBE[56].level = 0;
	CUBE[57].level = 0;
	CUBE[58].level = 0;
	CUBE[59].level = 0;
	CUBE[60].level = 0;
	CUBE[61].level = 0;
	CUBE[62].level = 0;
	CUBE[63].level = 0;
}

/****************************** LEFT_SHIFT ******************************
* Function to shift current frame one pixel to the left leaving right		*
* most plane blank.																											*
************************************************************************/
void LEFT_SHIFT(){
	//Second plane from left
	_LEFTSHIFT(2);
	_LEFTSHIFT(6);
	_LEFTSHIFT(10);
	_LEFTSHIFT(14);
	_LEFTSHIFT(18);
	_LEFTSHIFT(22);
	_LEFTSHIFT(26);
	_LEFTSHIFT(30);
	_LEFTSHIFT(34);
	_LEFTSHIFT(38);
	_LEFTSHIFT(42);
	_LEFTSHIFT(46);
	_LEFTSHIFT(50);
	_LEFTSHIFT(54);
	_LEFTSHIFT(58);
	_LEFTSHIFT(62);
	
	//Next plane from left
	_LEFTSHIFT(1);
	_LEFTSHIFT(5);
	_LEFTSHIFT(9);
	_LEFTSHIFT(13);
	_LEFTSHIFT(17);
	_LEFTSHIFT(21);
	_LEFTSHIFT(25);
	_LEFTSHIFT(29);
	_LEFTSHIFT(33);
	_LEFTSHIFT(37);
	_LEFTSHIFT(41);
	_LEFTSHIFT(45);
	_LEFTSHIFT(49);
	_LEFTSHIFT(53);
	_LEFTSHIFT(57);
	_LEFTSHIFT(61);
	
	//Far right plane
	_LEFTSHIFT(0);
	_LEFTSHIFT(4);
	_LEFTSHIFT(8);
	_LEFTSHIFT(12);
	_LEFTSHIFT(16);
	_LEFTSHIFT(20);
	_LEFTSHIFT(24);
	_LEFTSHIFT(28);
	_LEFTSHIFT(32);
	_LEFTSHIFT(36);
	_LEFTSHIFT(40);
	_LEFTSHIFT(44);
	_LEFTSHIFT(48);
	_LEFTSHIFT(52);
	_LEFTSHIFT(56);
	_LEFTSHIFT(60);
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

/***************************** RIGHT_SHIFT  *****************************
* Function to shift current frame one pixel to the right leaving left		*
* most plane blank.																											*
************************************************************************/
void RIGHT_SHIFT(){
	
	//Second plane from right
	_RIGHTSHIFT(1);
	_RIGHTSHIFT(5);
	_RIGHTSHIFT(9);
	_RIGHTSHIFT(13);
	_RIGHTSHIFT(17);
	_RIGHTSHIFT(21);
	_RIGHTSHIFT(25);
	_RIGHTSHIFT(29);
	_RIGHTSHIFT(33);
	_RIGHTSHIFT(37);
	_RIGHTSHIFT(41);
	_RIGHTSHIFT(45);
	_RIGHTSHIFT(49);
	_RIGHTSHIFT(53);
	_RIGHTSHIFT(57);
	_RIGHTSHIFT(61);
	
	//Next plane from right
	_RIGHTSHIFT(2);
	_RIGHTSHIFT(6);
	_RIGHTSHIFT(10);
	_RIGHTSHIFT(14);
	_RIGHTSHIFT(18);
	_RIGHTSHIFT(22);
	_RIGHTSHIFT(26);
	_RIGHTSHIFT(30);
	_RIGHTSHIFT(34);
	_RIGHTSHIFT(38);
	_RIGHTSHIFT(42);
	_RIGHTSHIFT(46);
	_RIGHTSHIFT(50);
	_RIGHTSHIFT(54);
	_RIGHTSHIFT(58);
	_RIGHTSHIFT(62);
	
	//Far left plane
	_RIGHTSHIFT(3);
	_RIGHTSHIFT(7);
	_RIGHTSHIFT(11);
	_RIGHTSHIFT(15);
	_RIGHTSHIFT(19);
	_RIGHTSHIFT(23);
	_RIGHTSHIFT(27);
	_RIGHTSHIFT(31);
	_RIGHTSHIFT(35);
	_RIGHTSHIFT(39);
	_RIGHTSHIFT(43);
	_RIGHTSHIFT(47);
	_RIGHTSHIFT(51);
	_RIGHTSHIFT(55);
	_RIGHTSHIFT(59);
	_RIGHTSHIFT(63);
	CUBE[3].level = 0;
	CUBE[7].level = 0;
	CUBE[11].level = 0;
	CUBE[15].level = 0;
	CUBE[19].level = 0;
	CUBE[23].level = 0;
	CUBE[27].level = 0;
	CUBE[31].level = 0;
	CUBE[35].level = 0;
	CUBE[39].level = 0;
	CUBE[43].level = 0;
	CUBE[47].level = 0;
	CUBE[51].level = 0;
	CUBE[55].level = 0;
	CUBE[59].level = 0;
	CUBE[63].level = 0;
}

/******************************* UP_SHIFT *******************************
* Function to shift current frame up one pixel leaving the bottom most	*
* plane blank.																													*
************************************************************************/
void UP_SHIFT(){
	
	//Second plane from top
	_UPSHIFT(32);
	_UPSHIFT(33);
	_UPSHIFT(34);
	_UPSHIFT(35);
	_UPSHIFT(36);
	_UPSHIFT(37);
	_UPSHIFT(38);
	_UPSHIFT(39);
	_UPSHIFT(40);
	_UPSHIFT(41);
	_UPSHIFT(42);
	_UPSHIFT(43);
	_UPSHIFT(44);
	_UPSHIFT(45);
	_UPSHIFT(46);
	_UPSHIFT(47);
	
	//Next plane from top
	_UPSHIFT(16);
	_UPSHIFT(17);
	_UPSHIFT(18);
	_UPSHIFT(19);
	_UPSHIFT(20);
	_UPSHIFT(21);
	_UPSHIFT(22);
	_UPSHIFT(23);
	_UPSHIFT(24);
	_UPSHIFT(25);
	_UPSHIFT(26);
	_UPSHIFT(27);
	_UPSHIFT(28);
	_UPSHIFT(29);
	_UPSHIFT(30);
	_UPSHIFT(31);
	
	//Bottom plane
	_UPSHIFT(0);
	_UPSHIFT(1);
	_UPSHIFT(2);
	_UPSHIFT(3);
	_UPSHIFT(4);
	_UPSHIFT(5);
	_UPSHIFT(6);
	_UPSHIFT(7);
	_UPSHIFT(8);
	_UPSHIFT(9);
	_UPSHIFT(10);
	_UPSHIFT(11);
	_UPSHIFT(12);
	_UPSHIFT(13);
	_UPSHIFT(14);
	_UPSHIFT(15);
	CUBE[0].level = 0;
	CUBE[1].level = 0;
	CUBE[2].level = 0;
	CUBE[3].level = 0;
	CUBE[4].level = 0;
	CUBE[5].level = 0;
	CUBE[6].level = 0;
	CUBE[7].level = 0;
	CUBE[8].level = 0;
	CUBE[9].level = 0;
	CUBE[10].level = 0;
	CUBE[11].level = 0;
	CUBE[12].level = 0;
	CUBE[13].level = 0;
	CUBE[14].level = 0;
	CUBE[15].level = 0;
}

/****************************** DOWN_SHIFT ******************************
* Function to shift current frame down one pixel leaving the top most  	*
* plane blank.																													*
************************************************************************/
void DOWN_SHIFT(){
	
	//Second plane from bottom
	_DOWNSHIFT(16);
	_DOWNSHIFT(17);
	_DOWNSHIFT(18);
	_DOWNSHIFT(19);
	_DOWNSHIFT(20);
	_DOWNSHIFT(21);
	_DOWNSHIFT(22);
	_DOWNSHIFT(23);
	_DOWNSHIFT(24);
	_DOWNSHIFT(25);
	_DOWNSHIFT(26);
	_DOWNSHIFT(27);
	_DOWNSHIFT(28);
	_DOWNSHIFT(29);
	_DOWNSHIFT(30);
	_DOWNSHIFT(31);
	
	//Next plane from bottom
	_DOWNSHIFT(32);
	_DOWNSHIFT(33);
	_DOWNSHIFT(34);
	_DOWNSHIFT(35);
	_DOWNSHIFT(36);
	_DOWNSHIFT(37);
	_DOWNSHIFT(38);
	_DOWNSHIFT(39);
	_DOWNSHIFT(40);
	_DOWNSHIFT(41);
	_DOWNSHIFT(42);
	_DOWNSHIFT(43);
	_DOWNSHIFT(44);
	_DOWNSHIFT(45);
	_DOWNSHIFT(46);
	_DOWNSHIFT(47);
	
	//top plane
	_DOWNSHIFT(48);
	_DOWNSHIFT(49);
	_DOWNSHIFT(50);
	_DOWNSHIFT(51);
	_DOWNSHIFT(52);
	_DOWNSHIFT(53);
	_DOWNSHIFT(54);
	_DOWNSHIFT(55);
	_DOWNSHIFT(56);
	_DOWNSHIFT(57);
	_DOWNSHIFT(58);
	_DOWNSHIFT(59);
	_DOWNSHIFT(60);
	_DOWNSHIFT(61);
	_DOWNSHIFT(62);
	_DOWNSHIFT(63);
	CUBE[48].level = 0;
	CUBE[49].level = 0;
	CUBE[50].level = 0;
	CUBE[51].level = 0;
	CUBE[52].level = 0;
	CUBE[53].level = 0;
	CUBE[54].level = 0;
	CUBE[55].level = 0;
	CUBE[56].level = 0;
	CUBE[57].level = 0;
	CUBE[58].level = 0;
	CUBE[59].level = 0;
	CUBE[60].level = 0;
	CUBE[61].level = 0;
	CUBE[62].level = 0;
	CUBE[63].level = 0;
}

/**************************** FORWARD_SHIFT  ****************************
* Function to shift current frame forward one pixel leaving the bottom	*
* most plane blank.																											*
************************************************************************/
void FORWARD_SHIFT(){
	
	//Second plane from front
	_FORWARDSHIFT(4);
	_FORWARDSHIFT(5);
	_FORWARDSHIFT(6);
	_FORWARDSHIFT(7);
	_FORWARDSHIFT(20);
	_FORWARDSHIFT(21);
	_FORWARDSHIFT(22);
	_FORWARDSHIFT(23);
	_FORWARDSHIFT(36);
	_FORWARDSHIFT(37);
	_FORWARDSHIFT(38);
	_FORWARDSHIFT(39);
	_FORWARDSHIFT(52);
	_FORWARDSHIFT(53);
	_FORWARDSHIFT(54);
	_FORWARDSHIFT(55);
	
	//Next plane from front
	_FORWARDSHIFT(8);
	_FORWARDSHIFT(9);
	_FORWARDSHIFT(10);
	_FORWARDSHIFT(11);
	_FORWARDSHIFT(24);
	_FORWARDSHIFT(25);
	_FORWARDSHIFT(26);
	_FORWARDSHIFT(27);
	_FORWARDSHIFT(40);
	_FORWARDSHIFT(41);
	_FORWARDSHIFT(42);
	_FORWARDSHIFT(43);
	_FORWARDSHIFT(56);
	_FORWARDSHIFT(57);
	_FORWARDSHIFT(58);
	_FORWARDSHIFT(59);
	
	//back plane
	_FORWARDSHIFT(12);
	_FORWARDSHIFT(13);
	_FORWARDSHIFT(14);
	_FORWARDSHIFT(15);
	_FORWARDSHIFT(28);
	_FORWARDSHIFT(29);
	_FORWARDSHIFT(30);
	_FORWARDSHIFT(31);
	_FORWARDSHIFT(44);
	_FORWARDSHIFT(45);
	_FORWARDSHIFT(46);
	_FORWARDSHIFT(47);
	_FORWARDSHIFT(60);
	_FORWARDSHIFT(61);
	_FORWARDSHIFT(62);
	_FORWARDSHIFT(63);
	CUBE[12].level = 0;
	CUBE[13].level = 0;
	CUBE[14].level = 0;
	CUBE[15].level = 0;
	CUBE[28].level = 0;
	CUBE[29].level = 0;
	CUBE[30].level = 0;
	CUBE[31].level = 0;
	CUBE[44].level = 0;
	CUBE[45].level = 0;
	CUBE[46].level = 0;
	CUBE[47].level = 0;
	CUBE[60].level = 0;
	CUBE[61].level = 0;
	CUBE[62].level = 0;
	CUBE[63].level = 0;
}

/****************************** BACK_SHIFT ******************************
* Function to shift current frame back one pixel leaving the top most 	*
* plane blank.																													*
************************************************************************/
void BACK_SHIFT(){
	
	//Second plane from back
	_BACKSHIFT(8);
	_BACKSHIFT(9);
	_BACKSHIFT(10);
	_BACKSHIFT(11);
	_BACKSHIFT(24);
	_BACKSHIFT(25);
	_BACKSHIFT(26);
	_BACKSHIFT(27);
	_BACKSHIFT(40);
	_BACKSHIFT(41);
	_BACKSHIFT(42);
	_BACKSHIFT(43);
	_BACKSHIFT(56);
	_BACKSHIFT(57);
	_BACKSHIFT(58);
	_BACKSHIFT(59);
	
	//Next plane from back
	_BACKSHIFT(4);
	_BACKSHIFT(5);
	_BACKSHIFT(6);
	_BACKSHIFT(7);
	_BACKSHIFT(20);
	_BACKSHIFT(21);
	_BACKSHIFT(22);
	_BACKSHIFT(23);
	_BACKSHIFT(36);
	_BACKSHIFT(37);
	_BACKSHIFT(38);
	_BACKSHIFT(39);
	_BACKSHIFT(52);
	_BACKSHIFT(53);
	_BACKSHIFT(54);
	_BACKSHIFT(55);
	
	//front plane
	_BACKSHIFT(0);
	_BACKSHIFT(1);
	_BACKSHIFT(2);
	_BACKSHIFT(3);
	_BACKSHIFT(16);
	_BACKSHIFT(17);
	_BACKSHIFT(18);
	_BACKSHIFT(19);
	_BACKSHIFT(32);
	_BACKSHIFT(33);
	_BACKSHIFT(34);
	_BACKSHIFT(35);
	_BACKSHIFT(48);
	_BACKSHIFT(49);
	_BACKSHIFT(50);
	_BACKSHIFT(51);
	CUBE[0].level = 0;
	CUBE[1].level = 0;
	CUBE[2].level = 0;
	CUBE[3].level = 0;
	CUBE[16].level = 0;
	CUBE[17].level = 0;
	CUBE[18].level = 0;
	CUBE[19].level = 0;
	CUBE[32].level = 0;
	CUBE[33].level = 0;
	CUBE[34].level = 0;
	CUBE[35].level = 0;
	CUBE[48].level = 0;
	CUBE[49].level = 0;
	CUBE[50].level = 0;
	CUBE[51].level = 0;
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
			//for (int i =0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
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
			//for (int i =0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
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
			//for (int i =0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
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
			//for (int i=0; i<64; i++) CUBE[i].level = (i%3 == 0) ? 15 : 0;		//For testing
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
			blankPixelCount = 0;
			while (currentLED->level == 0){
				currentLED = currentLED->next;
				
				blankPixelCount++;
				if (blankPixelCount > 63){
					DISABLE_TIMER0_COMPARE_A();
					DISABLE_TIMER0_COMPARE_B();
					PORTD = 0;
					PORTC = 0;
					DDRD  = 0b11111;
					DDRC  = 0b1111;
					break;
				}
			}
			
			if(blankPixelCount<64){
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
			
			nextPixel = 0;
		}
		
		if (overflowCounter == FRAME_TIME){
			overflowCounter = 0;
			frameCounter++;
			//SHIFT_TEST(frameCounter);
			//FADE_TEST(frameCounter);
			DRAW_X(0,15);
			
		}
	}
}
