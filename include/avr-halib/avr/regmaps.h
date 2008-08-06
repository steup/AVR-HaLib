/**
 *	\file	include/avr-halib/avr/regmaps.h
 *	\brief	Defines UseRegmap and SyncRegmap macros and includes current device register maps
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/memory.h"

/**
 *	\brief Instance generation for regmaps
 *	
 *	This macro creates a reference \p var of type \p {map &} which
 *	should be used to access the registers of a register map. Use this macro
 *	if you are developing a portable class for a AVR microcontroller feature
 *	utilizing the regmap concept.
 *
 *	Keep in mind that you have to use the SyncRegmap macro if you
 *	want to get input from the portmap pins.
 *
 *	\attention Do _not_ create an instance of a portmap in the usual way! It won't work!
 *	\see SyncRegmap
 */
#define UseRegmap(var,map) map & var = (*(map*)0x0)


/**
 *	\brief	Regmap synchronization point
 *
 *	Use this macro to set a point of synchronization which will be also hold with optimizations turned on.
 *	The compiler will finish all actions concerning the regmap structure that are in your code before this line.
 *	Behind this macro the compiler has no assumptions about the portmap content any more. In this way optimzations by the
 *	compiler are only done between two synchronization points and not beyond them.
 *
 *	Use it before you read registers from a regmap to indicate that values of \p var may have changed without compilers
 *	knowledge. Otherwise the compiler may optimize your code away because he does not
 *	know that regmap memory can be changed outside the operational sequence of your program. Using this synchronization
 *	method does not generate extra code, but it allows the compiler to do more optimizations than making the whole regmap
 *	volatile.
 *
 *	\see	UseRegmap
 *	\see	SyncObj
 *	\see	SyncMem
 */	
#define SyncRegmap(var) SyncObj(var)



#if defined(__AVR_AT90CAN128__)
#	include "avr-halib/avr/regmaps/at90can128.h"
#elif defined(__AVR_ATMEGA32__)
#	include "avr-halib/avr/regmaps/atmega32.h"
#elif defined(__AVR_ATMEGA128__)
#	include "avr-halib/avr/regmaps/atmega128.h"
#else
#	error "Library not ported to this platform yet."
#endif
