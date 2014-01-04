#pragma once

#include <stdint.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib config */
namespace config
{
    /** \brief SPI configuration */
    struct Spi
    {
        /** \brief TODO \todo */
        struct BitOrders
        {
            /** \brief TODO \todo */
            enum BitOrderType
            {
                mostFirst,
                leastFirst
            };
        };
        /** \brief TODO \todo */
        typedef BitOrders::BitOrderType BitOrderType;

        /** \brief TODO \todo */
        struct ClockPolarities
        {
            /** \brief TODO \todo */
            enum ClockPolarityType
            {
                idleOnLow,
                idleOnHigh
            };
        };
        /** \brief TODO \todo */
        typedef ClockPolarities::ClockPolarityType ClockPolarityType;

        /** \brief TODO \todo */
        struct SampleEdges
        {
            /** \brief TODO \todo */
            enum SampleEdgeType
            {
                leading,
                trailing
            };
        };
        /** \brief TODO \todo */
        typedef SampleEdges::SampleEdgeType SampleEdgeType;

        /** \brief TODO \todo */
        struct Modes
        {
            /** \brief TODO \todo */
            enum ModeType
            {
                master,
                slave
            };
        };
        /** \brief TODO \todo */
        typedef Modes::ModeType ModeType;

        /** \brief TODO \todo */
        typedef uint8_t PrescalerType;
    };
}
}
