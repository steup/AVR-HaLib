#pragma once

#include "can_handlers.h"

namespace avr_halib
{

namespace canary
{

namespace can_templates
{
/** \brief Helper class
  *	\tparam useInterrupts defines the usage of interrupts
  * \tparam config	supplied configuration
  
  * This helper class does the binding between hardware interrupts and driver,
  * depending on the useInterrupt parameter
  **/

template<bool useInterrupts, typename config>
class InterruptBindingHelper : public Handlers<config>
{
	protected:
		/** \brief registers hardware interrupt handler
		  
		  * By constructing this object the generalCallback function of this
		  * class will be registered as theinterrupt handler for all CAN
		  * interrupts excluding the overrun interrupt. Additionally the global
		  * interrupt enable bit of the can hardware will be set.
		  **/

		InterruptBindingHelper()
		{
			redirectISRM(SIG_CAN_INTERRUPT1,
					 	&InterruptBindingHelper::generalCallback,
					 	*this);

			this->enableInterrupts();
		}

	private:
		/** \brief Global callback of the CAN driver
		 *
		 *  This function calls the polling function.
		 **/

		void generalCallback()
		{
			this->checkEvents();
		}
};

template<typename config>
class InterruptBindingHelper<false, config> : public Handlers<config>
{};
}

/** \brief Config transformer to check if interrupts are needed
  * \tparam config the configuration to transform
**/
template<typename config> 
struct ConfigTransform_CheckInterruptUsage : public config 
{ 
	enum {
			useReceiveInterrupt		= config::useReceive	&& config::useReceiveInterrupt,
			useTransmitInterrupt	= config::useTransmit	&& config::useTransmitInterrupt,
			useErrorInterrupt		= config::useError		&& config::useErrorInterrupt,
			useInterrupts			= useReceiveInterrupt 	||
							  		  useTransmitInterrupt 	||
									  useErrorInterrupt
		}; 
}; 

/** Binding class between hardware interrupts and driver
 *	\tparam config supplied configuration
 *  
 *  If interrupts are needed, the class binds the polling function to the hardware interrupt.
 *  If not needed nothing will be done.
 **/
template<typename config>
class InterruptBinder : public can_templates::InterruptBindingHelper<config::useInterrupts, config>
{};
}

}
