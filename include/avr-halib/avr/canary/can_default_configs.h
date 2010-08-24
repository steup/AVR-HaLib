/**
 *	\file	avr-halib/avr/canary/can_default_configs.h
 *	\brief	Default configuration for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_enums.h"
#include "can_regmap.h"

namespace avr_halib
{
namespace canary
{

/** \brief This class is the default configuration for the Can driver.
 
 * Your configuration should inherit from this class, so that all values that
 * are unset by you, get the default value.  See the members of the anon
 * enumeration for configuration parameters and valid values
 **/

/**\ingroup canary*/
struct defaultCANConfig
{
	/** \brief Specification of Regmap to use **/
	template<Versions version>
	struct Regmap : public CANRegmap<version>{};

	/** \brief This typedef defines the baud rate, that the driver should use.
	 
	 * you should always use F_CPU as the first parameter, the second and third
	 * parameters are members of the Speeds and Subbits enumerations.

	 * \see Speeds
	 * \see Subbits
	 **/

	typedef BaudRateConfig<F_CPU, 
										   SPEED_1M, 
										   SUBBITS_16>
		baudRate;

	/** \brief This anon enumeration contains the different configuration
	 * parameters for the Can driver**/

	enum
	{
		version=CAN_20A,	/**< This parameter tells the Can
											 * 	 driver which version of the
											 *	 Can-Bus-Protocol
				   						   	 *   you want to use.
											 *
											 *   \see Versions
										   	 **/

		prescaler=255,					 	/**< This is the prescaler for the 
											 *   internal timestamp-counter of 
											 *   the Can driver.
											 *   The Formula is: 
											 *   \f$ T_{clk_{can}}=T_{clk_{io}}
											 *   \cdot 8 \cdot prescaler\f$.
											 **/

		useTimestamp=true,					/**< This parameter controls the 
											  *  usage of timestamps in the 
											  *  received messages
											  **/

		maxConcurrentMsgs=
			
			MAXCONCURRENTMSG,				/**< This parameter tells the driver
											  *	 how much concurrent messages 
											  *	 you want to use.
							 				  *  Every additional concurrent 
											  *  Message costs you 2 Bytes. If
							 				  *  the maximal number is reached
											  *  every send/receive operation
											  *  will return with NOFREEMOB.
							 				  *  The maximum value that can be 
											  *  used here is MAXCONCURRENTMSG,
											  * which is also the default.
							 				  **/

		useReceive=true, 					/**< Activates or deactivates the 
											  *	 receive functionality of the
						   	  				  *	 driver.
											  **/

		useReceiveInterrupt=true,			/**< Activates or deactivates the
											  usage of interrupts for
											  receiving.
											  **/

		useTransmit=true,					/**< Activates or deactivates the 
											  *	 transmit functionality of the 
											  *	 driver.
											  **/

		useTransmitInterrupt=true,			/**< Activates or deactivates the
											  usage of interrupts for
											  transmitting.
											  **/

		useError=true,						/**< Activates or deactivates the 
											  *	 error reporting functionality 
											  *	 of the driver
											  **/

		useErrorInterrupt=true,   			/**< Activates or deactivates the
											  usage of interrupts for
											  transmitting.
											  **/
	};
};

/** \example canConfig.h
 * This file contains the configurations, that are used for the example
 * CAN network.
 **/

/** \example canConfigNoInt.h
 * This file contains the configurations, that are used for the interrupt free example
 * CAN network.
 **/

}
}
