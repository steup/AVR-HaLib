/**
 *	\file	include/halib/avr/portmap.h
 *	\brief	Defines PORTMAP_INSTANCE macro
 *
 */




#pragma once


/**
 *	\brief Macro for accessing pins and virtual ports of portmaps
 *	
 *	Use this macro if you are developing a class for a peripheral device
 *	using a portmap. This macro will return an instance of the portmap you
 *	can use to access pins and virtual ports.
 *
 *	\attention Do _not_ create an instance of a portmap! Use this macro to access the instance.
 */
#define PORTMAP_INSTANCE(STRUCT) (*(STRUCT *)(0))
