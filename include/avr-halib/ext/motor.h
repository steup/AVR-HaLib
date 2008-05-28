/*!	\file avr-halib/motor.h
*	\brief	Definiert Motor-Klasse
*
*	... indem es die Definition für die passende Plattform included.
*
*/


#pragma once

#include "avr-halib/config.h"



#if defined (__AVR_ATmega32__)
#	include "avr-halib/atmega32/motor.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "avr-halib/at90can128/motor.h"
#else
#	error "device type not defined or no motor implemented for this device"
#endif
#endif




