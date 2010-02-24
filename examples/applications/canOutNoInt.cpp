/**
 *	\file	examples/application/canOutNoInt.cpp
 *	\brief	Example illustrating usage of CAN driver for reception
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfigNoInt.h"

#include <avr-halib/avr/timer.h>
#include <avr-halib/avr/regmaps.h>

/**Declares, that we want to use the output compare match interrupt of Timer 0**/
UseInterrupt(SIG_OUTPUT_COMPARE0);

/** Typedef for the EggTimer **/
typedef ExactEggTimer<Timer0> OneShotTimer;

/** Typedef for the configured driver **/
typedef CanNoInt<SendConfig> Can;

/** Typedefs for the different data structures of the driver **/
typedef Can::MsgSend CanMsgSend;
typedef Can::RTRSend CanRTRSend;


OneShotTimer timer;

/** \brief Handler class for the output test program
 
 * This class contains all interrupt handlers, that are needed for the output
 * test program.
 
 * In this special case the overflow callback is used.
 **/

class TransmissionHandler
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
		TransmissionHandler()
		{
			msg.id=can.getMaxId()-0xF;
			msg.length=sizeof(Can::IdType);
		}

		/** \brief transmission function
		 
		 * This function is used to send a message in a given time itervall
		 * with growing IDs.
		 **/

		void transmit()
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

			timer.start(100);
		}

		void check()
		{
			can.checkEvents();
		}
} handler;

/** \brief main function
 
 * Application entry point, creates the Interrupt Handler and registers it with
 * the can driver, afterwards entering sleep in endless-loop.
 **/

int main()
{
	timer.onTimerDelegate.bind<TransmissionHandler, &TransmissionHandler::transmit>(&handler);
	timer.start(100);
	while(true);
		handler.check();
	return 0;

}
