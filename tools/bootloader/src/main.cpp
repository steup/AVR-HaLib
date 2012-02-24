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

typedef avr_halib::drivers::Flash::configure<>::type Flash;

static const char* const content="HalloDu";

int main()
{
    Flash::FlashWriter writer;
    for(uint16_t i=0;i<512;i+=2)
        writer.write(((uint16_t)content[(i+1)%8]<<8)|content[i%8]);

    return 0;
}
