/*!	\file halib/motor.cpp
*	\brief	Implementiert Motor-Klasse
*
*	... indem es die Implementierung für die passende Plattform included.
*
*	\see halib/atmega32/motor.cpp
*/

#include "halib/motor.h"


#if defined (__AVR_ATmega32__)
#	include "./atmega32/motor.cpp"
#else
#ifdef __AVR_AT90CAN128__
#	include "./at90can128/motor.cpp"
#else
#	error "device type not defined or no motor implemented for this device"
#endif
#endif
