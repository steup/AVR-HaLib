#include <boost/mpl/vector.hpp>
#include <avr-halib/avr/sleep.h>

typedef boost::mpl::vector<>::type MorpheusSyncList;
typedef avr_halib::power::Morpheus<MorpheusSyncList> Morpheus;
namespace power=avr_halib::power;

#include <avr-halib/portmaps/robbyboard.h>
#include <avr-halib/regmaps/local.h>
#include <avr-halib/avr/newTimer.h>
#include <avr-halib/ext/newLed.h>

#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/avr/InterruptManager/SignalSemanticInterrupt.h>

using avr_halib::regmaps::local::Timer2;
using avr_halib::drivers::external::Led;
using avr_halib::config::TimerDefaultConfig;

struct TimerConfig : public TimerDefaultConfig<Timer2>
{
	enum Parameters
	{
		overflowInt = true,
		ocmAInt     = true,
		async       = true,
	};

	static const Timer2::Prescalers ps = Timer2::ps1024;
};

typedef avr_halib::drivers::Timer<TimerConfig> Timer;


IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(tick)
{
	Led<Led0> led;
	led.toggle();
};

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(tock)
{
	Led<Led1> led;
	led.toggle();
};

struct InterruptConfig 
{
    typedef boost::mpl::vector<
				Interrupt::Slot<Timer::Interrupts::overflow,
								::Interrupt::Binding::SignalSemanticFunction
				>::Bind<&tick>,
				Interrupt::Slot<Timer::Interrupts::matchA,
								::Interrupt::Binding::SignalSemanticFunction
				>::Bind<&tock>
            >::type config;
};

typedef InterruptManager<InterruptConfig::config, false> IM;

int main()
{
	IM::init();

	Timer timer;

	timer.setOutputCompareValue<Timer::matchA>(32);

	sei();
	timer.start();

	while(true)
		Morpheus::sleep<power::powerSave>();

	return 0;
}
