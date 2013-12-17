#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
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
            matchA = 21, /**< compare match in unit A **/
            matchB = 22, /**< compare match in unit B **/
            overflow = 23 /**< timer overflow **/
        };

        typedef avr_halib::interrupts::interrupt_manager::Slot<matchA, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchASlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<matchB, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchBSlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<overflow, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> OverflowSlot;

        typedef boost::mpl::vector<MatchASlot, MatchBSlot, OverflowSlot>::type Slots;
    };
}
}
}
