#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer1IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture  = 11,	/**< input capture**/
			matchA   = 12,	/**< compare match in unit A **/
			matchB   = 13,	/**< compare match in unit B **/
			matchC   = 14,	/**< compare match in unit C **/
			overflow = 15	/**< timer overflow **/
		};

		typedef ::Interrupt::Slot< capture,  ::Interrupt::Binding::DynamicPlainFunction > CaptureSlot;
		typedef ::Interrupt::Slot< matchA,   ::Interrupt::Binding::DynamicPlainFunction > MatchASlot;
		typedef ::Interrupt::Slot< matchB,   ::Interrupt::Binding::DynamicPlainFunction > MatchBSlot;
		typedef ::Interrupt::Slot< matchC,   ::Interrupt::Binding::DynamicPlainFunction > MatchCSlot;
		typedef ::Interrupt::Slot< overflow, ::Interrupt::Binding::DynamicPlainFunction > OverflowSlot;
		
		typedef boost::mpl::vector< CaptureSlot, 
                                    MatchASlot,
                                    MatchBSlot,
                                    MatchCSlot,
                                    OverflowSlot >::type                                  Slots;

	};
}

}
}
