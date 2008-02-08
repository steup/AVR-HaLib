/*!	\file halib/sensor.cpp
*	\brief	Implementiert AnalogSensor und DigitalSensor
*
*	... indem es die Implementierungen für die passende Plattform included.
*
*	\see halib/atmega32/sensor.cpp
*/


#include "halib/sensor.h"


#if defined (__AVR_ATmega32__)
#	include "./atmega32/sensor.cpp"
#else
#ifdef __AVR_AT90CAN128__
#	include "./at90can128/sensor.cpp"
#else
#	error "device type not defined or no sensor implemented for this device"
#endif
#endif