/** \addtogroup ext */
/*@{*/
/**
 *	\file		include/avr-halib/ext/motorControl.h
 *	\brief	Defines RobbyMotorControl
 *	\author	insert author here
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <avr-halib/avr/pwm.h>
#include <avr-halib/ext/odo.h>
#include <avr-halib/ext/l293e.h>
#include <avr-halib/common/pid.h>
#include <avr-halib/common/delegate.h>

#include <avr-halib/regmaps/local.h>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace drivers
{
namespace external
{
	/**
	*
	*	\brief	Robby motor control class
	*	\tparam	config Struct that configures motor control
	*/
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

		struct OdoConfigRight : public config::OdoConfig
		{
			typedef regmaps::local::ExternalInterrupt2 TickSource;
			typedef Delegate<void> EvalDelegate;
			typedef typename PWM::DutyFrequency EvalFrequency;
		};

		struct OdoConfigLeft : public config::OdoConfig
		{
			typedef regmaps::local::ExternalInterrupt3 TickSource;
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

		typedef L293E<platform::Motor0, false^config::leftInverse> DriverLeft;
		typedef L293E<platform::Motor1, true^config::rightInverse> DriverRight;

		typedef object::PIDControl<PIDConfig> PID;
		
		typedef typename config::SpeedType SpeedType;
		
		PID pid[2];
		OdoLeft odoLeft;
		OdoRight odoRight;
		PWM pwm;
		DriverLeft driverLeft;
		DriverRight driverRight;
		int16_t tickValues[2];

		static const int8_t lMod=config::leftInverse?-1:1;
		static const int8_t rMod=config::rightInverse?-1:1;

		private:
			void control()
			{
				odoLeft.eval();
				odoRight.eval();
				sei();
				if(driverLeft.state() == DriverLeft::backward)
					tickValues[0]+=-(lMod*odoLeft.getTicks());
				else
					tickValues[0]+=lMod*odoLeft.getTicks();

				if(driverRight.state() == DriverRight::backward)
					tickValues[1]+=-(rMod*odoRight.getTicks());
				else
					tickValues[1]+=rMod*odoRight.getTicks();

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

			typedef boost::mpl::insert_range<				 L1,
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
				pwm.template registerCallback<PWM::InterruptMap::overflow, RobbyMotorControl, &RobbyMotorControl::control>(*this);
			}

			/**
			 *	\brief Set speed of selected wheel
			 *	\tparam wheel Wheel to change speed of
			 *	\param speed Speed for the wheel
			 */
			template<Wheels wheel>
			void speed(const SpeedType& speed)
			{
				if(speed == 0)
				{
					switch(wheel)
					{
						case(left): driverLeft.state(DriverLeft::freeRunning);
									pid[left].target(0);
									break;
						case(right):driverRight.state(DriverRight::freeRunning);
									pid[right].target(0);
									break;
					}
					return;
				}

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
									return -(lMod*odoLeft.getValue());
								else
									return lMod*odoLeft.getValue();

					case(right):if(driverRight.state() == DriverRight::backward)
									return -(rMod*odoRight.getValue());
								else
									return rMod*odoRight.getValue();

					default: return 0;
				}
			}

			template<Wheels wheel>
			SpeedType targetSpeed() const
			{
				return pid[wheel].target();
			}

			template<Wheels wheel>
			const typename PID::CalcType& currentControlValue() const
			{
				return pid[wheel].current();
			}

			template<Wheels wheel>
			SpeedType ticks()
			{
				SpeedType temp=tickValues[wheel];
				tickValues[wheel]=0;
				return temp;
			}
	};
}
}
}

/*@}*/
