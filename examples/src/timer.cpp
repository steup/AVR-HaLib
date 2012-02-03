#include <config.h>

#include <avr-halib/avr/newTimer.h>
#include <avr-halib/ext/newLed.h>

typedef regmaps::local::Timer2 MyTimer;

struct TimerConfig : public config::TimerDefaultConfig<MyTimer>
{
	enum Parameters
	{
		overflowInt = true,
		ocmAInt     = true,
		async       = true,
	};

	static const MyTimer::Prescalers ps = MyTimer::ps1024;
};

typedef drivers::Timer<TimerConfig> Timer;

typedef avr_halib::drivers::external::Led< Led0 > LED0;
typedef avr_halib::drivers::external::Led< Led1 > LED1;

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
		Morpheus::sleep<power::powerSave>();

	return 0;
}
