#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer3IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture=31,			/**< input capture **/
			unitA=32,	/**< compare match in unit A **/
			unitB=33,	/**< compare match in unit B **/
			unitC=34,	/**< compare match in unit C **/
			overflow=35			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer3IntMap>
{
	private:
	typedef atmega1281::Timer3IntMap IntMap;
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
			case(IntMap::unitA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::unitC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
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
			case(IntMap::unitA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::unitC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
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
			case(IntMap::unitA) : redirectISRF(TIMER3_COMPA_vect, F);
					break;
			case(IntMap::unitB) : redirectISRF(TIMER3_COMPB_vect, F);
					break;
			case(IntMap::unitC) : redirectISRF(TIMER3_COMPC_vect, F);
					break;
		}
	}
};

}
}
