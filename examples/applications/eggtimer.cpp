/**
 *	\file	examples/applications/eggTimer.cpp
 *	\brief	Example for use of EggTimer and Delegate
 *	\author	Philipp Werner
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


UseInterrupt(SIG_OUTPUT_COMPARE2);


class Blinker : public EggTimer<Timer2>, Led<Led0>
{
public:
	// Timer Event Handler 1
	void onTimer1()
	{
		// Toggle Led
		toggle();
		
		// Next timer event after 1s
		startEggTimer(10);
		
		// Set a method as timer event handler
		setDelegateMethod(onEggTimerDelegate, Blinker, Blinker::onTimer2, *this);
	}
	
	// Timer Event Handler 2
	void onTimer2()
	{
		// Toggle Led
		toggle();
		
		// Next timer event after 2s
		startEggTimer(20);
		
		// Set a method as timer event handler (alternative way)
		onEggTimerDelegate.fromMethod<Blinker, & Blinker::onTimer1> (this);
	}
};

Blinker b;

// Timer Event Handler 0
void onTimer0()
{
	// Set a method as timer event handler
	setDelegateMethod(b.onEggTimerDelegate, Blinker, Blinker::onTimer1, b);

	// Next timer event after 1s
	b.startEggTimer(5);
}

int main()
{
	// Enable Interrupts
	sei();
	
	// Set a C function as timer event handler
	setDelegateFunction(b.onEggTimerDelegate, onTimer0);
	
	// Start timer (event after 0.5s)
	b.startEggTimer(5);
	
	
	while(1);
	return 0;
}
