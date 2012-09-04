#pragma once

#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
	struct Timer0
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA   = 21, /**< compare match in unit A **/
            matchB   = 22, /**< compare match in unit B **/
			overflow = 23  /**< timer overflow **/
		};

		typedef ::Interrupt::Slot<matchA, ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
        typedef ::Interrupt::Slot<matchB, ::Interrupt::Binding::DynamicPlainFunction> MatchBSlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;

		typedef boost::mpl::vector<MatchASlot, MatchBSlot, OverflowSlot>::type Slots;
	};
}
}
}
