#pragma once

namespace avr_halib{
namespace config
{
    struct Interrupt
    {
        struct InterruptTypes
        {
            enum InterruptType
            {
                fixed,
                dynamic,
            };
        };
        typedef InterruptTypes::InterruptType InterruptType;
    };
}
}
