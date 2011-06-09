#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega128rfa1
{
	struct Timer4IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture=41,			/**< input capture**/
			matchA=42,	/**< compare match in unit A **/
			matchB=43,	/**< compare match in unit B **/
			matchC=44,	/**< compare match in unit C **/
			overflow=45			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega128rfa1::Timer4IntMap>
{
	private:
	typedef atmega128rfa1::Timer4IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER4_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER4_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER4_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER4_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER4_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER4_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER4_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER4_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER4_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER4_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRF(TIMER4_OVF_vect, F);
					break;
			case(IntMap::capture)       : redirectISRF(TIMER4_CAPT_vect, F);
					break;
			case(IntMap::matchA) : redirectISRF(TIMER4_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER4_COMPB_vect, F);
					break;
			case(IntMap::matchC) : redirectISRF(TIMER4_COMPC_vect, F);
					break;
		}
	}
};

}
}
