/**
 *	\file	include/avr-halib/ext/button.h
 *	\brief	Defines class Button
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/portmap.h"


/**
 *	\class Button include/avr-halib/ext/button.h avr-halib/ext/button.h
 *	\brief A single Button
 *	\param ButtonPortmap	Portmap for this Button. See \ref doc_portmaps for details about the portmap concept.
 *	\attention	Keep in mind that the button may bounce if there is hardware mechanism to avoid it.
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
 *	\example	button.cpp
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
	bool isPressed()
	{
		UsePortmap(pm, ButtonPortmap);
		SyncPortmap(pm);
		return pm.button.pin == ButtonPortmap::pressedLevel;
	}

	///	Return whether the button is pressed. If it is pressed, waits until it is released.
	bool isPressedWait()
	{
		UsePortmap(pm, ButtonPortmap);
		SyncPortmap(pm);
		bool status = pm.button.pin == ButtonPortmap::pressedLevel;
		while (pm.button.pin == ButtonPortmap::pressedLevel)
			SyncPortmap(pm);
		return status;
	}

	///	Return true if the button is pressed and was not pressed when this function was called the last time.
	bool gotPressed()
	{
		static bool lastButtonStatus = false;
		if (!lastButtonStatus)
		{
			if (isPressed())
			{
				// Button gerade gedrckt und zuletzt nicht gedrckt gewesen...
				lastButtonStatus = true;
				return true;
			}
			return false;
		}
		else
		{
			if (!isPressed())
			{
				// Button nicht mehr gedrckt aber zuletzt gedrckt gewesen...
				lastButtonStatus = false;
			}
			return false;
		}
	}
	
};
