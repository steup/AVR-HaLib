#pragma once

#include "avr-halib/avr/uart.h"

namespace avr_halib {
namespace bootloader {
namespace interfaces {
    struct Uart{
        typedef drivers::Uart::DefaultConfig DefaultConfig;

        template<typename Config = DefaultConfig>
        struct configure{
            struct NoIntConfig : public Config{
                static const bool useInterrupt = false;
            };

           typedef typename 
               avr_halib::drivers::Uart::configure<NoIntConfig>::type type;
        };
    };
}
}
}
