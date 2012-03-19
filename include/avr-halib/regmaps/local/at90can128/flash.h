#pragma once

#include <stdint.h>
#include <avr-halib/regmaps/base/localRegMap.h>

namespace avr_halib{
namespace regmaps{
namespace local{
namespace at90can128{

    struct Flash : public base::LocalRegMap
    {
        public:
            struct Commands
            {
                enum CommandType
                {
                    bufferWrite    = 0x01,
                    pageErase      = 0x03,
                    pageWrite      = 0x05,
                    lockFuseAccess = 0x09,
                    enableRWW      = 0x11
                };
            };
            typedef Commands::CommandType CommandType;

            struct SpecialAddresses
            {
                enum SpecialAddressType
                {
                    lowFuse  = 0x00,
                    lock     = 0x01,
                    extFuse  = 0x02,
                    highFuse = 0x03
                };
            };

            typedef SpecialAddresses::SpecialAddressType SpecialAddressType;

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
