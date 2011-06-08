#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer0IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA=16,		/**< compare match in unit A **/
			overflow=17		/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<at90can128::Timer0IntMap>
{
	private:
	typedef at90can128::Timer0IntMap IntMap;
	typedef IntMap::Interrupts Int;
	
	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) 	: redirectISRM(TIMER0_COMP_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) 	: redirectISRM(TIMER0_COMP_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRF(TIMER0_OVF_vect, F);
					break;
			case(IntMap::matchA) 	: redirectISRF(TIMER0_COMP_vect, F);
					break;
		}
	}
};

}
}
