#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>

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

        typedef avr_halib::interrupts::interrupt_manager::Slot< externalInterrupt,
            avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction > ExternalInterruptSlot;

        typedef typename boost::mpl::vector< ExternalInterruptSlot >::type Slots;
    };
}
}
}
