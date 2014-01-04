#pragma once

//We want to declare our own output type for the logging framework
#define LOGGING_DEFINE_OWN_OUTPUT_TYPE
#include <logging/logging.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib object */
namespace object
{
    template<typename, uint8_t>
    class FixPointValue;
}
}

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib logging */
namespace logging
{
    /** \brief Extension of the ::logging::log struct
     *
     * This struct contains additional enumerations for LCD specific behaviour,
     * in this case: clear screen. The original functionality of ::logging::log
     * is still fully available.
     */
    struct log : public ::logging::log
    {
        public:
            /** \brief TODO \todo */
            typedef ::logging::Trace Trace;
            /** \brief TODO \todo */
            typedef ::logging::Info Info;
            /** \brief TODO \todo */
            typedef ::logging::Error Error;
            /** \brief TODO \todo */
            typedef ::logging::Warning Warning;
            /** \brief TODO \todo */
            typedef ::logging::Void Void;
            /** \brief TODO \todo */
            typedef ::logging::Debug Debug;
            ;
            /** \brief Internal structure to set position of LCD */
            struct SetPos
            {
                /**\brief position to set**/
                uint8_t pos;
            };

            /** \brief Internal structure to get position of LCD */
            struct GetPos
            {
                /** \brief storage to write position to */
                uint8_t *pos;
            };

            /** \brief TODO \todo */
            struct SetPrecision
            {
                /** \brief TODO \todo */
                uint8_t precision;
                /** \brief TODO \todo */
                SetPrecision(uint8_t value) : precision(value){};
            };

        public:
            /** \brief Extension of manipulator list */
            enum ExtendedManipulators
            {
                clear='\v'  /**< clear the logging device */
            };

            /** \brief parametrized manipulator for getting the current LCD position
             * \param ref a reference to store position into
             *
             * This call writes the curren position of the LCD into the reference supplyed by ref.
             */
            static GetPos getPos(uint8_t& ref)
            {
                GetPos pos;
                pos.pos=&ref;
                return pos;
            }

            /** \brief parametrized manipulator for setting the current LCD position
             * \param newPos the new position of the LCD
             *
             * This call sets the current position of the LCD to the value newPos.
             */
            static SetPos setPos(uint8_t newPos)
            {
                SetPos pos;
                pos.pos=newPos;
                return pos;
            }

            /** \brief TODO \todo */
            static SetPrecision setPrecision(uint8_t value)
            {
                return SetPrecision(value);
            }
    };

    /** \brief Extension to ::logging::OutputStream to make log::ExtendedManipulators known
     * \tparam base the OutputStream to use
     *
     * This class contains extended functionality for the
     * ::logging::OutputStream, it creates a special operator to cope with the
     * new ExtendedManipulators enumeration of the log struct.
     */
    template<typename base>
    class OutputStreamExtension : public base
    {
        private:
            /** \brief TODO \todo */
            uint8_t precision;

        public:
            OutputStreamExtension() : precision(3){}

            /** \brief Passes the extended manipulators to the LCD driver
             * \param m the manipulator
             * \return the current instance of the stream, for chaining.
             *
             * This function changes the type of the manipulator to char, so
             * that the underlying ::logging::OutputStream passes it unchanged
             * to the LCD driver, which can then react to it.
             */
            OutputStreamExtension& operator<<(const log::ExtendedManipulators m)
            {
                base::operator<<(static_cast<char>(m));
                return *this;
            }

            /** \brief Applies the parametrized manipulator getPos(&pos)
             *
             * \param pos an instance of the internal structure GetPos as
             * returned by the parametrized manipulator log::getPos(&pos)
             */
            OutputStreamExtension& operator<<(log::GetPos pos)
            {
                *(pos.pos)=base::getPos();
                return *this;
            }

            /** \brief Applies the parametrized manipulator setPos(pos)
             *
             * \param pos an instance of the internal structure SetPos as
             * returned by the parametrized manipulator log::setPos(pos)
             */
            OutputStreamExtension& operator<<(const log::SetPos pos)
            {
                base::setPos(pos.pos);
                return *this;
            }

            /** \brief TODO \todo */
            OutputStreamExtension& operator<<(const log::SetPrecision precision)
            {
                this->precision=precision.precision;
                return *this;
            }

            /** \brief TODO \todo
             *
             * \tparam T TODO \todo
             * \tparam offset TODO \todo
             */
            template<typename T, uint8_t offset>
            OutputStreamExtension& operator<<(const object::FixPointValue<T, offset>& f)
            {
                f.log(*this, this->getBase(), this->precision);
                return *this;
            }

            /** \brief Passes all unknown types to the ::logging::OutputStream
             * \tparam T the type of arguemnt
             * \param t the argument itself
             * \return the current instance of the stream, for chaining.
             *
             * This function passes every unknown type to the underlying
             * OuputStream for further processing.
             */
            template<typename T>
            OutputStreamExtension& operator<<(T t)
            {
                base::operator<<(t);
                return *this;
            }
    };

    namespace config
    {
        /** \brief Combined LCD Output stream
         * Create an LcdOutputStream object by combining all extension of the lcdExt module.
         */
        typedef OutputStreamExtension<
            ::logging::OutputStream<
            LoggingDevice
            >
            > OutputStream;
    }
}
}

LOGGING_DEFINE_OUTPUT( ::logging::OutputLevelSwitchDisabled< avr_halib::logging::config::OutputStream > );

typedef ::logging::detail::Logger<>::return_type LoggingOutput;
