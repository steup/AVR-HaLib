#pragma once

#include "avr-halib/avr/uart.h"
#include <interfaces.h>

namespace avr_halib
{
namespace bootloader
{
namespace communication
{
    struct Uart
    {
        typedef drivers::Uart::DefaultConfig DefaultConfig;

        template<typename Config = DefaultConfig>
        struct configure
        {
            struct NoIntConfig : public Config
            {
                static const bool useInterrupt = false;
            };

            struct type : public drivers::Uart::configure< NoIntConfig >::type, interfaces::CommunicationDevice
            {
                using drivers::Uart::configure< NoIntConfig >::type::put;
                using drivers::Uart::configure< NoIntConfig >::type::get;
            };
        };
    };
}
}
}
