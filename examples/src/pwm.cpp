#include <config.h>

#include <avr-halib/avr/pwm.h>

typedef Interrupt::InterruptManager<> IM;

typedef avr_halib::regmaps::local::Timer1 PWMTimer;

struct PWMConfig : public avr_halib::config::PWMDefaultConfig< PWMTimer >
{
	enum ChannelUsage
	{
		useChannelA = true,
		useChannelB = true,
		useChannelC = false
	};

	static const pwm::Cycle      cycle      = pwm::static10;
	static const pwm::OutputMode outputMode = pwm::normal;
	static const pwm::Type       correction = pwm::phaseCorrect;
	static const pwm::Prescalers ps      	= pwm::ps256;
};

typedef avr_halib::drivers::PWMGenerator< PWMConfig > PWM;


int main()
{
	PWM pwm;

	IM::init();

	log::emit() << "Tick-Freq: " << PWM::TickFrequency::numerator << " / " 
				<< PWM::TickFrequency::denominator << log::endl;

	log::emit() << "PWM-Freq: " << PWM::DutyFrequency::numerator << " / " 
				<< PWM::DutyFrequency::denominator << log::endl;

	pwm.start();
	pwm.value<PWM::channelA>(512);
	pwm.value<PWM::channelB>(256);

	while(true)
		Morpheus::sleep(Morpheus::SleepModes::idle);

	return 0;
}
