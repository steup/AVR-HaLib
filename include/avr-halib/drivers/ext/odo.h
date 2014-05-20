#pragma once

#include <avr-halib/drivers/avr/externalInterrupt.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <avr-halib/common/fixPoint.h>
#include <avr-halib/config/frequency.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** External-Drivers */
namespace ext
{
    /** \brief TODO \todo
     *
     * \tparam config TODO \todo
     */
    template<typename config>
    class OdometrieSensor : private avr_halib::drivers::avr::ExternalInterrupt< typename config::TickSource,
        config::TickSource::rising
        >
    {
        /** \brief TODO \todo */
        typedef avr_halib::drivers::avr::ExternalInterrupt< typename config::TickSource, config::TickSource::rising > TickSource;

        public:
            /** \brief TODO \todo */
            typedef typename config::EvalFrequency EvalFrequency;
            /** \brief TODO \todo */
            typedef ::avr_halib::config::Frequency< 60, config::ticksPerTurn > Faktor;
            /** \brief TODO \todo */
            typedef typename EvalFrequency::template mult< Faktor >::type UMinFreq;
            /** \brief TODO \todo */
            typedef typename config::CounterType CounterType;

        private:
            /** \brief TODO \todo */
            volatile CounterType count;
            /** \brief TODO \todo */
            volatile CounterType odoValue;
            /** \brief TODO \todo */
            void tick()
            {
                PINA |= boost::mpl::if_<typename boost::is_same<typename config::TickSource, regmaps::local::ExternalInterrupt3>::type, typename boost::mpl::int_<1>::type, typename boost::mpl::int_<2>::type>::type::value;
                count++;
            }

        public:
            /** \brief TODO \todo */
            typedef typename TickSource::InterruptMap InterruptMap;
            /** \brief TODO \todo */
            typedef typename TickSource::InterruptSlotList InterruptSlotList;

            /** \brief TODO \todo */
            OdometrieSensor() : count(0), odoValue(0)
            {
                this->setPullUp(true);
                this->template registerCallback<OdometrieSensor, &OdometrieSensor::tick>(*this);
            }

            /** \brief TODO \todo */
            void eval()
            {
                odoValue=count;
                count=0;
            }

            /** \brief TODO \todo */
            uint16_t getValue() const
            {
                typedef object::FixPointValue<uint32_t, 8> CalcType;
                CalcType temp=odoValue;
                temp*=(UMinFreq::numerator*256/UMinFreq::denominator);
                temp/=256;
                return temp.trunc();
            }

            /** \brief TODO \todo */
            uint16_t getTicks() const
            {
                return odoValue;
            }
    };
}
}
}
