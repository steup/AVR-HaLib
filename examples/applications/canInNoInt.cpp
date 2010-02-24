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

Can can;
LCD lcd;
CanMsgRecv msg;

/** \brief main function
 
 * Application entry point, fill the message structure with the information
 * needed for receiving. Afterwards it polls the driver for events and handle
 * every occuring event.
  
 **/

int main()
{
	msg.setCyclic(true);
	msg.id=can.getMaxId()-0xF;
	msg.idMask=can.getMaxId()-0xF;
	msg.length=MAXMSGLEN;
	Events event=can.recv(msg);
	if(event!=SUCCESS)
		lcd << "\nError: " << eventToString(event);

	while(true)
	{
		if(!can.checkEvents())
			continue;
		switch(msg.event)
		{
			case(RECEIVEOK):
				lcd.clear();
				lcd << "Received Msg:" << (int32_t)msg.length;
				lcd << "\nMsg: " << (int32_t)(*msg.data);
				lcd << "\nID: " << (int32_t)msg.id;
				break;
			default:
				lcd << "\n " << eventToString(msg.event);
				break;
		}
	}
	return 0;
}
