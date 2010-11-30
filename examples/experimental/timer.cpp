#include <avr-halib/portmaps/robbyboard.h>
#include <avr-halib/regmaps/local.h>
#include <avr-halib/avr/newTimer.h>
#include <avr-halib/avr/sleep.h>
#include <avr-halib/ext/newLed.h>

#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/avr/InterruptManager/SignalSemanticInterrupt.h>

#include <boost/mpl/vector.hpp>
#include <avr/interrupt.h>

using avr_halib::regmaps::local::Timer2;
using avr_halib::drivers::Timer;
using avr_halib::power::sleep;
using avr_halib::drivers::external::Led;
using avr_halib::config::TimerDefaultConfig;

namespace power=avr_halib::power;

struct TimerConfig : public TimerDefaultConfig<Timer2>
{
	enum Parameters
	{
		overflowInt=true,
		ocmAInt=true,
		async=true,
		dynamicPrescaler=true
	};

	static const Timer2::OutputCompareValueType ocmAValue = 32;

	static const Timer2::WaveForms 			waveForm = Timer2::normal;
	static const Timer2::CompareMatchModes 	ocmAMode = Timer2::noOutput;
	static const Timer2::Prescalers 		ps		 = Timer2::ps1024;
};

typedef Timer<TimerConfig> ThisTimer;

ThisTimer timer;

Led<Led0> led0;
Led<Led1> led1;

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(tick)
{
	led0.toggle();
};

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(tock)
{
	led1.toggle();
};

struct InterruptConfig 
{
    typedef boost::mpl::vector<
				Interrupt::Slot<Timer2::IntMap::overflow_Int,
								::Interrupt::Binding::SignalSemanticFunction
				>::Bind<&tick>,
				Interrupt::Slot<Timer2::IntMap::compareMatch_Int,
								::Interrupt::Binding::SignalSemanticFunction
				>::Bind<&tock>
            >::type config;
};

typedef InterruptManager<InterruptConfig::config, false> IM;

int main()
{
	IM::init();

	sei();
	timer.start();

	while(true)
		sleep<power::powerSave>();

	return 0;
}
