#pragma once

#include <avr-halib/drivers/avr/timer/outputCompareUnit.h>
#include <avr-halib/drivers/avr/timer/asyncHandler.h>
#include <avr-halib/interrupts/interrupt.h>

namespace avr_halib
{
namespace drivers
{
namespace avr
{
namespace timer
{
    template<typename config>
    struct HALSelector
    {
        enum Parameters
        {
            enableAsync=config::async && config::RegMap::Parameters::asyncCapability,
            numOCU=config::RegMap::Parameters::numOCU
        };

        typedef AsyncHandler<config, enableAsync> Async;
        typedef OutputCompareUnit<config, enableAsync, numOCU> OCUs;
        typedef interrupts::InterruptRegistration<typename config::RegMap::InterruptMap> InterruptRegistration;

        struct type : public Async, public OCUs, public InterruptRegistration {};
    };
}
}
}
}
