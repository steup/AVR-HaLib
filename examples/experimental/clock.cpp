#include <config.h>

#include <avr-halib/avr/clock.h>

typedef avr_halib::drivers::Clock<ClockConfig> Clock;

typedef InterruptManager<Clock::InterruptSlotList> IM;

Clock clock;

void onTick()
{
	Clock::Time t;
	clock.getTime(t);
	log::emit() << "Tick: " << t.ticks << ", " << t.microTicks << log::endl;
}

int main()
{
	IM::init();

	clock.registerCallback<&onTick>();
	sei();

	while(true)
		Morpheus::sleep<power::powerSave>();

	return 0;
}
