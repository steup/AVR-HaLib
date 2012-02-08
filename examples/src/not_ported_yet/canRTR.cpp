/**
 *	\file	examples/application/canRTR.cpp
 *	\brief	Example illustrating usage of CAN driver for handling RTRs
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfig.h"

#include <avr/sleep.h>
#include <string.h>

/*Definition of a template free configured driver*/
typedef Canary<RTRConfig> Can;

/*Definition of the used driver dependant types*/
typedef Can::MsgSend CanMsgSend;
typedef Can::MsgRecv CanMsgRecv;

/** \brief Handler class for the remote-transmission-request test program.
 
 * This class contains all interrupt handlers, that are needed for the
 * remote-transmission-request test program.
 
 * In this special case the receive, transmit are used.
 **/

class InterruptHandler
{
	private:
		Can can; /*< The configured Canary*/
		CanMsgRecv msg; /*< Message structure for incoming messages*/
		CanMsgRecv autoReply; /*< Message structure for auto reply*/
		CanMsgSend out; /*< Message structure for outgoing messages*/
	
	public:
		/** \brief Constructor
		 
		 *  Initializes the content of the RTR answer and readys a slot for
		 *  Auto-Reply
		 **/

		InterruptHandler() : can()
		{}

		~InterruptHandler(){}

		/** \brief sets up the auto reply message structure and activates it.
		 * The auto reply will stay active until setAutoReply(false) is called.
		 **/
		void setupAutoReply()
		{
			autoReply.id=can.getMaxId()-0x10;
			autoReply.idMask=can.getMaxId();
			memcpy(autoReply.data, "auto", 4);
			autoReply.length=4;
			can.setAutoReply(autoReply);
		}

		/**\brief registers and enables the used callbacks in the Canary**/

		void enable()
		{
			can.enableReceiveInterrupt<InterruptHandler, 
									   &InterruptHandler::receive>(*this);
		}
		
		/** \brief Receive callback
		 
		 * This function is called after receiving a message.  It answers the
		 * rtr, by sending a can message of the same id, without the rtr-flag.
		 
		 * @param msg a reference to the received message
		 **/

		void receive(CanMsgRecv &msg)
		{
			out.setRTR(false);
			if(msg.get20ACompat())
			{
				out.length=(sizeof(uint16_t)<msg.length)?
							sizeof(uint16_t):
							msg.length;

				out.set20ACompat(true);
				reinterpret_cast<uint16_t*>(out.data)[0]=msg.id;
			}
			else
			{
				out.length=(sizeof(uint32_t)<msg.length)?
					        sizeof(uint32_t):
						    msg.length;

				out.set20ACompat(true);
				reinterpret_cast<uint32_t*>(out.data)[0]=msg.id;
			}
			out.id=msg.id;
			can.send(out);
		}

		/** \brief Starts the receiving of a message.
		 *
		 * Sets the driver up so that it receives a message.
		 * This method uses the cyclic receive method, so it must only be called
		 * once for each method, until setCyclic(false) is done.
		 **/

		void startReceive()
		{
			msg.id=0x001;
			msg.idMask=can.getMaxId();
			msg.setRTR(true);
			msg.setCyclic(true);
			msg.setRTRMask(true);
			msg.set20ACompat(true);
			can.recv(msg);

		}
} handler;

/** \brief main function
 
 * Application entry point, creates the Interrupt-Handler and registers it with
 * the can driver, afterwards entering endless-loop.  During endless loop it
 * answers the last RTR, starts receiving of a new message and then goes to
 * sleep.
 **/

int main()
{
	handler.enable();

	handler.setupAutoReply();
	handler.startReceive();

	set_sleep_mode(0x1);
	cli();
	sleep_enable();
	sei();

	while(1)
		sleep_cpu();
}
