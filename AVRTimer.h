/*
* AVRtimers.h
*
* Created: 10/06/2017
* Author : Brett Smith
*
#ifndef _MY_AVR_TIMER_H_
#define _MY_AVR_TIMER_H_ */

/***************************** Timer 0 Functions *****************************/

void SET_TIMER0_PRESCALER(int prescaler);
void SET_TIMER0_VALUE(uint8_t value);
uint8_t GET_TIMER0_VALUE();
void SET_TIMER0_OUTPUT_COMPARE_A(uint8_t value);
void SET_TIMER0_OUTPUT_COMPARE_B(uint8_t value);
void ENABLE_TIMER0_COMPARE_A();
void DISABLE_TIMER0_COMPARE_A();
void ENABLE_TIMER0_COMPARE_B();
void DISABLE_TIMER0_COMPARE_B();
void ENABLE_TIMER0_OVERFLOW();
void DISABLE_TIMER0_OVERFLOW();

/************************* SET_TIMER0_PRESCALER *************************
* Set the prescaler value to define how many clock cycles it takes to   *
* increment the counter. Some MCU's use higher values to indicate       *
* external clock sources. RTFM!                                         *
* Possible values are 1,8,64,256,1024																		*
************************************************************************/
void SET_TIMER0_PRESCALER(int prescaler){
	TCCR0B &= 0xF8;			//clear previous prescaler values
	
	switch(prescaler){
		case 0:			TCCR0B |= 0x00; break;	//CS0[2:0] = 000 timer is off
		case 1:			TCCR0B |= 0x01; break;	//CS0[2:0] = 001
		case 8:			TCCR0B |= 0x02; break;	//CS0[2:0] = 010
		case 64:		TCCR0B |= 0x03; break;	//CS0[2:0] = 011
		case 256:		TCCR0B |= 0x04; break;  //CS0[2:0] = 100
		case 1024:	TCCR0B |= 0x05; break;	//CS0[2:0] = 101
	}
}

/************************* TIMER0 VALUE ACCESS  *************************
* Get and set routines for the value of timer0													*
************************************************************************/
void SET_TIMER0_VALUE(uint8_t value){
	TCNT0 = value;
}

uint8_t GET_TIMER0_VALUE(){
	return TCNT0;
}

/********************** SET TIMER0 COMPARE VALUES  **********************
* Set the value that the compare flag will be triggered at for either   *
* output compare A or B for the timer																		*
************************************************************************/
void SET_TIMER0_OUTPUT_COMPARE_A(uint8_t value){
	OCR0A = value;
}

void SET_TIMER0_OUTPUT_COMPARE_B(uint8_t value){
	OCR0B = value;
}

/******************* TIMER0 COMPARE INTERUPTS CONTROL *******************
* Enable or disable either the output compare A or B interrupts					*
************************************************************************/
void ENABLE_TIMER0_COMPARE_A(){
	TIMSK0 |= (1 << OCIE0A);
}
void DISABLE_TIMER0_COMPARE_A(){
	TIMSK0 &= ~(1 << OCIE0A);
}

void ENABLE_TIMER0_COMPARE_B(){
	TIMSK0 |= (1 << OCIE0B);
}
void DISABLE_TIMER0_COMPARE_B(){
	TIMSK0 &= ~(1 << OCIE0B);
}

/*************************** SET_TIMER0_VALUE ***************************
* Enable or disable the overflow interrupt															*
************************************************************************/
void ENABLE_TIMER0_OVERFLOW(){
	TIMSK0 |= (1 << TOIE0);
}
void DISABLE_TIMER0_OVERFLOW(){
	TIMSK0 &= ~(1 << TOIE0);
}

/***************************** Timer 1 Functions *****************************/

void SET_TIMER1_PRESCALER(int prescaler);
void SET_TIMER1_VALUE(uint16_t value);
uint16_t GET_TIMER1_VALUE();
void SET_TIMER1_OUTPUT_COMPARE_A(uint16_t value);
void SET_TIMER1_OUTPUT_COMPARE_B(uint16_t value);
void ENABLE_TIMER1_COMPARE_A();
void DISABLE_TIMER1_COMPARE_A();
void ENABLE_TIMER1_COMPARE_B();
void DISABLE_TIMER1_COMPARE_B();
void ENABLE_TIMER1_OVERFLOW();
void DISABLE_TIMER1_OVERFLOW();

/************************* SET_TIMER1_PRESCALER *************************
* Set the prescaler value to define how many clock cycles it takes to   *
* increment the counter. Some MCU's use higher values to indicate       *
* external clock sources. RTFM!                                         *
* Possible values are 1,8,64,256,1024																		*
************************************************************************/
void SET_TIMER1_PRESCALER(int prescaler){
	TCCR1B &= 0xF8;			//clear previous prescaler values
	
	switch(prescaler){
		case 0:			TCCR1B |= 0x00; break;	//CS1[2:0] = 000 timer is off
		case 1:			TCCR1B |= 0x01; break;	//CS1[2:0] = 001
		case 8:			TCCR1B |= 0x02; break;	//CS1[2:0] = 010
		case 64:		TCCR1B |= 0x03; break;	//CS1[2:0] = 011
		case 256:		TCCR1B |= 0x04; break;  //CS1[2:0] = 100
		case 1024:	TCCR1B |= 0x05; break;	//CS1[2:0] = 101
	}
}

/************************* TIMER1 VALUE ACCESS  *************************
* Get and set routines for the value of TIMER1													*
************************************************************************/
void SET_TIMER1_VALUE(uint16_t value){
	TCNT1 = value;
}

uint16_t GET_TIMER1_VALUE(){
	return TCNT1;
}

/********************** SET TIMER1 COMPARE VALUES  **********************
* Set the value that the compare flag will be triggered at for either   *
* output compare A or B for the timer																		*
************************************************************************/
void SET_TIMER1_OUTPUT_COMPARE_A(uint16_t value){
	OCR1A = value;
}

void SET_TIMER1_OUTPUT_COMPARE_B(uint16_t value){
	OCR1B = value;
}

/******************* TIMER1 COMPARE INTERUPTS CONTROL *******************
* Enable or disable either the output compare A or B interrupts					*
************************************************************************/
void ENABLE_TIMER1_COMPARE_A(){
	TIMSK1 |= (1 << OCIE1A);
}
void DISABLE_TIMER1_COMPARE_A(){
	TIMSK1 &= ~(1 << OCIE1A);
}

void ENABLE_TIMER1_COMPARE_B(){
	TIMSK1 |= (1 << OCIE1B);
}
void DISABLE_TIMER1_COMPARE_B(){
	TIMSK1 &= ~(1 << OCIE1B);
}

/*************************** SET_TIMER1_VALUE ***************************
* Enable or disable the overflow interrupt															*
************************************************************************/
void ENABLE_TIMER1_OVERFLOW(){
	TIMSK1 |= (1 << TOIE1);
}
void DISABLE_TIMER1_OVERFLOW(){
	TIMSK1 &= ~(1 << TOIE1);
}
