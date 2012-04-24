#include <platform.h>

#include <avr-halib/avr/clock.h>
#include <avr-halib/avr/sleep.h>

struct ClockConfig
{
    typedef uint16_t TickValueType;
    typedef avr_halib::regmaps::local::Timer1 Timer;
    typedef avr_halib::config::Frequency<1> TargetFrequency;
    typedef avr_halib::config::Frequency<F_CPU> TimerFrequency;
};

typedef avr_halib::drivers::Clock< ClockConfig > Clock;

typedef Interrupt::InterruptManager< Clock::InterruptSlotList > IM;

Clock clock;
Clock::Time now;

void onTick()
{
	clock.getTime(now);
	log::emit() << "Tick: " << now.ticks << ", " << now.microTicks << log::endl;
}

int main()
{
	IM::init();

	clock.registerCallback< &onTick >();
	sei();

	while(true)
		Morpheus::sleep< Morpheus::SleepModes::powerSave >();

	return 0;
}
