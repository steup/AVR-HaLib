#pragma once

#include <avr-halib/ext/lcm_16x4.h>
#include <avr-halib/share/singleton.h>

//We want to declare our own output type for the logging framework 
#define LOGGING_DEFINE_EXTENDED_OUTPUT_TYPE
#include <logging/logging.h>

namespace avr_halib
{
namespace logExt
{
	/**\brief Extension of the ::logging::log struct
	 * 
	 * This struct contains additional enumerations for LCD specific behaviour,
	 * in this case: clear screen. The original functionality of ::logging::log
	 * is still fully available.
	 **/
	struct log : public ::logging::log
	{	
		public:
			/**\brief Internal structure to set position of LCD**/
			struct SetPos
			{
				/**\brief position to set**/
				uint8_t pos;
			};

			/**\brief Internal structure to get position of LCD**/
			struct GetPos
			{
				/**\brief storage to write position to**/
				uint8_t *pos;
			};

		public:
			/**\brief Extension of manipulator list**/
			enum ExtendedManipulators
			{
				clear='\v'	/**< clear the screen of the LCD**/
			};

			/**\brief parametrized manipulator for getting the current LCD position
			 * \param ref a reference to store position into
			 *
			 * This call writes the curren position of the LCD into the reference supplyed by ref.
			 **/
			static GetPos getPos(uint8_t& ref)
			{
				GetPos pos;
				pos.pos=&ref;
				return pos;
			}

			/**\brief parametrized manipulator for setting the current LCD position
			 * \param newPos the new position of the LCD
			 *
			 * This call sets the current position of the LCD to the value newPos.
			 **/
			static SetPos setPos(uint8_t newPos)
			{
				SetPos pos;
				pos.pos=newPos;
				return pos;
			}
	};

	/**\brief Interface wrapper for LCD driver
	 * \tparam PortMap the port map, that defines the used pins for the LCD
	 *
	 * This class maps the needed functionality of the LCD to the interface,
	 * the ::logging::OutputStream needs of it`s base class.
	 **/
	template<typename PortMap>
	class LoggingLcd
	{
		private:
			/**\brief Instance of the LCD driver to use
			 *
			 * The usage of a singleton in this instance is a workaround for an
			 * avr-halib BUG.  The LCD class cann not be constructed globally,
			 * until this is fixed, the usage of a singleton is necessary to
			 * force non-static construction
			 **/
			typedef object::Singleton<Lcm_16x4<PortMap> > Base;

		public:
			/**\brief the low-level output operator,
			 * \param c the character to be printed on the LCD
			 *
			 * This operator is the basis of all the ::logging::OutputStream
			 * operators. It takes car of special manipulation characters like
			 * %\v, which tells the driver to do a clear screen. Every other
			 * character is forwarded to the LCD driver.
			 **/
			LoggingLcd& operator<<(const char c)
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

	/**\brief Extension to ::logging::OutputStream to make log::ExtendedManipulators known
	 * \tparam base the OutputStream to use
	 *
	 * This class contains extended functionality for the
	 * ::logging::OutputStream, it creates a special operator to cope with the
	 * new ExtendedManipulators enumeration of the log struct.
	 **/
	template<typename base>
	class LcdOutputStreamExtension : public base
	{
		private:

		public:
			/**\brief Passes the extended manipulators to the LCD driver
			 * \param m the manipulator
			 * \return the current instance of the stream, for chaining.
			 *
			 * This function changes the type of the manipulator to char, so
			 * that the underlying ::logging::OutputStream passes it unchanged
			 * to the LCD driver, which can then react to it.
			 **/
			LcdOutputStreamExtension& operator<<(const log::ExtendedManipulators m)
			{
				base::operator<<(static_cast<char>(m));
				return *this;
			}

			/**\brief Applies the parametrized manipulator getPos(&pos)
			 *
			 * \param pos an instance of the internal structure GetPos as
			 * returned by the parametrized manipulator log::getPos(&pos)
			 **/
			LcdOutputStreamExtension& operator<<(log::GetPos pos)
			{
				*(pos.pos)=base::getPos();
				return *this;
			}
			
			/**\brief Applies the parametrized manipulator setPos(pos)
			 *
			 * \param pos an instance of the internal structure SetPos as
			 * returned by the parametrized manipulator log::setPos(pos)
			 **/
			LcdOutputStreamExtension& operator<<(const log::SetPos pos)
			{
				base::setPos(pos.pos);
				return *this;
			}

			/**\brief Passes all unknown types to the ::logging::OutputStream
			 * \tparam T the type of arguemnt
			 * \param t the argument itself
			 * \return the current instance of the stream, for chaining.
			 *
			 * This function passes every unknown type to the underlying
			 * OuputStream for further processing.
			 **/
			template<typename T>
			LcdOutputStreamExtension& operator<<(T t)
			{
				base::operator<<(t);
				return *this;
			}
	};

	/**\brief Combined LCD Output stream
	 * Create an LcdOutputStream object by combining all extension of the lcdExt module.
	 **/
	typedef avr_halib::logExt::LcdOutputStreamExtension<
				::logging::OutputStream<
					LoggingLcd<
						LcdPortMap
					>
				>
			> LcdOutputStream;

}
}

/**\brief Appropriate type for the logging framework**/
typedef ::logging::OutputLevelSwitchDisabled <
	       	avr_halib::logExt::LcdOutputStream
		> LcdLoggingType;


//Set the created LCD output type in the logging framework
LOGGING_DEFINE_OUTPUT( LcdLoggingType )
