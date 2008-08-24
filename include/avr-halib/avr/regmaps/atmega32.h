/** \addtogroup atmega32 */
/*@{*/
/**
 *	\file	include/avr-halib/avr/regmaps/atmega32.h
 *	\brief	Contains atmega32 specific stuff
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/interrupt.h"

#include <stdint.h>

/**
 *	\brief	Namespace containing atmega32 specific stuff
 *
 *	Content in global namespace as well if compiling for atmega32.
 */
namespace atmega32
{

template <class Controller_Configuration> struct ADConv
{
private:
	uint8_t __base[0x24] ;//:8 ; // :0x78*8 __attribute__ ((vector_size (1024))); //__base[0x78];// 	uint8_t :0x78*8;
public:
	union
	{
		struct
		{
			uint8_t adcl:8;
			uint8_t adch:8;
		};
		struct
		{
			uint16_t adc:16;
		};
	};
	
	uint8_t adps : 3;
	bool	adie : 1;
	bool	adif : 1;
	bool	adate: 1;
	bool	adsc : 1;
	bool	aden : 1;
	union
	{
		uint8_t  admux	:8;
		struct{
			uint8_t  mux	:5;
			bool	 adlra	:1;
			uint8_t  refs	:2;
		};
	};
	
	enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7,ps_none };
	enum {ref_aref = 0, ref_avcc = 1, ref_internal2_56 = 3};
	
	enum
	{	
		__recommendedPrescaler = Controller_Configuration::controllerClk/200000UL,
		recommendedPrescalar = __recommendedPrescaler > 64? (ps128) : __recommendedPrescaler > 32? (ps64) :__recommendedPrescaler > (ps32)? 5:__recommendedPrescaler > 8? (ps16):__recommendedPrescaler > 4? (ps8):__recommendedPrescaler > 2? (ps4):(ps2)
	};
	
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
		static void setADCInterrupt(T & obj)
	{
		redirectISRM(SIG_ADC, Fxn, obj);
	}
	
	
};

}

#if defined(__AVR_AT90CAN128__)
using namespace at90can128;
#endif

/*@}*/
