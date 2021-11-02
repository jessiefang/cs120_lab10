/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #3
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
enum SPEAKER {StartSpeaker, Off, On} Speaker;
unsigned char threeled = 0x00;
unsigned char blinkled = 0x00;
unsigned char speaker = 0x00;
unsigned char tick = 0;

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
			PORTB = threeled | blinkled | speaker;
			break;
	}
}

void TickSpeaker(){
	unsigned char tmpA = ~PINA & 0x04;
	switch(Speaker){
		case StartSpeaker:
			Speaker = Off;
			break;

		case Off:
			if(tmpA){
				Speaker = On;
			}else{
				Speaker = Off;
			}
			break;

		case On:
			if(!tmpA){
				Speaker = Off;
			}else{
				Speaker = On;
			}
			break;
	}
	switch(Speaker){
		case StartSpeaker:
			break;

		case Off:
			speaker = 0x00;
			tick = 0;
			break;

		case On:
			if(tick <= 2){
				speaker = 0x10;
			}else if(tick <= 4){
				speaker = 0x00;
			}else{
				tick = 0;
			}
			tick ++;
			break;

	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;    PORTA = 0xFF;
    DDRB = 0xFF;    PORTB = 0x00;
    unsigned long Three_elapsedTime = 0;
    unsigned long Blink_elapsedTime = 0;
    const unsigned long timerPeriod = 1;
    TimerSet(1);
    TimerOn();
    ThreeLEDs = StartThree;
    BlinkLEDs = StartBlink;
    CombineLEDs = StartCombine;
    Speaker = StartSpeaker;
    /* Insert your solution below */
    while (1) {
    if (Three_elapsedTime >= 300) {
	TickThreeLeds();
	Three_elapsedTime = 0;
    }
    if (Blink_elapsedTime >= 1000) {
	TickBlinkLeds();
	Blink_elapsedTime = 0;
    }
    if (Speaker_elapsedTime >=2){
	TickSpeaker();
	Speaker_elapsedTime = 0;
    }
    TickSpeaker();
    TickCombineLeds();
    while(!TimerFlag) {};
    TimerFlag = 0;
    Three_elapsedTime += timerPeriod;
    Blink_elapsedTime += timerPeriod;
    Speaker_elapsedTime += timerPeriod;
    }
    return 1;
}
