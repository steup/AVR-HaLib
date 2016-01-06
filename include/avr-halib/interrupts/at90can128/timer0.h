#pragma once

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Timer0IntMap
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            matchA=16, /**< compare match in unit A **/
            overflow=17 /**< timer overflow **/
        };
    };
}
}
}
