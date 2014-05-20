/**
 *	\file	avr-halib/avr/canary/can_regmap.h
 *	\brief	CAN Regmap for at90canX
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_enums.h"

#include <inttypes.h>
#include <avr-halib/avr/regmaps.h>

namespace avr_halib
{
namespace canary
{
	namespace can_templates
	{
		/** \brief Helper class for the at90canX CAN driver regmap
	 	*
		* \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
		*
		*  This template is used to switch between the different register maps
		*  dependand on the CAN-Version that is being used.
		**/

		template<Versions>
		struct CANID;

		template<>
		struct CANID<CAN_20A>
		{
			union
			{
				struct
				{
					uint8_t canidt4;
					uint8_t canidt3;
					uint8_t canidt2;
					uint8_t canidt1;
				};
	
				struct
				{
					bool reservedBit0 : 1;
					uint8_t : 1;
					bool remoteTransmissionRequest : 1;
					uint32_t : 18;
					uint16_t id: 11;
				};
			};
	
			union
			{
				struct 
				{
					uint8_t canidm4;
					uint8_t canidm3;
						uint8_t canidm2;
					uint8_t canidm1;
				};

				struct
				{
					bool  idExtensionMask: 1;
					uint8_t : 1;
					bool remoteTransmissionMask : 1;
					uint32_t : 18;
					uint16_t idMask: 11;
				};
			};
		}__attribute__((packed));
	
		template<>
		struct CANID<CAN_20B>
		{
			union
			{
				struct
				{
					uint8_t canidt4;
					uint8_t canidt3;
					uint8_t canidt2;
					uint8_t canidt1;
				};
		
				struct
				{
					bool reservedBit0 : 1;
					uint8_t reserverdBit1: 1;
					bool remoteTransmissionRequest : 1;
					uint32_t id : 29;
				};
			};
	
			union
			{
				struct
				{
					uint8_t canidm4;
					uint8_t canidm3;
					uint8_t canidm2;
					uint8_t canidm1;
				};

				struct
				{
					bool  idExtensionMask: 1;
					uint8_t : 1;
					bool remoteTransmissionMask : 1;
					uint32_t idMask: 29;
				};
			};
		}__attribute__((packed));
	};

/** \brief the register map of the at90canX
 *
 * \tparam version CAN-Version to be used, may be: CAN_20A or CAN_20B
 
 * It contains all registers with the appropriate bits, that belong to the
 * CAN-Controller.  For further reference see the Atmel at90canX Manual.
 
 * The template parameter controls the CAN-Version that is being used.
 **/
template<Versions version>
class CANRegmap
{
	public:
		uint8_t __base[0xD8];
		union
		{
			uint8_t cangcon;
			struct
			{
				bool swReset : 1;
				bool enable : 1;
				bool __test : 1;
				bool listen : 1;
				bool syncTTCToEOF : 1;
				bool useTTC : 1;
				bool overloadFrame : 1;
				bool abort : 1;
			} generalControl;
		};
		union
		{
			uint8_t cangsta;
			struct
			{
				bool errorPassiveMode : 1;
				bool busOffMode : 1;
				bool enabled : 1;
				bool receiverBusy : 1;
				bool transmitterBusy : 1;
				bool __unused1 : 1;
				bool frameOverloaded : 1;
				bool __unused2: 1;
			} generalStatus;
		};
		union
		{
			uint8_t cangit;
			struct
			{
				bool ackError : 1;
				bool formError : 1;
				bool crcError : 1;
				bool stuffError : 1;
				bool frameBufferReceive : 1;
				bool timerOverrun : 1;
				bool bufOffEntered : 1;
				bool generalInt : 1;
			} generalInterrupt;
		};
		union
		{
			uint8_t cangie;
			struct
			{
				bool timerOverrun : 1;
				bool generalError: 1;
				bool frameBuffer : 1;
				bool MObError : 1;
				bool transmit : 1;
				bool receive : 1;
				bool busOff : 1;
				bool all : 1;
			} generalInterruptEnable;
		};
		union
		{
			struct
			{
				uint8_t canen2;
				uint8_t canen1;
			};
			uint16_t MObsAvailability : 15;
		};
		union
		{
			struct
			{
				uint8_t canie2;
				uint8_t canie1;
			};
			uint16_t MObsIntUsage : 15;
		};

		union
		{
			struct
			{
				uint8_t cansit2;
				uint8_t cansit1;
			};
			uint16_t MObsIntActive : 15;
		};

		struct
		{
			union
			{
				struct
				{
					uint8_t __reserved1 : 1;
					uint8_t brp        : 6;
					uint8_t __reserved2 : 1;
				};
			
				uint8_t canbt1;
			};

			union
			{
				struct
				{
					uint8_t __reserved3 : 1;
					uint8_t prs         : 3;
					uint8_t __reserved4 : 1;
					uint8_t sjw	    : 2;
					uint8_t __reserved5 : 1;
				};

				uint8_t canbt2;
			};

			union
			{
				struct
				{
					uint8_t smp 	    : 1;
					uint8_t phs1 	    : 3;
					uint8_t phs2 	    : 3;
					uint8_t __reserved6 : 1;
				};

				uint8_t canbt3;
			};

		} baudRateConfig;

		union
		{
			uint8_t cantcon;
			uint8_t timerPrescaler;
		};

		union
		{
			struct
			{
				uint8_t cantiml;
				uint8_t cantimh;
			};
			uint16_t timerCounter;
		};
	
		union
		{
			struct
			{
				uint8_t canttcl;
				uint8_t canttch;
			};
			uint16_t TTCCounter;
		};

		union
		{
			uint8_t cantec;
			uint8_t errorCounter;
		};

		union
		{
			uint8_t canrec;
			uint8_t receiveErrorCounter;
		};

		union
		{
			uint8_t canhpmob;
			struct
			{
				uint8_t gpBits : 4;
				uint8_t highPrivMOb : 4;
			};
		};

		union
		{
			uint8_t canpage;
			struct
			{
				uint8_t fifoIndex : 3;
				bool noAutoInc : 1;
				uint8_t curMObNum : 4;
			};
		};

		union
		{
			uint8_t canstmob;
			struct
			{
				bool ackError : 1;
				bool formError : 1;
				bool crcError : 1;
				bool stuffError : 1;
				bool bitError : 1;
				bool receiveOk : 1;
				bool transmitOk: 1;
				bool dataLengthWarning : 1;
			} mobStatus;
		};

		union
		{
			uint8_t cancdmob;
			struct
			{
				uint8_t dataLengthCode : 4;
				bool idExtension : 1;
				bool replyValid : 1;
				uint8_t config : 2;
			} mobConfig;
		};

		can_templates::CANID<version> idConfig;

		union
		{
			struct
			{
				uint8_t canstml;
				uint8_t canstmh;

			};
			uint16_t timeStamp;
		};

		uint8_t msg;
}__attribute__((packed));

}
}
