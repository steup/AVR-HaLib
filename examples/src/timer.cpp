#include <config.h>

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

typedef avr_halib::ext::Led< Led0 > LED0;
typedef avr_halib::ext::Led< Led1 > LED1;

LED0 led0;
LED1 led1;

typedef Interrupt::InterruptManager<Timer::InterruptSlotList> IM;

int main()
{
	IM::init();

	Timer::InterruptMap::OverflowSlot::bind< LED0, &LED0::toggle >( &led0 );
	Timer::InterruptMap::MatchASlot::bind  < LED1, &LED1::toggle >( &led1 );

	Timer timer;

	timer.setOutputCompareValue<Timer::matchA>(128);

	sei();
	timer.start();

	while(true)
		Morpheus::sleep<Morpheus::SleepModes::powerSave>();

	return 0;
}
