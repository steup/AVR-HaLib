/**	\file	include/halib/ext/button.h
 *	\brief	Defines class Button
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include "halib/avr/portmap.h"


/**
 *	\class Button button.h include/halib/ext/button.h
 *	\brief A single Button
 *	\param ButtonPortmap	Portmap for this Button TODO: Link to portmap docs
 *
 *	\portmapspec
 *		\portmappin{button}		pin the Button is connected to
 *		\portmapprop{pressedLevel}	true if the button is pressed on high level, false if pressed on low level
 *		\portmapprop{usePullup}		true for using pullup (normally a good idea), false for tri-state
 *
 *	\portmapexamples
 *		\portmapex
 * portmap Button0
 * {
 * 	pin button: a 4;
 * 	property pressedLevel = true;
 * 	property usePullup = true;
 * };
 *		\endportmapex
 *
 *	
 */
template <class ButtonPortmap>
class Button
{
public:
	///	Constructor
	Button()
	{
		UsePortmap(pm, ButtonPortmap);
		pm.button.ddr = false;				// configure pin as input
		pm.button.port = ButtonPortmap::usePullup;	// set pullup or not
	}

	///	Returns if the button is pressed at the moment
	inline bool isPressed()
	{
		UsePortmap(pm, ButtonPortmap);
		return pm.button.pin == ButtonPortmap::pressedLevel;
	}
};
