

#if defined(__AVR_AT90CAN128__)
#	define F_CPU 16000000UL
#	include "halib/portmaps/robbyboard.h"
#elif defined(__AVR_ATMEGA32__)
#	define F_CPU 8000000UL
#	include "halib/portmaps/bobbyboard.h"
#else
#	error "Example program not ported to this platform yet."
#endif


#include "halib/avr/timer.wip.h"
#include "halib/ext/led.h"


class Blinker : public EggTimer<Timer2>, Led<Led0>
{
public:
	
	void onEggTimer()
	{
		static uint8_t w = 10;
		toggle();
// 		if (w)
			startEggTimer(w);
// 		w -= 30;
	}
};

int main()
{
	sei();
	Blinker b;
	b.onEggTimer();
	
	while(1)
		;
	
	return 0;
}
