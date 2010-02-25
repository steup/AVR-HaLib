/**
 *	\file	examples/applications/digitalio.cpp
 *	\brief	Example for use of DigitalIn, DigitalOut and for writing portmap definition in place.
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

/*	This file contains an in place portmap definition
 *
 *	How to compile?
 *		Have a look at the Makefile (rule "digitalio.o : digitalio.cpp")
 */

#include "avr-halib/avr/digitalin.h"
#include "avr-halib/avr/digitalout.h"


#ifdef __AVR_AT90CAN128__
portmap Input for at90can128
{
	pins ipins : a 4-7;
	vport inPort { ipins };
	property usePullup = 0xf;
	property invertLevel = 0x5;	// invert two pins
};

portmap Output for at90can128
{
	pins opins : a 0-3;
	vport outPort { opins };
	property initValue = 0x0;
	property invertLevel = 0x0;
};
#else
#	error "Portmap missing"
#endif

int main()
{
	DigitalIn<Input> in;
	DigitalOut<Output> out;

	while (1)
		out.set(in.get());
	
	return 0;
}



