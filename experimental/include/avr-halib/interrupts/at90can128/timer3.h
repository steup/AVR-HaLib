#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer3IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture_Int=11,			/**< input capture **/
			compareMatchA_Int=12,	/**< compare match in unit A **/
			compareMatchB_Int=13,	/**< compare match in unit B **/
			compareMatchC_Int=14,	/**< compare match in unit C **/
			overflow_Int=15			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<at90can128::Timer3IntMap>
{
	private:
	typedef at90can128::Timer3IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER3_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER3_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER3_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER3_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER3_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER3_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER3_OVF_vect, F);
					break;
			case(IntMap::capture_Int)       : redirectISRF(TIMER3_CAPT_vect, F);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRF(TIMER3_COMPA_vect, F);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRF(TIMER3_COMPB_vect, F);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRF(TIMER3_COMPC_vect, F);
					break;
		}
	}
};

}
}
