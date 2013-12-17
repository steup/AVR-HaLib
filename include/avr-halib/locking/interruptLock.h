#pragma once

#include <stdint.h>

namespace avr_halib
{
namespace locking
{
    struct GlobalIntLock
    {
        private:
            uint8_t regSave;

        public:
            GlobalIntLock()
            {
                asm volatile("in %0,0x3F\n\t"
                    "cli"
                    :"=r"(regSave)
                );
            }

            ~GlobalIntLock()
            {
                asm volatile("out 0x3F, %0"
                    :
                    :"r"(regSave)
                );
            }
    };
}
}
