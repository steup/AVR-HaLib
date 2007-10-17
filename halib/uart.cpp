/*!
 *	\file halib/uart.cpp
 *	\brief	Implementiert Uart-Schnittstellen-Klasse
 *
 *	... indem es die Implementierung für die passende Plattform included.
 *
 *	\see halib/atmega32/uart.cpp
 */


#include "config.h"
#include "uart.h"


#if defined (__AVR_ATmega32__)
#	include "atmega32/uart.cpp"
#else
#ifdef __AVR_ATmega128__
#	include "atmega128/uart.cpp"
#else
#	error "device type not defined or no uart implemented for this device"
#endif
#endif
