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
 *		Have a look at the Makefile.
 */

#include "avr-halib/avr/digitalin.h"
#include "avr-halib/avr/digitalout.h"
#include "digitalio_portmap.h"

int main()
{
	DigitalIn<Input> in;
	DigitalOut<Output> out;

	while (1)
		out.set(in.get());
	
	return 0;
}



