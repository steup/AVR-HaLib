#pragma once

#include <avr-halib/drivers/avr/timer/outputCompareUnit.h>
#include <avr-halib/drivers/avr/timer/asyncHandler.h>
#include <avr-halib/interrupts/interrupt.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
/** Timer */
namespace timer
{
    /** \brief TODO \todo
     *
     * \tparam config Configuration
     */
    template<typename config>
    struct HALSelector
    {
        /** \brief TODO \todo */
        enum Parameters
        {
            enableAsync=config::async && config::RegMap::Parameters::asyncCapability,
            numOCU=config::RegMap::Parameters::numOCU
        };

        /** \brief TODO \todo */
        typedef AsyncHandler<config, enableAsync> Async;
        /** \brief TODO \todo */
        typedef OutputCompareUnit<config, enableAsync, numOCU> OCUs;
        /** \brief TODO \todo */
        typedef interrupts::InterruptRegistration<typename config::RegMap::InterruptMap> InterruptRegistration;

        /** \brief TODO \todo */
        struct type : public Async, public OCUs, public InterruptRegistration {};
    };
}
}
}
}
