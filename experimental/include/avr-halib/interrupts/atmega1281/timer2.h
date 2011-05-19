#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer2IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			unitA=13,		/**< compare match in unit A **/
			unitB=14,		/**< compare match in unit B **/
			overflow=15		/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer2IntMap>
{
	private:
	typedef atmega1281::Timer2IntMap IntMap;
	typedef IntMap::Interrupts Int;
	
	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::unitA) 	: redirectISRM(TIMER2_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) 	: redirectISRM(TIMER2_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::unitA) 	: redirectISRM(TIMER2_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) 	: redirectISRM(TIMER2_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRF(TIMER2_OVF_vect, F);
					break;
			case(IntMap::unitA) 	: redirectISRF(TIMER2_COMPA_vect, F);
					break;
			case(IntMap::unitB) 	: redirectISRF(TIMER2_COMPB_vect, F);
					break;
		}
	}
};

}
}
