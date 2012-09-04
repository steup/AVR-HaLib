//#define LOGGING_DISABLE

#include <platform.h>
#include <avr-halib/avr/spi.h>
#include <avr-halib/avr/portmap.h>
#include <avr-halib/avr/InterruptManager/InterruptManager.h>

using avr_halib::driver::Spi;

struct SpiConfig : public Spi::DefaultConfig
{
    static const bool useInterrupt = false;
    static const Spi::ClockPolarityType polarity = Spi::ClockPolarities::idleOnLow;
    static const Spi::PrescalerType prescaler    = 2;
};

typedef Spi::configure<SpiConfig>::type At86RF230Spi;
typedef ::Interrupt::InterruptManager< At86RF230Spi::InterruptSlotList > IM;

At86RF230Spi spi;

uint8_t readRegister(uint8_t reg)
{
    uint8_t value;

    UsePortmap(pm, platform::Radio);

    pm.cs.port=false;

    spi.put(0x80 | reg);
    while(!spi.ready());

    spi.put(0x80 | reg);
    while(!spi.ready());

    spi.get(value);

    pm.cs.port = true;
    SyncPortmap(rm);  

    return value;
}

int main()
{
    IM::init();
    sei();
    UsePortmap(pm, platform::Radio);

    pm.sleep.ddr = true;
    pm.sleep.port = false;
    pm.reset.ddr = true;
    pm.reset.port = true;
    pm.cs.ddr = true;
    pm.cs.port = true;
    SyncPortmap(rm);

    log::emit() << "SPI test" << log::endl;
    delay_ms(1000);

    log::emit() << "Part number   : " << (uint16_t)readRegister(0x1C) << log::endl;
    log::emit() << "Version number: " << (uint16_t)readRegister(0x1D) << log::endl;
    log::emit() << "Manufactor ID : " << ((uint16_t)readRegister(0x1F)<<8|readRegister(0x1E)) << log::endl;

    while(true)
        Morpheus::sleep<Morpheus::SleepModes::powerDown>();
    return 0;
}
