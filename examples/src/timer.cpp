#include <platform.h>

#include <avr-halib/avr/timer.h>
#include <avr-halib/ext/led.h>

typedef avr_halib::regmaps::local::Timer2 MyTimer;

struct TimerConfig : public avr_halib::config::TimerDefaultConfig<MyTimer>
{
	enum Parameters
	{
		overflowInt = true,
		ocmAInt     = true,
		async       = true,
	};

	static const MyTimer::Prescalers ps = MyTimer::ps1024;
};

typedef avr_halib::drivers::Timer<TimerConfig> Timer;

typedef avr_halib::ext::Led< platform::Led0 > Led0;
typedef avr_halib::ext::Led< platform::Led1 > Led1;

Led0 led0;
Led1 led1;

typedef Interrupt::InterruptManager<Timer::InterruptSlotList> IM;

int main()
{
	IM::init();

	Timer::InterruptMap::OverflowSlot::bind< Led0, &Led0::toggle >( &led0 );
	Timer::InterruptMap::MatchASlot::bind  < Led1, &Led1::toggle >( &led1 );

	Timer timer;

	timer.setOutputCompareValue<Timer::matchA>(128);

	sei();
	timer.start();

	while(true)
		Morpheus::sleep<Morpheus::SleepModes::powerSave>();

	return 0;
}
