/**
 *	\file	avr-halib/avr/canary/can_noint.h
 *	\brief	Interrpt free CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "canary.h"

namespace avr_halib
{
namespace canary
{
	namespace can_templates
	{
		/** \brief Transformator to deactivate interrupt usage **/
		template<typename userConfig>
		struct ConfigTransform_DisableInterrupts : public userConfig
		{
			enum
			{
				useReceiveInterrupt=false,
				useTransmitInterrupt=false,
				useErrorInterrupt=false
			};
		};
	}

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
	template<typename userConfig>
	class CanNoInt : public Canary<can_templates::ConfigTransform_DisableInterrupts<userConfig> >
	{};

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
