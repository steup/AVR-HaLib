#pragma once

#include <avr-halib/avr/pwm.h>
#include <avr-halib/ext/odo.h>
#include <avr-halib/ext/l293e.h>
#include <avr-halib/share/pid.h>

#include <avr-halib/portmaps/robbyboard.h>
#include <avr-halib/regmaps/local.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace drivers
{
namespace external
{
	template<typename config>
	class RobbyMotorControl
	{
		struct PWMConfig : public config::PWMConfig
		{
			enum{
				useChannelA = true,
				useChannelB = true,
				useChannelC = false
			};

			typedef regmaps::local::Timer1 Timer;

			struct TimerConfig : public avr_halib::config::PWMDefaultConfig<Timer>::TimerConfig
			{
				enum{
					overflowInt = true
				};
			};

			static const Timer::Prescalers ps = (Timer::Prescalers)config::PWMConfig::ps;

			
		};

		typedef PWMGenerator<PWMConfig> PWM;

		struct OdoConfigLeft : public config::OdoConfig
		{
			typedef regmaps::local::ExternalInterrupt4 TickSource;
			typedef Delegate<void> EvalDelegate;
			typedef typename PWM::DutyFrequency EvalFrequency;
		};

		struct OdoConfigRight : public config::OdoConfig
		{
			typedef regmaps::local::ExternalInterrupt5 TickSource;
			typedef Delegate<void> EvalDelegate;
			typedef typename PWM::DutyFrequency EvalFrequency;
		};

		struct PIDConfig : public config::PIDConfig
		{
			static const float max = PWM::pwmMax;
			static const float min = 0;
		};

		typedef OdometrieSensor<OdoConfigLeft> OdoLeft;
		typedef OdometrieSensor<OdoConfigRight> OdoRight;

		typedef L293E<Motor0, false> DriverLeft;
		typedef L293E<Motor1, true> DriverRight;

		typedef object::PIDControl<PIDConfig> PID;
		
		typedef typename config::SpeedType SpeedType;
		
		PID pid[2];
		OdoLeft odoLeft;
		OdoRight odoRight;
		PWM pwm;
		DriverLeft driverLeft;
		DriverRight driverRight;

		private:
			void control()
			{
				odoLeft.eval();
				odoRight.eval();

				SpeedType umin=odoLeft.getValue();
				SpeedType controlSpeed=pid[left].control(umin);
				pwm.template value<PWM::channelA>(controlSpeed);

				umin=odoRight.getValue();
				controlSpeed=pid[right].control(umin);
				pwm.template value<PWM::channelB>(controlSpeed);
			}

/*			typedef boost::mpl::insert_range< typename PWM::InterruptSlotList, 
											  typename boost::mpl::end< typename PWM::InterruptSlotList >::type,
											  typename OdoLeft::InterruptSlotList > L1;

			typedef boost::mpl::insert_range<         L1, 
											 typename boost::mpl::end< L1 >::type,
											 typename OdoRight::InterruptSlotList > L2;
*/
		public:
			
			typedef typename boost::mpl::vector< typename OdoLeft::InterruptMap::ExternalInterruptSlot,
												 typename OdoRight::InterruptMap::ExternalInterruptSlot,
												 typename PWM::InterruptMap::OverflowSlot
									  			>::type InterruptSlotList;

			enum Wheels
			{
					left,
					right
			};

			RobbyMotorControl()
			{
				PWM::InterruptMap::OverflowSlot::template bind<RobbyMotorControl, &RobbyMotorControl::control>(this);
			}

			template<Wheels wheel>
			void speed(const SpeedType& speed)
			{
				switch(wheel)
				{
					case(left): if(speed<0)
								{
									driverLeft.state(DriverLeft::backward);
									pid[left].target(-speed);
								}
								else
								{
									driverLeft.state(DriverLeft::forward);
									pid[left].target(speed);
								}
								break;
								
					case(right):if(speed<0)
								{
									driverRight.state(DriverRight::backward);
									pid[right].target(-speed);
								}
								else
								{
									driverRight.state(DriverRight::forward);
									pid[right].target(speed);
								}
								break;
				}
			}

			template<Wheels wheel>
			SpeedType speed() const
			{
				switch(wheel)
				{
					case(left): if(driverLeft.state() == DriverLeft::backward)
									return -odoLeft.getValue();
								else
									return odoLeft.getValue();

					case(right):if(driverRight.state() == DriverRight::backward)
									return -odoRight.getValue();
								else
									return odoRight.getValue();

					default: return 0;
				}
			}

			template<Wheels wheel>
			const typename PID::CalcType& currentControlValue() const
			{
				return pid[wheel].current();
			}
	};
}
}
}
