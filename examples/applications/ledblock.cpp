
#include "halib/ext/ledblock.h"
#include "halib/portmaps/robbyboard.h"

#define F_CPU 16000000UL

#include "halib/share/delay.h"

int main() 
{
	LedBlock<LedBlock0123> leds;
	
	while (1)
	{
		leds.setOn();			// Tuarn all Leds on
	
		delay_ms(1000);
	
		leds.setOff();			// Turn all LEDs off

		delay_ms(1000);

		leds.toggle();

		delay_ms(1500);

		for (uint8_t i = 0; i < 16; )
		{
			delay_ms(500);
			leds.set(i);
			i = leds.get() + 1;
		}

		delay_ms(1000);

		leds.setOff();			// Turn all LEDs off

		delay_ms(1000);
	}

	return 0;	
}

