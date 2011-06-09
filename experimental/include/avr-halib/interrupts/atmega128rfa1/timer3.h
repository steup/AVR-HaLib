#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega128rfa1
{
	struct Timer3IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture=31,			/**< input capture **/
			matchA=32,	/**< compare match in unit A **/
			matchB=33,	/**< compare match in unit B **/
			matchC=34,	/**< compare match in unit C **/
			overflow=35			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega128rfa1::Timer3IntMap>
{
	private:
	typedef atmega128rfa1::Timer3IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER3_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER3_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER3_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER3_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRF(TIMER3_OVF_vect, F);
					break;
			case(IntMap::capture)       : redirectISRF(TIMER3_CAPT_vect, F);
					break;
			case(IntMap::matchA) : redirectISRF(TIMER3_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER3_COMPB_vect, F);
					break;
			case(IntMap::matchC) : redirectISRF(TIMER3_COMPC_vect, F);
					break;
		}
	}
};

}
}
