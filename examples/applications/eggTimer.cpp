
#define F_CPU 16000000

#include "halib/avr/timer.wip.h"
#include "halib/ext/led.h"
#include "halib/portmaps/robbyboard.h"

class Blinker : public EggTimer<Timer0>, Led<Led0>
{
public:
	void start()
	{
		startEggTimer(1000);
	}
	
	void onEggTimer()
	{
		toggle();
		start();
	}
};

int main()
{
//	Led<Led2> l3;
//	l3.setOn();
	
	sei();
	Blinker b;
	b.start();
	
//	Led<Led1> l;
//	l.setOn();
	while(1)
		;
	
	return 0;
}
