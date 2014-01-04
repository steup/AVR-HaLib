#pragma once

#include <avr-halib/config/frequency.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib config */
namespace config
{
    using avr_halib::config::Frequency;

    /** \brief TODO \todo
     *
     * \tparam TimerRegMap TODO \todo
     */
    template<typename TimerRegMap>
    struct TimerDefaultConfig
    {
        /** \brief TODO \todo */
        typedef TimerRegMap RegMap;

        /** \brief TODO \todo */
        enum Parameters
        {
            overflowInt      = false,
            ocmAInt          = false,
            ocmBInt          = false,
            ocmCInt          = false,
            async            = false,
            asyncExtClock    = false,
            dynamicPrescaler = false
        };

        /** \brief TODO \todo */
        typedef Frequency<F_CPU> InputFrequency;

        /** \brief TODO \todo */
        typedef typename RegMap::WaveForms  WF;
        /** \brief TODO \todo */
        typedef typename RegMap::Prescalers PS;
        /** \brief TODO \todo */
        typedef typename RegMap::CompareMatchModes CM;

        /** \brief TODO \todo */
        static const WF waveForm = RegMap::normal;
        /** \brief TODO \todo */
        static const PS ps       = RegMap::noClock;
        /** \brief TODO \todo */
        static const CM ocmAMode = RegMap::noOutput;
        /** \brief TODO \todo */
        static const CM ocmBMode = RegMap::noOutput;
        /** \brief TODO \todo */
        static const CM ocmCMode = RegMap::noOutput;
    };
}
}
