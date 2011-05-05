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
			capture_Int=46,			/**< input capture**/
			compareMatchA_Int=47,	/**< compare match in unit A **/
			compareMatchB_Int=48,	/**< compare match in unit B **/
			compareMatchC_Int=49,	/**< compare match in unit C **/
			overflow_Int=50			/**< timer overflow **/
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
	static void setInt(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER5_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER5_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER5_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER5_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER5_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER5_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER5_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER5_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER5_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER5_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void setInt()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER5_OVF_vect, F);
					break;
			case(IntMap::capture_Int)       : redirectISRF(TIMER5_CAPT_vect, F);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRF(TIMER5_COMPA_vect, F);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRF(TIMER5_COMPB_vect, F);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRF(TIMER5_COMPC_vect, F);
					break;
		}
	}
};

}
}
