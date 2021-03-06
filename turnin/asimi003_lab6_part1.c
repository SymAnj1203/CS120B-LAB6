/*      Author: Angelica Simityan
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *     I acknowledge all content contained herein, excluding template or example
 *     code, is my own original work.
 *    Demo: https://drive.google.com/drive/folders/12kmxRGg9vPK05ZZhQIA1T9mxeqPqj4Yi?usp=sharing
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag=0;
unsigned long _avr_timer_M=1;
unsigned long _avr_timer_cntcurr=0;

void TimerOn()
{
	TCCR1B=0x0B;
	OCR1A=125;
	TIMSK1=0x02;
	TCNT1=0;
	_avr_timer_cntcurr=_avr_timer_M;
	SREG |=0x80;
}

void TimerOff()
{
	TCCR1B=0x00;
}

void TimerISR() { TimerFlag = 1;}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	
	if(_avr_timer_cntcurr==0)
	{
		TimerISR();
		_avr_timer_cntcurr=_avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M=M;
	_avr_timer_cntcurr=_avr_timer_M;
}

enum States {Begin, One, Two, Three}  State;

void TickCounter()
{
	
	switch(State)
	{
		case Begin:
			State = One;
			break;

		case One:
			State = Two;
			break;

		case Two:
			State = Three;
			break;

		case Three:
			State = One;
			break;

		default:
			State = Begin;
			break;
	}

	switch (State)
	{
		case One:
			PORTB = 0x01;
			break;

		case Two:
			PORTB = 0x02;
			break;

		case Three:
			PORTB = 0x04;
			break;
	}

}

int main(void)
{
	DDRB=0xFF; PORTB=0x00;

	State=Begin;

	TimerSet(1000);
	TimerOn();

	while (1)
	{
		TickCounterer();

		while(!TimerFlag){};

		TimerFlag=0;
	}
}
