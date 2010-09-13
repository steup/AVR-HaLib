#pragma once

#include <avr-halib/avr/twi.h>
#include <avr-halib/share/singleton.h>

namespace avr_halib
{
namespace regmaps
{
namespace base
{
namespace helpers
{
/**\brief TWI interface implementation
 * \tparam RegMap Register map for twi operation on mcu
 * \tparam chipAddress the address of the target chip
 *
 * This class is the basic implementation of a twi bus driver.  It can be used
 * to communicate with target devices on the twi bus, that contain 8 and 16 bit
 * registers. However this class is no singleton and therefore not suited to be
 * used directly with remote regMaps.
 **/

template<typename RegMap, uint8_t chipAddress>
struct TwiImpl : private TWIMaster< RegMap >
{
	private:
		/**\brief Basic TWI driver for bus operation**/
		typedef TWIMaster< RegMap > Base;
	public:

		/**\brief Constructor initializes bus**/
		TwiImpl()
		{
			Base::init(128);
		}

		/**\brief write 1 byte to a bus device
		 * \param address the address to write the data to
		 * \param data the data to write
		 **/
		bool write(uint8_t address, uint8_t data)
		{
			if(!Base::start(chipAddress, false))
				return false;
			if(!Base::write(address))
				return false;
			if(!Base::write(data))
				return false;
			Base::stop();
			return true;
		}

		/**\brief read 1 byte from a bus device
		 * \param address the address to read the data from
		 * \param data a reference to write the data to
		 **/
		bool read(uint8_t address, uint8_t &data)
		{
			if(!Base::start(chipAddress, false))
				return false;
			if(!Base::write(address))
				return false;
			if(!Base::start(chipAddress, true))
				return false;
			data=Base::read(true);
			Base::stop();
			return true;
		}

		/**\brief read 2 byte from a bus device
		 * \param address the address to read the data from
		 * \param data a reference to write the data to
		 **/
		bool read(uint8_t address, uint16_t &data)
		{
			if(!Base::start(chipAddress, false))
				return false;
			if(!Base::write(address))
				return false;
			if(!Base::start(chipAddress, true))
				return false;
			data=Base::read(false);
			data<<=8;
			data|=Base::read(true);
			Base::stop();
			return true;
		
		}
};
}

/**\brief TWI interface for distributed RegMaps
 * \tparam RegMap Register map for twi operation on mcu
 * \tparam chipAddress the address of the target chip
 *
 * This class is a singleton implementation of a twi bus driver.  It can be
 * used to communicate with target devices on the twi bus, that contain 8 and
 * 16 bit registers.
 **/
template<typename RegMap, uint8_t chipAddress>
struct Twi : public avr_halib::object::Singleton< helpers::TwiImpl<RegMap, chipAddress> >
{

};
}
}
}
