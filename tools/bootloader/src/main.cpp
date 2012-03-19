#include <bootloader.h>

using namespace avr_halib::bootloader;

struct Config : public Bootloader::DefaultConfig
{
    struct CommunicationConfig : public Bootloader::DefaultConfig::CommunicationConfig
    {
        typedef avr_halib::regmaps::local::Uart1 RegMap;
    };
};

int main()
{
    Bootloader::configure< Config >::type bl;
    bl.run();
    return 0;
}
