/**
 *	\file	avr-halib/avr/canary/canary.h
 *	\brief	Canary - CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_interruptBinder.h"

namespace avr_halib
{

namespace canary
{

/** \brief The CAN driver class for the AVR at90can128.

 *  \tparam userConfig A struct containing the configuration parameters for the CAN
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
template<class userConfig>
class Canary : public InterruptBinder<ConfigTransform_CheckInterruptUsage<userConfig> >
{
	private:
		typedef ConfigTransform_CheckInterruptUsage<userConfig> config;
		typedef Handlers<config> ConfiguredHandlers;
		typedef CANBase<config> ConfiguredBase;

	public:

		/** \brief The type for the ID of the CAN messages
		 
		 * This depends on the used CAN version.**/

		typedef typename ConfiguredBase::IdType IdType;

		/** \brief The standard message structure to send a CAN message.**/

		typedef typename ConfiguredHandlers::MsgSend MsgSend;

		/** \brief The standard message structure to receive a CAN message.

		 * It can also be used for sending a message.
		 **/

		 typedef typename ConfiguredHandlers::MsgRecv MsgRecv;

		 /** \brief The base message structure object.
		  
		  * Contains commons for send and receive messages.
		 **/

		 typedef typename ConfiguredBase::MsgSendBase MsgBase;

		/** \brief The standard message struct to send a Remote Transmission Request.
		 **/

		 typedef typename ConfiguredHandlers::MsgSendBase RTRSend;


		/**  \brief The error struct used by the error callback.
		 **/

		 typedef typename ConfiguredHandlers::Error Error;

		/** \brief The statistics and state information struct.
		 **/

		 typedef CANState State;
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
