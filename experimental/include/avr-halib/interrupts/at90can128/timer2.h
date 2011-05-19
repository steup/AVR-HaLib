#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer2IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			compareMatch_Int=9,	/**< compare match in unit A **/
			overflow_Int=10			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<at90can128::Timer2IntMap>
{
	private:
	typedef at90can128::Timer2IntMap IntMap;
	typedef IntMap::Interrupts Int;
	
	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::compareMatch_Int) : redirectISRM(TIMER2_COMP_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER2_OVF_vect, F, obj);
					break;
			case(IntMap::compareMatch_Int) : redirectISRM(TIMER2_COMP_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER2_OVF_vect, F);
					break;
			case(IntMap::compareMatch_Int) : redirectISRF(TIMER2_COMP_vect, F);
					break;
		}
	}
};

}
}
