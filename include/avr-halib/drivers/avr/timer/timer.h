#pragma once

#include <avr-halib/drivers/avr/timer/base.h>
#include <avr-halib/drivers/avr/timer/dynamicPrescaler.h>
#include <boost/mpl/if.hpp>
#include <avr-halib/drivers/avr/timer/interruptConfig.h>
#include <avr-halib/config/frequency.h>

namespace avr_halib
{
namespace drivers
{
namespace avr
{
namespace timer
{
    using boost::mpl::if_c;
    using config::Frequency;

    template<typename config>
    struct Timer : public if_c< config::dynamicPrescaler,
        DynamicPrescaler<config, Base>,
        Base<config>
        >::type
    {
        private:
            typedef typename if_c< config::dynamicPrescaler,
                DynamicPrescaler<config, Base>,
                Base<config>
                >::type BaseDriver;

        public:
            typedef typename config::RegMap RegMap;
            typedef typename RegMap::Parameters Parameters;
            typedef typename RegMap::InterruptMap::Slots InterruptSlotList;
            typedef typename RegMap::InterruptMap InterruptMap;
            typedef typename BaseDriver::Units Units;
            typedef typename BaseDriver::UnitType UnitType;
            typedef typename RegMap::ValueType ValueType;
            typedef typename RegMap::CompareMatchModes CompareMatchModes;
            typedef typename RegMap::WaveForms WaveForms;
            typedef typename config::InputFrequency InputFrequency;
            static const uint16_t prescaler = RegMap::template PSValue< config::ps >::value;
            static const uint16_t maxValue = RegMap::maxValue;
            typedef typename InputFrequency::template mult< Frequency<1, prescaler> >::type TimerFrequency;
    };
}
}
}
}
