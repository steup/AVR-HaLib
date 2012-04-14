#pragma once

#include "timer.h"
#include "timer/defaultConfig.h"
#include <avr-halib/common/singleton.h>
#include "InterruptManager/InterruptBinding.h"
#include "InterruptManager/Slot.h"
#include "interruptLock.h"
#include <avr-halib/common/delegate.h>

namespace avr_halib{
namespace drivers
{
    using ::Interrupt::Binding;
    using ::Interrupt::Slot;
    
    class OneShotTimer
    {
        private:
            typedef avr_halib::regmaps::local::Timer1 Timer;
            typedef avr_halib::config::TimerDefaultConfig<Timer> DefaultTimerConfig;
        public:
            struct DefaultConfig : public DefaultTimerConfig
            {
                enum Parameters
                {
                    ocmAInt     = true,
                    ocmBInt     = true,
                    ocmCInt     = true
                };
            };

            template<typename Config = DefaultConfig>
            class configure
            {
                class Implementation : public avr_halib::drivers::Timer<Config>
                {
                    private:
                        typedef avr_halib::drivers::Timer<Config> Base;
                        typedef Slot< Config::Timer::InterruptMap::matchA, Binding::FixedPlainFunction> FixedSlotA;
                        typedef Slot< Config::Timer::InterruptMap::matchB, Binding::FixedPlainFunction> FixedSlotB;
                        typedef Slot< Config::Timer::InterruptMap::matchC, Binding::FixedPlainFunction> FixedSlotC;

                    public:
                        typedef Config config;
                        typedef Delegate<void> CallbackType;
                        typedef typename Base::ValueType ValueType;
                        typedef typename Base::Units Units;
                        typedef typename Base::UnitType UnitType;
                        typedef avr_halib::object::Singleton< Implementation > Singleton;

                    private:
                        CallbackType callback[Config::Timer::numOCU];

                        static void triggerA()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchA >(false);
                            Singleton::getInstance().callback[Units::matchA]();
                        }

                        static void triggerB()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchB >(false);
                            Singleton::getInstance().callback[Units::matchB]();
                        }

                        static void triggerC()
                        {
                            Singleton::getInstance().template setOutputCompareInterrupt< Units::matchC >(false);
                            Singleton::getInstance().callback[Units::matchC]();
                        }

                        typedef typename FixedSlotA::template Bind< &Implementation::triggerA > BoundSlotA;
                        typedef typename FixedSlotB::template Bind< &Implementation::triggerB > BoundSlotB;
                        typedef typename FixedSlotC::template Bind< &Implementation::triggerC > BoundSlotC;

                    public:
                        typedef typename boost::mpl::vector< BoundSlotA, BoundSlotB, BoundSlotC
                                                           >::type InterruptSlotList;
                    protected:
                        Implementation()
                        {
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

                        template<UnitType unit>
                        void setup(ValueType value)
                        {
                            locking::GlobalIntLock lock;
                            this->template setOutputCompareValue<unit>(this->getCounter() + value);
                            this->template setOutputCompareInterrupt<unit>(true);
                        }

                        template<UnitType unit>
                        const CallbackType& getCallback() const
                        {
                            return callback[unit];
                        }

                        template<UnitType unit>
                        void setCallback(const CallbackType& cb)
                        {
                            callback[unit]=cb;
                        }
                };

                class Proxy
                {
                    private:
                        typedef typename Implementation::Singleton Base;

                    public:
                        typedef typename Base::Units Units;
                        typedef typename Base::UnitType UnitType;
                        typedef typename Base::ValueType ValueType;
                        typedef typename Base::CallbackType CallbackType;
                        typedef typename Base::InterruptSlotList InterruptSlotList;
                        
                        Proxy()
                        {
                            Base::getInstance();
                        }

                        template<UnitType unit>
                        void setup(ValueType value)
                        {
                            Base::getInstance().template setup<unit>(value);
                        }

                        template<UnitType unit>
                        void setCallback(const CallbackType& cb)
                        {
                            Base::getInstance().template setCallback<unit>(cb);
                        }

                        template<UnitType unit>
                        const CallbackType& setCallback() const
                        {
                            return Base::getInstance().template getCallback<unit>();
                        }        
                };

                public:
                    typedef Proxy type;
            };
     };
}
}
