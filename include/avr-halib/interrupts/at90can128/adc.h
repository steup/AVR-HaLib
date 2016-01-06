#pragma once

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Adc
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            conversionComplete=25,  /**< conversion complete interrupt **/
        };
    };
}
}
}
