
#include "avr-halib/ext/led.h"
#include "avr-halib/portmaps/robbyboard.h"


int main() 
{
	Led<Led0> led0;
	
	led0.setOn();
 	led0.setOff();
 	if (!led0.isOn())
 		led0.toggle();

	while (1)
		;
	return 0;	
}

