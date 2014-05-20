#pragma once

#include "timer.h"
#include "timer/defaultConfig.h"
#include <avr-halib/common/singleton.h>
#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <avr-halib/locking/interruptLock.h>
#include <avr-halib/common/delegate.h>
#include <avr-halib/config/frequency.h>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    using avr_halib::interrupts::interrupt_manager::Binding;
    using avr_halib::interrupts::interrupt_manager::Slot;
    using avr_halib::config::Frequency;
    using boost::mpl::if_;
    using boost::mpl::if_c;
    using boost::mpl::bool_;

    /** \brief One shot timer class
     *
     */
    class OneShotTimer
    {
        public:
            /** \brief OneShotTimer default configuration */
            struct DefaultConfig : public avr_halib::config::TimerDefaultConfig< avr_halib::regmaps::local::Timer1 >
            {
                /** \brief TODO \todo */
                typedef Frequency<1000> BaseFrequency;
            };

            /** \brief OneShotTimer configuration
             *
             * \tparam cfg Configuration
             */
            template<typename Config = DefaultConfig>
            class configure
            {
                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                class BaseFrequencyHelper
                {
                    /** \brief TODO \todo */
                    typedef typename Config::RegMap RegMap;
                    /** \brief TODO \todo */
                    typedef typename Config::InputFrequency Input;
                    /** \brief TODO \todo */
                    typedef typename Config::BaseFrequency Target;
                    /** \brief TODO \todo */
                    static const uint8_t maxI = RegMap::Parameters::numPS-1;

                    /** \brief TODO \todo
                     *
                     * \tparam i TODO \todo
                     */
                    template<uint8_t i>
                    struct calc
                    {
                        /** \brief TODO \todo */
                        typedef typename RegMap::template PSArray<i> currentPS;
                        /** \brief TODO \todo */
                        typedef Frequency<1, currentPS::value> mod;
                        /** \brief TODO \todo */
                        typedef typename Input::template mult< mod >::type currentFrequency;
                        /** \brief TODO \todo */
                        typedef typename currentFrequency::template div< Target >::type remainder;
                        /** \brief TODO \todo */
                        static const bool ok = ( remainder::value >= 1 );
                    };

                    /* \brief TODO \todo */
                    struct finalLoop
                    {
                        /** \brief TODO \todo
                         *
                         * \tparam i TODO \todo
                         */
                        template<uint8_t i>
                        struct apply
                        {
                            /** \brief TODO \todo */
                            typedef calc<i-1> type;
                        };
                    };

                    /** \brief TODO \todo */
                    struct loop
                    {
                        /** \brief TODO \todo
                         *
                         * \tparam i TODO \todo
                         */
                        template<uint8_t i>
                        struct apply
                        {
                            /** \brief TODO \todo */
                            typedef calc<i> current;
                            /** \brief TODO \todo */
                            typedef typename if_c< ( current::ok && i < maxI ), loop, finalLoop >::type next;
                            /** \brief TODO \todo */
                            typedef typename next::template apply<i+1>::type type;
                        };
                    };

                    public:
                        /** \brief TODO \todo */
                        typedef typename loop::template apply<0>::type type;
                };

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                struct TimerConfig : public Config
                {
                    /** \brief TODO \todo */
                    enum Parameters
                    {
                        ocmAInt     = true,
                        ocmBInt     = true,
                        ocmCInt     = true
                    };

                    /** \brief TODO \todo */
                    static const typename Config::RegMap::Prescalers ps = BaseFrequencyHelper::type::currentPS::code;
                    /** \brief TODO \todo */
                    static const typename Config::RegMap::WaveForms waveForm = Config::RegMap::normal;
                    /** \brief TODO \todo */
                    // static const typename Config::RegMap::CompareMatchModes ocmAMode = Config::RegMap::noOutput;
                    /** \brief TODO \todo */
                    static const typename Config::RegMap::CompareMatchModes ocmAMode = Config::RegMap::toggle;
                    /** \brief TODO \todo */
                    static const typename Config::RegMap::CompareMatchModes ocmBMode = Config::RegMap::noOutput;
                    /** \brief TODO \todo */
                    static const typename Config::RegMap::CompareMatchModes ocmCMode = Config::RegMap::noOutput;
                    /** \brief TODO \todo */
                    static const bool dynamicPrescaler = false;
                };

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                class Implementation : public avr_halib::drivers::avr::Timer<TimerConfig>
                {
                    private:
                        /** \brief TODO \todo */
                        typedef avr_halib::drivers::avr::Timer<TimerConfig> Base;
                        /** \brief TODO \todo */
                        typedef Slot< Base::InterruptMap::matchA, Binding::FixedPlainFunction> FixedSlotA;
                        /** \brief TODO \todo */
                        typedef Slot< Base::InterruptMap::matchB, Binding::FixedPlainFunction> FixedSlotB;
                        /** \brief TODO \todo */
                        typedef Slot< Base::InterruptMap::matchC, Binding::FixedPlainFunction> FixedSlotC;
                        /** \brief TODO \todo */
                        typedef typename BaseFrequencyHelper::type::remainder Factor;

                    public:
                        /** \brief TODO \todo */
                        typedef Config config;
                        /** \brief TODO \todo */
                        typedef avr_halib::common::Delegate<void> CallbackType;
                        /** \brief TODO \todo */
                        typedef typename Base::ValueType ValueType;
                        /** \brief TODO \todo */
                        typedef typename Base::Units Units;
                        /** \brief TODO \todo */
                        typedef typename Base::UnitType UnitType;
                        /** \brief TODO \todo */
                        typedef avr_halib::common::Singleton< Implementation > Singleton;

                    private:
                        /** \brief TODO \todo */
                        CallbackType callback[Base::Parameters::numOCU];

                        /** \brief TODO \todo */
                        static void triggerA()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchA >(false);
                            Singleton::getInstance().callback[Units::matchA]();
                        }

                        /** \brief TODO \todo */
                        static void triggerB()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchB >(false);
                            Singleton::getInstance().callback[Units::matchB]();
                        }

                        /** \brief TODO \todo */
                        static void triggerC()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchC >(false);
                            Singleton::getInstance().callback[Units::matchC]();
                        }

                        /** \brief TODO \todo */
                        typedef typename FixedSlotA::template Bind< &Implementation::triggerA > BoundSlotA;
                        /** \brief TODO \todo */
                        typedef typename FixedSlotB::template Bind< &Implementation::triggerB > BoundSlotB;
                        /** \brief TODO \todo */
                        typedef typename FixedSlotC::template Bind< &Implementation::triggerC > BoundSlotC;

                    public:
                        /** \brief TODO \todo */
                        typedef typename boost::mpl::vector< BoundSlotA, BoundSlotB, BoundSlotC
                            >::type InterruptSlotList;
                    protected:
                        Implementation()
                        {
                            callback[Units::matchA].reset();
                            callback[Units::matchB].reset();
                            callback[Units::matchC].reset();
                            this->template setOutputCompareInterrupt< Units::matchA >(false);
                            this->template setOutputCompareInterrupt< Units::matchB >(false);
                            this->template setOutputCompareInterrupt< Units::matchC >(false);
                            this->start();
                        }

                    public:
                        ~Implementation()
                        {
                            this->stop();
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         * \param value TODO \todo
                         */
                        template<UnitType unit>
                        void setup(ValueType value)
                        {
                            locking::GlobalIntLock lock;
                            this->template setOutputCompareValue<unit>(this->getCounter() + value*Factor::value);
                            this->template setOutputCompareInterrupt<unit>(true);
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         */
                        template<UnitType unit>
                        const CallbackType& getCallback() const
                        {
                            return this->callback[unit];
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         * \param cb TODO \todo
                         */
                        template<UnitType unit>
                        void setCallback(const CallbackType& cb)
                        {
                            this->callback[unit]=cb;
                        }
                };

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                class Proxy
                {
                    private:
                        /** \brief TODO \todo */
                        typedef typename Implementation::Singleton Base;

                    public:
                        /** \brief TODO \todo */
                        typedef typename Base::Units Units;
                        /** \brief TODO \todo */
                        typedef typename Base::UnitType UnitType;
                        /** \brief TODO \todo */
                        typedef typename Base::ValueType ValueType;
                        /** \brief TODO \todo */
                        typedef typename Base::CallbackType CallbackType;
                        /** \brief TODO \todo */
                        typedef typename Base::InterruptSlotList InterruptSlotList;

                        Proxy()
                        {
                            Base::getInstance();
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         * \param value TODO \todo
                         */
                        template<UnitType unit>
                        void setup(ValueType value)
                        {
                            Base::getInstance().template setup<unit>(value);
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         * \param cb TODO \todo
                         */
                        template<UnitType unit>
                        void setCallback(const CallbackType& cb)
                        {
                            Base::getInstance().template setCallback<unit>(cb);
                        }

                        /** \brief TODO \todo
                         *
                         * \tparam unit TODO \todo
                         */
                        template<UnitType unit>
                        const CallbackType& getCallback() const
                        {
                            return Base::getInstance().template getCallback<unit>();
                        }
                };

                public:
                    /** \brief TODO \todo */
                    typedef Proxy type;
            };
    };

    /** \example oneShot.cpp
     *
     * TODO \todo
     */
}
}
}
