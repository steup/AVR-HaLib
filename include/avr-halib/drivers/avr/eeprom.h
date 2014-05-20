#pragma once

#include <avr-halib/locking/interruptLock.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** \brief EEPROM Driver */
    struct EEPROM
    {
        /** \brief TODO \todo */
        typedef uint16_t AddressType;

        /** \brief EEPROM default configuration */
        struct DefaultConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::EEPROM RegMap;
            /** \brief TODO \todo */
            static const AddressType addressStart = 0;
            /** \brief TODO \todo */
            static const AddressType addressEnd   = 0xFFF;
        };

        /** \brief EEPROM configuration
         *
         * \tparam cfg Configuration
         */
        template< typename Config = DefaultConfig >
        struct configure
        {
            /** \brief EEPROM base type */
            struct type
            {
                private:
                    /** \brief TODO \todo */
                    typedef typename Config::RegMap RegMap;
                    /** \brief TODO \todo */
                    AddressType address;

                public:
                    type() : address(0){}

                    /** \brief TODO \todo
                     *
                     * \param address TODO \todo
                     */
                    bool setAddress(const AddressType address)
                    {
                        if(address < Config::addressStart ||
                            address > Config::addressEnd)
                            return false;

                        this->address = address;

                        return true;
                    }

                    /** \brief TODO \todo
                     *
                     * \param data TODO \todo
                     */
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

                    /** \brief TODO \todo
                     *
                     * \param data TODO \todo
                     */
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

    /** \example eeprom.cpp
     *
     * TODO \todo
     */
}
}
}
