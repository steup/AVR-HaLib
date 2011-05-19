#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer1IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture=16,		/**< input capture**/
			unitA=17,		/**< compare match in unit A **/
			unitB=18,		/**< compare match in unit B **/
			unitC=19,		/**< compare match in unit C **/
			overflow=20		/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer1IntMap>
{
	private:
	typedef atmega1281::Timer1IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER1_OVF_vect, F, obj);
					break;
			case(IntMap::capture)   : redirectISRM(TIMER1_CAPT_vect, F, obj);
					break;
			case(IntMap::unitA) 	: redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) 	: redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::unitC) 	: redirectISRM(TIMER1_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRM(TIMER1_OVF_vect, F, obj);
					break;
			case(IntMap::capture)   : redirectISRM(TIMER1_CAPT_vect, F, obj);
					break;
			case(IntMap::unitA) 	: redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::unitB) 	: redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::unitC) 	: redirectISRM(TIMER1_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)  : redirectISRF(TIMER1_OVF_vect, F);
					break;
			case(IntMap::capture)   : redirectISRF(TIMER1_CAPT_vect, F);
					break;
			case(IntMap::unitA) 	: redirectISRF(TIMER1_COMPA_vect, F);
					break;
			case(IntMap::unitB) 	: redirectISRF(TIMER1_COMPB_vect, F);
					break;
			case(IntMap::unitC) 	: redirectISRF(TIMER1_COMPC_vect, F);
					break;
		}
	}
};

}
}
