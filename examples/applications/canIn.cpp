/**
 *	\file	examples/application/canIn.cpp
 *	\brief	Example illustrating usage of CAN driver for reception
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfig.h"

#include <avr-halib/avr/canary/can_debug.h>

#include <avr/sleep.h>

/** Typedef for the configured driver **/
typedef Canary<RecvConfig> Can;

/** Typedefs for the different data structures of the driver **/
typedef Can::MsgRecv CanMsgRecv;
typedef Can::Error   CanError;

/** \brief Handler class for the input test program
 
 * This class contains all interrupt handlers, that are needed for the input
 * test program.
 
 * In this special case the receive and error callbacks are used.
 **/

class InterruptHandler
{
	private:
		Can can;
		LCD lcd;
		CanMsgRecv msg;

	public:
		/** \brief Constructor
		 *
		 * does nothing special
		 * */

		InterruptHandler(): can(), lcd(){}
		~InterruptHandler(){}

		/** \brief Receive callback
		 *
		 * This function is called after receiving a message.
		 * It prints the received message ont the lcd
		 *
		 * @param msg a reference to the received message
		 **/

		void receive(CanMsgRecv &msg)
		{
			lcd.clear();
			lcd << "Received Msg:" << (int32_t)msg.length;
			lcd << "\nMsg: " << (int32_t)(*msg.data);
			lcd << "\nID: " << (int32_t)msg.id;
			//lcd << "\nTS: " << msg.timeStamp;
		}

		/** \brief Starts the receiving of a message.
		 *
		 * Sets the driver up so that it receives a message.
		 **/

		void startReceive()
		{
			msg.setCyclic(true);
			msg.id=can.getMaxId()-0xF;
			msg.idMask=can.getMaxId()-0xF;
			msg.length=MAXMSGLEN;
			Events event=can.recv(msg);
			if(event!=SUCCESS)
			{
				lcd << "\nError: " << eventToString(event);
			}
		}

		/**\brief enables the different Callbacks in the CAN Driver**/

		void enable()
		{
			can.enableReceiveInterrupt<InterruptHandler,
				&InterruptHandler::receive>(*this);
			can.enableErrorInterrupt<InterruptHandler,
				&InterruptHandler::error>(*this);
		}

		/** \brief handler function for the transmit callback
		 
		 * This function is used to output a message everytime a message was
		 * send correctly.  It is called everytime the Internal Message-Slot is
		 * done sending.
		 
		 *The oldMsg parameter contains the information about the
		 sending-process, like ID, data, state and so on.
		 
		 * @param oldMsg a reference to the Message-structure, that was the
		 * parameter of the send-function
		 **/

		void error(CanError &oldMsg)
		{
			lcd << "\n " << eventToString(oldMsg.event);
		}
} handler;

/** \brief main function
 
 * Application entry point, creates the Interrupt Handler and registers it with
 * the can driver, afterwards entering endless-loop.  During endless loop it
 * starts receiving a new message and then goes to sleep.
 
 **/

int main()
{
	handler.enable();

	handler.startReceive();

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
