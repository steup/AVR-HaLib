#include <config.h>

#include <avr-halib/ext/motorControl.h>

struct RobbyMotorConfig
{
	typedef int16_t SpeedType;

	typedef avr_halib::regmaps::local::pwm pwm;

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

typedef drivers::external::RobbyMotorControl<RobbyMotorConfig> Motor;

typedef InterruptManager<Motor::InterruptSlotList, false> IM;

Motor motor;

int main()
{
	IM::init();

	sei();

	motor.speed<Motor::left>(100);
	motor.speed<Motor::right>(100);

	while(true)
	{
		Morpheus::sleep(power::idle);
		log::emit() << "Speed    [left] : " << motor.speed<Motor::left>() << log::endl;
		log::emit() << "Speed   [right] : " << motor.speed<Motor::right>() << log::endl;
		log::emit() << "Control  [left] : " << motor.currentControlValue<Motor::left>() << log::endl;
		log::emit() << "Control [right] : " << motor.currentControlValue<Motor::left>() << log::endl;
	}

	return 0;
}

