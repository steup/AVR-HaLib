/**
 *	\file	avr-halib/avr/canary/can_enums.h
 *	\brief	Enumrations for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

namespace avr_halib
{
namespace canary
{
	/** Constants that are used within the Can driver**/
	enum Constants
	{
		NUMMOBS=15,			/**< Number of available hardware message buffers**/
		MAXMSGLEN=8,		/**< Max length of one Can Message**/
		MAXCONCURRENTMSG=14	/**< Max number of concurrent Messages the driver 
							  *	 can handle
							  **/
	};

	/** States of the Can driver**/
	enum States
	{
		DISABLED,			/**< Can driver disabled, no transmission, no 
							  *  reception
							  **/

		ENABLED, 			/**< working, transmission and reception possible**/
		ERRORPASSIVE,		/**< error counter over 128, only reception, no 
							  *	 transmission
							  **/

		BUSOFF				/**< error counter over 255, not working, reset 
							  *  required
							  **/
	};

	/** Currently ongoing Activities in the driver**/
	enum Busy
	{
		NONE,				/**< nothing**/
		TRANSMITTER,		/**< sending**/
		RECEIVER			/**< receiving**/
	};

	/**Supported Can versions**/
	enum Versions
	{
		CAN_20A,			/**<Basic-CAN(CAN-2.0A) Id-length 11 bits**/
		CAN_20B 			/**<Extended-CAN(CAN-2.0B) Id-length 29 bits**/
	};

	/**Possible Events**/
	enum Events
	{
		SUCCESS,			/**< operation succedded**/
		NOTHING,			/**< nothing happend**/
		INVALIDID,			/**< invalid id for message**/
		INVALIDMSG,			/**< message is invalid**/
		NOFREEMOB,			/**< there were not enough free message objects to 
							  *  fullfill the requestet operation
							  **/

		STUFFERROR,			/**< stuff error on the bus**/
		CRCERROR,			/**< crc-check of the message failed*/
		FORMERROR,			/**< message ill formated on the bus**/
		ACKERROR,			/**< no acknowledgement received**/
		BITERROR,			/**< bit error on the bus**/
		RECEIVEOK,			/**< reception of message succedded**/
		TRANSMITOK			/**< transmission of message succedded**/
	};


	/**Possible configurations for the hardware message buffers**/
	enum MobConfigs
	{
		DISABLEDMOB ,		/**< disabled**/
		TXMOB,				/**< sending**/
		RXMOB,				/**< receiving**/
		FRAMEBUFFERMOB 		/**< frame reception mode**/
	};

	/**Possible bus-speeds for the Can-bus**/
	enum Speeds
	{
		SPEED_1M=1000,		/**< 1   Mbit/s**/
		SPEED_500K=500,		/**< 500 kbit/s**/
		SPEED_250K=250,		/**< 250 kbit/s**/
		SPEED_200K=200,		/**< 200 kbit/s**/
		SPEED_125K=125,		/**< 125 kbit/s**/
		SPEED_100K=100		/**< 100 kbit/s**/
	};

	/**Number of Subbits to be used, Subbits control the ammount of internal 
	 * values, that are taken to determine the actual Bit-Value of the CAN-Bus.
	 * More subbits can mean a little more power consumption,but can improve 
	 * signal-quality**/

	enum Subbits
	{
		SUBBITS_8,/**<8 Subbits**/
		SUBBITS_16/**<16 Subbits**/
	};

	/**Baudrate configuration for the Can driver supplied in the
	 * configuration**/

	template<unsigned long long, Speeds, Subbits=SUBBITS_8>
	struct BaudRateConfig;

	template<Speeds speed>
	struct BaudRateConfig<16000000ULL, speed, SUBBITS_16>
	{
		enum
		{
			brp=(SPEED_1M/speed-1),
			sjw=0,
			prs=6,
			phs1=3,
			phs2=3,
			smp=1
		};
	};

	template<Speeds speed>
	struct BaudRateConfig<16000000ULL, speed, SUBBITS_8>
	{
		enum
		{
			brp=(SPEED_1M/speed*2-1),
			sjw=0,
			prs=2,
			phs1=1,
			phs2=1,
			smp=1
		};
	};

	template<>
	struct BaudRateConfig<8000000ULL, SPEED_1M, SUBBITS_16>
	{
	};

	template<>
	struct BaudRateConfig<8000000ULL, SPEED_200K, SUBBITS_16>
	{
		enum
		{
			brp=1,
			sjw=0,
			prs=7,
			phs1=5,
			phs2=4,
			smp=1
		};
	};

	template<Speeds speed>
	struct BaudRateConfig<8000000ULL, speed, SUBBITS_16>
	{
		enum
		{
			brp=(SPEED_500K/speed-1),
			sjw=0,
			prs=6,
			phs1=3,
			phs2=3,
			smp=1
		};
	};

	template<Speeds speed>
	struct BaudRateConfig<8000000ULL, speed, SUBBITS_8>
	{
		enum
		{
			brp=(SPEED_1M/speed-1),
			sjw=0,
			prs=2,
			phs1=1,
			phs2=1,
			smp=1
		};
	};
}
}
