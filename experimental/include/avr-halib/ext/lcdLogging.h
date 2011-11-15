#pragma once

#include <avr-halib/ext/lcm_16x4.h>
#include <avr-halib/share/singleton.h>
#include <stdint.h>

namespace avr_halib
{
namespace logExt
{
namespace devices
{
	/**\brief Interface wrapper for LCD driver
	 * \tparam PortMap the port map, that defines the used pins for the LCD
	 *
	 * This class maps the needed functionality of the LCD to the interface,
	 * the ::logging::OutputStream needs of it`s base class.
	 **/
	template<typename config>
	class LCD
	{
		private:
			/**\brief Instance of the LCD driver to use
			 *
			 * The usage of a singleton in this instance is a workaround for an
			 * avr-halib BUG.  The LCD class cann not be constructed globally,
			 * until this is fixed, the usage of a singleton is necessary to
			 * force non-static construction
			 **/
			typedef object::Singleton<Lcm_16x4<config> > Base;

		public:
			/**\brief the low-level output operator,
			 * \param c the character to be printed on the LCD
			 *
			 * This operator is the basis of all the ::logging::OutputStream
			 * operators. It takes car of special manipulation characters like
			 * %\v, which tells the driver to do a clear screen. Every other
			 * character is forwarded to the LCD driver.
			 **/
			LCD& operator<<(const char c)
			{
				if(c=='\v')
					Base::getInstance().clear();
				else
					Base::getInstance().put(c);
				return *this;
			}
			
			/**\brief forward call to LCD getPos
			 *  \return the current LCD position
			 **/
			uint8_t getPos()
			{
				return Base::getInstance().getPos();
			}

			/**\brief forward call to LCD setPos
			 *  \param pos the target LCD position
			 **/
			void setPos(uint8_t pos)
			{
				return Base::getInstance().setPos(pos);
			}
	};
}
}
}
