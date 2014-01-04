#pragma once

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** \brief TWIMaster Driver
    *
    */
    struct TWIMaster
    {
        /** \brief TWI default configuration */
        struct DefaultConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::TWI RegMap;
        };

        /** \brief TWI configuration
         *
         * \tparam Config Configuration
         */
        template< typename Config = DefaultConfig >
        struct configure
        {
            /** \brief TODO \todo */
            struct type
            {
                private:
                    /** \brief TODO \todo */
                    typedef typename Config::RegMap RegMap;

                    /** \brief TODO \todo */
                    union Slave_Adress
                    {
                        /** \brief TODO \todo */
                        uint8_t raw_data;
                        struct
                        {
                            bool read:1;
                            uint8_t adress:7;
                        };
                    };

                public:
                    /** \brief TODO \todo
                     *
                     * \param br TODO \todo
                     */
                    void init(uint8_t br)
                    {
                        UseRegMap(rm, RegMap);
                        rm.twen=true;
                        SyncRegMap(rm);
                        rm.twbr=br;
                        rm.twcr=0x00;
                        rm.twint=false;
                        rm.twea=true;
                        rm.twsta=false;
                        rm.twsto=false;
                        rm.twwc=false;
                        rm.twen=true;
                        rm.twie=false;

                        rm.twsr=0x00;
                        rm.twps=rm.ps1;
                        SyncRegMap(rm);
                    }
                    // There are two posibilitis of writing i2c adresses 8 bit baseadress or 7 bit adress u have to use the 7 bit address here the shifting is done internal (rightshift ur 8 bit baseadress once)

                    /** \brief TODO \todo */
                    int start(uint8_t adress, bool read)
                    {
                        UseRegMap(rm, RegMap);
                        rm.twen=true;
                        rm.twsta=true;
                        rm.twint=true;
                        SyncRegMap(rm);
                        do{SyncRegMap(rm);}while(!rm.twint);
                        if(!(rm.tws==RegMap::st_start||rm.tws==RegMap::repeat_start))
                            return false;
                        Slave_Adress ad;
                        ad.adress = adress;
                        ad.read = read;
                        rm.twdr = ad.raw_data;
                        // rm.twdr = adress << 1 | read?RegMap::read:RegMap::write;
                        rm.twsta=false;
                        rm.twint=true;
                        SyncRegMap(rm);
                        do{SyncRegMap(rm);}while(!rm.twint);
                        return (rm.tws==RegMap::sla_write_ack||rm.tws==RegMap::sla_read_ack);
                    }

                    /** \brief TODO \todo */
                    void stop()
                    {
                        UseRegMap(rm, RegMap);
                        rm.twint=true;
                        rm.twen=true;
                        rm.twsto=true;
                        SyncRegMap(rm);
                    }

                    /** \brief TODO \todo
                     *
                     * \param byte TODO \todo
                     */
                    bool write(uint8_t byte)
                    {
                        UseRegMap(rm, RegMap);
                        rm.twdr = byte;
                        rm.twen = true;
                        rm.twint = true;
                        SyncRegMap(rm);
                        do{SyncRegMap(rm);}while(!rm.twint);
                        return (rm.tws==RegMap::m_data_tx_ack||rm.tws==RegMap::sl_data_tx_ack);
                    }

                    /** \brief TODO \todo */
                    uint8_t st()
                    {
                        UseRegMap(rm, RegMap);
                        SyncRegMap(rm);
                        return rm.tws;
                    }

                    /** \brief TODO \todo
                     *
                     * \param last_byte TODO \todo
                     */
                    uint8_t read(bool last_byte)
                    {
                        UseRegMap(rm, RegMap);
                        rm.twea = !last_byte;
                        rm.twint = true;
                        SyncRegMap(rm);
                        do{SyncRegMap(rm);}while(!rm.twint);
                        return rm.twdr;
                    }
                };
        };
    };
}
}
}
