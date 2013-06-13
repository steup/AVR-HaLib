/** \addtogroup ext */
/*@{*/
/**
 *	\file	include/avr-halib/ext/led.h
 *	\brief	Defines class Led
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/portmap.h"


/**
 *	\class Led led.h "avr-halib/ext/led.h"
 *	\brief A single LED
 *	\param LedPortmap	Portmap for this LED. See \ref doc_portmaps for details about the portmap concept.
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
namespace avr_halib
{
namespace ext
{
template <class LedPortmap>
class Led
{
public:
	///	Constructor
	Led()
	{
		UsePortmap(pm, LedPortmap);
		pm.led.ddr = true;	// configure pin as output
		setOff();		// init led
	}

	///	Turn LED on
	void setOn()
	{
		UsePortmap(pm, LedPortmap);
		pm.led.port = LedPortmap::onLevel;
        SyncPortmap(pm);
	}
	
	///	Turn LED off
	void setOff()
	{
		UsePortmap(pm, LedPortmap);
		pm.led.port = !LedPortmap::onLevel;
        SyncPortmap(pm);
	}

	/**	\brief Set LED
	 *	\param s	LED is turned on if true, off if false
	 */
	void set(bool s)
	{
		UsePortmap(pm, LedPortmap);
		pm.led.port = (s == LedPortmap::onLevel);
        SyncPortmap(pm);
	}

	///	Toggle LED (turn on if it is off and vice verca)
	void toggle()
	{
		UsePortmap(pm, LedPortmap);
		pm.led.pin = true;
		SyncPortmap(pm);
	}

	///	Returns true if the LED is turned on
	bool isOn()
	{
		UsePortmap(pm, LedPortmap);
		SyncPortmap(pm);
		return pm.led.port == LedPortmap::onLevel;
	}
};
}
}

/*@}*/
