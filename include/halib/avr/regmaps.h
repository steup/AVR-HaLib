/**
 *	\file	include/halib/avr/regmaps.h
 *	\brief	Includes current device register maps
 *
 */

#pragma once

#define UseRegmap(var,map) map &var=(*(map*)0x0)

#define UseRegmapVolatile(var,map) volatile map &var=(*(map*)0x0)

#if defined(__AVR_AT90CAN128__)
#	include "halib/avr/regmaps/at90can128.h"
#elif defined(__AVR_ATMEGA32__)
#	include "halib/avr/regmaps/atmega32.h"
#else
#	error "Library not ported to this platform yet."
#endif
