/*#include <bootloader.h>

struct Config : public avr_halib::bootloader::Bootloader::DefaultConfig{
    struct InterfaceConfig : public avr_halib::bootloader::Bootloader::DefaultConfig::InterfaceConfig{
        typedef avr_halib::regmaps::local::Uart1 RegMap;
    };
};

typedef typename avr_halib::bootloader::Bootloader::configure<Config>::type Bootloader;

Bootloader bl;

int main()
{
    bl.run();
    return 0;
}*/

#include <avr-halib/avr/flash.h>
#include <avr-halib/common/delay.h>

//typedef avr_halib::drivers::Flash::configure<>::type Flash;

int main()
{
//    Flash::FlashWriter writer;
//    for(uint8_t i=0;i<128;i++)
//        writer.write(0);

//    writer.writeCurrentPage();

    for(uint32_t addr=0;addr<256;addr+=2)
    {
        uint16_t data=0x0;
        asm volatile( "movw  r0, %4\n\t"
                      "sts %1, %3\n\t"
                      "sts %0, %2\n\t"
                      "spm\n\t"
                      "clr  r1\n\t"
                      :
                      : "i" ((uint16_t)0x57),
                        "i" ((uint16_t)0x5B),
                        "r" ((uint8_t)(0x1)),
                        "r" ((uint8_t)((addr>>16)&0xff)),
                        "r" ((uint16_t)data),
                        "z" ((uint16_t)(addr&0xffffUL))
                      : "r0"
                    ); 
    }
    uint32_t address=0;
/*    asm volatile( "sts %1, %3\n\t"
                  "sts %0, %2\n\t"
                  "spm\n\t"
                  :
                  : "i" ((uint16_t)0x57),
                    "i" ((uint16_t)0x5B),
                    "r" ((uint8_t)(0x3)),
                    "r" ((uint8_t)(address>>16)&0xff),
                    "z" ((uint16_t)(address&0xffff))
                );
    while(*((volatile uint8_t*)0x57)&0x1);*/
    asm volatile( "sts %1, %3\n\t"
                  "sts %0, %2\n\t"
                  "spm\n\t"
                  :
                  : "i" ((uint16_t)0x57),
                    "i" ((uint16_t)0x5B),
                    "r" ((uint8_t)(0x5)),
                    "r" ((uint8_t)(address>>16)&0xff),
                    "z" ((uint16_t)(address&0xffff))
                );
    while(*((volatile uint8_t*)0x57)&0x1);
    asm volatile( "sts %0, %1\n\t"
                  "spm\n\t"
                  :
                  : "i" ((uint16_t)0x57),
                    "r" ((uint8_t)0x11)
                );

    return 0;
}
