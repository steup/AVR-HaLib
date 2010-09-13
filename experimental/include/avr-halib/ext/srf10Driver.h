#pragma once

#include <avr-halib/regmaps/remote/srf10.h>
#include <avr-halib/regmaps/regmaps.h>

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
template<typename RM, typename RM::Index index>
struct Srf10Driver : public RM
{
	private:
		/**\brief All understood commands**/
		enum Commands
		{
			MeasureCm	=0x51,	/**<Start a measurment in centimeters**/
			MeasureInch	=0x50,	/**<Start a measurment in inches**/
			MeasureUs	=0x52,	/**<Start a measurment in microseconds**/
			ChangeID0	=0xA0,	/**<First command byte to change bus id**/
			ChangeID1	=0xA5,	/**<Second command byte to change bus id**/
			ChangeID2	=0xAA	/**<Third command byte to change bus id**/
		};

		typedef typename RM::template AddressedRegMap<index> RegMap;

	public:
		/**\brief The Address of this sensor on the bus**/
		enum Address
		{
			address=RegMap::address	/**<the effective address on the bus: \f$address_{read} = 2\cdot index + \textrm{0xE0}, 
									   									   address_{write} =  + index \textrm{0xE1}\f$.**/
		};

		typedef typename RM::Gain Gain;
		typedef typename RM::Range Range;
		typedef typename RM::Unit Unit;
	
	public:
		/**\brief Set the gain of the analogue amplification circuit
		 * \param gain the new gain value
		 *
		 * For gain values and their meaning consult the data sheet of the SRF10.
		 **/
		bool setGain(Gain gain)
		{
			UseRegMap(rm, RegMap);
			rm.gain=gain;
			return SyncRegister(rm, typename RM::GainRegister);
		}

		/**\brief Set the range value of the SRF10
		 * \param range the new range
		 *
		 * The range defines, how long the sensor will wait for incoming echos.
		 * A larger value enables to measure larger distances, but measurements take longer.
		 * The range can be calculated by \f$distance_{max}=(range+1)\cdot 43mm\f$.
		 **/
		bool setRange(Range range)
		{
			UseRegMap(rm, RegMap);
			rm.range=range;
			return SyncRegister(rm, typename RM::GainRegister);
		}

		/**\brief Get the software version of this SRF10
		 * \return the software version
		 **/
		uint8_t getVersion()
		{
			UseRegMap(rm, RegMap);
			SyncRegister(rm, typename RM::VersionRegister);
			return rm.version;
		}

		/**\brief Start a measurement with the given result unit
		 * \param m the results unit
		 **/
		bool startMeasurement(Unit m)
		{
			UseRegMap(rm, RegMap);
			switch(m)
			{
				case(RM::cm): 	rm.command=MeasureCm;
						  		break;

				case(RM::inch):	rm.command=MeasureInch;
						   		break;

				case(RM::us):	rm.command=MeasureUs;
								break;
			}
			return SyncRegister(rm, typename RM::CommandRegister);
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
			if(newIndex>15)
				return false;
			UseRegMap(rm, RegMap);
			rm.command=ChangeID0;
			if(!SyncRegister(rm, typename RM::CommandRegister))
				return false;
			rm.command=ChangeID1;
			if(!SyncRegister(rm, typename RM::CommandRegister))
				return false;
			rm.command=ChangeID2;
			if(!SyncRegister(rm, typename RM::CommandRegister))
				return false;
			rm.command=this->index2Address(newIndex);
			return SyncRegister(rm, typename RM::CommandRegister);
		}

		/**\brief Check the sensor for pending operations
		 * \return true if busy, false otherwise
		 **/
		bool isBusy()
		{
			/*UseRegMap(rm, RegMap);
			SyncRegister(rm, srf10::ReadyRegister);
			return (rm.ready==255);*/
			delay_ms(65);
			return false;
		}

		/**\brief Fetch the result of a measurement from the sensor
		 * \return the result of the last measurment
		 **/
		uint16_t fetchResult()
		{
			UseRegMap(rm, RegMap);
			SyncRegister(rm, typename RM::DistanceRegister);
			return rm.distance;
		}
};

/**\example srf10.cpp
 *
 * This is an example application, showing the usage of the SRF10 sensor. The
 * application prints the software version of the sensor and the current sensor
 * value on the lcd.
 **/

}
}
