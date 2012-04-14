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
	struct Timer3
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture  = 31, /**< input capture **/
			matchA   = 32, /**< compare match in unit A **/
			matchB   = 33, /**< compare match in unit B **/
			matchC   = 34, /**< compare match in unit C **/
			overflow = 35  /**< timer overflow **/
		};

		typedef ::Interrupt::Slot<capture,  ::Interrupt::Binding::DynamicPlainFunction> CaptureSlot;
		typedef ::Interrupt::Slot<matchA,   ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
		typedef ::Interrupt::Slot<matchB,   ::Interrupt::Binding::DynamicPlainFunction> MatchBSlot;
		typedef ::Interrupt::Slot<matchC,   ::Interrupt::Binding::DynamicPlainFunction> MatchCSlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;
		
		typedef boost::mpl::vector<CaptureSlot, MatchASlot, MatchBSlot, MatchCSlot, OverflowSlot>::type Slots;
	};
}
}
}
