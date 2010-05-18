/**
 *	\file	avr-halib/avr/canary/can_types.h
 *	\brief	Types for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <stdint.h>
#include "can_enums.h"

namespace avr_halib
{
	/** \brief Namespace of the can driver for the at90canXXX. **/
namespace canary
{
	/** \brief Struct that contains the state parameters of the CAN driver.
	 **/
	struct CANState
	{
		/** \brief Number of receive errors**/
		uint16_t receiveErrors;
		/** \brief Number of transmit errors**/
		uint16_t transmitErrors;
		/** \brief current state**/
		States state;
		/** \brief current activity**/
		Busy busy;
	};

	/** \brief this namespace contains some special templated data structures.
	 *
	 *  These are mainly for internal use.
	 **/
	namespace can_templates
	{

		/** \brief Class to handle the different CAN-Id's dependant on the used
		 * CAN-Version

		  * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
		  **/
		template<Versions version>
		class IdHelper;

		/** \brief Class to handle CAN-Id's for the CAN 2.0A specification. **/
		template<>
		class IdHelper<CAN_20A>
		{
			public:
				/** Class specific constant values*/
				enum Constants
				{
					idLength=11 /**< defines the length of the CAN ID in bits: 11**/
				};

				/**Declare the type that should be used for a CAN ID**/
				typedef uint16_t IdType;

				/** Return the maximum Id that is valid for CAN 2.0A

				 * @return the maximum Id for CAN 2.0A: 0x7FF.
				 **/

				static IdType getMaxId()
				{
					return 0x7FF;
				}
		};

		/** \brief Class to handle CAN-Id's for the CAN 2.0B specification. **/
		template<>
		class IdHelper<CAN_20B>
		{
			public:
				/** Class specific constant values*/
				enum Constants
				{
					idLength=29 /**< defines the length of the CAN ID in bits: 29**/
				};

				/**Declare the type that should be used for a CAN ID**/
				typedef uint32_t IdType;

				/** Return the maximum Id that is valid for CAN 2.0B

				 * @return the maximum Id for CAN 2.0B: 0x1FFFFFFF.
				 **/

				static IdType getMaxId()
				{
					return 0x1FFFFFFF;
				}
		};

	/** \brief Base struct of all messages
	 *
	 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
	 *
	 * This Message Structure is also the structure used to send RTRs.
	 **/

	template<Versions version>
	struct CANMsgSendBase
	{
		typedef typename IdHelper<version>::IdType idType;

		struct
		{
			/** Id of the CAN message**/
			idType id : sizeof(idType)*8-3;
			/** Flag defining this as a remote transmission request**/
			uint8_t rtr : 1;
			/**Flag defining the use of automatic reply**/
			uint8_t autoReply : 1;
			/**Flag defining the use of CAN 2.0A compatibility mode**/
			uint8_t compat : 1;
		};

		/** Length of the message in byte**/
		uint8_t length;

		/** The last event that occured \see Events for a list of possible
		 * events and their meaning.**/

		volatile Events event;

		/** \brief Standard Constructor

		 *  Sets the message to RTR, no auto reply, no CAN-2.0A compatibility
		 *  and set the event to NOTHING.
		 **/

		CANMsgSendBase() : rtr(1),
						   autoReply(0),
						   compat(0),
						   length(0),
						   event(NOTHING)
		{}

		/**\brief Sets the RTR state of this message.
		 *
		 * @param flag sets the RTR state to enabled(true) or disabled(false)
		 **/

		void setRTR(bool flag)
		{
			rtr=flag;
		}

		/**\brief Get the current RTR state of this message.
		 *
		 * @return true(enabled), false(disabled)
		 **/
		bool getRTR() const
		{
			return rtr;
		}

		/**\brief Sets the CAN 2.0A compatibility mode state of this message.

		 * @param flag sets the CAN 2.0A compatibility mode state to
		 * enabled(true) or disabled(false)
		 **/

		void set20ACompat(bool flag)
		{
			compat=flag;
		}

		/**\brief Gets the CAN 2.0A compatibility mode state of this message.
		 *
		 * @return true(enabled), false(disabled)
		 **/
		bool get20ACompat() const
		{
			return compat;
		}
	};

