#pragma once

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    struct Timer1IntMap
    {
        /** \brief interrupts defined by this device **/
        enum Interrupts
        {
            capture  = 11, /**< input capture**/
            matchA   = 12, /**< compare match in unit A **/
            matchB   = 13, /**< compare match in unit B **/
            matchC   = 14, /**< compare match in unit C **/
            overflow = 15 /**< timer overflow **/
        };
    };
}
}
}
