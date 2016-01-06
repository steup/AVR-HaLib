#include "platform.h"

#include <avr-halib/drivers/ext/motorControl.h>

struct RobbyMotorConfig
{
    typedef int16_t SpeedType;

    typedef avr_halib::regmaps::local::pwm pwm;

    static const bool leftInverse=false;
    static const bool rightInverse=false;

    struct PWMConfig
    {
        static const pwm::Cycle      cycle      = pwm::static10;
        static const pwm::OutputMode outputMode = pwm::normal;
        static const pwm::Type       correction = pwm::phaseCorrect;
        static const pwm::Prescalers ps         = pwm::ps256;
    };

    struct PIDConfig
    {
        typedef int32_t BaseType;
        static const uint8_t exponent=8;
        static const BaseType kp=1<<8;
        static const BaseType kd=0;
        static const BaseType ki=0;
    };

    struct OdoConfig
    {
        typedef uint16_t CounterType;
        static const CounterType ticksPerTurn=120;
    };
};

typedef avr_halib::drivers::ext::RobbyMotorControl<RobbyMotorConfig> Motor;

typedef avr_halib::interrupts::interrupt_manager::InterruptManager<Motor::InterruptSlotList> IM;

BIND_INTERRUPTS(IM);

Motor motor;

int main()
{
    sei();

    motor.speed<Motor::left>(100);
    motor.speed<Motor::right>(100);

    while(true)
    {
        Morpheus::sleep(Morpheus::SleepModes::idle);
        log::emit() << "Speed    [left] : " << motor.speed<Motor::left>() << log::endl;
        log::emit() << "Speed   [right] : " << motor.speed<Motor::right>() << log::endl;
        log::emit() << "Control  [left] : " << motor.currentControlValue<Motor::left>() << log::endl;
        log::emit() << "Control [right] : " << motor.currentControlValue<Motor::right>() << log::endl;
    }

    return 0;
}
