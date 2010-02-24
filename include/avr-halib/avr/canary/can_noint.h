/**
 *	\file	avr-halib/avr/canary/can_noint.h
 *	\brief	Interrpt free CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_base.h"
#include "can_enums.h"
#include "can_types.h"

namespace avr_halib
{
namespace canary
{

	/** \brief The base class of all interrupt free CAN operations
	 *
	 *  \tparam Config The configuration of this class

	 * This class contains the variables, that the different handler classes
	 * need to map messages structures to MObs. It also supplies the hardware
	 * abstracted interface to the handler classes.
	 **/
	template<typename Config>
	class InterruptFreeBase : public CANBase<Config>
	{
		protected:
			/** \brief An array of pointers to the supplied message structure of
			 * the currently active MObs**/
			void *msgPointers[Config::maxConcurrentMsgs];
	};

	/** \brief The handler class for a successful reception.
	 *
	 *  \tparam Config The configuration of this class
	 
	 *  This class contains the handler for a succesfull reception of a CAN
	 *  message.  It also supplies the functions needed to start reception of a
	 *  message.  It can be configured to be disabled, in this situation
	 *  reception of messages is not possible but the CAN driver itself will be
	 *  smaller.
	 **/

	template<bool use, typename Config>
	class InterruptFreeReceive : public InterruptFreeBase<Config>
	{
		protected:

			/** \brief Definition of the configured message structure for
			 * incoming messages **/

			typedef can_templates::CANMsgRecv<static_cast<Versions>(Config::version),
			Config::useTimestamp>
			MsgRecv;

			/** \brief Receive event handler

			 * This method is just a way to provide a unified interface for the
			 * InterruptFreeReceive in it`s different configurations.  It
			 * handles the occurence of an receive event, it copies the
			 * received data from the supplied MOb to the appropriate positions
			 * in the message structure and updated the event values of the
			 * message structure.

			 * @param event the event, that activated this handler
			 * @param i the MOb that this handler should work on
			 **/
			void handleReceive(Events event, uint8_t i)
			{
				MsgRecv *msg=static_cast<MsgRecv*>(this->msgPointers[i]);
				msg->event=event;
				if (msg->acceptBoth && this->isCompat())
					msg->compat=true;
				this->readData(*msg, msg->data);
				this->readTimeStamp(*msg);
				if (!msg->getCyclic())
				{
					this->freeMob();
					this->removeInterrupt(i);
				}
				else
					this->startRecv();
			}

		public:
			/** \brief Receives a message from the CAN-Bus.

			 * This method will occupie one Message-Slot until a message is
			 * received.  The current state will be updated after each call to
			 * checkEvents().

			  * @param msg The message structure containing the receive
			  * parameters and the space for the data
			 **/ 
			
				Events recv(MsgRecv &msg)
				{ 
					int8_t mob=this->selectMob();
					if(mob==-1||mob>Config::maxConcurrentMsgs) 
						return NOFREEMOB;

					this->msgPointers[mob]=&msg;
					msg.event=NOTHING;

					return this->setRead(msg);
				}
	};

	template<bool use, typename Config>
	class InterruptFreeTransmit : public InterruptFreeReceive<Config::useReceive, Config>
	{
		protected:

			/** \brief Transmit event handler

			 * This method is just a way to provide a unified interface for the
			 * InterruptFreeTransmit in it`s different configurations.  It
			 * handles the occurence of a transmit event, it resets the
			 * supplied MOb, set the appropriate event values in the message
			 * structure.

			 * @param event the event, that activated this handler
			 * @param i the MOb that this handler should work on
			 **/

			void handleTransmit(Events event, uint8_t i)
			{
				MsgSend *msg=static_cast<MsgSend*>(this->msgPointers[i]);
				msg->event=event;
				if (!msg->autoReply)
				{
					this->freeMob();
					this->removeInterrupt(i);
				}
				else
					this->startAutoReply();
			}

		public:

			/** \brief Definition of the configured message structure for
			 * outgoing messages **/

			typedef can_templates::CANMsgSend<static_cast<Versions>(Config::version)>
			MsgSend;

			/** \brief Definition of the configured message structure for
			 * incoming messages **/

			typedef can_templates::CANMsgRecv<static_cast<Versions>(Config::version),
			Config::useTimestamp>
			MsgRecv;

			/** \brief Definition of the configured message structure for
			 * outgoing remote transmission requests **/

			typedef typename CANBase<Config>::MsgSendBase RTRSend;

			/** \brief Send a remote-transmit-request over the CAN-Bus.

			 * This method will occupie one Message-Slot until the message is
			 * completely send. The message event filed will be updated when the
			 * checkEvents() function is called and the message transmission is
			 * finshed.

			 * @param msg The RTR-Message that is to be send
			 **/
			Events send(RTRSend &msg)
			{
				msg.setRTR(true);
				return send(static_cast<MsgSend&>(msg));
			}

			/** \brief Send a message over the CAN-Bus.

			 * This method will occupie one Message-Slot until the message is
			 * completely send.  The message event filed will be updated when
			 * the checkEvents() function is called and the message
			 * transmission is finshed.

			 * @param msg The message that is to be send
			 **/

			Events send(MsgRecv &msg)
			{
				return send(*(reinterpret_cast<MsgSend*>(&msg)));
			}

			/** \copydoc send(MsgRecv &msg) **/

			Events send(MsgSend &msg)
			{
				int8_t mob=this->selectMob();
				if (mob==-1||mob>=Config::maxConcurrentMsgs)
					return NOFREEMOB;

				this->msgPointers[mob]=&msg;
				msg.event=NOTHING;

				return this->setWrite(msg, msg.data);
			}

			/** \brief Set an autoreply for a RTR-Message, if a RTR is
			 * received, which ID matches the supplied mask of msg.

			 * then automatically send a reply with the content that is
			 * provided in msg.  This method will occupie one message-slot. It
			 * will continue replying until stopAutoReply(msg) is called.
			 *
			 * @param msg The Auto-Reply-Message that is used for auto reply
			 **/

			Events setAutoReply(MsgRecv &msg)
			{
				int8_t mob=this->selectMob();
				if (mob==-1||mob>Config::maxConcurrentMsgs)
					return NOFREEMOB;

				this->msgPointers[mob]=&msg;
				msg.event=NOTHING;

				return this->setRTRValid(msg, msg.data);
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
	};

	template<typename Config>
	class InterruptFreeReceive<false, Config> : public InterruptFreeBase<Config>
	{
		protected:
			void handleReceive(Events event, uint8_t i)
			{

			};
	};

	template<typename Config>
	class InterruptFreeTransmit<false, Config> : public InterruptFreeReceive<Config::useReceive, Config>
	{
		protected:
			void handleTransmit(Events event, uint8_t i)
			{

			};
	};



/** \brief An interrupt free CAN driver class for the AVR at90can128.

 *  \tparam Config A struct containing the configuration parameters for the CAN
 *  driver

 * This class provides the interface to use the CAN functionality of the
 * at90can128.  It uses no interrupts, therefor the user needs to call
 * checkEvents() everytime he wants to know if something happened. To save code
 * size the receive and transmit functionality can be (de)activated. If a
 * functionality is disabled, the public functions to use the functionality are
 * not existent. If a user tries to use them he will get a compile error,
 * telling him of his misconfiguration. The checkEvents() function will
 * automatically look up the correspondig message structure and update any
 * value that has changed. Normally every operation is one shot, therefore used
 * messages, that finished there operations can be used again for different
 * operations, the only exceptions are message that are in autoReply mode or in
 * cyclic receive mode.
 **/

/**\ingroup canary*/
template<typename Config>
class CanNoInt : public InterruptFreeTransmit<Config::useTransmit, Config>
{
	private: typedef CANRegmap<static_cast<Versions>(Config::version)>
			  Regmap;
	public:

		/** \copydoc Canary::IdType **/

		typedef typename CANBase<Config>::IdType IdType;

		/** \copydoc Canary::MsgSend **/

		typedef can_templates::CANMsgSend<
		static_cast<Versions>(Config::version)>
		MsgSend;

		/** \copydoc Canary::MsgRecv **/

		typedef can_templates::CANMsgRecv<
		static_cast<Versions>(Config::version),
		Config::useTimestamp>
		MsgRecv;

		/** \copydoc Canary::MsgBase **/

		typedef typename CANBase<Config>::MsgSendBase
		MsgBase;

		/** \copydoc Canary::RTRSend **/

		typedef typename CANBase<Config>::MsgSendBase
		RTRSend;

		/** \copydoc Canary::RTRRecv **/

		typedef typename CANBase<Config>::MsgRecvBase
		RTRRecv;

		/** \copydoc Canary::Error **/

		typedef typename CANBase<Config>::MsgSendBase
		Error;

		/** \copydoc Canary::State **/

		typedef CANState State;

		/** \brief Standard constructor
		 *
		 * 	Initializes the hardware, but does not activate interrupt handling facility.
		 * */

		CanNoInt()
		{
			this->restart();
			UseRegmap(can, Regmap);
			can.generalInterruptEnable.receive=Config::useReceive;
			can.generalInterruptEnable.transmit=Config::useTransmit;
			can.generalInterruptEnable.MObError=Config::useError;
			can.generalInterruptEnable.timerOverrun=false;
			can.generalInterruptEnable.all=true;
			can.MObsIntUsage=0xEF;
			SyncRegmap(can);
		}

		/** \brief Polling function
		*
		*  This function checks the used MObs if something should be done, and
		*  delegates the handling to the appropriate handler of the class.
		*
		*  \return true if something happend, false otherwise.
		**/

		bool checkEvents()
		{
			bool nothing=true;
			uint16_t intActivity=this->getInterruptActivity();
			for (uint8_t i=0;i<NUMMOBS; i++)
				if (intActivity&(0x1<<i))
				{
					nothing=false;
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
							Error *msg=static_cast<Error*>(this->msgPointers[i]);
							msg->event=event;
							this->freeMob();
							this->removeInterrupt(i);
							break;
					}
				}
			return !nothing;
		}
};

/** \example canInNoInt.cpp This is an example Application, that demonstrates
 * the receiving of can messages.  It uses the interrupt free receive method of
 * the CanNoInt. To display the received message, the LCD driver of the
 * avr_halib is used.

 * The configuration for this example can be found at \link canConfigNoInt.h
 * \endlink.
 **/

/** \example canOutNoInt.cpp This is an example Application, that demonstrates
 * the periodic sending of can messages.  It uses the ExactEggTimer of the
 * avr-halib, for periodic sending of the messages.  To show the flash size of
 * the driver, no ouput besides the message will be generated.
 
 * The configuration for this example can be found at \link canConfigNoInt.h
 * \endlink.
 **/

}
}
