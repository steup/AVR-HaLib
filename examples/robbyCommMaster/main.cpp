#include "platform.h"
#include <avr-halib/avr/spi.h>

using avr_halib::driver::Spi;

struct SpiConfig : public Spi::DefaultConfig
{
    static const bool useInterrupt = false;
    static const Spi::ClockPolarityType polarity = Spi::ClockPolarities::idleOnLow;
    static const Spi::PrescalerType prescaler    = 4;
};

typedef Spi::configure<SpiConfig>::type RobbyCommSpi;

enum Commands
{
    fetch = 0x1
};

uint8_t buffer[128];

RobbyCommSpi spi;

bool fetchRx(uint8_t* buffer)
{
    UseRegMap(rm, SpiConfig::RegMap);
    rm.ss.port=false;
    SyncRegMap(rm);

    spi.put(fetch);
    while(!spi.ready());
    delay_ms(1);
    spi.put(0x0);
    while(!spi.ready());
    uint8_t len=0;
    spi.get(len);
    if(!len)
    {
        rm.ss.port=true;
        SyncRegMap(rm);
        return false;
    }
    for(uint8_t i=0;i<len;i++)
    {
        spi.put(0x0);
        while(!spi.ready());
        spi.get(buffer[i]);
    }
    rm.ss.port=true;
    SyncRegMap(rm);
    buffer[len]='\0';
    return true;
}

int main()
{
    log::emit() << "Robby Communication Test Master" << log::endl;
    delay_ms(1000);
    UseRegMap(rm, SpiConfig::RegMap);
    rm.ss.ddr  = true;
    rm.ss.port = true;
    SyncRegMap(rm);
    for(uint8_t i=0;i<4;i++)
    {
        delay_ms(1000);
        log::emit() << "fetching message" << log::endl;
        if(fetchRx(buffer))
            log::emit() << "got: " << (char*)buffer << log::endl;
        else
            log::emit() << "Error during fetch!" <<log::endl;
    }
    return 0;
}
