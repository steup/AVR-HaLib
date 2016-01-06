#pragma once

#include <avr-halib/config/frequency.h>
#include <avr-halib/drivers/avr/timer.h>
#include <avr-halib/drivers/avr/timer/defaultConfig.h>
#include <boost/mpl/if.hpp>
#include <boost/mpl/assert.hpp>

//#include <avr-halib/regmaps/local.h>

/** AVR-HaLib */
namespace avr_halib
{
    /** AVR-HaLib config */
    namespace config
    {
        /** \brief PWM default configuration */
        template<typename TimerRegMap>
        struct PWMDefaultConfig
        {
            /** \brief TODO \todo */
            typedef regmaps::local::pwm pwm;

            /** \brief TODO \todo */
            enum ChannelUsage {
                useChannelA=false,
                useChannelB=false,
                useChannelC=false
            };

            /** \brief TODO \todo */
            typedef TimerRegMap Timer;
            /** \brief TODO \todo */
            static const pwm::Cycle cycle           = pwm::static8;
            /** \brief TODO \todo */
            static const pwm::OutputMode outputMode = pwm::normal;
            /** \brief TODO \todo */
            static const pwm::Type correction       = pwm::fast;
            /** \brief TODO \todo */
            static const pwm::Prescalers ps         = pwm::ps256;
            /** \brief TODO \todo */

            /** \brief TODO \todo */
            struct TimerConfig : public avr_halib::config::TimerDefaultConfig<Timer>{};
        };
    }

/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** AVR-Drivers helper */
    namespace helpers
    {
        using avr_halib::config::Frequency;
        using avr_halib::drivers::avr::Timer;
        using boost::mpl::if_c;

        using regmaps::local::pwm;

        /** \brief TODO \todo */
        struct PWMTimer8Config
        {
            /** \brief TODO \todo
             *
             * \tparam InputConfig TODO \todo
             */
            template<typename InputConfig>
            struct apply
            {
                /** \brief TODO \todo */
                struct type : public InputConfig
                {
                    /** \brief TODO \todo */
                    BOOST_MPL_ASSERT_MSG( InputConfig::cycle == pwm::static8,
                        DYNAMIC_PWM_CYCLES_NOT_IMPLEMENTED_YET, ());

                    /** \brief TODO \todo */
                    struct TimerConfig : public InputConfig::TimerConfig
                    {
                        /** \brief TODO \todo */
                        typedef typename InputConfig::Timer RegMap;
                        /** \brief TODO \todo */
                        static const typename RegMap::CompareMatchModes ocmAMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
                        /** \brief TODO \todo */
                        static const typename RegMap::WaveForms waveForm = (InputConfig::correction == pwm::fast) ? RegMap::fastPWM8 : RegMap::phaseCorrectPWM8;
                        /** \brief TODO \todo */
                        static const typename RegMap::Prescalers ps = (typename RegMap::Prescalers)InputConfig::ps;
                    };

                    /** \brief TODO \todo */
                    static const uint16_t pwmMax=(1UL<<8)-1;
                };
            };
        };

        /** \brief TODO \todo */
        struct PWMTimer16Config
        {
            /** \brief TODO \todo
             *
             * \tparam InputConfig TODO \todo
             */
            template<typename InputConfig>
            struct apply
            {
                /** \brief TODO \todo */
                struct type : public InputConfig
                {
                    /** \brief TODO \todo */
                    BOOST_MPL_ASSERT_MSG(InputConfig::cycle == pwm::static8 ||
                        InputConfig::cycle == pwm::static9 ||
                        InputConfig::cycle == pwm::static10,
                        DYNAMIC_PWM_CYCLES_NOT_IMPLEMENTED_YET,
                        ());

                    /** \brief TODO \todo */
                    BOOST_MPL_ASSERT_MSG( InputConfig::correction != pwm::phaseFreqCorrect ||
                        InputConfig::cycle == pwm::dynamicOCRA ||
                        InputConfig::cycle == pwm::dynamicICR
                        ,
                        INVALID_CONFIGURATION,
                        ());

