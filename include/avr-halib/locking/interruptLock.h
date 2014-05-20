#pragma once

#include <stdint.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib locking */
namespace locking
{
    /** \brief TODO \todo */
    struct GlobalIntLock
    {
        private:
            /** \brief TODO \todo */
            uint8_t regSave;

        public:
            /** \brief TODO \todo */
            GlobalIntLock()
            {
                asm volatile("in %0,0x3F\n\t"
                    "cli"
                    :"=r"(regSave)
                );
            }

            /** \brief TODO \todo */
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
