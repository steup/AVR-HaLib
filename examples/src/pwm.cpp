#include <platform.h>

#include <avr-halib/drivers/avr/pwm.h>
#include <avr-halib/drivers/ext/l293e.h>

typedef avr_halib::interrupts::interrupt_manager::InterruptManager<> IM;

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
    static const pwm::Prescalers ps         = pwm::ps256;
};

typedef avr_halib::drivers::avr::PWMGenerator< PWMConfig > PWM;
typedef avr_halib::drivers::ext::L293E<platform::Motor0> LeftMotor;
typedef avr_halib::drivers::ext::L293E<platform::Motor1, true> RightMotor;

int main()
{
    PWM pwm;
    LeftMotor left;
    RightMotor right;

    IM::init();

    log::emit() << "Tick-Freq: " << PWM::TickFrequency::numerator << " / "
        << PWM::TickFrequency::denominator << log::endl;

    log::emit() << "PWM-Freq: " << PWM::DutyFrequency::numerator << " / "
        << PWM::DutyFrequency::denominator << log::endl;

    pwm.start();
    left.state(LeftMotor::forward);
    right.state(RightMotor::forward);
    pwm.value<PWM::channelA>(512);
    pwm.value<PWM::channelB>(256);

    while(true)
        Morpheus::sleep(Morpheus::SleepModes::idle);

    return 0;
}
