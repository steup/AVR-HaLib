/**	\file	include/halib/avr/digitalout.h
 *	\brief	Defines class DigitalOut
 *	
 *	\author	Philipp Werner
 */

#pragma once

#include "halib/avr/portmap.h"


/**
 *	\class DigitalOut digitalout.h "halib/avr/digitalout.h"
 *	\brief A digital output
 *	\param DigitalOutPortmap	Portmap for this DigitalOut. \see doc_portmaps
 *
 *	\portmapspec
 *		\portmapvport{outPort}	Virtual port containing up to 8 pins that should be used for output
 *		\portmapprop{initValue}	One byte bit pattern that controls whether the DigitalOut pin is initialized
 *					to high (associated bit 1) or low level (associated bit 0)
 *
 *	\portmapexamples
 *		\portmapex
 * portmap SensorPowerSupply
 * {
 *	vport outPort
 *	{
 * 		pins op0: c 6-7;
 * 		pin op1: d 0;
 *	};
 * 	property initValue = 0x7;
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
		pm.outPort.ddr = 0xff;			// configure pin as output
		pm.outPort.port = pm::initValue;	// init vport
	}

	///	Set all pins to high level
	void setHigh()
	{
		UsePortmap(pm, LedPortmap);
		pm.outPort.setPort(0xff);
	}
	
	///	Set all pins to low level
	void setLow()
	{
		UsePortmap(pm, LedPortmap);
		pm.outPort.setPort(0);
	}

	/**	\brief Set pins output levels
	 *	\param s	Bit pattern that controls new pins status; if a bit is 0 the associated pin is turned low, high otherwise
	 */
	void set(uint8_t s)
	{
		UsePortmap(pm, LedPortmap);
		pm.outPort.setPort(s);
	}

	///	Toggle level of all pins
	void toggle()
	{
		UsePortmap(pm, LedPortmap);
		pm.outPort.setPort(~pm.outPort.getPort());
	}

	///	Returns the pins bit pattern
	uint8_t get()
	{
		UsePortmap(pm, LedBlockPortmap);
		return pm.outPort.getPort();	// ledsPort eq onLevel
	}
};