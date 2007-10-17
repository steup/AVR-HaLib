/*!	\file halib/spi.cpp
 *	\brief	Implementiert SPI-Schnittstellen-Klasse
 *
 *	... indem es die Implementierung für die passende Plattform included.
 *
 *	\see halib/atmega32/spi.cpp
 */


#include "config.h"
#include "spi.h"



#if defined (__AVR_ATmega32__)
#	include "atmega32/spi.cpp"
#else
#ifdef __AVR_ATmega128__
#	include "atmega128/spi.cpp"
#else
#  error "device type not defined or no spi implemented for this device"
#endif
#endif