/**
 *	\file	avr-halib/avr/canary/can_interrupts.h
 *	\brief	Interrupt handlers for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_regmap.h"
#include "can_types.h"
#include "can_base.h"
#include "can_interrupts.h"

#include <stdlib.h>

namespace avr_halib
{
namespace canary
{

/** \brief The base class of all handlers for CAN operations
 *
 *  \tparam config The configuration of this class
 
 * This class contains api functions, that are called by the polling function.
 * It also provided the base infrastructure to associate message strcutures and
 * MObs.
 **/ 
	
template<class config> class HandlerBase : public Interrupts<config>
{
	protected:

		/** \brief Definition of the configured message structure for incoming
	 	* messages **/

		typedef can_templates::CANMsgRecv<static_cast<Versions>(config::version),
									  config::useTimestamp>
			MsgRecv;

		/** \brief Definition of the configured message structure for outgoing
	 	* messages **/

		typedef can_templates::CANMsgSend<static_cast<Versions>(config::version)> 
			MsgSend;

		/** \brief Definition of the configured message structure for outgoing
	 	* remote transmission requests **/

		typedef typename CANBase<config>::MsgSendBase RTRSend;

		/** \brief Definition of the configured structure for error reporting **/
		typedef typename CANBase<config>::MsgSendBase Error;
	protected:
		/** \brief Buffer association data structure
		 
		 * 	 This array contains the information, which hardware MOb is
		 * 	 connected to which user supplied buffer.
		 **/	
		void *msgPointers[config::maxConcurrentMsgs];
};

/** \brief The handler class for a receive event
 *
 *  \tparam config The configuration of this class
 *
 *  This class contains the handler function for a receive event.
 *  It can be configured to be disabled, in this situation reception 
 *  of messages is not possible but the CAN driver itself will be smaller.
 **/

template<bool useReceive, class config>
class ReceiveHandler : public HandlerBase<config>
{
	protected:
		/** \brief Forward Declaration **/
		typedef typename HandlerBase<config>::MsgRecv MsgRecv;

	public:
		/** \brief Receives a message from the CAN-Bus.
		 
		 * This method will occupie one Message-Slot until a message is
		 * received.  If the Receive-Interrupt is activated, the appropriate
		 * callback is called after the message was received.
		 
		 * @param msg The message structure containing the receive parameters and the space for the data
		 **/

		Events recv(MsgRecv &msg) 
		{
			int8_t mob=this->selectMob();
			if(mob==-1||mob>config::maxConcurrentMsgs)
				return NOFREEMOB;

			this->msgPointers[mob]=&msg;

			this->setInterrupt(mob);

			Events event=this->setRead(msg);
			if(event!=SUCCESS)
			{
				this->removeInterrupt(mob);
				return event;
			}

			return event;
		}

	protected:
		/** \brief Receive event handler
		 
		 * This method is just a way to provide a unified interface for the
		 * ReceiveHandler in it`s different configurations.  It handles the
		 * occurence of an receive event, it copies the received data from the
		 * supplied MOb to the appropriate positions in the message structure
		 * and calls the ReceiveInterrupt callback.
		 
		 * @param event the event, that activated this handler
		 * @param i the MOb that this handler should work on
		 **/

		void handleReceive(Events event, uint8_t i)
		{
			MsgRecv *msg=static_cast<MsgRecv*>(this->msgPointers[i]);
			msg->event=event;
			if(msg->acceptBoth && this->isCompat())
					msg->compat=true;
			this->readData(*msg, msg->data);
			this->readTimeStamp(*msg);
			if(!msg->getCyclic())
			{
				this->freeMob();
				this->removeInterrupt(i);
			}

			receiveCallback(*msg);

			if(msg->getCyclic())
				this->startRecv();
		}
};

/** \brief The handler class for a transmit event
 
 *  This class contains the handler function for a transmit event.  It can
 *  be configured to be disabled, in this situation the transmission of
 *  messages will not be possible.
 **/

template<bool use, class config> 
class TransmitHandler : public ReceiveHandler<config::useReceive, config>
{
	protected:
		/** \brief Forward Declaration **/
		typedef typename HandlerBase<config>::MsgSend MsgSend;

		/** \brief Forward Declaration **/
		typedef typename HandlerBase<config>::RTRSend RTRSend;

		/** \brief Forward Declaration **/
		typedef typename HandlerBase<config>::MsgRecv MsgRecv;

	public:
		/** \brief Send a message over the CAN-Bus.
		 
		 * This method will occupie one Message-Slot until the message is
		 * completely send.  After succesfull sending the of message the
		 * event-field of the Message-Struct contains TRANSMITOK, if the
		 * Transmit-Interrupt is activated, the registered transmit-callback is
		 * also called.
		 
		 * @param msg The message that is to be send
		 **/

		Events send(MsgSend &msg) 
		{
			int8_t mob=this->selectMob();
			if(mob==-1||mob>=config::maxConcurrentMsgs)
				return NOFREEMOB;

			this->msgPointers[mob]=&msg;

			this->setInterrupt(mob);

			Events event=this->setWrite(msg, msg.data);
			if(event!=SUCCESS)
			{
				this->removeInterrupt(mob);
				return event;
			}

			return SUCCESS;
		}

