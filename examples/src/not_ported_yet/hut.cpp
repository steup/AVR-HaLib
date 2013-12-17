#include "platform.h"

#include <avr-halib/avr/basicADC.h>
#include <avr-halib/avr/pwm.h>
#include <avr-halib/regmaps/local.h>

typedef avr_halib::regmaps::local::pwm pwm;
typedef avr_halib::regmaps::local::Timer1 PWMTimer1;
typedef avr_halib::regmaps::local::Timer2 PWMTimer2;
typedef avr_halib::regmaps::local::Timer3 PWMTimer3;

struct PWMConfig1 : public avr_halib::config::PWMDefaultConfig<PWMTimer1>
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

struct PWMConfig2 : public avr_halib::config::PWMDefaultConfig<PWMTimer2>
{
	enum{
		useChannelA = true
	};

	static const pwm::Cycle      cycle      = pwm::static8;
	static const pwm::OutputMode outputMode = pwm::normal;
	static const pwm::Type       correction = pwm::fast;
	static const pwm::Prescalers ps         = pwm::ps256;
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

typedef avr_halib::drivers::PWMGenerator<PWMConfig1> PWM1;
typedef avr_halib::drivers::PWMGenerator<PWMConfig2> PWM2;
typedef avr_halib::drivers::PWMGenerator<PWMConfig3> PWM3;
typedef avr_halib::drivers::BasicADC::configure<>::type Adc;


class Accel
{
	private:
		Adc adc;

	public:
		uint16_t bat()
		{
			uint16_t value;
			delay_ms(1);
			adc.setup<uint16_t>(Adc::Channels::bandgap11, Adc::References::vcc);
			delay_ms(1);
			adc.startConversion();
			delay_ms(1);
			adc.fetchValue(value);
			return value;
		}

		uint8_t x()
		{
			uint8_t value;
			delay_ms(1);
			adc.setup<uint8_t>(Adc::Channels::channel5, Adc::References::vcc);
			delay_ms(1);
			adc.startConversion();
			delay_ms(1);
			adc.fetchValue(value);
			return value;
		}

		uint8_t y()
		{
			uint8_t value;
			delay_ms(1);
			adc.setup<uint8_t>(Adc::Channels::channel3, Adc::References::vcc);
			delay_ms(1);
			adc.startConversion();
			delay_ms(1);
			adc.fetchValue(value);
			return value;
		}

		uint8_t z()
		{
			uint8_t value;
			delay_ms(1);
			adc.setup<uint8_t>(Adc::Channels::channel1, Adc::References::vcc);
			delay_ms(1);
			adc.startConversion();
			delay_ms(1);
			adc.fetchValue(value);
			return value;
		}
};

class LEDs
{
	private:
		PWM1 pwm1;
		PWM2 pwm2;
		PWM3 pwm3;

	public:
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

		void start()
		{
			pwm1.stop();
			pwm2.stop();
			pwm3.stop();
			pwm1.start();
			delay_ms(1);
			pwm2.start();
			delay_ms(1);
			pwm3.start();
		}

		void value(Letters letter, uint8_t brightness)
		{
			if(letter==f)
				pwm3.value<PWM3::channelA>(brightness);
			if(letter==a)
				pwm3.value<PWM3::channelC>(brightness);
			if(letter==m)
				pwm3.value<PWM3::channelB>(brightness);
			if(letter==o1)
				pwm2.value<PWM2::channelA>(brightness);
			if(letter==u)
				pwm1.value<PWM1::channelB>(brightness);
			if(letter==s)
				pwm1.value<PWM1::channelA>(brightness);
			if(letter==o2)
				pwm1.value<PWM1::channelC>(brightness);
		}
};

const int8_t xP[][7]={ {-30, -20, -10,   0,  10,  20,  30},
					   {-25, -17,  -9,   0,   9,  17,  25},
					   {-20, -13,  -6,   0,   6,  13,  20},
					   {-15, -10,  -5,   0,   5,  10,  15},
					   {-10,  -7,  -4,   0,   4,   7,  10},
					   {- 5,  -3,  -1,   0,   1,   3,   5},
					   {  0,   0,   0,   0,   0,   0,   0},
   					   {  5,   3,   1,   0,  -1,  -3,  -5},
					   { 10,   7,   4,   0,  -4,  -7, -10},
					   { 15,  10,   5,   0,  -5, -10, -15},
					   { 20,  13,   6,   0,  -6, -13, -20},
					   { 25,  17,   9,   0,  -9, -17, -25},
					   { 30,  20,  10,   0, -10, -20, -30}};

const uint8_t xO[]={3,3,4,3,3,4,3,3,4,3,3,4};

const uint16_t batIndex[]={255, 290, 310, 312, 316, 318, 320, 330};
const uint8_t batComp[]=  {0,     4,   5,   6,   8,  10,  14,  19,  21};

uint8_t xF[128];
uint8_t yF[128];
uint8_t zF[128];
uint16_t batF[128];
uint8_t j=0;

int main()
{
	cli();
	LEDs leds;
	Accel acc;
	uint16_t base=128;

	leds.start();

	while(true)
	{
		uint16_t x=0;
		uint16_t y=0;
		uint16_t z=0;
		uint32_t bat=0;

		xF[j]=acc.x();
		yF[j]=acc.y();
		zF[j]=acc.z();
		batF[j++]=acc.bat();
		if(j==16)
			j=0;
		for(uint8_t k=0;k<16;k++)
		{
			x+=xF[k];
			y+=yF[k];
			z+=zF[k];
			bat+=batF[k];
		}
		x>>=4;
		y>>=4;
		z>>=4;
		bat>>=4;

		uint8_t i=0;
		do
		{
			if(bat<batIndex[i])
				break;
			i++;
		}while(i<sizeof(batIndex)/2);

		x+=batComp[i];

		log::emit() << "( " << (uint16_t)x << ", " << (uint16_t)y << ", " << (uint16_t)z << ") - " << (uint16_t)bat << log::endl;
		log::emit() << (uint16_t)batComp[i] << log::endl;

		i=0;
		do
		{
			if(x<80)
				break;

			x-=xO[i];

			i++;

		}while( i < sizeof(xO) );

		log::emit() << (uint16_t)i << log::endl;
		
		leds.value(LEDs::f,  base+4*xP[i][0]);
		leds.value(LEDs::a,  base+4*xP[i][1]);
		leds.value(LEDs::m,  base+4*xP[i][2]);
		leds.value(LEDs::o1, base+4*xP[i][3]);
		leds.value(LEDs::u,  base+4*xP[i][4]);
		leds.value(LEDs::s,  base+4*xP[i][5]);
		leds.value(LEDs::o2, base+4*xP[i][6]);
	}

	return 0;
}


