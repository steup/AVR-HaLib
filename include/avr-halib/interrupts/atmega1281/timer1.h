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
	struct Timer1
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture  = 16,	/**< input capture**/
			matchA   = 17,	/**< compare match in unit A **/
			matchB   = 18,	/**< compare match in unit B **/
			matchC   = 19,	/**< compare match in unit C **/
			overflow = 20	/**< timer overflow **/
		};

		typedef ::Interrupt::Slot< capture,  ::Interrupt::Binding::DynamicPlainFunction > CaptureSlot;
		typedef ::Interrupt::Slot< matchA,   ::Interrupt::Binding::DynamicPlainFunction > MatchASlot;
		typedef ::Interrupt::Slot< matchB,   ::Interrupt::Binding::DynamicPlainFunction > MatchBSlot;
		typedef ::Interrupt::Slot< matchC,   ::Interrupt::Binding::DynamicPlainFunction > MatchCSlot;
		typedef ::Interrupt::Slot< overflow, ::Interrupt::Binding::DynamicPlainFunction > OverflowSlot;
		
		typedef boost::mpl::vector< CaptureSlot, MatchASlot, MatchBSlot, MatchCSlot, OverflowSlot >::type Slots;
	};
}
}
}
