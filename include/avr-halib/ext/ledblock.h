/** \addtogroup ext */
/*@{*/
/**
 *	\file	include/avr-halib/ext/ledblock.h
 *	\brief	Defines class LedBlock	
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <stdint.h>

#include "avr-halib/avr/portmap.h"


/**
 *	\class	LedBlock ledblock.h "avr-halib/ext/ledblock.h"
 *	\brief	Up to 8 LEDs
 *	\param	LedBlockPortmap	Portmap for this LED Block. See \ref doc_portmaps for details about the portmap concept.
 *
 *	\portmapspec
 *		\portmapvport{leds}	Up to 8 pins the LEDs are connected to
 *		\portmapprop{onLevel}	One byte bit pattern that controls whether the LEDs are active on high level
*					(associated bit is 1) or on low level (bit is 0)
 *
 *	\portmapexamples
 *		\portmapex
 * portmap LedBlock5231
 * {
 *	pin Led5: a 5;
 *	pins Led23: a 2-3;
 *	pin Led1: a 1;
 *	vport leds
 *	{
 *		Led5,		// LED controlled by least significant bit (0)
 *		Led23,		// LEDs controlled by bit 1 and 2
 *		Led1		// LED controlled by bit 3
 *	};
 * 	property onLevel = 0xff;
 * };
 *		\endportmapex
 */
template <class LedBlockPortmap>
class LedBlock
{
public:
	///	Constructor
	LedBlock()
	{
		UsePortmap(pm, LedBlockPortmap);
		pm.leds.setDdr(0xff);	// configure all pins as output
		setOff();		// init led
	}

	///	Turn LEDs on
	void setOn()
	{
		UsePortmap(pm, LedBlockPortmap);
 		pm.leds.setPort(LedBlockPortmap::onLevel);
	}
	
	///	Turn LEDs off
	void setOff()
	{
		UsePortmap(pm, LedBlockPortmap);
 		pm.leds.setPort(LedBlockPortmap::onLevel ^ 0xff);	// set inverted onLevel
	}

	/**	\brief Set LED
	 *	\param s	Bit pattern that controls new LEDs status; if a bit is 0 the associated LED is turned off, on otherwise
	 */
	void set(uint8_t s)
	{
		UsePortmap(pm, LedBlockPortmap);
		pm.leds.setPort(~(s ^ LedBlockPortmap::onLevel));	// set (s eq onLevel)
	}

	///	Toggle LED (turn on if it is off and vice verca)
	void toggle()
	{
		UsePortmap(pm, LedBlockPortmap);
		pm.leds.setPort(~pm.leds.getPort());
	}

	///	Returns the LED bit pattern
	uint8_t get()
	{
		UsePortmap(pm, LedBlockPortmap);
		return ~(pm.leds.getPort() ^ LedBlockPortmap::onLevel);	// ledsPort eq onLevel
	}
};

/*@}*/
