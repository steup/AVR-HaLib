#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega1281
{
    struct Timer2
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            matchA = 13, /**< compare match in unit A **/
            matchB = 14, /**< compare match in unit B **/
            overflow = 10 /**< timer overflow **/
        };

        typedef avr_halib::interrupts::interrupt_manager::Slot<matchA,   avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchASlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<matchB,   avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchBSlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<overflow, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> OverflowSlot;

        typedef boost::mpl::vector<MatchASlot, MatchBSlot, OverflowSlot>::type Slots;
    };
}
}
}
