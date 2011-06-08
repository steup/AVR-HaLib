#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer0IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA=21,	/**< compare match in unit A **/
			matchB=22,	/**< compare match in unit B **/
			overflowt=23			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer0IntMap>
{
	private:
	typedef atmega1281::Timer0IntMap IntMap;
	typedef IntMap::Interrupts Int;
	
	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER0_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER0_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER0_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER0_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRF(TIMER0_OVF_vect, F);
					break;
			case(IntMap::matchA) : redirectISRF(TIMER0_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER0_COMPB_vect, F);
					break;
		}
	}
};

}
}
