#pragma once

#include <avr-halib/drivers/avr/uart.h>
#include <stdint.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib logging */
namespace logging
{
/** AVR-HaLib logging devices */
namespace devices
{
    /** \brief TODO \todo */
    struct Uart : public drivers::avr::Uart
    {
        /** \brief TODO \todo
         *
         * \tparam Config TODO \todo
         */
        template< typename Config = DefaultConfig >
        struct configure
        {
            /** \brief TODO \todo */
            struct type : public avr_halib::drivers::avr::Uart::configure< Config >::type
            {
                /** \brief TODO \todo */
                typedef typename avr_halib::drivers::avr::Uart::configure< Config >::type Uart;

                public:
                    /** \brief the low-level output operator,
                     * \param c the character to be printed on the LCD
                     *
                     * This operator is the basis of all the ::logging::OutputStream
                     * operators. It takes car of special manipulation characters like
                     * %\v, which tells the driver to do a clear screen. Every other
                     * character is forwarded to the LCD driver.
                     */
                    Uart& operator<<(const char c)
                    {
                        if(c!='\v')
                        {
                            while(!this->ready());
                            this->put((uint8_t)c);
                        }

                        return *this;
                    }

                    /** \brief always 0
                     * \return the current position
                     */
                    uint8_t getPos()
                    {
                        return 0;
                    }

                    /** \brief empyt call
                     * \param pos the target position
                     */
                    void setPos(uint8_t pos)
                    {
                    }
            };
        };
    };
}
}
}
