#include "config.h"

typedef avr_halib::power::Morpheus<MorpheusSyncList> Morpheus;

#include <avr-halib/avr/interruptADC.h>
#include <avr-halib/share/delay.h>

UseInterrupt(SIG_ADC);

using avr_halib::drivers::InterruptADC;

typedef InterruptADC<ConfiguredADCRegMap> Adc;

Adc adc;

void onConversionComplete()
{
	uint16_t value;
	adc.fetchValue(value);
	log::emit() << "Value: " << value << log::endl;
};

int main()
{
	adc.registerCallback<&onConversionComplete>();

	sei();

	while(true)
	{
		if(!adc.configure<uint16_t>(ConfiguredADCRegMap::channel0, ConfiguredADCRegMap::internal, ConfiguredADCRegMap::ps128))
			log::emit() << "Error: ADC still busy" << log::endl;
		else
			adc.startConversion(false);
		delay_ms(1000);
	}
	return 0;
}
