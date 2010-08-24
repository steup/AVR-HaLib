/**
 *	\file	examples/application/canOutNoInt.cpp
 *	\brief	Example illustrating usage of CAN driver for reception
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#include "canConfigNoInt.h"

/** Typedef for the configured driver **/
typedef CanNoInt<SendConfig> Can;

/** Typedefs for the different data structures of the driver **/
typedef Can::MsgSend CanMsgSend;
typedef Can::RTRSend CanRTRSend;

/**\brief initialization of messages
 * \param can the can driver to use
 * \param msg the normal message to initialize
 * \param rtrA the CAN2.0A-RTR-message to initialize
 * \param rtrB the CAN2.0B-RTR-message to initialize
 **/

void initMsg(Can &can, CanMsgSend &msg, CanRTRSend &rtrA, CanRTRSend &rtrB)
{
	msg.id=can.getMaxId()-0xF;
	msg.length=sizeof(Can::IdType);
	reinterpret_cast<CanMsgSend::idType*>(msg.data)[0]=msg.id;

	rtrA.length=sizeof(uint16_t);
	rtrA.id=0x1;
	rtrA.set20ACompat(true);

	rtrB.length=sizeof(Can::IdType);
	rtrB.set20ACompat(false);
	rtrB.id=can.getMaxId()-0x10;
}

/**\brief update normal message content
 * \param can the can driver to use
 * \param msg the message to update
 **/

void updateMsg(Can &can, CanMsgSend &msg)
{
	if(msg.id++==can.getMaxId())
		msg.id=can.getMaxId()-0xF;

	reinterpret_cast<CanMsgSend::idType*>(msg.data)[0]=msg.id;
}

/** \brief main function
 
 * Application entry point, creates the Interrupt Handler and registers it with
 * the can driver, afterwards entering endless-loop.
 **/

int main()
{
	/** The configured instance of the driver**/
	Can can;
	/** The message structure for the message that will be send **/
	CanMsgSend msg;
	/** The RTR message structure for the CAN-2.0A-RTR that will be send **/
	CanRTRSend rtrA;
	/** The RTR message structure for the CAN-2.0B-RTR that will be send **/
	CanRTRSend rtrB;
	/** Storage for retrun values, for error handling**/
	Events e;
	
	initMsg(can, msg, rtrA, rtrB);

	while(true)
	{
		if(can.send(msg)==SUCCESS)
			while(!can.checkEvents() || msg.event==NOTHING);

		if(can.send(rtrA)==SUCCESS)
			while(!can.checkEvents() || msg.event==NOTHING);

		if(can.send(rtrB)==SUCCESS)
			while(!can.checkEvents() || msg.event==NOTHING);

		updateMsg(can, msg);

		delay_ms(1000);
	}
	return 0;
}
