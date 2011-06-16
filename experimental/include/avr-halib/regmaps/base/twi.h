#pragma once

#include <avr-halib/avr/twi.h>
#include <avr-halib/share/singleton.h>

namespace avr_halib
{
namespace regmaps
{
namespace base
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
struct Twi
{
	private:
		/**\brief Basic TWI driver for bus operation**/
		typedef typename avr_halib::object::Singleton<TWIMaster< RegMap > >::type Base;
	public:

		/**\brief Constructor initializes bus**/
		Twi()
		{
			Base::getInstance().init(128);
		}

		/**\brief write 1 byte to a bus device
		 * \param address the address to write the data to
		 * \param data the data to write
		 **/
		bool write(uint8_t address, uint8_t* data, uint8_t size)
		{
			Base& base=Base::getInstance();

			if(!base.start(chipAddress, false))
				return false;
			if(!base.write(address))
				return false;
			while(size--)
				if(!base.write(*data++))
					return false;
			base.stop();
			return true;
		}

		/**\brief read 2 byte from a bus device
		 * \param address the address to read the data from
		 * \param data a reference to write the data to
		 **/
		bool read(uint8_t address, uint8_t* data, uint8_t size)
		{
			Base& base=Base::getInstance();

			if(!base.start(chipAddress, false))
				return false;
			if(!base.write(address))
				return false;
			if(!base.start(chipAddress, true))
				return false;
			while(size--)
				*data++=base.read(((size-1)?true:false));
			base.stop();
			return true;
		
		}
};

}
}
}
