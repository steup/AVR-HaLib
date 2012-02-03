#include "config.h"

#include <avr-halib/avr/interruptADC.h>

typedef avr_halib::drivers::InterruptADC<ADCRegMap> Adc;

Adc adc;

void onConversionComplete()
{
	uint16_t value;
	adc.fetchValue(value);
	log::emit() << "Value: " << value << log::endl;
};

typedef Interrupt::InterruptManager<Adc::InterruptSlotList, false> IM;


int main()
{
	IM::init();

	adc.registerCallback<onConversionComplete>();

	log::emit() << "ADC with interrupts" << log::endl;

	sei();

	while(true)
	{
		if(!adc.configure<uint16_t>(ADCRegMap::channel0, ADCRegMap::internal))
			log::emit() << "Error: ADC still busy" << log::endl;
		else
			adc.startConversion(false);
//		delay_ms(1000);
	}
	return 0;
}
