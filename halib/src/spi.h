/*!	\file halib/spi.h
 *	\brief	Definiert SPI-Schnittstellen-Klasse
 *
 *	... indem es die Definition für die passende Plattform included.
 *
 *	\see halib/atmega32/spi.h
 */

#pragma once

#include "config.h"
#include "queuebuffer.h"
#include "cdevice.h"


// Include Template Implementation
#if defined (__AVR_ATmega32__)
#	include "atmega32/spi.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "at90can128/spi.h"
#else
#  error "device type not defined or no spi implemented for this device"
#endif
#endif





