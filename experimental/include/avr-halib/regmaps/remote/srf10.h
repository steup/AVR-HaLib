#pragma once

#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/regmaps/base/remoteRegMap.h>
#include <avr-halib/regmaps/base/twi.h>
#include <boost/mpl/list.hpp>
#include <boost/mpl/int.hpp>

namespace avr_halib
{
namespace regmaps
{
namespace remote
{
/**\brief SRF10 Distributed RegMap
 * \tparam TwiRegmap the regmap for the twi interface
 * \tparam twiAddress the bus address of the sensor
 *
 * This RegMap can be used to access a SRF10 sensor attached to a TWI.
 * A platform specific TwiRegMap is needed to access the mcu`s TWI hardware.
 * Also the bus address of the sensor must be provided.
 **/
template<typename TwiRegmap>
struct SRF10RegMap
{
		/**\brief The version register of the SRF10
 *
 * This register reports the version if idle, if busy it always contains a 0xFF.
 * Size: 8 bit.
 **/
struct VersionRegister
{
	/**\brief mandatory register parameters**/
	enum RegisterParameters
	{
		address=0x0,	/**< Address: 0x0**/
		mode=base::read	/**< RW: read**/
	};

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
struct DistanceRegister
{
	/**\brief mandatory register parameters**/
	enum RegisterParameters
	{
		address=0x2,	/**< Address: 0x2**/
		mode=base::read	/**< RW: read**/
	};

	/**\brief the measured distance**/
	uint16_t distance;
};

/**\brief The command register of the SRF10
 *
 * Write operations to this register are interpreted as commands by the SRF10.
 * Size: 8 bit.
 **/
struct CommandRegister
{
	/**\brief mandatory register parameters**/
	enum RegisterParameters
	{
		address=0x0,	/**< Address: 0x0**/
		mode=base::write	/**< RW: write**/
	};

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
	/**\brief mandatory register parameters**/
	enum RegisterParameters
	{
		address=0x1,	/**< Address: 0x1**/
		mode=base::write	/**< RW: write**/
	};

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
	/**\brief mandatory register parameters**/
	enum RegisterParameters
	{
		address=0x2,	/**< Address: 0x2**/
		mode=base::write	/**< RW: write**/
	};
	/**\brief the range value**/
	uint8_t range;
};

/**\brief Definition of the final list of register**/
typedef typename boost::mpl::list<	VersionRegister,
							DistanceRegister,
							CommandRegister,
							GainRegister,
							RangeRegister
						>::type SRF10RegisterList;

	/**\brief Possible Units for measurements**/
	enum Unit
	{
		cm,		/**<Measure in centimeters**/
		inch,	/**<Measure in inches**/
		us		/**Measure in microseconds**/
	};

	typedef uint8_t Index;
	typedef uint8_t Range;
	typedef uint8_t Gain;

	static const uint8_t baseAddress=0x70;

	uint8_t index2Address(Index i) { return baseAddress+i; }

	template<Index i>
	struct AddressedRegMap : public base::RemoteRegMap< base::Twi< 
							 								 	TwiRegmap,
																baseAddress+i
															  >,
														      SRF10RegisterList
														   >
	{
		static const uint8_t address=baseAddress+i;
	};
};
}
}
}
