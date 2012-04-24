#include "platform.h"

#include <avr-halib/avr/basicADC.h>

typedef avr_halib::drivers::BasicADC::configure<>::type Adc;

Adc adc;
SensorPower sensorPower;

int main()
{
	sensorPower.setActive(0);
	while(true)
	{	
		log::emit() << log::clear;
		uint16_t value=0;
		if( !adc.setup< uint16_t >( Adc::Channels::channel0, Adc::References::internal ) )
		{
			log::emit() << "Error: ADC still busy" << log::endl;
			continue;
		}
		adc.startConversion();
		while( !adc.isDone() );
		adc.fetchValue( value );
		log::emit() << "Value: " << value << log::endl;

		delay_ms(100);
	}
	return 0;
}
