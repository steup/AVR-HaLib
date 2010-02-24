/**
 *	\file	avr-halib/avr/canary/can_base.h
 *	\brief	Base class for CAN drivers
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_types.h"
#include "can_enums.h"
#include "can_regmap.h"

namespace avr_halib
{
namespace canary
{
	/** \brief The base class of the CAN-Driver
	 *
	 *  \tparam Config The configuration of this class
	 *
	 *  This class implements the basic and more chip-specific access-functions.
	 **/
	template<class config>
	class CANBase : 
		public can_templates::IdHelper<static_cast<Versions>(config::version)>
	{
		protected:
			/** The special regmap to use for the hardware-access **/

			typedef CANRegmap<static_cast<Versions>(config::version)>
				Regmap;

			/** The special CAN-Base-Message for sending a message **/

			typedef can_templates::CANMsgSendBase<
						static_cast<Versions>(config::version)>
				MsgSendBase;

			/** The special CAN-Base-Message for receiving a message without
			 * timestamp**/

			typedef can_templates::CANMsgRecvBase<
						static_cast<Versions>(config::version)>
				MsgRecvBase;

			/** The special CAN-Base-Message for sending a message with
			 * timestamp**/

			typedef can_templates::CANMsgRecvTimeStamp<
						static_cast<Versions>(config::version)>
				MsgRecvTimeStamp;

			/** The structure, that contains the current state-values**/

			typedef CANState
				State;

			/** The special IdHelper to manage information about the CAN-ID **/

			typedef typename can_templates::IdHelper<
								static_cast<Versions>(config::version)>::IdType
				IdType;

		protected:

		/** \brief The general driver start/restart function.

		 *  It resets the hardware and configures the constant parameters, that
		 *  are stated in the template-configuration.
		
		 *  If the hardware has no CAN-Hardware, an infinite loop is possible.
		 *  **/

		void restart() 
		{
			UseRegmap(can, Regmap);
			can.generalControl.swReset=true;
			SyncRegmap(can);

			for(uint8_t i=0;i<NUMMOBS;i++)
			{
				can.curMObNum=i;
				freeMob();
			}

			can.baudRateConfig.brp=config::baudRate::brp;
			can.baudRateConfig.sjw=config::baudRate::sjw;
			can.baudRateConfig.prs=config::baudRate::prs;
			can.baudRateConfig.phs1=config::baudRate::phs1;
			can.baudRateConfig.phs2=config::baudRate::phs2;
			can.baudRateConfig.smp=config::baudRate::smp;

			can.timerPrescaler=config::prescaler;

			can.cangie=0;
			can.canie1=0;
			can.canie2=0;
			can.cansit1=0;
			can.cansit2=0;

			can.generalControl.enable=true;

			while(!can.generalStatus.enabled)
				SyncRegmap(can);
		}		
		
		/** \brief Search the Message-Object-Buffers(MOb) for the next free one
		 * and sets it to active
		 
		 * @return the current active MOb or -1 if none was available**/

		int8_t selectMob() 
		{
			UseRegmap(can, Regmap);
			for(uint8_t i=0;i<NUMMOBS;i++)
				if(!(can.MObsAvailability&(0x1<<i)))
				{
					can.curMObNum=i;
					SyncRegmap(can);
					return i;
				}
			return -1;
		}

		/** \brief Start the sending of a message
		 
		 *  There will be no check, wether the message was send successful or
		 *  not.
		 
		 * @param msg a reference to the CAN-Message-structure that is to be
		 * send
		 
		 * @param data the data of the CAN-Message that should be send

		 * @return the event that occured during the start of the sending
		 * possible values: INVALIDID, INVALIDMSG, SUCCESS
		 **/

		Events setWrite(MsgSendBase &msg, const uint8_t *data) 
		{
			msg.autoReply=false;

			if(!checkId(msg.id))
				return INVALIDID;

			if(!setMsgLen(msg))
				return INVALIDMSG;

			setId(msg);

			if(!msg.getRTR())
				sendData(msg, data);

			startSend();

			return SUCCESS;
		}

		/** \brief Sets a MOb to function as a AutoReply for RTRs.
		 
		 * @param msg a reference to the CAN-Message-structure, that contains
		 * the information for the AutoReply

		 * @param data the data, that is to be send on a automatic reply

		 * @return the event that occured during the start of the sending
		 * possible values: INVALIDID, INVALIDMSG, SUCCESS
		 **/ Events setRTRValid(MsgRecvBase &msg, const uint8_t *data) {
			 if(!checkId(msg.id)) return INVALIDID;

			if(!setMsgLen(msg))
				return INVALIDMSG;

			msg.autoReply=true;
			msg.setRTRMask(true);
			msg.setRTR(true);

			setIdAndIdMask(msg);

			sendData(msg, data);
			
			startAutoReply();

			return SUCCESS;
		}

		/** \brief Start the receiving of a message.
		 *  There is no check if a message was received successfull.
		 
		 * @param msg a reference to the CAN-Message-structure that is to be
		 * used

		 * @return the event that occured during the start of the sending
		 * possible values: INVALIDID, INVALIDMSG, SUCCESS
		 **/

		Events setRead(MsgRecvBase &msg) 
		{
			msg.autoReply=false;

			if(!checkId(msg.id))
				return INVALIDID;

			if(!setMsgLen(msg))
				return INVALIDMSG;

			setIdAndIdMask(msg);

			startRecv();

			return SUCCESS;
		}

		/** \brief Read the received data and the ID

		 *  of the received frame from the hardware into the
		 *  CAN-Message-Structure.
		 
		 * @param msg a reference to the CAN-Message-Structure that is to be
		 * used

		 * @param data the buffer in which the data should be placed, should be
		 * 8 bytes long
		 **/

		void readData(MsgRecvBase &msg, uint8_t *data) 
		{
			UseRegmap(can, Regmap);

			can.fifoIndex=0;
			SyncRegmap(can);

			if(!msg.getRTR())
				for(uint8_t i=0; i<can.mobConfig.dataLengthCode; i++)
				{
					data[i]=can.msg;
					SyncRegmap(can);
				}

			if(static_cast<Versions>(config::version)==CAN_20B
			   && !can.mobConfig.idExtension)

				msg.id=reinterpret_cast<can_templates::CANID<CAN_20A>*>(&can.idConfig)->id;

			else

				msg.id=can.idConfig.id;

			msg.length=can.mobConfig.dataLengthCode;
			SyncRegmap(can);
		}

		/** \brief Read the timestamp of the currently received message
		 * into the CAN-Message-Structure if timestamps are enabled.
		 
		 * @param msg a reference to the CAN-Message-Structure that is to be
		 * used
		 **/

		void readTimeStamp(MsgRecvTimeStamp &msg) 
		{
			UseRegmap(can, Regmap);

			msg.timeStamp=can.timeStamp;
			SyncRegmap(can);
		}

		/**\brief Do nothing.
		 *  This method just exist to be used when timestamps are deactivated.
		 
		 * @param msg a reference to the CAN-Message-Structure that is to be
		 * used
		 **/

		void readTimeStamp(MsgRecvBase &msg)
		{

		}

		/** \brief Get the last event that occured.
		 *   All satus flags will be reseted by calling this function.
		 *
		 * @return the event that occured
		 **/

		Events getEvent() 
		{
			UseRegmap(can, Regmap);

			if(can.mobStatus.ackError)
			{
				can.mobStatus.ackError=false;
				SyncRegmap(can);
				return ACKERROR;
			}
			if(can.mobStatus.formError)
			{
				can.mobStatus.formError=false;
				SyncRegmap(can);
				return FORMERROR;
			}
			if(can.mobStatus.crcError)
			{
				can.mobStatus.crcError=false;
				SyncRegmap(can);
				return CRCERROR;
			}
			if(can.mobStatus.stuffError)
			{
				can.mobStatus.stuffError=false;
				SyncRegmap(can);
				return STUFFERROR;
			}
			if(can.mobStatus.bitError)
			{
				can.mobStatus.bitError=false;
				SyncRegmap(can);
				return BITERROR;
			}
			if(can.mobStatus.receiveOk)
			{
				can.mobStatus.receiveOk=false;
				SyncRegmap(can);
				return RECEIVEOK;
			}
			if(can.mobStatus.transmitOk)
			{
				can.mobStatus.transmitOk=false;
				SyncRegmap(can);
				return TRANSMITOK;
			}
			SyncRegmap(can);
			return NOTHING;
		}

		/** \brief Disables the currently used MOb.
		 * The current action will be completed before deactivation will happen
		 **/

		void freeMob()
		{
			UseRegmap(can, Regmap);

			can.mobConfig.config=DISABLEDMOB;
		}

		public:

			/** \brief Tell the CAN driver, to send an overload frame after the
			 * next received frame
			 **/

			void requestOverloadFrame()
			{
				UseRegmap(can, Regmap);
				can.overloadFrame=true;
			}

			/** \brief Allows the user to track the overload frame
			 *
			 * @return true if overload frame still active, false if not
			 **/

			bool watchOverloadFrame() const
			{
				UseRegmap(can, Regmap);

				return can.frameOverloaded;
			}
			
			/** \brief Fill the supplied CANState structure with the current
			 * state values of the CAN driver.
			 
			 * @param state The supplied CANState structure
			 **/

			void getState(State &state) const
			{
				UseRegmap(can, Regmap);

				state.receiveErrors=can.canrec;
				state.transmitErrors=can.cantec;

				if(!can.generalStatus.enabled)
					state.state=DISABLED;
				else if(can.generalStatus.errorPassiveMode)
					state.state=ERRORPASSIVE;
				else if(can.generalStatus.busOffMode)
					state.state=BUSOFF;
				else state.state=ENABLED;

				state.busy=NONE;
				if(can.generalStatus.transmitterBusy)
					state.busy=TRANSMITTER;
				if(can.generalStatus.receiverBusy)
					state.busy=RECEIVER;
			}

		protected:

		/** \brief Gets the number of the currently active MOb
		 *
		 * @return number of the current MOb
		 **/
		uint8_t getCurrentMob() const
		{
			UseRegmap(can, Regmap);
			SyncRegmap(can);
			return can.curMObNum;
		}


		/** \brief Activates a MOb
		 *
		 * @param mobNum number of the MOb, that is to be activated
		 **/
		void useMob(uint8_t mobNum)
		{
			UseRegmap(can, Regmap);
			can.curMObNum=mobNum;
			SyncRegmap(can);
		}

		/** \brief Gets the MObs, which currently have an active interrupt
		 
		 * @return a bitfield indicating an interrupt by 1 on pos mobNum for
		 * each MOb
		 **/

		uint16_t getInterruptActivity() const
		{
			UseRegmap(can, Regmap);
			SyncRegmap(can);
			return can.MObsIntActive;
		}

		/** \brief Activates interrupts for a given MOb
		 *
		 * @param mob the mob for which interrupts should be enabled
		 */
		void setInterrupt(uint8_t mob)
		{
			UseRegmap(can, Regmap);

			can.MObsIntUsage|=0x1<<mob;

			SyncRegmap(can);
		}

		/** \brief Deactivates interrupts for a given MOb
		 *
		 * @param mob the mob for which interrupts should be disabled
		 */
		void removeInterrupt(uint8_t mob)
		{
			UseRegmap(can, Regmap);

			can.MObsIntUsage&=~(0x1<<mob);

			SyncRegmap(can);
		}

		/** \brief Gets the compatibility flag of the current MOb
		 *
		 * @return compatibility state of the current MOb
		 **/

		bool isCompat() const
		{
			UseRegmap(can, Regmap);
			return can.mobConfig.idExtension;
		}

		/** \brief Sets the ID, IDExtension-Flag and RTR-Flag
		 *   from the message-structure into the hardware-controller.
		 *
		 * To be used for sending.
		 *
		 * @param msg reference to the message that contains the ID
		 **/
		void setId(const MsgSendBase &msg) 
		{
			UseRegmap(can, Regmap);

			can.idConfig.remoteTransmissionRequest=msg.rtr;

			if(static_cast<Versions>(config::version)==CAN_20B)
			{
				if(msg.compat)
				{
					can.mobConfig.idExtension=false;
					reinterpret_cast<can_templates::CANID<CAN_20A>*>(&can.idConfig)->id=msg.id;
				}
				else
				{
					can.mobConfig.idExtension=true;
					can.idConfig.id=msg.id;
				}
			}				
			else
			{
				can.mobConfig.idExtension=false;
				can.idConfig.id=msg.id;
			}

			SyncRegmap(can);
		}
		
		/** \brief Sets the ID, IDMask, IDExtension-Flag and RTR-Flag 
		 * from the message-structure into the hardware-controller.
		 *
		 * To be used for receiving.
		 *
		 * @param msg reference to the message that contains the ID
		 **/
		void setIdAndIdMask(const MsgRecvBase &msg)
		{
			UseRegmap(can, Regmap);

			setId(msg);

			if(static_cast<Versions>(config::version)==CAN_20B)
			{
				if(msg.compat)
					reinterpret_cast<can_templates::CANID<CAN_20A>*>(&can.idConfig)->idMask=msg.idMask;
				else
					can.idConfig.idMask=msg.idMask;

				can.idConfig.idExtensionMask=!msg.acceptBoth;
			}
			else
			{
				can.idConfig.idMask=msg.idMask;
				can.idConfig.idExtensionMask=true;
			}

			can.mobConfig.replyValid=false;

			can.idConfig.idExtensionMask=true;
			can.idConfig.remoteTransmissionMask=msg.rtrMask;

			SyncRegmap(can);
		}

		/** \brief Checks if the id fits in the ID-value-space of the current
		 * CAN-Version
		 
		 * CAN_20A: 0<= id <= 0x7FF
		 * CAN_20B: 0<= id <= 0x1FFFFFFF
		 
		 * @param id the ID that is to be checked

		 * @return true if ok, false if out of range
		 **/

		bool checkId(IdType id)
		{
			return id<=can_templates::IdHelper<
						static_cast<Versions>(config::version)>::getMaxId();
		}

		/** \brief Sets the length of the message
		 *  from the message-structure into the hardware-controller
		 * Also checks if the length of the message is between 0 and 8 bytes
		 *
		 * @param msg reference to the CAN-message-structure that is to be send
		 * @return false if message is too long, true otherwise
		 **/

		bool setMsgLen(const MsgSendBase &msg) 
		{
			UseRegmap(can, Regmap);

			if(msg.length>MAXMSGLEN)
				return false;

			can.mobConfig.dataLengthCode=msg.length;

			SyncRegmap(can);

			return true;
		}

		/** \brief Copies the data that is to be send

		 * from the message into the hardware-controller.
		 
		 *  @param msg a reference to the message containing the length of the
		 *  data

		 *  @param data the data that is to be send
		 **/

		void sendData(const MsgSendBase &msg, const uint8_t *data) 
		{
			UseRegmap(can, Regmap);

			can.fifoIndex=0;
			SyncRegmap(can);

			for(uint8_t i=0;i<msg.length;i++)
			{
				can.msg=data[i];
				SyncRegmap(can);
			}
		}

		/** \brief Starts the send process of the current MOb**/

		void startSend() 
		{
			UseRegmap(can, Regmap);

			can.mobConfig.config=TXMOB;
			SyncRegmap(can);
		}

		/** \brief Starts the receive process of the current MOb**/

		void startRecv() 
		{
			UseRegmap(can, Regmap);

			can.mobConfig.config=RXMOB;
			SyncRegmap(can);
		}

		/** \brief Sets the current MOb into Auto-Reply-Mode**/

		void startAutoReply() 
		{
			UseRegmap(can, Regmap);

			can.idConfig.remoteTransmissionRequest=true;
			can.mobConfig.replyValid=true;
			can.mobConfig.config=RXMOB;
			SyncRegmap(can);
		}
	};
}
}
