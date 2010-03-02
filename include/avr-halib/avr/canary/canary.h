/**
 *	\file	avr-halib/avr/canary/canary.h
 *	\brief	Canary - CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_interrupts.h"

namespace avr_halib
{

namespace canary
{

/** \brief The CAN driver class for the AVR at90can128.

 *  \tparam Config A struct containing the configuration parameters for the CAN
 *  driver

 * This class provides the interface to use the CAN functionality of the
 * at90can128.  It uses an callback system to define the appropriate handling
 * of the configured interrutps. There are 4 possible callbacks that can be
 * registered, these are Overrun, Receive, Error and Transmit. The availability
 * of the callbacks depends on the configuration, if a functionality is
 * disabled, the function to set and unset the callback are not available.  The
 * Overrun callback is activated when the internal timer of the CAN controller
 * switches from 0xFFFF to 0x0000.  The Receive callback is activated when a
 * message with correct ID is received.  The Error callback is activated when
 * an error occured during send or receive operations and the Transmit callback
 * is activated when an message is completely send.  If the functionality is
 * activated, but no callback is registered, no function will be called, but
 * the state of the message structure will be updated. Therefore user
 * deactivate the appropriate callback the function will not be called, but the
 * state infromation in the Message Object, that was provided for the operation
 * will be updatet. Therefore it is possible to use polling to handle events.
 **/

/**\ingroup canary*/
template<class Config>
class Canary : public ErrorHandler<Config::useError, Config>
{

	public:

		/** \brief The type for the ID of the CAN messages
		 
		 * This depends on the used CAN version.**/

		typedef typename CANBase<Config>::IdType IdType;

		/** \brief The standard message structure to send a CAN message.**/

		typedef can_templates::CANMsgSend<
					static_cast<Versions>(Config::version)>
				MsgSend;

		/** \brief The standard message structure to receive a CAN message.

		 * It can also be used for sending a message.
		 **/

		 typedef can_templates::CANMsgRecv<
					static_cast<Versions>(Config::version),
					Config::useTimestamp>
				 MsgRecv;

		 /** \brief The base message structure object.
		  
		  * Contains commons for send and receive messages.
		 **/

		 typedef typename CANBase<Config>::MsgSendBase
						  MsgBase;

		/** \brief The standard message struct to send a Remote Transmission Request.
		 **/

		 typedef typename CANBase<Config>::MsgSendBase
						  RTRSend;

		 /** \brief The standard message struct to receive a Remote Transmission Request.
		 **/

		 typedef typename CANBase<Config>::MsgRecvBase
						  RTRRecv;

		/**  \brief The error struct used by the error callback.
		 **/

		 typedef typename CANBase<Config>::MsgSendBase
						  Error;

		/** \brief The statistics and state information struct.
		 **/

		 typedef CANState State;

	public:
		/** Default Constructor
		 *
		 *  Registers the internal callback, and activates the interrupt
		 *  handling of the chip.  */

		Canary()
		{
			redirectISRM(SIG_CAN_INTERRUPT1,
						 &Canary<Config>::generalCallback,
						 *this);

			this->enableInterrupts();
		}


	private:

		/** \brief Global callback of the CAN driver
		 *
		 *  This function delegates all the interrupts to their respective
		 *  callbacks.  The callbacks for each subfunctionality are definied in
		 *  the appropriate handler classes.
		 **/

		void generalCallback()
		{
			uint8_t savedMobNum=this->getCurrentMob();
			uint16_t intActivity=this->getInterruptActivity();
			for (uint8_t i=0;i<NUMMOBS; i++)
				if (intActivity&(0x1<<i))
				{
					this->useMob(i);
					Events event=this->getEvent();
					switch (event)
					{
						case(RECEIVEOK):
							this->handleReceive(event, i);
							break;

						case(TRANSMITOK):
							this->handleTransmit(event, i);
							break;

						default:
							this->handleError(event, i);
							break;
					}

				}
			this->useMob(savedMobNum);
		}
};

/** \example canIn.cpp
 * This is an example Application, that demonstrates the
 * receiving of can messages.  It uses the Interrupt driven receive method of
 * the Canary. To display the received message, the LCD driver of the avr_halib
 * is used.
 *
 * The configuration for this example can be found at \link canConfig.h \endlink.
 **/

/** \example canOut.cpp
 * This is an example Application, that demonstrates the
 * periodic sending of can messages.  It uses the overflow interrupt of the can
 * driver, to send a message in a periodic way.  To show the flash size of the
 * driver, no ouput besides the message will be generated.
 *
 * The configuration for this example can be found at \link canConfig.h \endlink.
 **/

/** \example canRTR.cpp
 * This is an example Application, that demonstrates the
 * answering to remote transmission requests.  It uses the Interrupt driven
 * receive method of the Canary, to receive the remote transmission requests.
 * An answer will be generated automatically for the id, which is set to auto
 * reply. The other message will be answered by sending a appropriate can
 * message. To show the size of a can application, that uses both receive and
 * transmit, no additional output or error handling will be done.
 *
 * The configuration for this example can be found at \link canConfig.h \endlink.
 **/

}
}
