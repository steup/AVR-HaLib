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
#include "avr-halib/share/cdevice.h"    // implements character device interface
#include "avr-halib/share/cdeviceframe.h"     // implementation cframe
#include "avr-halib/share/delay.h"      // timings and delays
/* === macros =============================================================== */
UseInterrupt(SIG_UART0_RECV);
UseInterrupt(SIG_UART0_DATA);
/* === types ================================================================ */
struct RBoardController                 // configuration of hardware platform
{
	enum
	{
#ifdef __AVR_ATmega128__
		controllerClk = 8000000
#else
		controllerClk = 16000000
#endif
	};
};
struct UCFG0 : public Uart0<RBoardController> { enum { baudrate = 19200 }; };
struct UCFG1 : public Uart1<RBoardController> { enum { baudrate = 19200 }; };

typedef COutDevice< SecOut< Uartnoint< UCFG1 > > > cdev_t;  // character device
typedef CDeviceFrame< Uart< UCFG0 >, uint8_t > fdev_t;       // frame device!!!
typedef fdev_t::mob_t mob_t;
/* === globals ============================================================== */
volatile bool status;
cdev_t cdev;
fdev_t fdev;
const char* aMsg = "012345Test";
mob_t message;
/* === functions ============================================================ */
/*! \brief  If device is ready send message and waiting for a new message.*/
void handleReady()
{
	fdev.send( message );           // see CFrame for available interfaces
	fdev.onReady.reset();
}

void bindHandleReady()
{
	fdev.onReady.bind <& handleReady>();
	fdev.enableonReady();               // send message if ready
}

/*! \brief  If message was received read it out and send it back.*/
void handleReceive()
{
	if ( fdev.recv( message ) != 0 )    // see CFrame for available interfaces
	{
		delay_ms(100);
		status = true;
		cdev << "[Message: [" << (int32_t)message.size << "] [";
		for(uint8_t index = 0; index < message.size; index++)
		{
			cdev.put( (char)message.payload[index] );
		}
		cdev << "]]\n\r";
		delay_ms(100);
		bindHandleReady();           // send message if ready
	}
}

void bindHandleReceive()
{
	fdev.onReceive.bind <& handleReceive>();
	fdev.enableonReceive();
}

/* === main ================================================================= */
int main()
{
	message.size       = 0;             // initialize message
	for(; message.size < 10; message.size++)
		message.payload[message.size] = aMsg[message.size];
	status = true;
	//----------------------------------------------------------------
	sei();                              // enable interrupts
	bindHandleReady();
	bindHandleReceive();
	//----------------------------------------------------------------
	cdev << "Starting CFrame Example:\n\r";
	do                                  // duty cycle
	{
		delay_ms(500);
	}
	while(true);
	//----------------------------------------------------------------
}

