#include "config.h"

#include <avr-halib/avr/interruptADC.h>
#include <avr-halib/ext/sensorPowerControl.h>

UseInterrupt(SIG_ADC);

using avr_halib::drivers::InterruptADC;
using avr_halib::drivers::robby::SensorPowerControl;

typedef InterruptADC<ConfiguredADCRegMap> Adc;
typedef SensorPowerControl<PowerPortMap> Power;

Adc adc;
Power power;

volatile bool conversionComplete;

void onConversionComplete()
{
	conversionComplete=true;
}

int main()
{
	adc.registerCallback<onConversionComplete>();
	sei();
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
		conversionComplete=false;
		adc.startConversion(true);
		while(!conversionComplete);
		adc.fetchValue(value);
		log::emit() << "Value: " << value << log::endl;
		delay_ms(100);
	}
	return 0;
}
