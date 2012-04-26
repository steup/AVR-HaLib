#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>
#include <stdint.h>

namespace avr_halib{
namespace regmaps{
namespace local{
namespace atmega128rfa1{
/**\brief EEPROM register map for atmega128rfa1
 *
 *  Abstraction of EEPROM access registers.
 *  Especially EECR, EEARL, EEARH, EEDR.
 **/
struct EEPROM : public base::LocalRegMap
{
	public:
		/** \brief Write mode for EEPROM access **/
		enum WriteMode
		{
			atomic    = 0, /**< atomic access, erase and write in one operation **/
			eraseOnly = 1, /**< erase operation only, data register ignored **/
			writeOnly = 2  /**< write operation only, operation represents new = old & new **/
		};
	private:
		uint8_t pad0[0x3F];
	public:
		union
		{
			struct
			{	/** \brief value of true and SyncRegMap() starts a read operation **/
				bool   startRead       : 1;
				/** \brief value of true and SyncRegMap() only starts a write operation, if enableWrite was set not more than 4 cycles ago**/
				bool   startWrite      : 1;
				/** \brief activates startWrite for 4 cycles **/
				bool   enableWrite     : 1;
				/** \brief activates EEPROM operation complete interrupt **/
				bool   interruptEnable : 1;
				/** \brief set write mode
				 *
				 *  \see WriteMode
				 **/
				WriteMode mode         : 2;
			};
			uint8_t eecr;
		};
		union
		{	/**\brief data register**/
			uint8_t data;
			uint8_t eedr;
		};
		union
		{
			struct
			{
				uint8_t eearl;
				uint8_t eearh;
			};
			/**\brief data address **/
			uint16_t address;
		};
};

}
}
}
}
