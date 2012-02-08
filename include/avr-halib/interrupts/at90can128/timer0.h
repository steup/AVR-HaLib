#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer0IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			matchA=16,		/**< compare match in unit A **/
			overflow=17		/**< timer overflow **/
		};

		typedef ::Interrupt::Slot<matchA, ::Interrupt::Binding::DynamicPlainFunction> MatchASlot;
		typedef ::Interrupt::Slot<overflow, ::Interrupt::Binding::DynamicPlainFunction> OverflowSlot;
		typedef boost::mpl::vector<MatchASlot, OverflowSlot>::type Slots;
	};
}

}
}
