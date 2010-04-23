/*! \file   examples/application/ucframe.cpp
 *  \brief  Example illustrating usage of the CFrame layer.
 *  \author Thomas Kiebel, Karl Fessel
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */
/* ========================================================================== */
#ifdef __AVR_ATmega128__
#	define CPU_FREQUENCY 8000000UL
#else
#	define CPU_FREQUENCY 16000000UL
#endif
#define F_CPU CPU_FREQUENCY
/* === includes ============================================================= */
#include "avr-halib/avr/uart.h"         // implementation uart
#include "avr-halib/share/cframe.h"     // implementation cframe
#include "avr-halib/share/delay.h"      // timings and delays
/* === macros =============================================================== */
/* === types ================================================================ */
struct RBoardController                 // configuration of hardware platform
{
    enum
    {
#ifdef __AVR_ATmega128__
        controllerClk=8000000
#else
        controllerClk=16000000
#endif
    };
};
struct UCFG1 : public Uart1<RBoardController> { enum{ baudrate=19200 }; };

typedef CFrameNoInt< Uart< UCFG1 >, uint8_t > fdev_t;       // frame device!!!
typedef fdev_t::mob_t mob_t;
/* === globals ============================================================== */
fdev_t fdev;
const char* aMsg = "01234567Test";
/* === functions ============================================================ */
/* === main ================================================================= */
int main()
{
    mob_t message;
    sei();                              // enable interrupts
    //----------------------------------------------------------------
    message.size       = 0;             // initialize message
    for(;message.size < 12;message.size++)
        message.payload[message.size] = aMsg[message.size];
    //----------------------------------------------------------------
    if ( !fdev.send( message ) )
    {
        //TODO put error handling here
    }
    do {                                // duty cycle
        if ( fdev.recv( message ) )     // wait for a message
        {
            if ( !fdev.send( message ) )
            {
                //TODO put error handling here
            }
        }
        delay_ms(50);
    } while(true);
    //----------------------------------------------------------------
}
