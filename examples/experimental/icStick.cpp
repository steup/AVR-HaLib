#include <logConf.h>
#include <avr-halib/regmaps/local.h>
#include <avr-halib/avr/clock.h>
#include <avr-halib/avr/sleep.h>
#include <avr-halib/portmaps/icstick_portmap.h>
#include <avr-halib/ext/led.h>
#include <avr-halib/ext/button.h>

UseInterrupt(SIG_OUTPUT_COMPARE2A);

struct ClockConfig
{
	typedef uint16_t TickValueType;
	typedef Frequency<1> TargetFrequency;
	typedef /*CPUClock*/ Frequency<32768> TimerFrequency;
	typedef avr_halib::regmaps::local::Timer2 Timer;
};

typedef avr_halib::drivers::Clock<ClockConfig> ThisClock;

ThisClock clock;
Led<Led0> led;
Led<DevelLed0> dLed0;
Led<DevelLed1> dLed1;
Led<DevelLed2> dLed2;
Led<DevelLed3> dLed3;
Button<DevelButton0> b0;
Button<DevelButton1> b1;


void onTick()
{
	ThisClock::Time t;
	clock.getTime(t);
	log::emit() << "Tick: " << (uint16_t)t.ticks 
				<< ", " << (uint16_t)t.microTicks << log::endl;
	led.toggle();
	dLed2.toggle();
	dLed3.toggle();
}

int main()
{
	clock.registerCallback<&onTick>();
	sei();

	while(true)
	{
		dLed0.set(b0.isPressed());
		dLed1.set(b1.isPressed());
	}

	return 0;
}
