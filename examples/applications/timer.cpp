/**
 *	\file	examples/application/timer.cpp
 *	\brief	Example illustrating usage of Timer
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





Led<Led0> led0;

void toggle()
{
	static int i = 0;
	i++;
	if (i % 10 == 0)
		led0.toggle();
}

UseInterrupt(SIG_OVERFLOW3);

int main()
{
	Timer<Timer3> timer;
	timer.selectClock(Timer3::ps64);
	
	timer.setInterruptMask(Timer3::im_overflowEnable);
	redirectISRF(SIG_OVERFLOW3, &toggle);
	
	sei();
	
	while (0);
	return 0;
}
