#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Timer2IntMap
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            matchA=9, /**< compare match in unit A **/
            overflow=10 /**< timer overflow **/
        };

        typedef avr_halib::interrupts::interrupt_manager::Slot<matchA, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchASlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<overflow, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> OverflowSlot;
        typedef boost::mpl::vector<MatchASlot, OverflowSlot>::type Slots;
    };
}
}
}
