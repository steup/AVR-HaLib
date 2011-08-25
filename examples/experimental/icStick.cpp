#include <config.h>

#include <avr-halib/avr/clock.h>
#include <avr-halib/ext/led.h>
#include <avr-halib/ext/button.h>

#ifndef __AVR_ATmega1281__
#error "This example only supports the ATmega1281"
#endif

namespace ICStick
{
	#include <avr-halib/portmaps/icstick_portmap.h>
}

typedef avr_halib::drivers::Clock<ClockConfig> Clock;

typedef InterruptManager<Clock::InterruptSlotList, false> IM;

Clock clock;
Led< ICStick::Led0 > led;
Led< ICStick::DevelLed0 > dLed0;
Led< ICStick::DevelLed1 > dLed1;
Led< ICStick::DevelLed2 > dLed2;
Led< ICStick::DevelLed3 > dLed3;
Button< ICStick::DevelButton0 > b0;
Button< ICStick::DevelButton1 > b1;


void onTick()
{
	Clock::Time t;
	clock.getTime(t);
	log::emit() << "Tick: " << (uint16_t)t.ticks 
				<< ", " << (uint16_t)t.microTicks << log::endl;
	led.toggle();
	dLed2.toggle();
	dLed3.toggle();
}

int main()
{
	IM::init();

	clock.registerCallback<&onTick>();
	sei();

	while(true)
	{
		dLed0.set(b0.isPressed());
		dLed1.set(b1.isPressed());
	}

	return 0;
}
