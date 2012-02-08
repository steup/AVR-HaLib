#pragma once

#include <avr-halib/regmaps/local.h>
#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/avr/interruptLock.h>

namespace avr_halib{
namespace regmaps{
namespace base{
/** \brief Accessor for remote register maps targeting EEPROM
 *
 *  \tparam EepromRegMap chip specific local register map containing eeprom access registers
 **/

struct EEPROM
{
    struct DefaultConfig
    {
        typedef local::EEPROM RegMap;
    };

    template<typename Config = DefaultConfig>
    struct configure
    {
        struct type
        {
            typedef typename Config::RegMap RegMap;

            /** \brief read access to EEPROM
             *
             *  \param address address of data in EEPROM
             *  \param data pointer to ram buffer
             *  \param size size of data to be fed into ram buffer from EEPROM
             * */
            bool read(uint16_t address, uint8_t* data, uint8_t size)
            {
                UseRegMap(eeprom, RegMap);

                do
                    SyncRegMap(eeprom);
                while(eeprom.startWrite);
                    

                for(uint8_t i=0;i<size;i++)
                {
                    avr_halib::locking::GlobalIntLock lock;

                    eeprom.address   = address + i;
                    eeprom.startRead = true;
                    SyncRegMap(eeprom);
                    data[i]          = eeprom.data;
                }
                return true;
            }

            /** \brief write access to EEPROM
             *
             *  \param address address of data in EEPROM
             *  \param data pointer to ram buffer
             *  \param size size of data to be fed from ram buffer to EEPROM
             * */
            bool write(uint16_t address, uint8_t* data, uint8_t size)
            {
                UseRegMap(eeprom, RegMap);

                do
                    SyncRegMap(eeprom);
                while(eeprom.startWrite);
                    

                for(uint8_t i=0;i<size;i++)
                {
                    avr_halib::locking::GlobalIntLock lock;

                    eeprom.address     = address + i;
                    eeprom.data        = data[i];
                    SyncRegMap(eeprom);

                    //the eepe must be set within maximal 4 clock cycles
                    //after the setting of eempe. Therefore it is necessary
                    //to provide special assembly code to make sure, that
                    //this timing behaviour is met in each optimization
                    //level
                    asm volatile(
                        "sbi %0, %1\n\t"
                            "sbi %0, %2\n\t"
                        :
                        :"I"(&eeprom.eecr-0x20), "I"(EEMPE), "I"(EEPE)
                    );
                }
                return true;
            }
        };
    };
};

}
}
}
