/**	\file	include/avr-halib/avr/digitalout.h
 *	\brief	Defines class DigitalOut
 *	
 *	\author	Philipp Werner
 *
 *	\example digitalio.h
 */

#pragma once

#include "avr-halib/avr/portmap.h"

#include <stdint.h>

/**
 *	\class DigitalOut digitalout.h "avr-halib/avr/digitalout.h"
 *	\brief A digital output
 *	\param DigitalOutPortmap	Portmap for this DigitalOut. See \ref doc_portmaps.
 *
 *	\portmapspec
 *		\portmapvport{outPort}	Virtual port containing up to 8 pins that should be used for output
 *		\portmapprop{initValue}	One byte bit pattern that controls whether the DigitalOut pins are
 *					initialized on (associated bit 1) or off (associated bit 0)
 *		\portmapprop{invertLevel}	One byte bit pattern that controls which pins show inverted behaviour.
 *						For this bits <i>On</i> means <i>low level</i> and <i>Off</i> means <i>high level</i>.
 *						Without inversion <i>Off</i> means <i>low level</i> and <i>On</i> means <i>high level</i>.
 *
 *	\portmapexamples
 *		\portmapex
 * portmap SensorPowerSupply
 * {
 * 	pins op0: c 6-7;
 * 	pin op1: d 0;
 *	vport outPort { op0, op1 };
 * 	property initValue = 0x7;	// Init to on
 * 	property invertLevel = 0x7;	// on = low level
 * };
 *		\endportmapex
 */
template <class DigitalOutPortmap>
class DigitalOut
{
public:
	///	Constructor
	DigitalOut()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setDdr(0xff);				// configure pin as output
		pm.outPort.setPort(pm.initValue ^ pm.invertLevel);	// init vport
	}

	///	Set all pins to high level (independend of property invertLevel)
	void setHigh()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(0xff);
	}
	
	///	Set all pins to low level (independend of property invertLevel)
	void setLow()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(0);
	}

	///	Set all pins (on)
	void setOn()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(0xff ^ pm.invertLevel);
	}
	
	///	Reset all pins (off)
	void setOff()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(0 ^ pm.invertLevel);
	}

	/**	\brief Set pins output levels
	 *	\param s	Bit pattern that controls new pins status; if a bit is 0 the associated pin is turned off, on otherwise
	 */
	void set(uint8_t s)
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(s ^ pm.invertLevel);
	}

	///	Toggle level of all pins
	void toggle()
	{
		UsePortmap(pm, DigitalOutPortmap);
		pm.outPort.setPort(~pm.outPort.getPort());
	}

	///	Returns the pins bit pattern
	uint8_t get()
	{
		UsePortmap(pm, DigitalOutPortmap);
		return pm.outPort.getPort() ^ pm.invertLevel;
	}
};
