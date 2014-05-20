#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
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
            capture = 46, /**< input capture **/
            matchA = 47, /**< compare match in unit A **/
            matchB = 48, /**< compare match in unit B **/
            matchC = 49, /**< compare match in unit C **/
            overflow = 50  /**< timer overflow **/
        };
    
        typedef avr_halib::interrupts::interrupt_manager::Slot<capture,  avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> CaptureSlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<matchA,   avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchASlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<matchB,   avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchBSlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<matchC,   avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> MatchCSlot;
        typedef avr_halib::interrupts::interrupt_manager::Slot<overflow, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction> OverflowSlot;
    
        typedef boost::mpl::vector<CaptureSlot, MatchASlot, MatchBSlot, MatchCSlot, OverflowSlot>::type Slots;
  };
}
}
}
