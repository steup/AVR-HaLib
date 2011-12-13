#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace atmega1281 {

    struct Spi : public base::LocalRegMap
    {
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
                        bool    spr0 :1;    ///<
                        bool    spr1 :1;    ///<
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

            struct{
                union{
                    struct{  // pin ss: b 0;
                        uint8_t __pad0 [0x23];
                        bool pin : 1;   // PINB (0x36), bit 0
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 0
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 0
                    } ss;
                    struct{  // pin sck: b 1;
                        uint8_t __pad0 [0x23];
                        uint8_t  : 1;
                        bool pin : 1;   // PINB (0x36), bit 1
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 1
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 1
                    } sck;
                    struct{  // pin mosi: b 2;
                        uint8_t __pad0 [0x23];
                        uint8_t  : 2;
                        bool pin : 1;   // PINB (0x36), bit 2
                        uint8_t  : 7;
                        bool ddr : 1;   // DDRB (0x37), bit 2
                        uint8_t  : 7;
                        bool port : 1;  // PORTB (0x38), bit 2
                    } mosi;
                    struct{  // pin miso: b 3;
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

        enum Prescalers
        {
            ps2   = 0,
            ps4   = 1,
            ps8   = 2,
            ps16  = 3,
            ps32  = 4,
            ps64  = 5,
            ps128 = 7
        };

        enum BitOrder
        {
            msb = 0,
            lsb = 1
        };

        enum LeadingEdge
        {
            rising  = 0,
            falling = 1
        };

        enum SampleEdge
        {
            leading  = 0,
            trailing = 1
        };

    } __attribute__((packed));
}
}
}
}
