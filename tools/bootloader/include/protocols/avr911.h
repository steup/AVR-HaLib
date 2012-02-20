#pragma once

namespace avr_halib{
namespace bootloader{
namespace protocols{
    struct AVR911{
        struct DefaultConfig{
        };

        template<typename Config = DefaultConfig>
        struct configure{
            struct type{
                bool handle(uint8_t input, uint8_t& response){
                    response=input;
                    return true;
                }
            };
        };
    };
}
}
}
