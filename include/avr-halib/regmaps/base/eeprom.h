#pragma once

#include <avr-halib/avr/eeprom.h>

namespace avr_halib{
namespace regmaps{
namespace base{
/** \brief Accessor for remote register maps targeting EEPROM
 *
 *  \tparam EepromRegMap chip specific local register map containing eeprom access registers
 **/

struct EEPROM
{
    typedef drivers::EEPROM::DefaultConfig DefaultConfig;

    template<typename Config = DefaultConfig>
    struct configure
    {
        struct type
        {
            typedef typename drivers::EEPROM::configure<Config>::type Driver;

            /** \brief read access to EEPROM
             *
             *  \param address address of data in EEPROM
             *  \param data pointer to ram buffer
             *  \param size size of data to be fed into ram buffer from EEPROM
             * */
            bool read(uint16_t address, uint8_t* data, uint8_t size)
            {
                Driver drv;
                
                drv.setAddress(address);

                while(size)
                    if( ! drv.write(data[size--]) )
                        return false;

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
                Driver drv;
                
                drv.setAddress(address);

                while(size)
                    if( ! drv.read(data[size--]) )
                        return false;

                return true;
            }
        };
    };
};

}
}
}
