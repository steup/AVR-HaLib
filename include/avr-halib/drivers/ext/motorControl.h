#pragma once

#include <avr-halib/drivers/avr/pwm.h>
#include <avr-halib/drivers/ext/odo.h>
#include <avr-halib/drivers/ext/l293e.h>
#include <avr-halib/common/pid.h>
#include <avr-halib/common/delegate.h>

#include <avr-halib/regmaps/local.h>
#include <boost/mpl/vector.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** External-Drivers */
namespace ext
{
    /** \brief Robby motor control class
     *
     * \tparam config Struct that configures motor control
     */
    template<typename config>
    class RobbyMotorControl
    {
        /** \brief TODO \todo */
        struct PWMConfig : public config::PWMConfig
        {
            /** \brief TODO \todo */
            enum
            {
                useChannelA = true,
                useChannelB = true,
                useChannelC = false
            };

            /** \brief TODO \todo */
            typedef regmaps::local::Timer1 Timer;

            /** \brief TODO \todo */
            struct TimerConfig : public avr_halib::config::PWMDefaultConfig<Timer>::TimerConfig
            {
                /** \brief TODO \todo */
                enum
                {
                  overflowInt = true
                };
            };

            /** \brief TODO \todo */
            static const Timer::Prescalers ps = (Timer::Prescalers)config::PWMConfig::ps;
        };

        /** \brief TODO \todo */
        typedef PWMGenerator<PWMConfig> PWM;

        /** \brief TODO \todo */
        struct OdoConfigRight : public config::OdoConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::ExternalInterrupt2 TickSource;
            /** \brief TODO \todo */
            typedef avr_halib::common::Delegate<void> EvalDelegate;
            /** \brief TODO \todo */
            typedef typename PWM::DutyFrequency EvalFrequency;
        };

        /** \brief TODO \todo */
        struct OdoConfigLeft : public config::OdoConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::ExternalInterrupt3 TickSource;
            /** \brief TODO \todo */
            typedef avr_halib::common::Delegate<void> EvalDelegate;
            /** \brief TODO \todo */
            typedef typename PWM::DutyFrequency EvalFrequency;
        };

        /** \brief TODO \todo */
        struct PIDConfig : public config::PIDConfig
        {
            /** \brief TODO \todo */
            static const float max = PWM::pwmMax;
            /** \brief TODO \todo */
            static const float min = 0;
        };

        /** \brief TODO \todo */
        typedef OdometrieSensor<OdoConfigLeft> OdoLeft;
        /** \brief TODO \todo */
        typedef OdometrieSensor<OdoConfigRight> OdoRight;

        /** \brief TODO \todo */
        typedef L293E<platform::Motor0, false^config::leftInverse> DriverLeft;
        /** \brief TODO \todo */
        typedef L293E<platform::Motor1, true^config::rightInverse> DriverRight;

        /** \brief TODO \todo */
        typedef object::PIDControl<PIDConfig> PID;

        /** \brief TODO \todo */
        typedef typename config::SpeedType SpeedType;

        /** \brief TODO \todo */
        PID pid[2];
        /** \brief TODO \todo */
        OdoLeft odoLeft;
        /** \brief TODO \todo */
        OdoRight odoRight;
        /** \brief TODO \todo */
        PWM pwm;
        /** \brief TODO \todo */
        DriverLeft driverLeft;
        /** \brief TODO \todo */
        DriverRight driverRight;
        /** \brief TODO \todo */
        int16_t tickValues[2];

        /** \brief TODO \todo */
        static const int8_t lMod=config::leftInverse?-1:1;
        /** \brief TODO \todo */
        static const int8_t rMod=config::rightInverse?-1:1;

        private:
            /** \brief TODO \todo */
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

            /* typedef boost::mpl::insert_range< typename PWM::InterruptSlotList,
                typename boost::mpl::end< typename PWM::InterruptSlotList >::type,
                typename OdoLeft::InterruptSlotList > L1;

            typedef boost::mpl::insert_range<L1,
                typename boost::mpl::end< L1 >::type,
                typename OdoRight::InterruptSlotList > L2;*/
        public:
            /** \brief TODO \todo */
            typedef typename boost::mpl::vector< typename OdoLeft::InterruptMap::ExternalInterruptSlot,
                typename OdoRight::InterruptMap::ExternalInterruptSlot,
                typename PWM::InterruptMap::OverflowSlot
                >::type InterruptSlotList;

            /** \brief TODO \todo */
            enum Wheels
            {
                left,
                right
            };

            /** \brief TODO \todo */
            RobbyMotorControl()
            {
                pwm.template registerCallback<PWM::InterruptMap::overflow, RobbyMotorControl, &RobbyMotorControl::control>(*this);
            }

            /** \brief Set speed of selected wheel
             *
             * \tparam wheel Wheel to change speed of
             * \param speed Speed for the wheel
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
                    case(left):
                        if(speed<0)
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

                    case(right):
                        if(speed<0)
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

            /** \brief Get speed of selected wheel
             *
             * \tparam wheel Wheel to get speed of
             */
            template<Wheels wheel>
            SpeedType speed() const
            {
                switch(wheel)
                {
                    case(left):
                        if(driverLeft.state() == DriverLeft::backward)
                            return -(lMod*odoLeft.getValue());
                        else
                            return lMod*odoLeft.getValue();

                    case(right):
                        if(driverRight.state() == DriverRight::backward)
                            return -(rMod*odoRight.getValue());
                        else
                            return rMod*odoRight.getValue();

                    default:
                        return 0;
                }
            }

            /** \brief TODO \todo
             *
             * \tparam wheel Wheel to get speed of
             */
            template<Wheels wheel>
            SpeedType targetSpeed() const
            {
                return pid[wheel].target();
            }

            /** \brief TODO \todo
             *
             * \tparam wheel Wheel to get speed of
             */
            template<Wheels wheel>
            const typename PID::CalcType& currentControlValue() const
            {
                return pid[wheel].current();
            }

            /** \brief TODO \todo
             *
             * \tparam wheel Wheel to get speed of
             */
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