		/** \brief Send a message over the CAN-Bus.
		 
		 * This method will occupie one Message-Slot until the message is
		 * completely send.  After succesfull sending the of message the
		 * event-field of the Message-Struct contains TRANSMITOK, if the
		 * Transmit-Interrupt is activated, the registered transmit-callback is
		 * also called.
		 
		 * @param msg The message that is to be send
		 **/

		Events send(MsgRecv &msg)
		{
			return send(*(reinterpret_cast<MsgSend*>(&msg)));
		}

		/** \brief Send a remote-transmit-request over the CAN-Bus.
		 
		 * This method will occupie one Message-Slot until the message is
		 * completely send.  After succesfull sending the of message the
		 * event-field of the Message-Struct contains RTRT, if the
		 * Transmit-Interrupt is activated, the registered transmit-callback is
		 * also called.
		 
		 * @param msg The RTR-Message that is to be send
		 **/
		Events send(RTRSend &msg) 
		{
			msg.setRTR(true);
			return send(static_cast<MsgSend&>(msg));
		}

		/** \brief Set an autoreply for a RTR-Message, if a RTR is received,
		 * which ID matches the supplied mask of msg.
		 
		 * then automatically send a reply with the content that is provided in
		 * msg.  This method will occupie one message-slot until an auto-reply
		 * was send.  After succesfull sending a reply and if the
		 * Transmit-Interrupt is activated, the registered transmit-callback is
		 * called.
		 *
		 * @param msg The Auto-Reply-Message that is used for auto reply
		 **/

		Events setAutoReply(MsgRecv &msg)
		{
			int8_t mob=this->selectMob();
			if(mob==-1||mob>config::maxConcurrentMsgs)
				return NOFREEMOB;

			
			this->msgPointers[mob]=&msg;

			this->setInterrupt(mob);

			Events event=this->setRTRValid(msg, msg.data);
			if(event!=SUCCESS)
			{
				this->removeInterrupt(mob);
				return event;
			}

			return event;
		}

		/** \brief Stop the auto reply function of this message structure
		 *
		 * @param msg the message structure for which auto reply should be
		 * disabled.
		 **/

		void stopAutoReply(MsgRecv &msg)
		{
			msg.autoReply=false;
		}

	protected:
		/** \brief Transmit event handler
		 
		 * This method is just a way to provide a unified interface for the
		 * TransmitHandler in it`s different configurations.  It handles the
		 * occurence of a transmit event, it resets the supplied MOb, set the
		 * appropriate states values in the message structure and calls the
		 * TransmitInterrupt callback.
		 
		 * @param event the event, that activated this handler
		 * @param i the MOb that this handler should work on
		 **/
		void handleTransmit(Events event, uint8_t i)
		{
			RTRSend *msg=static_cast<RTRSend*>(this->msgPointers[i]);
			msg->event=event;
			if(!msg->autoReply)
			{
				this->freeMob();
				this->removeInterrupt(i);
				transmitCallback(*msg);
			}
			else
				this->startAutoReply();
		}
};

/** \brief The handler class for an error event
 
 *  This class contains the handler callback for an error reporting event.
 *  It can be configured to be disabled, in this situation no error will be
 *  reported back to the user.
 **/

template<bool use, class config>
class ErrorHandler: public TransmitHandler<config::useTransmit, config>
{
	protected:
		/** \brief Forward Declaration **/
		typedef typename HandlerBase<config>::Error Error;

	protected:
		/**\brief Error event handler
		 
		 * This method is just a way to provide a unified interface for the
		 * ErrorHandler in it`s different configurations.  It handles the
		 * occurence of an error event, it resets the supplied MOb, set the
		 * appropriate states values in the message structure and calls the
		 * ErrorInterrupt callback.
		 
		 * @param event the event, that activated this handler
		 * @param i the MOb that this handler should work on
		 **/
		void handleError(Events event, uint8_t i)
		{
			Error *error=static_cast<Error*>(this->msgPointers[i]);
			error->event=event;
			this->freeMob();
			this->removeInterrupt(i);
			errorCallback(*error);
		}
};

template<class config>
class ReceiveHandler<false, config> : public HandlerBase<config>
{
	protected:
		void handleReceive(Events event, uint8_t i)
		{

		}
};

template<class config>
class TransmitHandler<false, config> : public ReceiveHandler<config::useReceive,
	  														 config>
{
	protected:
		void handleTransmit(Events event, uint8_t i)
		{

		}
};

template<class config>
class ErrorHandler<false, config> : public TransmitHandler<config::useTransmit,
	  													   config>
{
	protected:

		void handleError(Events event, uint8_t i)
		{

		}
};

/** \brief Unification class
 *  \tparam config supplied configuration
 
 *  This class unificates the configured functionality of all event
 *  handlers for the CAN driver. It also provides the polling function, to check for events.
 **/

template<class config>
class Handlers : public ErrorHandler<config::useError, config>
{
	protected:
		typedef typename HandlerBase<config>::MsgRecv MsgRecv;
		typedef typename HandlerBase<config>::RTRSend RTRSend;
		typedef typename HandlerBase<config>::Error Error;

	public:
		/** \brief Polling function
		*
		*  This function checks the used MObs if something should be done, and
		*  delegates the handling to the appropriate handler of the class.
		*
		*  \return true if something happend, false otherwise.
		**/

		bool checkEvents()
		{
			if(!this->checkForAnything())
				return false;
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
							this->handleReceive(event,  i);
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
			return true;
		}
};

}
}
