#pragma once

#include <avr-halib/avr/uart.h>
#include <stdint.h>

namespace avr_halib
{
namespace logging
{
namespace devices
{
	struct Uart : public drivers::Uart
    {
        template< typename Config = DefaultConfig >
        struct configure
        {
            struct type : public avr_halib::drivers::Uart::configure< Config >::type
            {
                typedef typename avr_halib::drivers::Uart::configure< Config >::type Uart;

                public:
                    /**\brief the low-level output operator,
                     * \param c the character to be printed on the LCD
                     *
                     * This operator is the basis of all the ::logging::OutputStream
                     * operators. It takes car of special manipulation characters like
                     * %\v, which tells the driver to do a clear screen. Every other
                     * character is forwarded to the LCD driver.
                     **/
                    Uart& operator<<(const char c)
                    {
                        if(c!='\v')
                        {
                            while(!this->ready());
                            this->put((uint8_t)c);
                        }

                        return *this;
                    }
                    
                    /**\brief always 0
                     *  \return the current position
                     **/
                    uint8_t getPos()
                    {
                        return 0;
                    }

                    /**\brief empyt call
                     *  \param pos the target position
                     **/
                    void setPos(uint8_t pos)
                    {
                    }
            };
        };
	};
}
}
}
