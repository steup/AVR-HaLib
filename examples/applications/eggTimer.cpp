
#define F_CPU 16000000

#include "halib/avr/timer.wip.h"
#include "halib/ext/led.h"
#include "halib/portmaps/robbyboard.h"

class Blinker : public EggTimer<Timer0>, Led<Led0>
{
public:
	
	void onEggTimer()
	{
		static uint8_t w = 10;
		toggle();
		//if (w)
			startEggTimer(w);
		//w /= 2;
	}
};

int main()
{
//	Led<Led2> l3;
//	l3.setOn();
	
	sei();
	Blinker b;
	b.onEggTimer();
	
//	Led<Led1> l;
//	l.setOn();
	while(1)
		;
	
	return 0;
}
