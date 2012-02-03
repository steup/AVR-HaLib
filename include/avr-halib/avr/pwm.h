#pragma once

#include <avr-halib/share/freq.h>
#include <avr-halib/avr/newTimer.h>
#include <avr-halib/avr/timer/defaultConfig.h>
#include <boost/mpl/if.hpp>

#include <avr-halib/regmaps/local.h>

namespace avr_halib
{
namespace config
{

	template<typename TimerRegMap>
	struct PWMDefaultConfig
	{
		typedef regmaps::local::pwm pwm;

		enum ChannelUsage{
			useChannelA=false,
			useChannelB=false,
			useChannelC=false
		};

		typedef TimerRegMap Timer;
		static const pwm::Cycle cycle           = pwm::static8;
		static const pwm::OutputMode outputMode = pwm::normal;
		static const pwm::Type correction       = pwm::fast;
		static const pwm::Prescalers ps         = pwm::ps256;

		struct TimerConfig : public avr_halib::config::TimerDefaultConfig<Timer>{};
	};

}
namespace drivers
{
	namespace helpers
	{
		using avr_halib::config::Frequency;
		using avr_halib::drivers::Timer;
		using boost::mpl::if_c;

		using regmaps::local::pwm;

		struct PWMTimer8Config
		{
			template<typename InputConfig>
			struct apply
			{
				struct type : public InputConfig
				{
					BOOST_MPL_ASSERT_MSG( InputConfig::cycle == pwm::static8 ,
									      DYNAMIC_PWM_CYCLES_NOT_IMPLEMENTED_YET, 
										  ());

					struct TimerConfig : public InputConfig::TimerConfig
					{
						typedef typename InputConfig::Timer RegMap;
						static const typename RegMap::CompareMatchModes ocmAMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
						static const typename RegMap::WaveForms waveForm = (InputConfig::correction == pwm::fast) ? RegMap::fastPWM8 : RegMap::phaseCorrectPWM8;
						static const typename RegMap::Prescalers ps = (typename RegMap::Prescalers)InputConfig::ps;

					};

					static const uint16_t pwmMax=(1UL<<8)-1;
				};
			};
		};


		struct PWMTimer16Config
		{
			template<typename InputConfig>
			struct apply
			{
				struct type : public InputConfig
							  
				{
					BOOST_MPL_ASSERT_MSG(InputConfig::cycle == pwm::static8 || 
										 InputConfig::cycle == pwm::static9 || 
										 InputConfig::cycle == pwm::static10, 
										 DYNAMIC_PWM_CYCLES_NOT_IMPLEMENTED_YET, 
										 ());

					BOOST_MPL_ASSERT_MSG( InputConfig::correction != pwm::phaseFreqCorrect ||
										  InputConfig::cycle == pwm::dynamicOCRA ||
										  InputConfig::cycle == pwm::dynamicICR
										  , 
										 INVALID_CONFIGURATION,
										 ());

					struct TimerConfig : public InputConfig::TimerConfig
					{
						typedef typename InputConfig::Timer RegMap;
						static const typename RegMap::CompareMatchModes ocmAMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
						static const typename RegMap::CompareMatchModes ocmBMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
						static const typename RegMap::CompareMatchModes ocmCMode = (typename RegMap::CompareMatchModes)InputConfig::outputMode;
						static const typename RegMap::WaveForms waveForm = (InputConfig::correction==pwm::fast)?(
																				(InputConfig::cycle==pwm::static8)?RegMap::fastPWM8:
																					(InputConfig::cycle==pwm::static9)?RegMap::fastPWM8:
																						RegMap::fastPWM10):
																		        ((InputConfig::cycle==pwm::static8)?RegMap::phaseCorrectPWM8:
																					(InputConfig::cycle==pwm::static9)?RegMap::phaseCorrectPWM9:
																						RegMap::phaseCorrectPWM10);
																				

						static const typename RegMap::Prescalers ps = (typename RegMap::Prescalers)InputConfig::ps;
					};

					static const uint32_t pwmMax = ((InputConfig::cycle==pwm::static8)?1UL<<8:
														(InputConfig::cycle==pwm::static9)?1UL<<9:
															1UL<<10)-1;
				};
			};
		};

		template<typename InputConfig>
		struct PWMConfigurator
		{
			
			typedef typename  if_c< (sizeof(typename InputConfig::Timer::ValueType) > 1) ,
									 PWMTimer16Config, 
									 PWMTimer8Config
								  >::type Configurator;
			typedef typename Configurator::template apply<InputConfig>::type type;
		};

		template<typename RegMap, typename  Base, bool usage>
		struct ChannelA;

