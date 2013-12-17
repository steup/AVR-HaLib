#pragma once

#include <stdint.h>

namespace avr_halib
{
namespace config
{
    struct Spi
    {
        struct BitOrders
        {
            enum BitOrderType
            {
                mostFirst,
                leastFirst
            };
        };
        typedef BitOrders::BitOrderType BitOrderType;

        struct ClockPolarities
        {
            enum ClockPolarityType
            {
                idleOnLow,
                idleOnHigh
            };
        };
        typedef ClockPolarities::ClockPolarityType ClockPolarityType;

        struct SampleEdges
        {
            enum SampleEdgeType
            {
                leading,
                trailing
            };
        };
        typedef SampleEdges::SampleEdgeType SampleEdgeType;

        struct Modes
        {
            enum ModeType
            {
                master,
                slave
            };
        };
        typedef Modes::ModeType ModeType;

        typedef uint8_t PrescalerType;
    };
}
}
