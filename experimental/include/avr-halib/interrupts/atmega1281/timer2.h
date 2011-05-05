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
			compareMatchA_Int=13,	/**< compare match in unit A **/
			compareMatchB_Int=14,	/**< compare match in unit B **/
			overflow_Int=15			/**< timer overflow **/
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
	static void setInt(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER2_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER2_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER2_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER2_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void setInt()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER2_OVF_vect, F);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRF(TIMER2_COMPA_vect, F);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRF(TIMER2_COMPB_vect, F);
					break;
		}
	}
};

}
}
