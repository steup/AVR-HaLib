/*! \file   examples/application/ucframeIRQ.cpp
 *  \brief  Example illustrating usage of the CFrame layer with interrupts.
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
UseInterrupt(SIG_UART1_RECV);
UseInterrupt(SIG_UART1_DATA);
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

typedef CFrame< Uart< UCFG1 >, uint8_t > fdev_t;       // frame device!!!
typedef fdev_t::mob_t mob_t;
/* === globals ============================================================== */
fdev_t fdev;
const char* aMsg = "01234567Test";
mob_t message;
/* === functions ============================================================ */
/*! \brief  If device is ready send message and waiting for a new message.*/
void handleReady()
{
    fdev.send( message );               // see CFrame for available interfaces
}
/*! \brief  If message was received read it out and send it back.*/
void handleReceive()
{
    fdev.put('-');
    if ( fdev.recv( message ) != 0 ) {  // see CFrame for available interfaces
        fdev.enableonReady();           // send message if ready FIXME
    }
}
/* === main ================================================================= */
int main()
{
    message.size       = 0;             // initialize message
    for(;message.size < 12;message.size++)
        message.payload[message.size] = aMsg[message.size];
    //----------------------------------------------------------------
    sei();                              // enable interrupts
    fdev.onReady.bind <& handleReady>();
    fdev.onReceive.bind <& handleReceive>();
    fdev.enableonReady();               // send message if ready FIXME
    //----------------------------------------------------------------
    do {                                // duty cycle
        fdev.put('.');
        delay_ms(500);
    } while(true);
    //----------------------------------------------------------------
}

