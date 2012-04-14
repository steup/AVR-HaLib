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
	struct Timer5
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture  = 46, /**< input capture **/
			matchA   = 47, /**< compare match in unit A **/
			matchB   = 48, /**< compare match in unit B **/
			matchC   = 49, /**< compare match in unit C **/
			overflow = 50  /**< timer overflow **/
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
