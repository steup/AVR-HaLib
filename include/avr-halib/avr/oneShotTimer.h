#pragma once

#include "timer.h"
#include "timer/defaultConfig.h"
#include <avr-halib/common/singleton.h>
#include "InterruptManager/InterruptBinding.h"
#include "InterruptManager/Slot.h"
#include "interruptLock.h"
#include <avr-halib/common/delegate.h>
#include <avr-halib/common/frequency.h>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

namespace avr_halib{
namespace drivers
{
    using ::Interrupt::Binding;
    using ::Interrupt::Slot;
	using avr_halib::config::Frequency;
	using boost::mpl::if_;
	using boost::mpl::if_c;
	using boost::mpl::bool_;
    
    class OneShotTimer
    {
        public:
            struct DefaultConfig : public avr_halib::config::TimerDefaultConfig< avr_halib::regmaps::local::Timer1 >
            {
				typedef Frequency<1000> BaseFrequency;
            };

            template<typename Config = DefaultConfig>
            class configure
            {
				class BaseFrequencyHelper
				{
					typedef typename Config::RegMap RegMap;
					typedef typename Config::InputFrequency Input;
					typedef typename Config::BaseFrequency Target;
					static const uint8_t maxI = RegMap::Parameters::numPS-1;

					template<uint8_t i>
					struct calc
					{
						typedef typename RegMap::template PSArray<i> currentPS;
						typedef Frequency<1, currentPS::value> mod;
					    typedef typename Input::template mult< mod >::type currentFrequency;
						typedef typename currentFrequency::template div< Target >::type remainder;
						static const bool ok = ( remainder::value >= 1 );
					};

					
					struct finalLoop
					{
						template<uint8_t i>
						struct apply
						{
							typedef calc<i-1> type;
						};
					};

					
					struct loop
					{
						template<uint8_t i>
						struct apply
						{
							typedef calc<i> current;
							typedef typename if_c< ( current::ok && i < maxI ), loop, finalLoop >::type next;
							typedef typename next::template apply<i+1>::type type;
						};
					};


					public:
						typedef typename loop::template apply<0>::type type;
				};

				struct TimerConfig : public Config
				{
					enum Parameters
					{
						ocmAInt     = true,
						ocmBInt     = true,
						ocmCInt     = true
					};
					
					static const typename Config::RegMap::Prescalers ps = BaseFrequencyHelper::type::currentPS::code;
                    static const typename Config::RegMap::WaveForms         waveForm = Config::RegMap::normal;
//                    static const typename Config::RegMap::CompareMatchModes ocmAMode = Config::RegMap::noOutput;
                    static const typename Config::RegMap::CompareMatchModes ocmAMode = Config::RegMap::toggle;
                    static const typename Config::RegMap::CompareMatchModes ocmBMode = Config::RegMap::noOutput;
                    static const typename Config::RegMap::CompareMatchModes ocmCMode = Config::RegMap::noOutput;
                    static const bool dynamicPrescaler = false;
				};

                class Implementation : public avr_halib::drivers::Timer<TimerConfig>
                {
                    private:
                        typedef avr_halib::drivers::Timer<TimerConfig> Base;
                        typedef Slot< Base::InterruptMap::matchA, Binding::FixedPlainFunction> FixedSlotA;
                        typedef Slot< Base::InterruptMap::matchB, Binding::FixedPlainFunction> FixedSlotB;
                        typedef Slot< Base::InterruptMap::matchC, Binding::FixedPlainFunction> FixedSlotC;
						typedef typename BaseFrequencyHelper::type::remainder Factor;

                    public:
                        typedef Config config;
                        typedef Delegate<void> CallbackType;
                        typedef typename Base::ValueType ValueType;
                        typedef typename Base::Units Units;
                        typedef typename Base::UnitType UnitType;
                        typedef avr_halib::object::Singleton< Implementation > Singleton;

                    private:
                        CallbackType callback[Base::Parameters::numOCU];

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

                        template<UnitType unit>
                        void setup(ValueType value)
                        {
                            locking::GlobalIntLock lock;
                            this->template setOutputCompareValue<unit>(this->getCounter() + value*Factor::value);
                            this->template setOutputCompareInterrupt<unit>(true);
                        }

                        template<UnitType unit>
                        const CallbackType& getCallback() const
                        {
                            return this->callback[unit];
                        }

                        template<UnitType unit>
                        void setCallback(const CallbackType& cb)
                        {
                            this->callback[unit]=cb;
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
