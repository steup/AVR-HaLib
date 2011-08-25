#include "config.h"

#include <avr-halib/avr/basicADC.h>

typedef avr_halib::drivers::BasicADC<ADCRegMap> Adc;

Adc adc;
SensorPower sensorPower;

int main()
{
	NoInt::init();

	sensorPower.setActive(0);
	while(true)
	{	
		log::emit() << log::clear;
		uint16_t value=0;
		if(!adc.configure<uint16_t>(ADCRegMap::channel0, ADCRegMap::internal))
		{
			log::emit() << "Error: ADC still busy" << log::endl;
			continue;
		}
		adc.startConversion();
		while(!adc.isDone());
		adc.fetchValue(value);
		log::emit() << "Value: " << value << log::endl;

		delay_ms(100);
	}
	return 0;
}