                    /** \brief TODO \todo */
                    struct TimerConfig : public InputConfig::TimerConfig
                    {
                        /** \brief TODO \todo */
                        typedef typename InputConfig::Timer RegMap;
                        /** \brief TODO \todo */
                        static const typename RegMap::CompareMatchModes ocmAMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
                        /** \brief TODO \todo */
                        static const typename RegMap::CompareMatchModes ocmBMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
                        /** \brief TODO \todo */
                        static const typename RegMap::CompareMatchModes ocmCMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
                        /** \brief TODO \todo */
                        static const typename RegMap::WaveForms waveForm = (InputConfig::correction==pwm::fast)?(
                            (InputConfig::cycle==pwm::static8)?RegMap::fastPWM8:
                            (InputConfig::cycle==pwm::static9)?RegMap::fastPWM8:
                            RegMap::fastPWM10):
                            ((InputConfig::cycle==pwm::static8)?RegMap::phaseCorrectPWM8:
                            (InputConfig::cycle==pwm::static9)?RegMap::phaseCorrectPWM9:
                            RegMap::phaseCorrectPWM10);

                        /** \brief TODO \todo */
                        static const typename RegMap::Prescalers ps = (typename RegMap::Prescalers)InputConfig::ps;
                    };

                    /** \brief TODO \todo */
                    static const uint32_t pwmMax = ((InputConfig::cycle==pwm::static8)?1UL<<8:
                        (InputConfig::cycle==pwm::static9)?1UL<<9:
                        1UL<<10)-1;
                };
            };
        };

        /** \brief TODO \todo
         *
         * \tparam InputConfig TODO \todo
         */
        template<typename InputConfig>
        struct PWMConfigurator
        {
            /** \brief TODO \todo */
            typedef typename  if_c< (sizeof(typename InputConfig::Timer::ValueType) > 1),
                PWMTimer16Config,
                PWMTimer8Config
                >::type Configurator;
            /** \brief TODO \todo */
            typedef typename Configurator::template apply<InputConfig>::type type;
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         * \tparam usage TODO \todo
         */
        template<typename RegMap, typename  Base, bool usage>
        struct ChannelA;

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename Base>
        struct ChannelA<RegMap, Base, true>
        {
            /** \brief TODO \todo */
            enum Channels
            {
                channelA=Base::Units::matchA
            };

