
#include "halib/timer.h"
#include "halib/misc.h"
#include <avr/io.h>
LedBlock leds(PORTA, DDRA, 0x0f ,0);

class Blink:public Timer2
{
	void onTimer()
	{
		leds.setLedPattern(leds.getLedPattern()-1);
	}

};

int main()
{
	Blink b;
	leds.setLedPattern(0x7);
	b.start(TIMER_PS_1024);
	
	while(1)
		;

}