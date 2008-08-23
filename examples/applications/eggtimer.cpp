/**
 *	\file	examples/applications/eggtimer.cpp
 *	\brief	Example for use of EggTimer and Delegate (ugly style, but showing Delegate advantages)
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#if defined(__AVR_AT90CAN128__)
#	define F_CPU 16000000UL
#	include "avr-halib/portmaps/robbyboard.h"
#elif defined(__AVR_ATMEGA32__)
#	define F_CPU 8000000UL
#	include "avr-halib/portmaps/bobbyboard.h"
#else
#	error "Example program not ported to this platform yet."
#endif

#include "avr-halib/avr/timer.h"
#include "avr-halib/ext/led.h"


// Interrupt needed for EggTimer<Timer2>
UseInterrupt(SIG_OUTPUT_COMPARE2);


class Blinker
{
public:
	EggTimer<Timer2> timer;
	Led<Led0> led;

	// Timer Event Handler 1
	void onTimer1()
	{
		// Toggle Led
		led.toggle();
		
		// Next timer event after 1s
		timer.start(10);
		
		// Set a method as timer event handler
		setDelegateMethod(timer.onTimerDelegate, Blinker, Blinker::onTimer2, *this);
	}
	
	// Timer Event Handler 2
	void onTimer2()
	{
		// Toggle Led
		led.toggle();
		
		// Next timer event after 2s
		timer.start(20);
		
		// Set a method as timer event handler (alternative way)
		timer.onTimerDelegate.fromMethod<Blinker, & Blinker::onTimer1> (this);
	}
};

Blinker b;

// Timer Event Handler 0
void onTimer0()
{
	// Set a method as timer event handler
	setDelegateMethod(b.timer.onTimerDelegate, Blinker, Blinker::onTimer1, b);

	// Next timer event after 0.5s
	b.timer.start(5);
}

int main()
{
	// Enable Interrupts
	sei();
	
	// Set a C function as timer event handler
	setDelegateFunction(b.timer.onTimerDelegate, onTimer0);
	
	// Start timer (event after 0.5s)
	b.timer.start(5);
	
	
	while(1);
	return 0;
}
