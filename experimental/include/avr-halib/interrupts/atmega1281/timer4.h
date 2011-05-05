#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer4IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture_Int=41,			/**< input capture**/
			compareMatchA_Int=42,	/**< compare match in unit A **/
			compareMatchB_Int=43,	/**< compare match in unit B **/
			compareMatchC_Int=44,	/**< compare match in unit C **/
			overflow_Int=45			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<atmega1281::Timer4IntMap>
{
	private:
	typedef atmega1281::Timer4IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER4_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER4_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER4_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER4_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER4_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER4_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER4_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER4_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER4_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER4_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void setInt()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER4_OVF_vect, F);
					break;
			case(IntMap::capture_Int)       : redirectISRF(TIMER4_CAPT_vect, F);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRF(TIMER4_COMPA_vect, F);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRF(TIMER4_COMPB_vect, F);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRF(TIMER4_COMPC_vect, F);
					break;
		}
	}
};

}
}
