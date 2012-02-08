#pragma once

#include <stdint.h>

namespace avr_halib{
namespace regmaps{
namespace local{
namespace at90can128{
    struct Uart0 : public base::LocalRegMap{
        private:
            uint8_t __base [0xc0];
        public:
            union{
                uint8_t ucsra;
                struct{
                    bool mpcm : 1;
                    bool u2x  : 1;
                    bool pe   : 1;
                    bool dor  : 1;
                    bool fe   : 1;
                    bool udre : 1;
                    bool txc  : 1;
                    bool rxc  : 1;
                };
            };
            union{
                uint8_t ucsrb;
                struct{
                    bool txb8  : 1;
                    bool rxb8  : 1;
                    bool ucsz2 : 1;
                    bool txen  : 1;
                    bool rxen  : 1;
                    bool udrie : 1;
                    bool txcie : 1;
                    bool rxcie : 1;
                };
            };
            union{
                uint8_t ucsrc;
                struct {
                    bool	ucpol : 1;
                    bool	ucsz0 : 1;
                    bool	ucsz1 : 1;
                    bool	usbs  : 1;
                    uint8_t	upm   : 2;
                    bool	umsel : 1;
                    bool          : 1;
                };
            };
            
        private:
            uint8_t __pad0;
        public:
            union{
                uint16_t ubbr;
                struct{
                    uint8_t ubrrl;
                    uint8_t ubrrh;
                };
            };
            uint8_t udr;
    }__attribute__((packed));
}
}
}
}
