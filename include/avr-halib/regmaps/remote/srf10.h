#pragma once

#include <avr-halib/regmaps/base/register.h>
#include <boost/mpl/list.hpp>
#include <boost/mpl/int.hpp>

namespace avr_halib
{
namespace regmaps
{
namespace remote
{
    using base::RWModes;
    using base::RWModeType;

    /**\brief SRF10 Distributed RegMap
     * \tparam TwiRegmap the regmap for the twi interface
     * \tparam twiAddress the bus address of the sensor
     *
     * This RegMap can be used to access a SRF10 sensor attached to a TWI.
     * A platform specific TwiRegMap is needed to access the mcu`s TWI hardware.
     * Also the bus address of the sensor must be provided.
     **/
    struct SRF10
    {
            /**\brief The version register of the SRF10
     *
     * This register reports the version if idle, if busy it always contains a 0xFF.
     * Size: 8 bit.
     **/
    struct VersionRegister
    {
        static const uint16_t address  = 0x0;
        static const RWModeType mode = RWModes::read;

        union
        {
            /**\brief the software version of this SRF10 sensor**/
            uint8_t version;
            /**\brief 0xFF if busy, anything else if idle**/
            uint8_t ready;
        };
    };

    /**\brief Alias name for the VersionRegister**/
    typedef VersionRegister ReadyRegister;

    /**\brief The distance register of the SRF10
     *
     * This register contains the distance after a successfull measurement.
     * Size: 16 bit.
     **/
    struct DistanceHighRegister
    {
        static const uint16_t address  = 0x2;
        static const RWModeType mode = RWModes::read;

        /**\brief the measured distance**/
        uint8_t distanceHigh;

    };

    struct DistanceLowRegister
    {
        static const uint16_t address  = 0x2;
        static const RWModeType mode = RWModes::read;

        /**\brief the measured distance**/
        uint8_t distanceLow;

    };

    /**\brief The command register of the SRF10
     *
     * Write operations to this register are interpreted as commands by the SRF10.
     * Size: 8 bit.
     **/
    struct CommandRegister
    {
        static const uint16_t address  = 0x0;
        static const RWModeType mode = RWModes::write;

        /**\brief the issued command**/
        uint8_t command;
    };

    /**\brief The gain-control register of the SRF10
     *
     * Write operation to this register changes the gain of the analog amplification circuit.
     * Size: 8 bit.
     **/
    struct GainRegister
    {
        static const uint16_t address  = 0x1;
        static const RWModeType mode = RWModes::write;

        /**\brief the gain value**/
        uint8_t gain;
    };

    /**\brief The range register of the SRF10
     *
     * Write operation to this register changes the timeout before an measurement
     * finished, bigger values represent more range, but increase measurement
     * duration.
     *
     * Size: 8 bit.
     **/
    struct RangeRegister
    {
        static const uint16_t address  = 0x2;
        static const RWModeType mode = RWModes::write;
        /**\brief the range value**/
        uint8_t range;
    };

    /**\brief Definition of the final list of register**/
    typedef typename boost::mpl::list<	VersionRegister,
                                        DistanceHighRegister,
                                        DistanceLowRegister,
                                        CommandRegister,
                                        GainRegister,
                                        RangeRegister
                            >::type RegisterList;

        /**\brief Possible Units for measurements**/
        struct Units
        {
            enum UnitType
            {
                cm,		/**<Measure in centimeters**/
                inch,	/**<Measure in inches**/
                us		/**Measure in microseconds**/
            };
        };
        
        typedef Units::UnitType UnitType;

        /**\brief All understood commands**/
        struct Commands
        {
            enum CommandType
            {
                MeasureCm	=0x51,	/**<Start a measurement in centimeters**/
                MeasureInch	=0x50,	/**<Start a measurement in inches**/
                MeasureUs	=0x52,	/**<Start a measurement in microseconds**/
                ChangeID0	=0xA0,	/**<First command byte to change bus id**/
                ChangeID1	=0xA5,	/**<Second command byte to change bus id**/
                ChangeID2	=0xAA	/**<Third command byte to change bus id**/
            };
        };

        typedef Commands::CommandType CommandType;

        typedef uint8_t IndexType;
        typedef uint8_t RangeType;
        typedef uint8_t GainType;

        static const uint8_t   baseAddress = 0x70;
        static const IndexType maxIndex    = 15;
};
}
}
}
