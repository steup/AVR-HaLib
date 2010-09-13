#include "config.h"

#include <avr-halib/avr/basicADC.h>
#include <avr-halib/ext/sensorPowerControl.h>

using avr_halib::drivers::BasicADC;
using avr_halib::drivers::robby::SensorPowerControl;

typedef BasicADC<ConfiguredADCRegMap> Adc;
typedef SensorPowerControl<PowerPortMap> Power;

Adc adc;
Power power;

int main()
{
	power.setActive(0);
	while(true)
	{	
		log::emit() << log::clear;
		uint16_t value=0;
		if(!adc.configure<uint16_t>(ConfiguredADCRegMap::channel0, ConfiguredADCRegMap::internal))
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