		template<typename RegMap, typename  Base>
		struct ChannelA<RegMap, Base, true>
		{
			enum Channels
			{
				channelA=Base::matchA
			};

			ChannelA()
			{
				UseRegMap(rm, RegMap);
//				SyncRegMap(rm);
				rm.ocmAOutput=true;
//				SyncRegMap(rm);
			}
		};

		template<typename RegMap, typename  Base>
		struct ChannelA<RegMap, Base, false>{};

		template<typename RegMap, typename  Base, bool usage>
		struct ChannelB;

		template<typename RegMap, typename  Base>
		struct ChannelB<RegMap, Base, true>
		{
			BOOST_MPL_ASSERT_MSG( RegMap::numOCU >= 2,
								  TIMER_HAS_NOT_ENOUGH_OCUs,
								  ());

			enum Channels
			{
				channelB=Base::matchB
			};

			ChannelB()
			{
				UseRegMap(rm, RegMap);
//				SyncRegMap(rm);
				rm.ocmBOutput=true;
//				SyncRegMap(rm);
			}
		};

		template<typename RegMap, typename  Base>
		struct ChannelB<RegMap, Base, false>{};

		template<typename RegMap, typename  Base, bool usage>
		struct ChannelC;

		template<typename RegMap, typename  Base>
		struct ChannelC<RegMap, Base, true>
		{
			BOOST_MPL_ASSERT_MSG( RegMap::numOCU>=3, 
					              TIMER_HAS_NOT_ENOUGH_OCUs, 
								  ());

			enum Channels
			{
				channelC=Base::matchC
			};

			ChannelC()
			{
				
				UseRegMap(rm, RegMap);
//				SyncRegMap(rm);
				rm.ocmCOutput=true;
//				SyncRegMap(rm);
			}
		};

		template<typename RegMap, typename  Base>
		struct ChannelC<RegMap, Base, false>{};

		template<typename config>
		struct PWMBase : public Timer< typename config::TimerConfig >,
						 public ChannelA< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelA >,
						 public ChannelB< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelB >,
						 public ChannelC< typename config::Timer, Timer< typename config::TimerConfig >, config::useChannelC >
		{
			private:
				
				typedef typename config::Timer RegMap;
				typedef typename regmaps::local::pwm pwm;
				

				typedef Frequency<1> SingleSlopeFrequency;
				typedef Frequency<1, 2> DualSlopeFrequency;

				

			public:
				typedef Timer< typename config::TimerConfig > BaseTimer;

				typedef typename if_c< config::correction == pwm::fast,
									   SingleSlopeFrequency,
									   DualSlopeFrequency 
									 >::type SlopeFrequency;

				typedef Frequency<1, config::pwmMax> PWMMaxFreq;

				typedef Frequency<1, config::Timer::template PSValue<config::ps>::value > PrescalerFrequency;

				static const uint32_t pwmMax=config::pwmMax;

				typedef typename BaseTimer::InputFrequency::template mult<PrescalerFrequency>::type TickFrequency;

				typedef typename TickFrequency::template mult<SlopeFrequency>::type::template mult<PWMMaxFreq>::type DutyFrequency;
				typedef DutyFrequency OverflowFrequency;

				typedef typename BaseTimer::ValueType ValueType;

				enum Channels
				{
					channelA=BaseTimer::matchA
				};

				PWMBase()
				{
					UseRegMap(rm, RegMap);
					SyncRegMap(rm);
					start();
				}

				template<uint8_t chan>
				void value(ValueType value)
				{
/*					BOOST_MPL_ASSERT_MSG(chan<RegMap::numOCU, NO_SUCH_OCU, ());
					BOOST_MPL_ASSERT_MSG( !( !config::useChannelB && chan==BaseTimer::matchB ) &&
										  !( !config::useChannelC && chan==BaseTimer::matchC ), 
										  OCU_NOT_AVAILABLE, 
										  ());*/
					static const typename BaseTimer::OutputCompareUnits unit=(typename BaseTimer::OutputCompareUnits)(chan);
					this->template setOutputCompareValue<unit>(value);
				}

				template<uint8_t chan>
				ValueType value() const
				{
/*					BOOST_MPL_ASSERT_MSG(chan<RegMap::numOCU, NO_SUCH_OCU, ());
					BOOST_MPL_ASSERT_MSG( !( !config::useChannelB && chan==BaseTimer::matchB ) &&
										  !( !config::useChannelC && chan==BaseTimer::matchC ), 
										  OCU_NOT_AVAILABLE, 
										  ());*/
					static const typename BaseTimer::OutputCompareUnits unit=(typename BaseTimer::OutputCompareUnits)(chan);
					return this->template getOutputCompareValue<unit>();
				}

				void start()
				{
					this->BaseTimer::start();
				}

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
