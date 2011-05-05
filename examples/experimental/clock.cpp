#include <logConf.h>
#include <avr-halib/regmaps/local.h>
#include <avr-halib/avr/clock.h>
#include <avr-halib/avr/sleep.h>

typedef avr_halib::power::Morpheus<MorpheusSyncList> Morpheus;

UseInterrupt(SIG_OUTPUT_COMPARE2A);

using avr_halib::drivers::Clock;

using namespace avr_halib::regmaps;

namespace power=avr_halib::power;

struct ClockConfig
{
	typedef uint16_t TickValueType;
	typedef Frequency<1> TargetFrequency;
	typedef /*CPUClock*/ Frequency<32768> TimerFrequency;
	typedef local::Timer2 Timer;
};

typedef Clock<ClockConfig> ThisClock;

ThisClock clock;

void onTick()
{
	ThisClock::Time t;
	clock.getTime(t);
	log::emit() << "Tick: " << t.ticks << ", " << t.microTicks << log::endl;
}

int main()
{
	clock.registerCallback<&onTick>();
	sei();

	while(true)
		Morpheus::sleep<power::powerSave>();

	return 0;
}
