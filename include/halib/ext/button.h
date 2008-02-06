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
 *		\portmappin{Button}		pin the Button is connected to
 *		\portmapprop{pressedLevel}	true if the button is pressed on high level, false if pressed on low level
 *		\portmapprop{usePullup}		true for using pullup (normally a good idea), false for tri-state
 *
 *	\portmapexamples
 *		\portmapex
 * portmap Button0
 * {
 * 	Button: a 4;
 * 	property pressedLevel = true;
 * 	property usePullups = true;
 * };
 *		\endportmapex
 *
 *	
 */
template <class ButtonPortmap>
class Button
{
public:
#define pm PORTMAP_INSTANCE(ButtonPortmap)

	///	Constructor
	Button()
	{
		pm.ddrButton = false;				// configure pin as input
		pm.portButton = ButtonPortmap::usePullup;	// set pullup or not
	}

	///	Returns if the button is pressed at the moment
	inline bool isPressed()
	{
		return pm.pinButton == ButtonPortmap::pressedLevel;
	}
#undef pm
};
