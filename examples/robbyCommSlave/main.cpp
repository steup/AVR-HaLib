#include "platform.h"
#include <avr-halib/avr/spi.h>
#include <string.h>

using avr_halib::driver::Spi;

struct SpiConfig : public Spi::DefaultConfig
{
    static const bool                   useInterrupt = false;
    static const Spi::ModeType          mode         = Spi::Modes::slave;
    static const Spi::ClockPolarityType polarity     = Spi::ClockPolarities::idleOnLow;
    static const Spi::PrescalerType     prescaler    = 128;
};

typedef Spi::configure<SpiConfig>::type RobbyCommSpi;

RobbyCommSpi spi;

const char* rxFifo[]={"Hello World", "Test", "LimitedData"};


enum Commands
{
    fetch = 0x1
};

void deliverRx(int8_t& fifoIndex)
{
    if(fifoIndex<0)
    {
        spi.put(0x0);
        while(!spi.ready());
        return;
    }
    const char* rxBuf=rxFifo[fifoIndex];
    uint8_t len = strlen(rxBuf);
    spi.put(len);
    uint8_t i=0;
    while(true)
    {
        while(!spi.ready());
        spi.put(rxBuf[i]);
        if(++i==len)
            break;
    }
    fifoIndex--;
}

int main()
{
    log::emit() << "Robby Communication Test Slave" << log::endl;
    int8_t fifoIndex = 2;
    while(true)
    {   
        uint8_t c=0;
        UseRegMap(rm, SpiConfig::RegMap);
        spi.put(0x0);
        while(!spi.ready());
        spi.get(c);
        switch(c)
        {
            case(fetch): deliverRx(fifoIndex);
                          break;
        }
        log::emit() << "Got command: " << uint16_t(c) << log::endl;
    }
    return 0;
}
