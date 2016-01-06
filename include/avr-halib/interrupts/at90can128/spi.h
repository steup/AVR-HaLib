#pragma once

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
    };
}
}
}
