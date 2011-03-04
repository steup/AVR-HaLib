#pragma once

#include <avr-halib/avr/newTimer.h>

namespace avr_halib
{
namespace drivers
{
namespace helpers
{
	template<uint32_t freqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i=0, bool psOK=false>
	struct PrescalerSelector;

	template<uint32_t freqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
	struct PrescalerSelector<freqRatio, PSArray, maxI, i, false>
	{
		typedef PSArray<i> PS;

		static const bool psOK = freqRatio/PS::value<1;

		static const uint8_t nextIndex= i+((psOK)?0:1);

		typedef typename PrescalerSelector<freqRatio, PSArray, maxI, nextIndex, psOK>::type type;
	};

	template<uint32_t freqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
	struct PrescalerSelector<freqRatio, PSArray, maxI, i, true>
	{
		 typedef PSArray<i> type;
	};

	template<bool ratioTooSmall, uint32_t freqRatio, template<uint8_t> class PSArray, uint8_t maxI>
	struct PrescalerSelector<freqRatio, PSArray, maxI, maxI, ratioTooSmall>
	{
		typedef PSArray<maxI-1> type;
	};

	template<typename config>
	struct ClockConfigurator
	{
		typedef typename config::Timer 			 Timer;
		typedef typename config::TickValueType 	 TickValueType;
		typedef typename Timer::ValueType 		 MicroTickValueType;

		static const uint32_t maxNumMicroTicks = 1ULL << sizeof(typename Timer::ValueType)*8;

		static const uint32_t freqRatio = config::TimerFrequency::value  /
										  config::TargetFrequency::value /
										  maxNumMicroTicks;

		typedef typename helpers::PrescalerSelector<freqRatio, 
										   			Timer::template PSArray,
													Timer::numPS
											   			>::type SelectedPrescaler;

		typedef typename config::TargetFrequency TickFrequency;

		static const MicroTickValueType microTickMax	=
			config::TimerFrequency::value/TickFrequency::value/SelectedPrescaler::value;

		struct timerConfig : public ::avr_halib::config::TimerDefaultConfig<Timer>
		{
			enum Parameters
			{
				ocmAInt=true,
				async=!(config::TimerFrequency::value==F_CPU)
			};

			static const typename Timer::CompareMatchModes ocmAMode = Timer::noOutput;

			static const typename Timer::WaveForms   waveForm 		= Timer::ctc;
			static const typename Timer::Prescalers  ps		  		= SelectedPrescaler::code;
		};
	};

	template<typename Config>
	class ClockImpl : public Timer<typename Config::timerConfig>
	{
		public:
			struct config
			{
				typedef typename Config::TickValueType TickValueType;
				typedef typename Config::MicroTickValueType MicroTickValueType;

				struct Time
				{
					TickValueType ticks;
					MicroTickValueType microTicks;
				};

				static const MicroTickValueType microTickMax = Config::microTickMax;
				typedef typename Config::SelectedPrescaler selectedPrescaler;
			};

			typedef typename config::Time Time;

		private:
			typename config::TickValueType ticks;
			Delegate<void> callback;

		private:
			void tick()
			{
				ticks++;
				if(callback)
					callback();
			}

		public:
			ClockImpl() : ticks(0)
			{
		        this->template registerCompareMatchCallback<ClockImpl::unitA, ClockImpl, &ClockImpl::tick>(*this);
				this->template setOutputCompareValue       <ClockImpl::unitA> (config::microTickMax);
				this->start();
			}

			~ClockImpl()
			{
				this->stop();
			}

			void getTime(Time& t)
			{
				t.ticks=this->ticks;
				t.microTicks=this->getCounter();
			}

			template<typename T, void (T::*F)(void)>
			void registerCallback(T& obj)
			{
				callback.template bind<T, F>(&obj);
			}

			template<typename T, void (T::*F)(void)>
			void registerCallback(const T& obj)
			{
				callback.template bind<T, F>(&obj);
			}

			template<void (*F)(void)>
			void registerCallback()
			{
				callback.template bind<F>();
			}

			void unregisterCallback()
			{
				callback.reset();
			}
	};
}
template<typename Config>
struct Clock : public helpers::ClockImpl<helpers::ClockConfigurator<Config> >{};
}
}
