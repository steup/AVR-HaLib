/**
 *	\file	examples/application/canOut.cpp
 *	\brief	Example illustrating usage of CAN driver for transmission
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfig.h"

#include <avr/sleep.h>

/**Declares, that we want to use the overflow interrupt of the CAN timer**/
UseInterrupt(SIG_CAN_OVERFLOW1);

/** Typedef for the configured driver **/
typedef Canary<SendConfig> Can;

/** Typedefs for the different data structures of the driver **/
typedef Can::MsgSend CanMsgSend;
typedef Can::RTRSend CanRTRSend;

/** \brief Handler class for the output test program
 
 * This class contains all interrupt handlers, that are needed for the output
 * test program.
 
 * In this special case the overflow callback is used.
 **/

class InterruptHandler
{
	private:
		/** The can driver **/
		Can can;
		/** The message structure for the message that will be sended **/
		CanMsgSend msg;
		/** The RTR message structure for the RTR that will be sended **/
		CanRTRSend rtr;
	
	public:
		/** \brief Constructor
		 *
		 *  Initializes the changing ID and the size of the message
		 * */
		InterruptHandler()
		{
			msg.id=can.getMaxId()-0xF;
			msg.length=sizeof(Can::IdType);
		}
		~InterruptHandler(){}

		/**\brief enables the different Callbacks in the CAN Driver**/

		void enable()
		{
			can.enableOverrunInterrupt<InterruptHandler, &InterruptHandler::overrun>(*this);
		}

		/** \brief handler function for the overrun callback
		 
		 * This function is used to send a message in a given time itervall
		 * with growing IDs It is called everytime the internal CAN-Timer
		 * overflows.
		 **/

		void overrun()
		{	
			reinterpret_cast<CanMsgSend::idType*>(msg.data)[0]=msg.id;

			can.send(msg);
			rtr.length=sizeof(uint16_t);
			rtr.id=0x1;
			rtr.setRTR(true);
			rtr.set20ACompat(true);
			Events event=can.send(rtr);
			if(event==SUCCESS)
			{
				rtr.length=sizeof(Can::IdType);
				rtr.set20ACompat(false);
				rtr.id=can.getMaxId()-0x10;
				event=can.send(rtr);
			}
			
			if(msg.id++==can.getMaxId())
				msg.id=can.getMaxId()-0xF;
		}
} handler;

/** \brief main function
 
 * Application entry point, creates the Interrupt Handler and registers it with
 * the can driver, afterwards entering sleep in endless-loop.
 **/

int main()
{
	handler.enable();

	set_sleep_mode(0x1);
	while(1)
	{
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
