/*!	\file avr-halib/spi.h
 *	\brief	Definiert SPI-Schnittstellen-Klasse
 *
 *	... indem es die Definition für die passende Plattform included.
 *
 *	\see avr-halib/atmega32/spi.h
 */

#pragma once

#include "avr-halib/config.h"
#include "avr-halib/queuebuffer.h"
#include "avr-halib/cdevice.h"


// Include Template Implementation
#if defined (__AVR_ATmega32__)
#	include "avr-halib/atmega32/spi.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "avr-halib/at90can128/spi.h"
#else
#  error "device type not defined or no spi implemented for this device"
#endif
#endif





