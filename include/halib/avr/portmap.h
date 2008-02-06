
#pragma once

#if defined (__AVR_AT90CAN128__)
#	define __FIRSTPIN_ADRESS 0x20
//#elif defined (__AVR_ATmega32__)
//#	define __PINA_ADRESS 
#else
#	error "device type not implementet in include/halib/avr/portmap.h"
#endif


/**
 *	\brief Macro for accessing pins and virtual ports of portmaps
 *	
 *	Use this macro if you are developing a class for a peripheral device
 *	using a portmap. This macro will return an instance of the portmap you
 *	can use to access pins and virtual ports.
 *
 *	\attention Do _not_ create an instance of a portmap! Use this macro to access the instance.
 */
#define PORTMAP_INSTANCE(STRUCT) (*(STRUCT *)(__FIRSTPIN_ADRESS))
