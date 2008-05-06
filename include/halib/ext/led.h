/**	\file	include/halib/ext/led.h
 *	\brief	Defines class Led
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include "halib/avr/portmap.h"


/**
 *	\class Led led.h "halib/ext/led.h"
 *	\brief A single LED
 *	\param LedPortmap	Portmap for this LED TODO: Link to portmap docs
 *
 *	\portmapspec
 *		\portmappin{led}	Pin the LED is connected to
 *		\portmapprop{onLevel}	true if LED is active on high level, false if active on low level
 *
 *	\portmapexamples
 *		\portmapex
 * portmap Led0
 * {
 * 	pin led: a 0;
 * 	property onLevel = true;
 * };
 *		\endportmapex
 */
template <class LedPortmap>
class Led
{
public:
#define pm PORTMAP_INSTANCE(LedPortmap)

	///	Constructor
	Led()
	{
		pm.led.ddr = true;	// configure pin as output
		setOff();		// init led
	}

	///	Turn LED on
	inline void setOn()
	{
		pm.led.port = LedPortmap::onLevel;
	}
	
	///	Turn LED off
	inline void setOff()
	{
		pm.led.port = !LedPortmap::onLevel;
	}

	/**	\brief Set LED
	 *	\param s	LED is turned on if true, off if false
	 */
	inline void set(bool s)
	{
		pm.led.port = (s == LedPortmap::onLevel);
	}

	///	Toggle LED (turn on if it is off and vice verca)
	inline void toggle()
	{
		pm.led.port = !pm.led.port;
	}

	///	Returns true if the LED is turned on
	inline bool isOn()
	{
		return pm.led.port == LedPortmap::onLevel;
	}
#undef pm
};
