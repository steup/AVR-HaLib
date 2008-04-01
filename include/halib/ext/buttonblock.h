/**	\file	include/halib/ext/buttonblock.h
 *	\brief	Defines class ButtonBlock
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include <stdint.h>

#include "halib/avr/portmap.h"


/**
 *	\class	ButtonBlock buttonblock.h include/halib/ext/buttonblock.h
 *	\brief	Up to 8 Buttons
 *	\param	ButtonBlockPortmap	Portmap for this ButtonBlock (see \ref dec_portmaps )
 *
 *	\portmapspec
 *		\portmapvport{Buttons}		Up to 8 pins the Buttons are connected to
 *		\portmapprop{pressedLevel}	one byte bit pattern that controls whether the Buttons are on high level when pressed
 *						(associated bit is 1) or on low level (bit is 0)
 *		\portmapprop{usePullup}		one byte bit pattern that controls whether a pullup should be used for this button
 *						(associated bit is 1, normally a good idea), otherwise tri-state
 *
 *	\portmapexamples
 *		\portmapex
 * portmap ButtonBlock5231
 * {
 *	vport Buttons
 *	{
 *		pin Button0: b 5;		// Button status in least significant bit (0)
 *		pins Buttons23: b 2-3;		// Button status in bit 1 and 2
 *		pin Button0: d 1;		// Button status in bit 3
 *	};
 * 	property pressedLevel = 0xff;
 * 	property usePullups = 0xff;
 * };
 *		\endportmapex
 *
 *	
 */
template <class ButtonBlockPortmap>
class ButtonBlock
{
public:
#define pm PORTMAP_INSTANCE(ButtonBlockPortmap)

	///	Constructor
	ButtonBlock()
	{
		pm.setButtonsDdr(0x00);					// configure pin as input
		pm.setButtonsPort(ButtonBlockPortmap::usePullup);	// set pullup or not
	}

	/**	\brief	Returns if the buttons are pressed at the moment
	 *	\returns	Bit pattern in which a bit is 1 if the associated button is pressed at the moment
	 */
	uint8_t get()
	{
		return ~(pm.getButtonsPin() ^ ButtonBlockPortmap::pressedLevel);
	}
#undef pm
};
