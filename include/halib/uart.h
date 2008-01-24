/*!	
 * 	\file halib/uart.h
 *	\brief	Definiert Uart-Schnittstellen-Klasse
 *
 *	... indem es die Definition für die passende Plattform included.
 *
 *	\see halib/atmega32/uart.h
 */


#pragma once

#include "halib/config.h"
#include "halib/queuebuffer.h"
#include "halib/cdevice.h"


// Include Template Implementation
#if defined (__AVR_ATmega32__)
#	include "halib/atmega32/uart.h"
#else
#ifdef __AVR_AT90CAN128__
#	include "halib/at90can128/uart.h"
#else
#  error "device type not defined or no uart implemented for this device"
#endif
#endif

