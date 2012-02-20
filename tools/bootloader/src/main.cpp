#include <bootloader.h>

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
}