	/** \brief Base struct for message objects used for receiving CAN messages
	 *
	 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
	 *
	 * This struct is also used to receive RTRs.
	 * **/
	template<Versions version>
	struct CANMsgRecvBase : public CANMsgSendBase<version>
	{
		typedef typename IdHelper<version>::IdType idType;


		/*\brief Standard Constructor

		 * Sets the message to receive only RTRs and deactivates cyclic
		 * reception
		 **/

		CANMsgRecvBase() : CANMsgSendBase<version>(),
						   rtrMask(1),
						   cyclic(0),
						   acceptBoth(0)
		{};

		struct
		{
			/** Id Mask of the CAN Message**/
			idType idMask : sizeof(idType)*8-3;
			/**Flag defining the reception of RTRs**/
			uint8_t rtrMask : 1;
			/**Flag defining the use of cyclic reception**/
			uint8_t cyclic : 1;
			/**Flag defining the accpeptance of can20A and B messages**/
			uint8_t acceptBoth : 1;
		};


		/**\brief Sets the cycle mode of this CAN message struct

		 * @param flag if true driver will continue receiving, even after one
		 *		  message was successfully received if false, the driver will
		 *		  stop receiving after one complete message.
		 **/

		 void setCyclic(bool flag) { cyclic=flag; }

		/**\brief Gets the cycle mode of this CAN message struct
		 *
		 * @return true if cyclic receiving is activated, false otherwise.
		 **/
		bool getCyclic() const
		{
			return cyclic;
		}

		/**\brief Sets the RTR-Mask state of this message.

		 * @param rtrMask sets the RTR-Mask state to enabled(true) or
		 * disabled(false)
		 **/

		void setRTRMask(bool rtrMask)
		{
			this->rtrMask=rtrMask;
		}

		/**\brief Get the current RTR-Mask state of this message.
		 * @return true(enabled), false(disabled)
		 **/
		bool getRTRMask() const
		{
			return rtrMask;
		}

		/**\brief Sets the acceptance of CAN 2.0 A and B  of this message.
		 * @param flag sets the acceptance of both CAN 2.0 A and B messages.
		 **/

		void setAcceptBoth(bool flag)
		{
			acceptBoth=flag;
		}

		/**\brief Sets the acceptance of CAN 2.0 A and B of this message.
		 * @return the acceptance of both CAN 2.0 A and B messages.
		 **/
		bool getAcceptBoth() const
		{
			return acceptBoth;
		}
	};

	/** \brief Receive struct with additional timestamp
	 *
	 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
	 * **/
	template<Versions version>
	struct CANMsgRecvTimeStamp : public CANMsgRecvBase<version>
	{
		/** 2 byte timestamp**/
		uint16_t timeStamp;
	};

	/** \brief Struct for sending messages over the CAN-Bus
	 *
	 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
	 * **/
	template<Versions version>
	struct CANMsgSend : public CANMsgSendBase<version>
	{
		/*\brief Standard Constructor
		 *
		 * Sets the message to non-RTR**/
		CANMsgSend() : CANMsgSendBase<version>()
		{
			this->setRTR(0);
		}
		/** Contained Data of the CAN message**/
		uint8_t data[MAXMSGLEN];
	};

	/** \brief Struct for receiving CAN Messages with timestamps
	 *
	 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B

	 * \tparam withTimestamp decides if timestamp information will be included
	 * or not
	 ***/

	template<Versions version, bool withTimestamp>
	struct CANMsgRecv : public CANMsgRecvTimeStamp<version>
	{
		/*\brief Standard Constructor
		 *
		 * Sets the message to receive only non-RTRs**/

		CANMsgRecv() : CANMsgRecvTimeStamp<version>()
		{
			this->setRTR(0);
			this->setRTRMask(0);
		}

		/** class specific constant values */
		enum Constants
		{
			useTimestamp=withTimestamp /**< usage of timestamps**/
		};

		/** Contained Data of the CAN message**/
		uint8_t data[MAXMSGLEN];
	};

	template<Versions version>
	struct CANMsgRecv<version, false> : public CANMsgRecvBase<version>
	{

		CANMsgRecv() : CANMsgRecvBase<version>()
		{
			this->setRTR(0);
			this->setRTRMask(0);
		}

		enum
		{
			useTimestamp=false
		};

		uint8_t data[MAXMSGLEN];
	};

	};
}
}
