/**
 *	\file	avr-halib/avr/canary/can_interrupts.h
 *	\brief	Interrupt handlers for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_regmap.h"
#include "can_types.h"
#include "can_enums.h"
#include "can_base.h"

#include <avr-halib/share/delegate.h>
#include <stdlib.h>

namespace avr_halib
{
namespace canary
{

/** \brief The base class of all interrupt driven CAN operations
 *
 *  \tparam Config The configuration of this class
 
 * This class contains base functions, that are needed to use the CAN driver in
 * an interrupt driven way.  It also delivers functionality to use the
 * Overrun-Interrupt of the integrated timer.
 **/ 
	
template<class Config> class InterruptBase : public CANBase<Config>
{
	private: typedef CANRegmap<static_cast<Versions>(Config::version)>
			  Regmap;

	protected:
		/** \brief Standard constructor
		 *
		 * Registers the internal callback and activates the appropriate
		 * interrupts, but leaves the general interrupt disabled.
		 * It should be enabled if a general callback is registered.
		 **/
		InterruptBase()
		{
			this->restart();
			UseRegmap(can, Regmap);

			can.generalInterruptEnable.all=false;
			can.generalInterruptEnable.receive=Config::useReceive;
			can.generalInterruptEnable.transmit=Config::useTransmit;
			can.generalInterruptEnable.MObError=Config::useError;
			can.generalInterruptEnable.timerOverrun=false;
			can.MObsIntUsage=0xEF;
			SyncRegmap(can);
		}

		/** \brief Enables the general Callback**/
		void enableInterrupts()
		{
			UseRegmap(can, Regmap);
			can.generalInterruptEnable.all=true;
			SyncRegmap(can);
		}

	public:
		/** \brief Disables the registered callback for the Overrun-Interrupt**/
		void disableOverrunInterrupt()
		{
			UseRegmap(can, Regmap);
			can.generalInterruptEnable.timerOverrun=false;
			SyncRegmap(can);
		}
		
		/** \brief Registers a callback for the Overrun-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * @param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)()>
		void enableOverrunInterrupt(T& obj)
		{
			UseRegmap(can, Regmap);
			
			redirectISRM(SIG_CAN_OVERFLOW1, Fxn, obj);
			can.generalInterruptEnable.timerOverrun=true;

			SyncRegmap(can);
		}

		/** \copydoc InterruptBase::enableOverrunInterrupt(T& obj) 
		 **/

		template<class T, void (T::*Fxn)() const>
		void enableOverrunInterrupt(const T& obj)
		{
			UseRegmap(can, Regmap);
			
			redirectISRM(SIG_CAN_OVERFLOW1, Fxn, obj);
			can.generalInterruptEnable.timerOverrun=true;

			SyncRegmap(can);
		}

		/** \brief Registers a callback for the Overrun-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)()>
		void enableOverrunInterrupt()
		{
			UseRegmap(can, Regmap);
			
			redirectISRF(SIG_CAN_OVERFLOW1, Fxn);
			can.generalInterruptEnable.timerOverrun=true;

			SyncRegmap(can);
		}

	protected:

		/** \brief An array of pointers to the supplied message structure of
		 * the currently active MObs**/

		void *msgPointers[Config::maxConcurrentMsgs];
};

/** \brief The handler class for a receive interrupt
 *
 *  \tparam Config The configuration of this class
 *
 *  This class contains the handler callback for a receive interrupt.
 *  It can be configured to be disabled, in this situation reception 
 *  of messages is not possible but the CAN driver itself will be smaller.
 **/

template<bool use, class Config>
class ReceiveHandler : public InterruptBase<Config>
{

	/** \brief Definition of the configured message structure for incoming
	 * messages **/

	typedef can_templates::CANMsgRecv<static_cast<Versions>(Config::version),
									  Config::useTimestamp>
		MsgRecv;

	private:
		/** The delegate that holds the possible receive callback **/
		Delegate<MsgRecv&> receiveFun;

	public:
		/** \brief Registers a callback for the Receive-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * \param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(MsgRecv &msg)>
		void enableReceiveInterrupt(T& obj)
		{
			receiveFun.template bind<T, Fxn>(&obj);
		}

		/**\copydoc ReceiveHandler::enableReceiveInterrupt(T&)**/

