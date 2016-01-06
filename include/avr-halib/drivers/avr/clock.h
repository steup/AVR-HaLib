#pragma once

#include <avr-halib/drivers/avr/timer.h>
#include <avr-halib/common/delegate.h>
#include <avr-halib/config/frequency.h>
#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>

#include <boost/mpl/vector.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** AVR-Drivers helpers */
    namespace helpers
    {
        using avr_halib::config::Frequency;
        using avr_halib::interrupts::interrupt_manager::Slot;
        using avr_halib::interrupts::interrupt_manager::Binding;

        /** \brief TODO \todo
         *
         * \tparam FreqRatio TODO \todo
         * \tparam PSArray TODO \todo
         * \tparam maxI TODO \todo
         * \tparam i TODO \todo
         * \tparam psOK TODO \todo
         */
        template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i=0, bool psOK=false>
        struct PrescalerSelector;

        /** \brief TODO \todo
         *
         * \tparam FreqRatio TODO \todo
         * \tparam PSArray TODO \todo
         * \tparam maxI TODO \todo
         * \tparam i TODO \todo
         */
        template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
        struct PrescalerSelector<FreqRatio, PSArray, maxI, i, false>
        {
            /** \brief TODO \todo */
            typedef PSArray<i> PS;
            /** \brief TODO \todo */
            static const bool psOK = FreqRatio::template div< Frequency<PS::value> >::type::value < 1;
            /** \brief TODO \todo */
            static const uint8_t nextIndex= i+((psOK)?0:1);
            /** \brief TODO \todo */
            typedef typename PrescalerSelector<FreqRatio, PSArray, maxI, nextIndex, psOK>::type type;
        };

        /** \brief TODO \todo
         *
         * \tparam FreqRatio TODO \todo
         * \tparam PSArray TODO \todo
         * \tparam maxI TODO \todo
         * \tparam i TODO \todo
         */
        template<typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI, uint8_t i>
        struct PrescalerSelector<FreqRatio, PSArray, maxI, i, true>
        {
            /** \brief TODO \todo */
            typedef PSArray<i> type;
        };

        /** \brief TODO \todo
         *
         * \tparam ratioTooSmall TODO \todo
         * \tparam FreqRatio TODO \todo
         * \tparam PSArray TODO \todo
         * \tparam maxI TODO \todo
         */
        template<bool ratioTooSmall, typename FreqRatio, template<uint8_t> class PSArray, uint8_t maxI>
        struct PrescalerSelector<FreqRatio, PSArray, maxI, maxI, ratioTooSmall>
        {
            /** \brief TODO \todo */
            typedef PSArray<maxI-1> type;
        };

        /** \brief TODO \todo
         *
         * \tparam config TODO \todo
         */
        template<typename config>
        struct ClockConfigurator
        {
            /** \brief TODO \todo */
            typedef typename config::Timer Timer;
            /** \brief TODO \todo */
            typedef typename config::TickValueType TickValueType;
            /** \brief TODO \todo */
            typedef typename Timer::ValueType MicroTickValueType;
            /** \brief TODO \todo */
            static const uint32_t maxNumMicroTicks = 1ULL << sizeof(typename Timer::ValueType)*8;
            /** \brief TODO \todo */
            typedef typename config::TimerFrequency::template div<
                typename config::TargetFrequency>::type FreqRatio;
            /** \brief TODO \todo */
            typedef typename FreqRatio::template div<
                Frequency<maxNumMicroTicks> >::type
                TargetRatio;
            /** \brief TODO \todo */
            typedef typename helpers::PrescalerSelector<TargetRatio,
                Timer::template PSArray,
                Timer::Parameters::numPS
                >::type SelectedPrescaler;
            /** \brief TODO \todo */
            typedef typename config::TargetFrequency TickFrequency;
            /** \brief TODO \todo */
            static const MicroTickValueType microTickMax =
                FreqRatio::template div< Frequency<SelectedPrescaler::value> >::type::value;

            /** \brief TODO \todo */
            struct timerConfig : public ::avr_halib::config::TimerDefaultConfig<Timer>
            {
                /** \brief TODO \todo */
                enum Parameters
                {
                    ocmAInt=true,
                    async=!(config::TimerFrequency::value==F_CPU)
                };

                /** \brief TODO \todo */
                static const typename Timer::CompareMatchModes ocmAMode = Timer::noOutput;
                /** \brief TODO \todo */
                static const typename Timer::WaveForms waveForm = Timer::ctc;
                /** \brief TODO \todo */
                static const typename Timer::Prescalers ps = SelectedPrescaler::code;
            };
        };

        /** \brief TODO \todo
         *
         * \tparam config TODO \todo
         */
        template<typename Config>
        class ClockImpl : public Timer<typename Config::timerConfig>
        {
            public:
                /** \brief TODO \todo */
                struct config
                {
                    /** \brief TODO \todo */
                    typedef typename Config::TickValueType TickValueType;
                    /** \brief TODO \todo */
                    typedef typename Config::MicroTickValueType MicroTickValueType;

                    /** \brief TODO \todo */
                    struct Time
                    {
                        /** \brief TODO \todo */
                        TickValueType ticks;
                        /** \brief TODO \todo */
                        MicroTickValueType microTicks;
                    };

                    /** \brief TODO \todo */
                    static const MicroTickValueType microTickMax = Config::microTickMax;
                    /** \brief TODO \todo */
                    typedef typename Config::SelectedPrescaler selectedPrescaler;
                    /** \brief TODO \todo */
                    typedef typename Config::TickFrequency Frequency;
                };

                /** \brief TODO \todo */
                typedef typename config::Time Time;

            private:
                /** \brief TODO \todo */
                typename config::TickValueType ticks;
                /** \brief TODO \todo */
                avr_halib::common::Delegate<void> callback;
                /** \brief TODO \todo */
                static ClockImpl* instance;

                /** \brief TODO \todo */
                typedef Timer<typename Config::timerConfig> Base;

                /** \brief TODO \todo */
                void tick() {
                    ticks++;
                    callback();
                }

            public:
                /** \brief TODO \todo */
                typedef typename boost::mpl::vector< typename Slot< Config::Timer::InterruptMap::matchA, Binding::SingletonFunction >::template Bind< ClockImpl, &ClockImpl::tick, &instance > >::type InterruptSlotList;

                /** \brief TODO \todo */
                ClockImpl() : ticks(0)
                {
                    instance = this;
                    this->template setOutputCompareValue<ClockImpl::Units::matchA> (config::microTickMax);
                    this->template setOutputCompareInterrupt<ClockImpl::Units::matchA> (true);
                    this->start();
                }

                /** \brief TODO \todo */
                ~ClockImpl()
                {
                    this->stop();
                }

                /** \brief TODO \todo
                 *
                 * \param t TODO \todo
                 */
                void getTime(Time& t)
                {
                    t.ticks=this->ticks;
                    t.microTicks=this->getCounter();
                }

                /** \brief TODO \todo */
                const avr_halib::common::Delegate<void>& getCallback() const
                {
                    return callback;
                }

                /** \brief TODO \todo
                 *
                 * \param cb TODO \todo
                 */
                void setCallback(const avr_halib::common::Delegate<void>& cb)
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
}
