/*!	\file halib/motor.h
*	\brief	Definiert Motor-Klasse
*
*	... indem es die Definition für die passende Plattform included.
*
*	\see halib/atmega32/motor.h
*/


#pragma once

#include "config.h"



#if defined (__AVR_ATmega32__)
#	include "atmega32/motor.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "at90can128/motor.h"
#else
#  error "device type not defined or no motor implemented for this device"
#endif
#endif