		template<class T, void (T::*Fxn)(MsgRecv &msg) const>
		void enableReceiveInterrupt(const T& obj)
		{
			receiveFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Receive-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(MsgRecv &msg)>
		void enableReceiveInterrupt()
		{
			receiveFun.template bind<Fxn>();
		}

		/** \brief Disables the registered callback for the Receive-Interrupt
		 **/
		void disableReceiveInterrupt()
		{
			receiveFun.reset();
		}

		/** \brief Receives a message from the CAN-Bus.
		 
		 * This method will occupie one Message-Slot until a message is
		 * received.  If the Receive-Interrupt is activated, the appropriate
		 * callback is called after the message was received.
		 
		 * @param msg The message structure containing the receive parameters and the space for the data
		 **/

		Events recv(MsgRecv &msg) 
		{
			int8_t mob=this->selectMob();
			if(mob==-1||mob>Config::maxConcurrentMsgs)
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
		/** \brief Receive interrupt handler
		 
		 * This method is just a way to provide a unified interface for the
		 * ReceiveHandler in it`s different configurations.  It handles the
		 * occurence of an receive event, it copies the received data from the
		 * supplied MOb to the appropriate positions in the message structure
		 * and calls the registered callback function if one is registered.
		 
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
			if(!receiveFun.isEmpty())
				receiveFun(*msg);
			if(msg->getCyclic())
				this->startRecv();
		}
};

/** \brief The handler class for a transmit interrupt
 
 *  This class contains the handler callback for a transmit interrupt.  It can
 *  be configured to be disabled, in this situation the transmission of
 *  messages will not be possible.
 **/

template<bool use, class Config> 
class TransmitHandler : public ReceiveHandler<Config::useReceive, Config>
{

	/** \brief Definition of the configured message structure for outgoing
	 * messages **/

	typedef can_templates::CANMsgSend<static_cast<Versions>(Config::version)> 
		MsgSend;

	/** \brief Definition of the configured message structure for incoming
	 * messages **/

	typedef can_templates::CANMsgRecv<static_cast<Versions>(Config::version), 
									  Config::useTimestamp> 
		MsgRecv;

	/** \brief Definition of the configured message structure for outgoing
	 * remote transmission requests **/

	typedef typename CANBase<Config>::MsgSendBase RTRSend;

	private:
		/** The delegate that holds the possible transmit callback **/
		Delegate<MsgSend&> transmitFun;

	public:
		/** \brief Registers a callback for the Transmit-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * @param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(MsgSend &msg)>
		void enableTransmitInterrupt(T& obj)
		{
			transmitFun.template bind<T, Fxn>(&obj);
		}

		/** \copydoc TransmitHandler::enableTransmitInterrupt(T&)
		 **/

		template<class T, void (T::*Fxn)(MsgSend &msg) const>
		void enableTransmitInterrupt(const T& obj)
		{
			transmitFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Transmit-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(MsgSend &msg)>
		void enableTransmitInterrupt()
		{
			transmitFun.template bind<Fxn>();
		}

		/** \brief Disables the registered callback for the Transmit-Interrupt
		 **/
		void disableTransmitInterrupt()
		{
			transmitFun.reset();
		}

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
			if(mob==-1||mob>=Config::maxConcurrentMsgs)
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
			if(mob==-1||mob>Config::maxConcurrentMsgs)
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
		/** \brief Transmit interrupt handler
		 
		 * This method is just a way to provide a unified interface for the
		 * TransmitHandler in it`s different configurations.  It handles the
		 * occurence of a transmit event, it resets the supplied MOb, set the
		 * appropriate states values in the message structure and calls the
		 * registered callback function if one is registered.
		 
		 * @param event the event, that activated this handler
		 * @param i the MOb that this handler should work on
		 **/
		void handleTransmit(Events event, uint8_t i)
		{
			MsgSend *msg=static_cast<MsgSend*>(this->msgPointers[i]);
			msg->event=event;
			if(!msg->autoReply)
			{
				this->freeMob();
				this->removeInterrupt(i);
				if(!transmitFun.isEmpty())
					transmitFun(*msg);
			}
			else
				this->startAutoReply();
		}
};

/** \brief The handler class for an error interrupt
 
 *  This class contains the handler callback for an error reporting interrupt.
 *  It can be configured to be disabled, in this situation no error will be
 *  reported back to the user.
 **/

template<bool use, class Config>
class ErrorHandler: public TransmitHandler<Config::useTransmit, Config>
{
	/** \brief Definition of the configured structure for error reporting **/
	typedef typename CANBase<Config>::MsgSendBase Error;

	private:
		/** The delegate that holds the possible error callback**/
		Delegate<Error&> errorFun;

	public:
		/** \brief Disables the registered callback for the Error-Interrupt
		 **/
		void disableErrorInterrupt()
		{
			errorFun.reset();
		}

		/** \brief Registers a callback for the Error-Interrupt.
		 *
		 *  \tparam T class which contains the method
		 *  \tparam Fxn the method of class T, which should be used
		 *
		 * @param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(Error &msg)>
		void enableErrorInterrupt(T& obj)
		{
			errorFun.template bind<T, Fxn>(&obj);
		}

		/** \copydoc ErrorHandler::enableErrorInterrupt(T&)
		 **/

		template<class T, void (T::*Fxn)(Error &msg) const >
		void enableErrorInterrupt(const T& obj)
		{
			errorFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Error-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(Error &msg)>
		void enableErrorInterrupt()
		{
			errorFun.template bind<Fxn>();
		}

	protected:
	
		/**\brief Error interrupt handler
		 
		 * This method is just a way to provide a unified interface for the
		 * ErrorHandler in it`s different configurations.  It handles the
		 * occurence of an error event, it resets the supplied MOb, set the
		 * appropriate states values in the message structure and calls the
		 * registered callback function if one is registered.
		 
		 * @param event the event, that activated this handler
		 * @param i the MOb that this handler should work on
		 **/
		void handleError(Events event, uint8_t i)
		{
			Error *msg=static_cast<Error*>(this->msgPointers[i]);
			msg->event=event;
			this->freeMob();
			this->removeInterrupt(i);
		      	if(!errorFun.isEmpty())
				errorFun(*msg);
		}
};

template<class Config>
class TransmitHandler<false, Config> : public ReceiveHandler<Config::useReceive,
	  														 Config>
{
	protected:
		void handleTransmit(Events event, uint8_t i)
		{

		}
};

template<class Config>
class ReceiveHandler<false, Config> : public InterruptBase<Config>
{
	protected:
		void handleReceive(Events event, uint8_t i)
		{

		}
};

template<class Config>
class ErrorHandler<false, Config> : public TransmitHandler<Config::useTransmit,
	  													   Config>
{
	protected:

		void handleError(Events event, uint8_t i)
		{

		}
};

}
}
