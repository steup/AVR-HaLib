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
			capture=27,			/**< input capture **/
			matchA=28,	/**< compare match in unit A **/
			matchB=29,	/**< compare match in unit B **/
			matchC=30,	/**< compare match in unit C **/
			overflow=31			/**< timer overflow **/
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
struct InterruptRegistration<at90can128::Timer3IntMap, false>
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
			case(IntMap::overflow)      : redirectISRM(TIMER3_OVF_vect, F, obj);
					break;
			case(IntMap::capture)       : redirectISRM(TIMER3_CAPT_vect, F, obj);
					break;
			case(IntMap::matchA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
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
			case(IntMap::matchA) : redirectISRM(TIMER3_COMPA_vect, F, obj);
					break;
			case(IntMap::matchB) : redirectISRM(TIMER3_COMPB_vect, F, obj);
					break;
			case(IntMap::matchC) : redirectISRM(TIMER3_COMPC_vect, F, obj);
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
			case(IntMap::matchA) : redirectISRF(TIMER3_COMPA_vect, F);
					break;
			case(IntMap::matchB) : redirectISRF(TIMER3_COMPB_vect, F);
					break;
			case(IntMap::matchC) : redirectISRF(TIMER3_COMPC_vect, F);
					break;
		}
	}
};

}
}
