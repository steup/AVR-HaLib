#include "config.h"

typedef avr_halib::power::Morpheus<MorpheusSyncList> Morpheus;

#include <avr-halib/avr/InterruptManager/InterruptManager.h>
#include <avr-halib/avr/InterruptManager/SignalSemanticInterrupt.h>
#include <avr-halib/avr/interruptADC.h>
#include <avr-halib/share/delay.h>

using avr_halib::drivers::InterruptADC;

typedef InterruptADC<ConfiguredADCRegMap> Adc;

Adc adc;

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(onConversionComplete)
{
	uint16_t value;
	adc.fetchValue(value);
	log::emit() << "Value: " << value << log::endl;
};

struct InterruptConfig 
{
    typedef boost::mpl::vector<
				Interrupt::Slot<Adc::Interrupts::conversionComplete,
								::Interrupt::Binding::SignalSemanticFunction
				>::Bind<&onConversionComplete>
            >::type config;
};

typedef InterruptManager<InterruptConfig::config, false> IM;


int main()
{
	IM::init();

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
