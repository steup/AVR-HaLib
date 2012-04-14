#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega128rfa1
{
	struct Timer2
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA   = 13, /**< compare match in unit A **/
			matchB   = 14, /**< compare match in unit B **/
			overflow = 10  /**< timer overflow **/
		};

		typedef ::Interrupt::Slot<matchA,   ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
   		typedef ::Interrupt::Slot<matchB,   ::Interrupt::Binding::DynamicPlainFunction> MatchBSlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;
		
		typedef boost::mpl::vector<MatchASlot, MatchBSlot, OverflowSlot>::type Slots;
	};
}
}
}
