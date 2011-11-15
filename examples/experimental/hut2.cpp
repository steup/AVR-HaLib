#include "config.h"

#include <avr-halib/avr/basicADC.h>
#include <avr-halib/avr/interrupt.h>
#include <avr-halib/avr/pwm.h>
#include <avr-halib/regmaps/local.h>

#include "avr-halib/avr/InterruptManager/SignalSemanticInterrupt.h"

typedef avr_halib::power::Morpheus<MorpheusSyncList> Morpheus;

typedef avr_halib::regmaps::local::pwm pwm;
typedef avr_halib::regmaps::local::Timer0 PWMTimer0;
typedef avr_halib::regmaps::local::Timer1 PWMTimer1;
typedef avr_halib::regmaps::local::Timer2 PWMTimer2;
typedef avr_halib::regmaps::local::Timer3 PWMTimer3;

struct PWMConfig0 : public avr_halib::config::PWMDefaultConfig<PWMTimer0>
{
	enum{ useChannelA = true };

	static const pwm::Cycle      cycle      = pwm::static8;
	static const pwm::OutputMode outputMode = pwm::normal;
	static const pwm::Type       correction = pwm::fast;
	static const pwm::Prescalers ps         = pwm::ps256;
};

struct PWMConfig1 : public avr_halib::config::PWMDefaultConfig<PWMTimer1>
{
	enum{
		useChannelA = true,
		useChannelB = true
	};

	static const pwm::Cycle      cycle      = pwm::static8;
	static const pwm::OutputMode outputMode = pwm::normal;
	static const pwm::Type       correction = pwm::fast;
	static const pwm::Prescalers ps         = pwm::ps256;
};

struct TimerConfig : public config::TimerDefaultConfig<PWMTimer2>
{
	enum Parameters
	{
		overflowInt = true,
		ocmAInt     = true
	};

	static const PWMTimer2::Prescalers ps = PWMTimer2::ps256;
};

struct PWMConfig3 : public avr_halib::config::PWMDefaultConfig<PWMTimer3>
{
	enum{
		useChannelA = true,
		useChannelB = true,
		useChannelC = true
	};

	static const pwm::Cycle      cycle      = pwm::static8;
	static const pwm::OutputMode outputMode = pwm::normal;
	static const pwm::Type       correction = pwm::fast;
	static const pwm::Prescalers ps         = pwm::ps256;
};



typedef avr_halib::drivers::PWMGenerator<PWMConfig0> PWM0;
typedef avr_halib::drivers::PWMGenerator<PWMConfig1> PWM1;
typedef avr_halib::drivers::Timer<TimerConfig> PWM2;
typedef avr_halib::drivers::PWMGenerator<PWMConfig3> PWM3;
typedef avr_halib::drivers::BasicADC<ADCRegMap> Adc;

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(off)
{
	PORTB&=~0x80;
}

IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(on)
{
	PORTB|=0x80;
}

typedef InterruptManager<
			boost::mpl::vector<
				Interrupt::Slot<PWM2::InterruptMap::matchA,
							    ::Interrupt::Binding::SignalSemanticFunction
							   >::Bind<&off>,
							
				Interrupt::Slot<PWM2::InterruptMap::overflow,
								::Interrupt::Binding::SignalSemanticFunction
								>::Bind<&on>
							 >::type , false> IM;

Adc adc;

uint8_t accX()
{
	uint8_t value;
	while(!adc.configure<uint8_t>(ADCRegMap::channel5, ADCRegMap::vcc));
	adc.startConversion();
	while(!adc.isDone());
	adc.fetchValue(value);
	return value;
}

uint8_t accY()
{
	uint8_t value;
	while(!adc.configure<uint8_t>(ADCRegMap::channel3, ADCRegMap::vcc));
	adc.startConversion();
	while(!adc.isDone());
	adc.fetchValue(value);
	return value;
}

uint8_t accZ()
{
	uint8_t value;
	while(!adc.configure<uint8_t>(ADCRegMap::channel1, ADCRegMap::vcc));
	adc.startConversion();
	while(!adc.isDone());
	adc.fetchValue(value);
	return value;
}

class LEDs
{
	private:
		PWM0 pwm0;
		PWM1 pwm1;
		PWM2 pwm2;
		PWM3 pwm3;

	public:
		LEDs(){pwm2.start();sei();}
		enum Letters
		{
			f=0,
			a,
			m,
			o1,
			u,
			s,
			o2
		};

		void value(Letters letter, uint8_t brightness)
		{
			switch(letter)
			{
				case(f):  pwm3.value<PWM3::channelA>(brightness);
							break;
				case(a):  pwm3.value<PWM3::channelC>(brightness);
							break;
				case(m):  pwm3.value<PWM3::channelB>(brightness);
							break;
				case(o1): pwm0.value<PWM0::channelA>(brightness);
							break;
				case(u):  pwm1.value<PWM1::channelA>(brightness);
							break;
				case(s):  pwm1.value<PWM1::channelB>(brightness);
							break;
				case(o2): pwm2.setOutputCompareValue<PWM2::matchA>(brightness);
							break;
			}
		}
};

int main()
{
	LEDs leds;
	IM::init();
	DDRB|=0x80;

	leds.value(LEDs::f,30);
	leds.value(LEDs::a,30);
	leds.value(LEDs::m,30);
	leds.value(LEDs::o1,30);
	leds.value(LEDs::u,30);
	leds.value(LEDs::s,30);
	leds.value(LEDs::o2,30);

	while(true)
		log::emit() << "(" << (uint16_t)accX() << ", " << (uint16_t)accY() << ", " << (uint16_t)accZ() << ")" << log::endl;

	return 0;
}


