#pragma once

#include <avr-halib/avr/timer.h>
#include <avr-halib/common/delegate.h>
#include <avr-halib/common/frequency.h>
#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/avr/InterruptManager/Slot.h>
#include <avr-halib/avr/InterruptManager/InterruptBinding.h>

namespace avr_halib
{
namespace drivers
{
namespace helpers
{
	using avr_halib::config::Frequency;
    using ::Interrupt::Slot;
    using ::Interrupt::Binding;

	template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i=0, bool psOK=false>
	struct PrescalerSelector;

	template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
	struct PrescalerSelector<FreqRatio, PSArray, maxI, i, false>
	{
		typedef PSArray<i> PS;

		static const bool psOK = FreqRatio::template div< Frequency<PS::value> >::type::value < 1;

		static const uint8_t nextIndex= i+((psOK)?0:1);

		typedef typename PrescalerSelector<FreqRatio, PSArray, maxI, nextIndex, psOK>::type type;
	};

	template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
	struct PrescalerSelector<FreqRatio, PSArray, maxI, i, true>
	{
		 typedef PSArray<i> type;
	};

	template<bool ratioTooSmall, typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI>
	struct PrescalerSelector<FreqRatio, PSArray, maxI, maxI, ratioTooSmall>
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

		typedef typename config::TimerFrequency::template div< 
                typename config::TargetFrequency>::type FreqRatio;
        typedef typename FreqRatio::template div<
                         Frequency<maxNumMicroTicks> >::type 
                TargetRatio;

		typedef typename helpers::PrescalerSelector<TargetRatio, 
										   			Timer::template PSArray,
													Timer::Parameters::numPS
											   			>::type SelectedPrescaler;

		typedef typename config::TargetFrequency TickFrequency;

		static const MicroTickValueType microTickMax	=
			FreqRatio::template div< Frequency<SelectedPrescaler::value> >::type::value;

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
				typedef typename Config::TickFrequency Frequency;
			};

			typedef typename config::Time Time;

		private:
			typename config::TickValueType ticks;
			Delegate<void> callback;
            static ClockImpl* instance;

			typedef Timer<typename Config::timerConfig> Base;

		private:
			static void tick()
			{
				instance->ticks++;
				instance->callback();
			}

		public:

            typedef typename boost::mpl::vector< typename Slot< Config::Timer::InterruptMap::matchA, Binding::FixedPlainFunction >::template Bind< &ClockImpl::tick > >::type InterruptSlotList;

			ClockImpl() : ticks(0)
			{
                instance = this;
				this->template setOutputCompareValue<ClockImpl::Units::matchA> (config::microTickMax);
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

            const Delegate<void>& getCallback() const
            {
                return callback;
            }

            void setCallback(const Delegate<void>& cb)
            {
                callback=cb;
            }
	};

    template<typename Config>
    ClockImpl<Config>* ClockImpl<Config>::instance;
}
template<typename Config>
struct Clock : public helpers::ClockImpl<helpers::ClockConfigurator<Config> >{};
}
}
