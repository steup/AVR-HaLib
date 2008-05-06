/**	\file	include/halib/ext/ledblock.h
 *	\brief	Defines class LedBlock
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include <stdint.h>

#include "halib/avr/portmap.h"


/**
 *	\class	LedBlock ledblock.h "halib/ext/ledblock.h"
 *	\brief	Up to 8 LED
 *	\param	LedBlockPortmap	Portmap for this LED Block (see \ref dec_portmaps )
 *
 *	\portmapspec
 *		\portmapvport{leds}	Up to 8 pins the LEDs are connected to
 *		\portmapprop{onLevel}	one byte bit pattern that controls whether the LEDs are active on high level
*					(associated bit is 1) or on low level (bit is 0)
 *
 *	\portmapexamples
 *		\portmapex
 * portmap LedBlock5231
 * {
 *	vport leds
 *	{
 *		pin Led0: a 5;		// LED controlled by least significant bit (0)
 *		pins Led23: a 2-3;	// LEDs controlled by bit 1 and 2
 *		pin Led0: a 1;		// LED controlled by bit 3
 *	};
 * 	property onLevel = 0xff;
 * };
 *		\endportmapex
 */
template <class LedBlockPortmap>
class LedBlock
{
public:
#define pm PORTMAP_INSTANCE(LedBlockPortmap)

	///	Constructor
	LedBlock()
	{
		pm.leds.setDdr(0xff);	// configure all pins as output
		setOff();		// init led
	}

	///	Turn LEDs on
	inline void setOn()
	{
 		pm.leds.setPort(LedBlockPortmap::onLevel);
	}
	
	///	Turn LEDs off
	inline void setOff()
	{
 		pm.leds.setPort(LedBlockPortmap::onLevel ^ 0xff);	// set inverted onLevel
	}

	/**	\brief Set LED
	 *	\param s	Bit pattern that controls new LEDs status; if a bit is 0 the associated LED is turned off, on otherwise
	 */
	inline void set(uint8_t s)
	{
		pm.leds.setPort(~(s ^ LedBlockPortmap::onLevel));	// set (s eq onLevel)
	}

	///	Toggle LED (turn on if it is off and vice verca)
	inline void toggle()
	{
		pm.leds.setPort(~pm.leds.getPort());
	}

	///	Returns the LED bit pattern
	inline uint8_t get()
	{
		return ~(pm.leds.getPort() ^ LedBlockPortmap::onLevel);	// ledsPort eq onLevel
	}
#undef pm
};
