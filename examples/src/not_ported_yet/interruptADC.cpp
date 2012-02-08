#include "config.h"

#include <avr-halib/avr/interruptADC.h>

typedef avr_halib::drivers::InterruptADC::configure<>::type Adc;

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

	adc.registerCallback< onConversionComplete >();

	log::emit() << "ADC with interrupts" << log::endl;

	sei();

	while(true)
	{
		if(!adc.setup<uint16_t>(Adc::Channels::channel0, Adc::References::internal))
			log::emit() << "Error: ADC still busy" << log::endl;
		else
			adc.startConversion(false);
//		delay_ms(1000);
	}
	return 0;
}
