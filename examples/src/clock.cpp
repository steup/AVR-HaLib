#include <platform.h>

#include <avr-halib/drivers/avr/clock.h>
#include <avr-halib/common/sleep.h>
#include <avr-halib/interrupts/interrupt.h>

using namespace avr_halib;
using namespace regmaps::local;
using namespace config;
using namespace drivers;
using common::Delegate;
using interrupts::interrupt_manager::InterruptManager;

struct ClockConfig
{
    typedef uint16_t TickValueType;
    typedef Timer1 Timer;
    typedef Frequency<1> TargetFrequency;
    typedef Frequency<F_CPU> TimerFrequency;
};

typedef avr::Clock< ClockConfig > Clock;

typedef InterruptManager< Clock::InterruptSlotList > IM;

BIND_INTERRUPTS(IM);

Clock clock;
Clock::Time now;

void onTick()
{
    clock.getTime(now);
    log::emit() << "Tick: " << now.ticks << ", " << now.microTicks << log::endl;
}

int main()
{
    log::emit() << "Clock started: " << log::endl;
		Delegate<void> d;
		d.bind<&onTick>();
    clock.setCallback(d);
    sei();

    while(true);
        Morpheus::sleep< Morpheus::SleepModes::powerSave >();

    return 0;
}
