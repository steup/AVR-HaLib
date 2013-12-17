#pragma once

#include <avr-halib/drivers/avr/externalInterrupt.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <avr-halib/common/fixPoint.h>
#include <avr-halib/config/frequency.h>

namespace avr_halib
{
namespace drivers
{
namespace ext
{
    template<typename config>
    class OdometrieSensor : private avr_halib::drivers::avr::ExternalInterrupt< typename config::TickSource,
        config::TickSource::rising
        >
    {
        typedef avr_halib::drivers::avr::ExternalInterrupt< typename config::TickSource, config::TickSource::rising > TickSource;

        public:
            typedef typename config::EvalFrequency EvalFrequency;
            typedef ::avr_halib::config::Frequency< 60, config::ticksPerTurn > Faktor;
            typedef typename EvalFrequency::template mult< Faktor >::type UMinFreq;
            typedef typename config::CounterType CounterType;

        private:
            volatile CounterType count;
            volatile CounterType odoValue;
            void tick()
            {
                PINA |= boost::mpl::if_<typename boost::is_same<typename config::TickSource, regmaps::local::ExternalInterrupt3>::type, typename boost::mpl::int_<1>::type, typename boost::mpl::int_<2>::type>::type::value;
                count++;
            }

        public:
            typedef typename TickSource::InterruptMap InterruptMap;
            typedef typename TickSource::InterruptSlotList InterruptSlotList;

            OdometrieSensor() : count(0), odoValue(0)
            {
                this->setPullUp(true);
                this->template registerCallback<OdometrieSensor, &OdometrieSensor::tick>(*this);
            }

            void eval()
            {
                odoValue=count;
                count=0;
            }

            uint16_t getValue() const
            {
                typedef object::FixPointValue<uint32_t, 8> CalcType;
                CalcType temp=odoValue;
                temp*=(UMinFreq::numerator*256/UMinFreq::denominator);
                temp/=256;
                return temp.trunc();
            }

            uint16_t getTicks() const
            {
                return odoValue;
            }
    };
}
}
}
