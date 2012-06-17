#include <platform.h>

#include <avr-halib/avr/timer.h>
#include <avr-halib/ext/led.h>

using namespace avr_halib::config;

struct TimerConfig : public platform::Timer1BaseConfig
{
	static const bool useOverflowInt = true;
	static const timer::PrescalerType ps = timer::Prescalers::ps1024;
};

typedef avr_halib::drivers::Timer::configure<TimerConfig>::type Timer;

typedef avr_halib::ext::Led< platform::Led0 > Led0;

Led0 led0;

typedef Timer::StaticSlots::Overflow::bind<Led0, &>Led0::toggle>(led0) OverflowSlot;
typedef boost::mpl::vector< OveflowSlot >::type IntList;
typedef Interrupt::InterruptManager< IntList > IM;

int main()
{
	IM::init();

	Timer timer;

	sei();
	timer.start();

	while(true)
		Morpheus::sleep<Morpheus::SleepModes::idle>();

	return 0;
}
