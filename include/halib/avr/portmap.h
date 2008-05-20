/**
 *	\file	include/halib/avr/portmap.h
 *	\brief	Defines UsePortmap & UsePortmapVolatile macros
 *
 */




#pragma once


/**
 *	\brief Instance generation for portmaps
 *	
 *	This macro creates a reference \p var of type \p {map &} which
 *	should be used to access pins and virtual ports. Use this macro
 *	if you are developing a portable class for a peripheral device
 *	utilizing the portmap concept.
 *
 *	In Contrast to \c UsePortmapVolatile access to the instance created by \c UsePortmap
 *	will be optimized by the compiler. This will generate less code, but sometimes
 *	\c UsePortmapVolatile is necessary, for example if you do something that looks
 *	senseless for the compiler like toggling a bit multiple times without a noteworthy
 *	action in between. But this may be essential for doing something with the hardware
 *	whose registers/ports are mapped into memory. \c UsePortmapVolatile lets the compiler
 *	know that every action on this portmap has to be taken literally.
 *
 *	\attention Do _not_ create an instance of a portmap in the usual way! It won't work!
 *	\see UsePortmapVolatile
 */
#define UsePortmap(var,map) map & var = (*(map*)0x0)

/**
 *	\brief Volatile instance generation for portmaps
 *	
 *	This macro creates a reference \p var of type \p {volatile map &} which
 *	should be used to access pins and virtual ports. Use this macro
 *	if you are developing a portable class for a peripheral device
 *	utilizing the portmap concept.
 *
 *	In Contrast to \c UsePortmap access to the instance created by \c UsePortmapVolatile
 *	will not be optimized by the compiler. This will generate more code, but sometimes
 *	it's necessary, for example if you do something that looks
 *	senseless for the compiler like toggling a bit multiple times without a noteworthy
 *	action in between. But this may be essential for doing something with the hardware
 *	whose registers/ports are mapped into memory. \c UsePortmapVolatile lets the compiler
 *	know that every action on this portmap has to be taken literally.
*
 *	\attention Do _not_ create an instance of a portmap in the usual way! It won't work!
 *	\see UsePortmap
 */
#define UsePortmapVolatile(var,map) volatile map & var = (*(map*)0x0)


//make the compiler belive that var can cange makes it really volatile
#define Sync(var) Volatile(var)
#define Volatile(var) asm("":"=m" (var):);
