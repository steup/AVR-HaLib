/** \addtogroup avr */
/*@{*/
/**
 *	\file	include/avr-halib/avr/portmap.h
 *	\brief	Defines #UsePortmap and #SyncPortmap macros
 *
 *	See \ref doc_portmaps for details about the portmap concept.
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */


#pragma once

#include "avr-halib/avr/memory.h"

#include <stdint.h>

/**
 *	\brief Instance generation for portmaps
 *	
 *	This macro creates a reference \p var of type <tt>map&</tt> which
 *	should be used to access pins and virtual ports. Use this macro
 *	if you are developing a portable class for a peripheral device
 *	utilizing the portmap concept.
 *
 *	Keep in mind that you have to use the #SyncPortmap macro if you
 *	want to get input from the portmap pins.
 *
 *	\attention Do _not_ create an instance of a portmap in the usual way! It won't work!
 *	\see 	\ref doc_portmaps, SyncPortmap
 */
#define UsePortmap(var,map) map & var = (*(map*)0x0)


/**
 *	\brief	Portmap synchronization point
 *
 *	Use this macro to set a point of synchronization which will be also hold with optimizations turned on.
 *	The compiler will finish all actions concerning the portmap structure that are in your code before this line.
 *	Behind this macro the compiler has no assumptions about the portmap content any more. In this way optimzations by the
 *	compiler are only done between two synchronization points and not beyond them.
 *
 *	Use it before you read pins from a portmap to indicate that values of \p var may have changed without compilers
 *	knowledge. Otherwise the compiler may optimize your code away because he does not
 *	know that portmap memory can be changed outside the operational sequence of your program. Using this synchronization
 *	method does not generate extra code, but it allows the compiler to do more optimizations than making the whole portmap
 *	volatile.
 *
 *	\see	\ref doc_portmaps, UsePortmap, SyncObj, SyncMem
 */	
#define SyncPortmap(var) SyncObj(var)

/*@}*/
