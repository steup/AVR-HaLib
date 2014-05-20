#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Spi
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            operationComplete = 20,  /**< operation complete **/
        };

        typedef avr_halib::interrupts::interrupt_manager::Slot< operationComplete,
            avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction
            > OperationCompleteSlot;

        typedef boost::mpl::vector< OperationCompleteSlot >::type Slots;
    };
}
}
}
