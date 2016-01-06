#include <platform.h>

#include <avr-halib/drivers/avr/pwm.h>
#include <avr-halib/interrupts/interrupt.h>

typedef avr_halib::interrupts::interrupt_manager::InterruptManager<> IM;

using avr_halib::regmaps::local::Timer1;
using avr_halib::regmaps::local::Timer2;
using avr_halib::config::PWMDefaultConfig;
using avr_halib::drivers::avr::PWMGenerator;

struct PWM1Config : public PWMDefaultConfig< Timer1 >
{
    enum ChannelUsage
    {
        useChannelA = true,
        useChannelB = true,
        useChannelC = true
    };

    static const pwm::Cycle      cycle      = pwm::static10;
    static const pwm::OutputMode outputMode = pwm::normal;
    static const pwm::Type       correction = pwm::phaseCorrect;
    static const pwm::Prescalers ps         = pwm::ps64;
};

struct PWM2Config : public PWMDefaultConfig< Timer2 >
{
    enum ChannelUsage
    {
        useChannelA = true,
				useChannelB = false,
				useChannelC = false
    };

    static const pwm::Cycle      cycle      = pwm::static8;
    static const pwm::OutputMode outputMode = pwm::normal;
    static const pwm::Type       correction = pwm::fast;
    static const pwm::Prescalers ps         = pwm::ps256;
};

typedef PWMGenerator< PWM2Config > PWM2;
typedef PWMGenerator< PWM1Config > PWM1;

BIND_INTERRUPTS(IM);

int main()
{
    PWM2 pwm2;
    PWM1 pwm1;

    log::emit() << "PWM2 Tick-Freq: " << PWM2::TickFrequency::numerator << " / "
        << PWM2::TickFrequency::denominator << log::endl;

    log::emit() << "PWM2 PWM-Freq: " << PWM2::DutyFrequency::numerator << " / "
        << PWM2::DutyFrequency::denominator << log::endl;
    
		log::emit() << "PWM1 Tick-Freq: " << PWM1::TickFrequency::numerator << " / "
        << PWM1::TickFrequency::denominator << log::endl;

    log::emit() << "PWM1 PWM-Freq: " << PWM1::DutyFrequency::numerator << " / "
        << PWM1::DutyFrequency::denominator << log::endl;

    pwm1.start();
    pwm1.value<PWM1::channelA>(512);
    pwm1.value<PWM1::channelB>(256);
    pwm1.value<PWM1::channelC>(1000);
    
		pwm2.start();
    pwm2.value<PWM2::channelA>(128);

    while(true)
        Morpheus::sleep(Morpheus::SleepModes::idle);

    return 0;
}
