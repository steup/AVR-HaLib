#pragma once

#include <avr-halib/avr/InterruptManager/InterruptBinding.h>
#include <avr-halib/avr/InterruptManager/Slot.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	template<uint8_t nr>
	struct ExternalInterruptIntMap
	{
		static const uint8_t baseNumber = 1;

		enum Interrupts
		{
			externalInterrupt= baseNumber + nr,
		};

		typedef ::Interrupt::Slot< externalInterrupt, 
                                   ::Interrupt::Binding::DynamicPlainFunction > ExternalInterruptSlot;
		typedef typename boost::mpl::vector< ExternalInterruptSlot >::type Slots;
	};
}

}
}
