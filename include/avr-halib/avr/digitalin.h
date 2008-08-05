/**	\file	include/avr-halib/avr/digitalin.h
 *	\brief	Defines class DigitalIn
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include "avr-halib/avr/portmap.h"


/**
 *	\class DigitalIn digitalin.h "avr-halib/avr/digitalin.h"
 *	\brief A digital input
 *	\param DigitalInPortmap	Portmap for this DigitalIn. See \ref doc_portmaps.
 *
 *	\portmapspec
 *		\portmapvport{inPort}		Virtual port containing up to 8 pins that should be used for input
 *		\portmapprop{invertLevels}	One byte bit pattern that controls which bits should be inverted
 *		\portmapprop{usePullup}		One byte bit pattern that controls whether a pullup should be used for input pins,
 *						otherwise tri-state
 *
 *	\portmapexamples
 *		\portmapex
 * portmap DigiInPort
 * {
 *	pin pin0: b 5;
 *	pins pin12: b 2-3;
 *	pin pin3: d 1;
 *	vport inPort
 *	{
 *		pin0,	// Input level in least significant bit (0)
 *		pin12,	// Input level in bit 1 and 2
 *		pin3	// Input level in bit 3
 *	};
 * 	property invertLevels = 0x00;	// No inversion
 * 	property usePullup = 0xff;	// Pullups
 * };
 *		\endportmapex
 *
 */
template <class DigitalInPortmap>
class DigitalIn
{
public:
	///	Constructor
	DigitalIn()
	{
		UsePortmap(pm, DigitalInPortmap);
		pm.inPort.setDdr(0x00);			// configure pin as input
		pm.inPort.setPort(pm.usePullup);	// set pullup or not
	}

	/**	\brief	Returns the input
	 *	\returns	Bit pattern in which a bit is 1 if the associated level is high (bits may be inverted, as configured)
	 */
	uint8_t get()
	{
		UsePortmap(pm, ButtonBlockPortmap);
		return pm.buttons.getPin() ^ pm.invertLevel;
	}


};
