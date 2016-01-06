#pragma once

#include "avr-halib/regmaps/local.h"
#include "avr-halib/config/frequency.h"
#include "avr-halib/common/delay.h"
#include <stdint.h>

#include <boost/mpl/if.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** \brief UART Driver
     * For reading and writing single characters over a serial connection with and without interrupt support
     */
    struct Uart
    {
        /** \brief TODO \todo */
        typedef uint32_t BaudRateType;
        /** \brief TODO \todo */
        typedef uint8_t  DataBitType;
        /** \brief TODO \todo */
        typedef uint8_t  StopBitType;

        /** \brief TODO \todo */
        struct Parity
        {
            /** \brief TODO \todo */
            enum ParityType
            {
                none = 0,
                even = 1,
                odd  = 2
            };
        };
        /** \brief TODO \todo */
        typedef Parity::ParityType ParityType;

        /** \brief UART default configuration */
        struct DefaultConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::Uart0      RegMap;
            /** \brief TODO \todo */
            typedef config::Frequency<F_CPU>   BaseClock;

            /** \brief TODO \todo */
            static const bool         useInterrupt = false;
            /** \brief TODO \todo */
            static const bool         highSpeed    = false;
            /** \brief TODO \todo */
            static const BaudRateType baudRate     = 19200;
            /** \brief TODO \todo */
            static const DataBitType  dataBits     = 8;
            /** \brief TODO \todo */
            static const StopBitType  stopBits     = 1;
            /** \brief TODO \todo */
            static const ParityType   parity       = Parity::none;
        };

        /** \brief UART configuration
         *
         * \tparam Config TODO \todo
         */
        template<typename Config = DefaultConfig>
        struct configure
        {
            /** \brief TODO \todo */
            typedef typename Config::RegMap RegMap;
            /** \brief TODO \todo */
            struct Core
            {
                /** \brief TODO \todo */
                struct SleepSync
                {
                    /** \brief TODO \todo */
                    void sync()
                    {
                        delay_us(12*1000000ULL/Config::baudRate);
                    }
                };

                Core()
                {
                    UseRegMap(rm, RegMap);
                    
                    typedef          config::Frequency< Config::baudRate*((Config::highSpeed)?8:16)> divider;
                    typedef typename Config::BaseClock::template div< divider >::type                UBBRConfig;

                    rm.ubbr  = (uint16_t)UBBRConfig::value - 1;

                    rm.ucsrc = 0;
                    rm.umsel = false;
                    rm.ucsz2 = (Config::dataBits == 9);
                    rm.ucsz1 = ((Config::dataBits-5)&0x2)>>1;
                    rm.ucsz0 = (Config::dataBits-5)&0x1;
                    rm.usbs  = (Config::stopBits == 2);
                    rm.upm   = Config::parity;

                    // Enable UART Receiver and Transmitter, enable Receive-Interrupt
                    rm.rxen  = true;
                    rm.txen  = true;

                    rm.rxcie = false;
                    rm.txcie = false;
                    rm.udrie = false;

                    rm.rxb8  = false;
                    rm.txb8  = false;

                    // Flush Receive-Buffer
                    do
                    {
                        uint8_t dummy;
                        (void) (dummy = rm.udr);
                        SyncRegMap(rm);
                    }
                    while (rm.rxc);

                    // Reset Receive and Transmit Complete-Flags
                    rm.rxc  = false;
                    rm.txc  = false;
                    rm.u2x  = Config::highSpeed;
                    rm.mpcm = false;

                    // write errorflags false
                    rm.fe   = false;
                    rm.pe   = false;
                    rm.dor  = false;
                    rm.udre = false;

                    SyncRegMap(rm);
                }

                /** \brief TODO \todo
                 *
                 * \param c TODO \todo
                 */
                void put(const uint8_t c)
                {
                    UseRegMap(rm, RegMap);
                    rm.txc=true;
                    rm.udr = c;
                    SyncRegMap(rm);
                }

                /** \brief TODO \todo */
                bool ready()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    return rm.udre;
                }

                /** \brief Reads a character from the input buffer
                 * \param c Reference to variable which shall store the character
                 * \return true if a character was read
                 */
                bool get(uint8_t& c)
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    if( rm.rxc )
                    {
                        c=rm.udr;
                        return true;
                    }else
                        return false;
                }
            };

            /** \brief TODO \todo */
            struct InterruptExtension : public Core
            {
                // \todo Include interrupt manager support

                /** \brief TODO \todo */
                void enableonReceive()
                {
                    UseRegMap(rm, RegMap);
                    rm.rxcie=true;
                    SyncRegMap(rm);
                }

                /** \brief TODO \todo */
                void enableonReady()
                {
                    UseRegMap(rm, RegMap);
                    rm.udrie=true;
                    SyncRegMap(rm);
                }

                /** \brief TODO \todo */
                void disableonReceive()
                {
                    UseRegMap(rm, RegMap);
                    rm.rxcie=false;
                    SyncRegMap(rm);
                }

                /** \brief TODO \todo */
                void disableonReady()
                {
                    UseRegMap(rm, RegMap);
                    rm.udrie=false;
                    SyncRegMap(rm);
                }
            };

            /** \brief TODO \todo */
            typedef typename boost::mpl::if_c< Config::useInterrupt,
                                               InterruptExtension,
                                               Core
                                             >::type type;
            };
    };
}
}
}
