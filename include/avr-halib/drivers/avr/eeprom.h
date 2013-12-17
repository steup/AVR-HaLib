#pragma once

#include <avr-halib/locking/interruptLock.h>

namespace avr_halib
{
namespace drivers
{
namespace avr
{
    /** \brief EEPROM Driver
    *
    */
    struct EEPROM
    {
        typedef uint16_t AddressType;

        struct DefaultConfig
        {
            typedef regmaps::local::EEPROM RegMap;
            static const AddressType addressStart = 0;
            static const AddressType addressEnd   = 0xFFF;
        };

        template< typename Config = DefaultConfig >
        struct configure
        {
            struct type
            {
                private:
                    typedef typename Config::RegMap RegMap;
                    AddressType address;

                public:
                    type() : address(0){}

                    bool setAddress(const AddressType address)
                    {
                        if( address < Config::addressStart ||
                            address > Config::addressEnd )
                            return false;

                        this->address = address;

                        return true;
                    }

                    bool read(uint8_t& data)
                    {
                        avr_halib::locking::GlobalIntLock lock;

                        UseRegMap(eeprom, RegMap);

                        do
                            SyncRegMap(eeprom);
                        while(eeprom.startWrite);

                        eeprom.address   = address++;
                        eeprom.startRead = true;
                        SyncRegMap(eeprom);
                        data          = eeprom.data;
                
                        return true;
                    }

                    bool write(uint8_t data)
                    {
                        avr_halib::locking::GlobalIntLock lock;
                        UseRegMap(eeprom, RegMap);

                        do
                            SyncRegMap(eeprom);
                        while(eeprom.startWrite);
                    

                        eeprom.address     = address++;
                        eeprom.data        = data;
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

                        return true;
                    }
            };
        };
    };
}
}
}
