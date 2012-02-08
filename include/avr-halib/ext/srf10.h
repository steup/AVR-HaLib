#pragma once

#include <avr-halib/regmaps/remote/srf10.h>
#include <avr-halib/regmaps/base/twi.h>
#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/regmaps/base/remoteRegMap.h>
#include <avr-halib/common/delay.h>

namespace avr_halib
{
namespace drivers
{


/**\brief SRF10 Ultra sonic sensor
 * \tparam TwiRegMap the RegMap to access the MCU`s TWI hardware
 * \tparam index the index of this sensor in the TWI bus
 *
 * This driver grants access to a SRF10 ultra-sonic sensor attached to the TWI
 * bus.  It uses a Distributed RegMap to access the remote registers of the
 * sensor.  All functionality of the sensor is available with this driver
 * including different measurement result, resetting of bus ids and setting
 * gain and range.
 **/

struct SRF10
{
    struct DefaultConfig
    {
        typedef regmaps::remote::SRF10 RegisterDescription;
        typedef regmaps::base::TWI     RemoteInterface;
        static const uint8_t           index = 0;
    };

    template< typename Config = DefaultConfig >
    struct configure
    {
        struct type
        {
            private:
                struct RemoteInterfaceConfig : public Config::RemoteInterface::DefaultConfig
                {
                    static const typename Config::RemoteInterface::AddressType address = 
                        Config::RegisterDescription::baseAddress + Config::index;
                };

                typedef typename Config::RemoteInterface::template configure< RemoteInterfaceConfig >::type Interface;

                typedef typename Config::RegisterDescription      RegisterDescription;
                typedef typename RegisterDescription::CommandType CommandType;
                typedef typename RegisterDescription::Commands    Commands;

                typedef typename regmaps::base::RemoteRegMap::configure< RegisterDescription,
                                                                         Interface
                                                                        >::type RegMap;

                uint8_t softwareVersion;

            public:
                typedef          Config                           config;
                typedef typename RegisterDescription::UnitType    UnitType;
                typedef typename RegisterDescription::Units       Units;
                typedef typename RegisterDescription::GainType    GainType;
                typedef typename RegisterDescription::RangeType   RangeType;
            
            public:

                type() : softwareVersion(getVersion()){}
                /**\brief Set the gain of the analogue amplification circuit
                 * \param gain the new gain value
                 *
                 * For gain values and their meaning consult the data sheet of the SRF10.
                 **/
                bool setGain(GainType gain)
                {
                    UseRegMap(rm, RegMap);
                    rm.gain=gain;
                    return SyncRegister(rm, typename RegMap::Registers::GainRegister);
                }

                /**\brief Set the range value of the SRF10
                 * \param range the new range
                 *
                 * The range defines, how long the sensor will wait for incoming echos.
                 * A larger value enables to measure larger distances, but measurements take longer.
                 * The range can be calculated by \f$distance_{max}=(range+1)\cdot 43mm\f$.
                 **/
                bool setRange(RangeType range)
                {
                    UseRegMap(rm, RegMap);
                    rm.range=range;
                    return SyncRegister(rm, typename RegMap::Registers::GainRegister);
                }

                /**\brief Get the software version of this SRF10
                 * \return the software version
                 **/
                uint8_t getVersion()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegister(rm, typename RegMap::Registers::VersionRegister);
                    return rm.version;
                }

                /**\brief Start a measurement with the given result unit
                 * \param m the results unit
                 **/
                bool startMeasurement(UnitType m)
                {
                    UseRegMap(rm, RegMap);
                    switch(m)
                    {
                        case(Units::cm)  : rm.command=Commands::MeasureCm;
                                              break;

                        case(Units::inch): rm.command=Commands::MeasureInch;
                                              break;

                        case(Units::us)  : rm.command=Commands::MeasureUs;
                                              break;
                    }
                    return SyncRegister(rm, typename RegMap::Registers::CommandRegister);
                }

                /**\brief Set the eeprom saved bus id of the Sensor
                 * \param newIndex the new Index \f$Index_{new}\in[0,15]\f$
                 *
                 * This function is only allowed if there is only one SRF10 connected
                 * to the TWI bus.  The supplied index will be converted to an TWI
                 * address and the sensor will be flashed with the new ID value.  The
                 * formula to calculate the TWI address is: \f$Address_{new}=2\cdot Index_{new} + \textrm{0xE0}\f$.
                 **/
                bool setID(uint8_t newIndex)
                {
                    if( newIndex > RegisterDescription::maxIndex)
                        return false;

                    UseRegMap(rm, RegMap);

                    rm.command = Commands::ChangeID0;
                    if(!SyncRegister(rm, typename RegMap::Registers::CommandRegister))
                        return false;

                    rm.command = Commands::ChangeID1;
                    if(!SyncRegister(rm, typename RegMap::Registers::CommandRegister))
                        return false;

                    rm.command = Commands::ChangeID2;
                    if(!SyncRegister(rm, typename RegMap::Registers::CommandRegister))
                        return false;

                    rm.command = RegisterDescription::baseAddress + newIndex;

                    return SyncRegister(rm, typename RegMap::Registers::CommandRegister);
                }

                /**\brief Check the sensor for pending operations
                 * \return true if busy, false otherwise
                 **/
                bool isBusy()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegister(rm, typename RegMap::Registers::ReadyRegister);
                    return (rm.ready!=softwareVersion);
                    /*delay_ms(65);
                    return false;*/
                }

                /**\brief Fetch the result of a measurement from the sensor
                 * \return the result of the last measurment
                 **/
                uint16_t fetchResult()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegister(rm, typename RegMap::Registers::DistanceLowRegister);
                    SyncRegister(rm, typename RegMap::Registers::DistanceHighRegister);
                    return ((uint16_t)rm.distanceHigh)<<8 | rm.distanceLow;
                }
            };
        };
};

/**\example srf10.cpp
 *
 * This is an example application, showing the usage of the SRF10 sensor. The
 * application prints the software version of the sensor and the current sensor
 * value on the lcd.
 **/

}
}
