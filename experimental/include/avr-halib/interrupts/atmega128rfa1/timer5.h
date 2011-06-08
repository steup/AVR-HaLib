#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer5IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture=46,			/**< input capture**/
			matchA=47,	/**< compare match in unit A **/
			matchB=48,	/**< compare match in unit B **/
			matchC=49,	/**< compare match in unit C **/
			overflow=50			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer5IntMap>
{
	private:
	typedef atmega1281::Timer5IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER5_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER5_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER5_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER5_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER5_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER5_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER5_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER5_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER5_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER5_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRF(TIMER5_OVF_vect, F);
					break;
			case(IntMap::capture)       : redirectISRF(TIMER5_CAPT_vect, F);
					break;
			case(IntMap::matchA) : redirectISRF(TIMER5_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER5_COMPB_vect, F);
					break;
			case(IntMap::matchC) : redirectISRF(TIMER5_COMPC_vect, F);
					break;
		}
	}
};

}
}
