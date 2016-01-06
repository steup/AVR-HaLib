#pragma once

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
    template<uint8_t nr>
    struct ExternalInterruptIntMap
    {
        static const uint8_t baseNumber = 1;

        enum Interrupts
        {
            externalInterrupt= baseNumber + nr,
        };
    };
}
}
}
