/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Link Demo: 
 */
#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
 TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum THREELEDS {StartThree, FirstLed, SecondLed, ThirdLed} ThreeLEDs;
enum BLINKLEDS {StartBlink, FourthLed, Blink} BlinkLEDs;
enum COMBINELEDS {StartCombine, Combine} CombineLEDs;
unsigned char threeled = 0x00;
unsigned char blinkled = 0x00;

void TickThreeLeds(){
	switch(ThreeLEDs){
		case StartThree:
			ThreeLEDs = FirstLed;
			break;

		case FirstLed:
			ThreeLEDs = SecondLed;
			break;

		case SecondLed:
			ThreeLEDs = ThirdLed;
			break;
	}
	switch(ThreeLEDs){
		case StartThree:
			break;

		case FirstLed:
			threeled = 0x01;
			break;

		case SecongLed:
			threeled = 0x02;
			break;

		case ThirdLed:
			threeled = 0x04;
			break;
	}
}

void TickBlinkLeds(){
	switch(BlinkLEDs){
		case StartBlink:
			BlinkLEDs = Blink;
			break;

		case Blink:
			BlinkLEDs = FourthLed;
			break;

		case FourthLed:
			BlinkLEDs = Blink;
			break;
	}
	switch(BlinkLEDs){
		case StartBlink:
			blinkled = 0x00;
			break;

		case Blink:
			blinkled = 0x00;
			break;

		case FourthLed:
			blinkled = 0x08;
			break;
	}
}

void TickCombineLeds(){
	switch(CombineLEDs){
		case StartCombine:
			CombineLEDs = Combine;
			break;

		case Combine:
			CombineLEDs = Combine;
			break;
	}
	switch(CombineLEDs){
		case StartCombine:
			break;

		case Combine:
			PORTB = blinkled | threeled;
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF;    PORTB =0x00;
    TimerSet(1000);
    TimerOn();
    ThreeLEDs = StartThree;
    BlinkLEDs = StartBlink;
    CombineLEDs = StartCombine;
    /* Insert your solution below */
    while (1) {
    TickThreeLeds();
    TickBlinkLeds();
    TickCombineLeds();
    while(!TimerFlag) {};
    TimerFlag = 0;
    }
    return 1;
}
