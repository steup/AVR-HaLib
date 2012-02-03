/**
 *	\file	examples/applications/buttonblock.cpp
 *	\brief	Example for usage of ButtonBlock and LedBlock (very simple)
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "avr-halib/ext/buttonblock.h"
#include "avr-halib/ext/ledblock.h"

#include "avr-halib/portmaps/robbyboard.h"

int main()
{
	ButtonBlock<ButtonBlock0123> buttons;
	LedBlock<LedBlock0123> leds;

	while (1)
	{
		leds.set(buttons.get());
	}
}
