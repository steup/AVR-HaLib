#pragma once

#include "avr-halib/regmaps/local.h"
#include "avr-halib/common/frequency.h"
#include "avr-halib/common/delay.h"
#include <stdint.h>

namespace avr_halib{
namespace drivers{

    /** \brief UART Driver
     *
     *	For reading and writing single characters over a serial connection with and without interrupt support
     **/
    struct Uart
    {
        typedef uint32_t BaudRateType;
        typedef uint8_t  DataBitType;
        typedef uint8_t  StopBitType;

        struct Parity
        {
            enum ParityType
            {
                none = 0,
                even = 1,
                odd  = 2
            };
        };
        typedef Parity::ParityType ParityType;

        struct DefaultConfig
        {
            typedef regmaps::local::Uart0      RegMap;
            typedef config::Frequency<F_CPU>   BaseClock;

            static const bool         useInterrupt = false;
            static const BaudRateType baudRate     = 19200;
            static const DataBitType  dataBits     = 8;
            static const StopBitType  stopBits     = 1;
            static const ParityType   parity       = Parity::none;
        };

        template<typename Config = DefaultConfig>
        struct configure
        {
            typedef typename Config::RegMap RegMap;
            struct Core
            {
                struct SleepSync
                {
                    void sync()
                    {
                        delay_us(12*1000000ULL/Config::baudRate);
                    }
                };

                Core()
                {
                    UseRegMap(rm, RegMap);
                    
                    typedef config::Frequency< Config::baudRate, 8 > divider;

                    rm.ubbr  = ( (uint16_t)Config::BaseClock::template div< divider >::type::value - 1 ) / 2;

                    rm.ucsrc = 0;
                    rm.umsel = false;
                    rm.ucsz2 = (Config::dataBits == 9);
                    rm.ucsz1 = (Config::dataBits >  6);
                    rm.ucsz0 = (Config::dataBits != 5 && 
                                Config::dataBits != 7);
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
                    rm.u2x  = true;
                    rm.mpcm = false;
                    
                    //write errorflags false
                    rm.fe   = false;
                    rm.pe   = false;
                    rm.dor  = false;
                    rm.udre = false;

                    SyncRegMap(rm);
                }
                
                void put(const char c)
                {
                    UseRegMap(rm, RegMap);
                    rm.txc=true; 
                    rm.udr = c;
                    SyncRegMap(rm);
                }

                bool ready()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    return rm.udre;
                }
                
                /**	\brief	Reads a character from the input buffer
                 *	\param	c	Reference to variable which shall store the character
                 *	\return		true if a character was read
                 */
                bool get(char & c)
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

            struct InterruptExtension : public Core
            {
                //TODO Include interrupt manager support

                void enableonReceive()
                {	
                    UseRegMap(rm, RegMap);
                    rm.rxcie=true;
                    SyncRegMap(rm);
                }

                void enableonReady()
                {
                    UseRegMap(rm, RegMap);
                    rm.udrie=true;
                    SyncRegMap(rm);
                }
                void disableonReceive()
                {	
                    UseRegMap(rm, RegMap);
                    rm.rxcie=false;
                    SyncRegMap(rm);
                }

                void disableonReady()
                {
                    UseRegMap(rm, RegMap);
                    rm.udrie=false;
                    SyncRegMap(rm);
                }
            };

            typedef typename boost::mpl::if_c< Config::useInterrupt, 
                                               InterruptExtension,
                                               Core
                                             >::type type;
            };
    };
}
}
