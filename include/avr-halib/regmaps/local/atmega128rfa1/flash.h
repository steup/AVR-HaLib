#pragma once

#include <stdint.h>
#include <avr-halib/regmaps/base/localRegMap.h>

namespace avr_halib{
namespace regmaps{
namespace local{
namespace atmega128rfa1{

    struct Flash : public base::LocalRegMap
    {
        public:
            struct Commands
            {
                enum CommandType
                {
                    bufferWrite = 0x1,
                    pageErase   = 0x3,
                    pageWrite   = 0x5,
                    lockWrite   = 0x7,
                    enableRWW   = 0x11
                };
            };
            typedef Commands::CommandType CommandType;
        private:
            uint8_t __pad0[0x57];
        public:
            union{
                struct{
                    uint8_t spmen   : 1;
                };
                struct{
                    CommandType cmd : 5;
                    uint8_t         : 1;
                    uint8_t rwwsb   : 1;
                    uint8_t spmie   : 1;
                };
                uint8_t spmcsr;
            };
        private:
            uint8_t __pad1[0x5B-0x57-1];

        public:
            uint8_t rampz;
    };
}
}
}
}
