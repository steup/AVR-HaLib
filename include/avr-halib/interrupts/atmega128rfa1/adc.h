#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace interrupts
{
namespace atmega128rfa1
{
    struct Adc
    {
      /** \brief interrupts defined by this device **/
      enum Interrupts
      {
          conversionComplete = 25, /**< conversion complete interrupt **/
      };

      typedef avr_halib::interrupts::interrupt_manager::Slot< conversionComplete,
          avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction
          > ConversionCompleteSlot;

      typedef boost::mpl::vector< ConversionCompleteSlot >::type Slots;
    };
}
}
}
