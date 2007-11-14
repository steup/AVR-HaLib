/*!	\file halib/timer.h
*	\brief	Definiert Timer-Basisklasse und spezielle Timer
*
*	... indem es die Definition für die passende Plattform included.
*
*	\see halib/atmega32/timer.h
*/


#pragma once

#include "halib/config.h"


/**
 *	\brief Abstrakte Basisklasse für alle Timer
 */
class Timer
{
protected:
	/// Abtrakte Methode, die das Timer-Event behandelt
	virtual void onTimer() = 0;
};



#if defined (__AVR_ATmega32__)
#	include "halib/atmega32/timer.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "halib/at90can128/timer.h"
#else
#	error "device type not defined or no timers defined for this device"
#endif
#endif







