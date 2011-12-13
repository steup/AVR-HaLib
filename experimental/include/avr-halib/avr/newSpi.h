#pragma once

#include "avr-halib/regmaps/regmaps.h"

namespace avr_halib{
namespace drivers{

    /**
     *	\class Spi spi.h "avr-halib/avr/spi.h"
     *	\brief SPI Interface
     *	\param SpiRegmap Register map and configuration
     *	\param length_t	Type used for size of the buffers and addressing the buffers
     *	\param oBufLen	Size of output buffer
     *	\param iBufLen	Size of input buffer
     *
     *	For reading and writing strings and integers see \see doc_cdevices
     **/
    template<typename Config>
    class Spi
    {
        protected:
            typedef typename Config::RegMap RegMap;

        public:
        
            Spi()
            {
                reset();
            }
            
            void reset()
            {
                UseRegMap(rm, RegMap);
                
                rm.mosi.ddr=true;
                rm.miso.ddr=false;
                rm.sck.ddr=true;
                rm.ss.ddr=true;
                rm.mosi.port=false;
                rm.miso.port=false;
                rm.sck.port=false;
                rm.ss.port=false;
                
                SyncRegMap(rm);
                
                rm.mstr = true;
                rm.spe = true;
                
                rm.spie = false;
                
                rm.dord = Config::dataDirection;
                rm.cpol = Config::leadingEdge;
                rm.cpha = Config::sampleEdge;
                
                rm.spr0 = 0x02 & Config::clockPrescaler;
                rm.spr1 = 0x04 & Config::clockPrescaler;
                rm.spi2x = !(0x01 & Config::clockPrescaler); //Bit negieren
                
            
                // Reset Flags
                
                SyncRegMap(rm);
                while (rm.spif || rm.wcol)
                {
                    uint8_t dummy;
                    dummy = rm.spdr;
                    SyncRegmap(rm);
                    rm.spdr=dummy;
                }
            }
            
            void put(const unsigned char c)
            {
                UseRegmap(rm, RegMap);
                rm.spdr = c;
                SyncRegMap(rm);
                while(Config::busywaitput && !rm.spif)
                    SyncRegMap(rm);
            }
            
            /**	\brief	Reads a character from the spdr buffer
             *	\param	c	Reference to variable which shall store the character
             *	\return		true if a character was read
             **/
            bool ready()
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.spif;
            }

            /**	\brief	Reads a character from the spdr buffer
             *	\param	c	Reference to variable which shall store the character
             *	\return		true if a character was read
             **/
            bool get(unsigned char & c)
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                c = rm.spdr;
                return true;
            }
        

    };
}
}
