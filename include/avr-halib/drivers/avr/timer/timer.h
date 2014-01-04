#pragma once

#include <avr-halib/drivers/avr/timer/base.h>
#include <avr-halib/drivers/avr/timer/dynamicPrescaler.h>
#include <boost/mpl/if.hpp>
#include <avr-halib/drivers/avr/timer/interruptConfig.h>
#include <avr-halib/config/frequency.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
/** Timer */
namespace timer
{
    using boost::mpl::if_c;
    using config::Frequency;

    /** \brief TODO \todo
     *
     * \tparam config Configuration
     */
    template<typename config>
    struct Timer : public if_c< config::dynamicPrescaler,
        DynamicPrescaler<config, Base>,
        Base<config>
        >::type
    {
        private:
            /** \brief TODO \todo */
            typedef typename if_c< config::dynamicPrescaler,
                DynamicPrescaler<config, Base>,
                Base<config>
                >::type BaseDriver;

        public:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::Parameters Parameters;
            /** \brief TODO \todo */
            typedef typename RegMap::InterruptMap::Slots InterruptSlotList;
            /** \brief TODO \todo */
            typedef typename RegMap::InterruptMap InterruptMap;
            /** \brief TODO \todo */
            typedef typename BaseDriver::Units Units;
            /** \brief TODO \todo */
            typedef typename BaseDriver::UnitType UnitType;
            /** \brief TODO \todo */
            typedef typename RegMap::ValueType ValueType;
            /** \brief TODO \todo */
            typedef typename RegMap::CompareMatchModes CompareMatchModes;
            /** \brief TODO \todo */
            typedef typename RegMap::WaveForms WaveForms;
            /** \brief TODO \todo */
            typedef typename config::InputFrequency InputFrequency;
            /** \brief TODO \todo */
            static const uint16_t prescaler = RegMap::template PSValue< config::ps >::value;
            /** \brief TODO \todo */
            static const uint16_t maxValue = RegMap::maxValue;
            /** \brief TODO \todo */
            typedef typename InputFrequency::template mult< Frequency<1, prescaler> >::type TimerFrequency;
    };
}
}
}
}
