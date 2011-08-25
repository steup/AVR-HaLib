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
			matchA=17,		/**< compare match in unit A **/
			matchB=18,		/**< compare match in unit B **/
			matchC=19,		/**< compare match in unit C **/
			overflow=20		/**< timer overflow **/
		};

		typedef ::Interrupt::Slot<capture, ::Interrupt::Binding::DynamicPlainFunction> CaptureSlot;
		typedef ::Interrupt::Slot<matchA, ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
		typedef ::Interrupt::Slot<matchB, ::Interrupt::Binding::DynamicPlainFunction> MatchBSlot;
		typedef ::Interrupt::Slot<matchC, ::Interrupt::Binding::DynamicPlainFunction> MatchCSlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;
		
		typedef boost::mpl::vector<CaptureSlot, MatchASlot, MatchBSlot, MatchCSlot, OverflowSlot>::type Slots;
	};
}

template<>
struct InterruptRegistration<atmega1281::Timer1IntMap>
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
			case(IntMap::matchA) 	: redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) 	: redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) 	: redirectISRM(TIMER1_COMPC_vect, F, obj);
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
			case(IntMap::matchA) 	: redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) 	: redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) 	: redirectISRM(TIMER1_COMPC_vect, F, obj);
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
			case(IntMap::matchA) 	: redirectISRF(TIMER1_COMPA_vect, F);
					break;
			case(IntMap::matchB) 	: redirectISRF(TIMER1_COMPB_vect, F);
					break;
			case(IntMap::matchC) 	: redirectISRF(TIMER1_COMPC_vect, F);
					break;
		}
	}
};

}
}
