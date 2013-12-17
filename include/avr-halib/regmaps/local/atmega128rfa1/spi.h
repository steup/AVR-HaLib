#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <avr-halib/config/spi.h>
#include <avr-halib/interrupts/atmega128rfa1/spi.h>

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega128rfa1
{
    struct Spi : public base::LocalRegMap, private config::Spi
    {
        template<BitOrderType order>
        struct BitOrderConfig
        {
            static const bool dord = (order==BitOrders::leastFirst);
        };

        template<PrescalerType ps, ClockPolarityType pol, SampleEdgeType edge>
        struct ClockConfig
        {
            static const bool     spi2x = (ps==2 || ps==8 || ps==32);
            static const uint8_t  spr   = (ps==4)?0x0:
                                            (ps==16)?0x1:
                                                (ps==64)?0x2:3;
            static const bool cpha = (edge==SampleEdges::trailing);
            static const bool cpol = (pol==ClockPolarities::idleOnHigh);
        };

        template<ModeType mode>
        struct ModeConfig
        {
            static const bool mstr = (mode==Modes::master);
        };

        typedef interrupts::atmega128rfa1::Spi InterruptMap;

        union
        {
            struct
            {
                uint8_t _pad[0x4c];

                union
                {
                    uint8_t spcr;
                    struct
                    {
                        bool    spr  :2;    ///<
                        bool    cpha :1;    ///<
                        bool    cpol :1;    ///<
                        bool    mstr :1;    ///<
                        bool    dord :1;    ///<
                        bool    spe  :1;    ///<
                        bool    spie :1;    ///<
                    };
                };
                union
                {
                    uint8_t spsr;
                    struct
                    {
                        bool    spi2x :1;    ///<
                        bool          :5;    ///< reserved
                        bool    wcol  :1;    ///<
                        bool    spif  :1;    ///<
                    };
                };
                uint8_t spdr;
            };

            struct
            {
                union
                {
                    struct
                    {  // pin ss: b 0;
                        uint8_t __pad0 [0x23];
                        bool pin : 1;   // PINB (0x36), bit 0
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 0
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 0
                    } ss;
                    struct
                    {  // pin sck: b 1;
                        uint8_t __pad0 [0x23];
                        uint8_t  : 1;
                        bool pin : 1;   // PINB (0x36), bit 1
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 1
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 1
                    } sck;
                    struct
                    {  // pin mosi: b 2;
                        uint8_t __pad0 [0x23];
                        uint8_t  : 2;
                        bool pin : 1;   // PINB (0x36), bit 2
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 2
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 2
                    } mosi;
                    struct
                    {  // pin miso: b 3;
                        uint8_t __pad0 [0x23];
                        uint8_t  : 3;
                        bool pin : 1;   // PINB (0x36), bit 3
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 3
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 3
                    } miso;
                };
            };
        };
    } __attribute__((packed));
}
}
}
}
