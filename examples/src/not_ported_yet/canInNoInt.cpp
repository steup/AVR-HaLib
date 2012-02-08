/**
 *	\file	examples/application/canInNoInt.cpp
 *	\brief	Example illustrating usage of CAN driver for reception
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfigNoInt.h"

#include <avr-halib/avr/canary/can_debug.h>

/** Typedef for the configured driver **/
typedef CanNoInt<RecvConfig> Can;

/** Typedefs for the different data structures of the driver **/
typedef Can::MsgRecv CanMsgRecv;
typedef Can::Error   CanError;

/**\brief initialization of messages
 * \param can the can driver to use
 * \param msg the normal message to initialize
 **/

void initMsg(Can &can, CanMsgRecv &msg)
{
	msg.id=can.getMaxId()-0xF;
	msg.idMask=can.getMaxId()-0xF;
	msg.length=sizeof(Can::IdType);
	msg.setCyclic(true);
}

/** \brief main function
 
 * Application entry point, fill the message structure with the information
 * needed for receiving. Afterwards it polls the driver for events and handle
 * every occuring event.
  
 **/

int main()
{
	Can can;
	LCD lcd;
	CanMsgRecv msg;
	Events e;
	
	initMsg(can, msg);

	e=can.recv(msg);
	if(e!=SUCCESS)
		lcd << "Error: " << eventToString(e);
	else
		lcd << "Start receiving";

	while(true)
	{
		if(!can.checkEvents())
			continue;
		switch(msg.event)
		{
			case(RECEIVEOK):
				lcd.clear();
				lcd << "Received Msg:" << (int32_t)msg.length;
				lcd << "\nMsg: " << ((int32_t*)msg.data)[0];
				lcd << "\nID: " << (int32_t)msg.id;
				lcd << "\nTS: " << (int32_t)msg.timeStamp;

				break;
			default:
				lcd << "\n " << eventToString(msg.event);
				break;
		}
	}
	return 0;
}
