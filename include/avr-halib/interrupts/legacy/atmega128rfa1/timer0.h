#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace atmega128rfa1
{
	struct Timer0IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA=21,	/**< compare match in unit A **/
			matchB=22,	/**< compare match in unit B **/
			overflow=23			/**< timer overflow **/
		};

		typedef ::Interrupt::Slot<matchA, ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
		typedef ::Interrupt::Slot<matchB, ::Interrupt::Binding::DynamicPlainFunction> MatchBSlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;
		typedef boost::mpl::vector<MatchASlot, MatchBSlot, OverflowSlot>::type Slots;
	};
}

template<>
struct InterruptRegistration<atmega128rfa1::Timer0IntMap, false>
{
	private:
	typedef atmega128rfa1::Timer0IntMap IntMap;
	typedef IntMap::Interrupts Int;
	
	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER0_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER0_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void registerCallback(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRM(TIMER0_OVF_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER0_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER0_COMPB_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void registerCallback()
	{
		switch(i)
		{
			case(IntMap::overflow)      : redirectISRF(TIMER0_OVF_vect, F);
					break;
			case(IntMap::matchA) : redirectISRF(TIMER0_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER0_COMPB_vect, F);
					break;
		}
	}
};

}
}