            /** \brief TODO \todo */
            ChannelA()
            {
                UseRegMap(rm, RegMap);
                //SyncRegMap(rm);
                rm.ocmAOutput=true;
                //SyncRegMap(rm);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename Base>
        struct ChannelA<RegMap, Base, false>{};

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         * \tparam usage TODO \todo
         */
        template<typename RegMap, typename  Base, bool usage>
        struct ChannelB;

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename Base>
        struct ChannelB<RegMap, Base, true>
        {
            /** \brief TODO \todo */
            BOOST_MPL_ASSERT_MSG( RegMap::Parameters::numOCU >= 2,
                TIMER_HAS_NOT_ENOUGH_OCUs,
                ());

            /** \brief TODO \todo */
            enum Channels
            {
                channelB=Base::Units::matchB
            };

            /** \brief TODO \todo */
            ChannelB()
            {
                UseRegMap(rm, RegMap);
                //SyncRegMap(rm);
                rm.ocmBOutput=true;
                //SyncRegMap(rm);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename Base>
        struct ChannelB<RegMap, Base, false>{};

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         * \tparam usage TODO \todo
         */
        template<typename RegMap, typename  Base, bool usage>
        struct ChannelC;

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename Base>
        struct ChannelC<RegMap, Base, true>
        {
            /** \brief TODO \todo */
            BOOST_MPL_ASSERT_MSG( RegMap::Parameters::numOCU>=3,
                TIMER_HAS_NOT_ENOUGH_OCUs,
                ());

            /** \brief TODO \todo */
            enum Channels
            {
                channelC=Base::Units::matchC
            };

            /** \brief TODO \todo */
            ChannelC()
            {
                UseRegMap(rm, RegMap);
                //SyncRegMap(rm);
                rm.ocmCOutput=true;
                //SyncRegMap(rm);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam Base TODO \todo
         */
        template<typename RegMap, typename  Base>
        struct ChannelC<RegMap, Base, false>{};

        /** \brief TODO \todo
         *
         * \tparam config TODO \todo
         */
        template<typename config>
        struct PWMBase : public Timer< typename config::TimerConfig >,
            public ChannelA< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelA >,
            public ChannelB< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelB >,
            public ChannelC< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelC >
        {
            private:
                /** \brief TODO \todo */
                typedef typename config::Timer RegMap;
                /** \brief TODO \todo */
                typedef typename regmaps::local::pwm pwm;
                /** \brief TODO \todo */
                typedef Frequency<1> SingleSlopeFrequency;
                /** \brief TODO \todo */
                typedef Frequency<1, 2> DualSlopeFrequency;

            public:
                /** \brief TODO \todo */
                typedef Timer< typename config::TimerConfig > BaseTimer;

                /** \brief TODO \todo */
                typedef typename if_c< config::correction == pwm::fast,
                    SingleSlopeFrequency,
                    DualSlopeFrequency
                    >::type SlopeFrequency;

                /** \brief TODO \todo */
                typedef Frequency<1, config::pwmMax> PWMMaxFreq;
                /** \brief TODO \todo */
                typedef Frequency<1, config::Timer::template PSValue<config::ps>::value > PrescalerFrequency;
                /** \brief TODO \todo */
                static const uint32_t pwmMax=config::pwmMax;
                /** \brief TODO \todo */
                typedef typename BaseTimer::InputFrequency::template mult<PrescalerFrequency>::type TickFrequency;
                /** \brief TODO \todo */
                typedef typename TickFrequency::template mult<SlopeFrequency>::type::template mult<PWMMaxFreq>::type DutyFrequency;
                /** \brief TODO \todo */
                typedef DutyFrequency OverflowFrequency;
                /** \brief TODO \todo */
                typedef typename BaseTimer::ValueType ValueType;

                /** \brief TODO \todo */
                enum Channels
                {
                    channelA=BaseTimer::Units::matchA
                };

                /** \brief TODO \todo */
                PWMBase()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    start();
                }

                /** \brief TODO \todo
                 *
                 * \tparam chan TODO \todo
                 * \param value TODO \todo
                 */
                template<uint8_t chan>
                void value(ValueType value)
                {
        /*          BOOST_MPL_ASSERT_MSG(chan<RegMap::numOCU, NO_SUCH_OCU, ());
                  BOOST_MPL_ASSERT_MSG( !( !config::useChannelB && chan==BaseTimer::matchB ) &&
                              !( !config::useChannelC && chan==BaseTimer::matchC ),
                              OCU_NOT_AVAILABLE,
                              ());*/
                    static const typename BaseTimer::UnitType unit=(typename BaseTimer::UnitType)(chan);
                    this->template setOutputCompareValue<unit>(value);
                }

                /** \brief TODO \todo
                 *
                 * \tparam chan TODO \todo
                 */
                template<uint8_t chan>
                ValueType value() const
                {
        /*          BOOST_MPL_ASSERT_MSG(chan<RegMap::numOCU, NO_SUCH_OCU, ());
                  BOOST_MPL_ASSERT_MSG( !( !config::useChannelB && chan==BaseTimer::matchB ) &&
                              !( !config::useChannelC && chan==BaseTimer::matchC ),
                              OCU_NOT_AVAILABLE,
                              ());*/
                    static const typename BaseTimer::UnitType unit=(typename BaseTimer::UnitType)(chan);
                    return this->template getOutputCompareValue<unit>();
                }

                /** \brief TODO \todo */
                void start()
                {
                    this->BaseTimer::start();
                }

                /** \brief TODO \todo */
                void stop()
                {
                    this->BaseTimer::stop();
                }
        };

    }

    template<typename config>
    struct PWMGenerator : public helpers::PWMBase<
        typename helpers::PWMConfigurator<
        config >::type >{};
}
}
}
