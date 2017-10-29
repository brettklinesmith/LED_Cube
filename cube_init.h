#ifndef CUBE_INIT_H
#define CUBE_INIT_H

/*
* cube_init.h
*
* Created: 10/06/2017
* Author : Brett Smith
*
*
*
**/

/****************************** DECLARE MACROS  ******************************/
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8
#define I 9

/***************************** DEFINE STRUCTURE  *****************************/

/***************************** struct _LED  *****************************
* Container to define a single LED.                              	      *
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

/***************************** PIN LOOKUP TABLE  *****************************/

// led0_a,led0_c,led1_a,led1_c,led2_a,led2_c,...
uint8_t init[128] = {
		E,F,A,F,F,B,A,B,E,G,A,G,F,C,A,C, //Bottom row
		E,H,A,H,F,D,A,D,E,I,A,I,F,E,A,E,
		G,F,B,F,G,B,C,B,F,G,B,G,G,C,B,C, //Second row
		F,H,B,H,G,D,B,D,F,I,B,I,G,E,B,E,
		H,F,C,F,H,B,D,B,H,G,C,G,H,C,D,C, //Third row
		G,H,C,H,H,D,C,D,G,I,C,I,H,E,C,E,
		I,F,D,F,I,B,E,B,I,G,D,G,I,C,E,C, //Top row
		I,H,D,H,I,D,E,D,H,I,D,I,I,E,D,E
};

/*old array declaration tha worked
left for reference against new method

#int ledArray[HEIGHT][DEPTH][WIDTH][2] = {
	{{{E,F},{A,F},{F,B},{A,B}}, //Bottom row
	{{E,G},{A,G},{F,C},{A,C}},
	{{E,H},{A,H},{F,D},{A,D}},
	{{E,I},{A,I},{F,E},{A,E}}},
	{{{G,F},{B,F},{G,B},{C,B}}, //Second row
	{{F,G},{B,G},{G,C},{B,C}},
	{{F,H},{B,H},{G,D},{B,D}},
	{{F,I},{B,I},{G,E},{B,E}}},
	{{{H,F},{C,F},{H,B},{D,B}}, //Third row
	{{H,G},{C,G},{H,C},{D,C}},
	{{G,H},{C,H},{H,D},{C,D}},
	{{G,I},{C,I},{H,E},{C,E}}},
	{{{I,F},{D,F},{I,B},{E,B}}, //Top row
	{{I,G},{D,G},{I,C},{E,C}},
	{{I,H},{D,H},{I,D},{E,D}},
	{{H,I},{D,I},{I,E},{D,E}}}
	*/
