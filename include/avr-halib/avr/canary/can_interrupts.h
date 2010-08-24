#pragma once

#include "can_base.h"
#include <avr-halib/share/delegate.h>

namespace avr_halib
{
	namespace canary
	{

/** \brief Base class for all interrupt driven operations
 *  \tparam config the supplied configuration
 
 *	This class provides the overrun interrupt functionality, because it does cost nothing if unused.
 *	If one wants to use this interrupt, there must be an UseInterrupt(SIG_CAN_OVERRUN1) ONCE in the code.
 *	Additionally the basic data strcutures for the interrupt handler classed are provided.
 **/
template<typename config>
class InterruptBase : public CANBase<config>
{	
	protected:
		/** \brief Forward declaration of the Regmap **/
		typedef typename CANBase<config>::Regmap Regmap;

		/** \brief Definition of the configured message structure for incoming
	 	* messages **/

		typedef can_templates::CANMsgRecv<static_cast<Versions>(config::version),
									  config::useTimestamp>
			MsgRecv;

		/** \brief Definition of the configured message structure for outgoing
	 	* messages **/

		typedef can_templates::CANMsgSendBase<static_cast<Versions>(config::version)> 
			RTRSend;

		/** \brief Definition of the configured structure for error reporting **/
		typedef typename CANBase<config>::MsgSendBase Error;

		/** \brief Standard constructor
		 *
		 * Registers the internal callback and activates the appropriate
		 * interrupts, but leaves the general interrupt disabled.
		 * It should be enabled if a general callback is registered.
		 **/
		InterruptBase()
		{
			UseRegmap(can, Regmap);

			can.generalInterruptEnable.all=false;
			can.generalInterruptEnable.receive=config::useReceive;
			can.generalInterruptEnable.transmit=config::useTransmit;
			can.generalInterruptEnable.MObError=config::useError;
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
};

/** \brief Interrupt handler class for reception
 *  \tparam useReceiveInterrupt defines the usage of interrupts for reception
 *  \tparam config the supplied configuration
 
 *  This class handles the registration and the interface to user supplied
 *  interrupt callbacks for receive events.  If receive interrupts are disables
 *  this class will not provide any interface to register or deregister
 *  callbacks.  The calling of the callbacks is done automatically by the
 *  polling function, which is registered as the hardware interrupt handler, if
 *  useReceiveInterrupt is true.
 **/

template<bool useReceiveInt, typename config>
class ReceiveInterrupt : public InterruptBase<config>
{
	private:
		/** \brief Forward declaration **/
		typedef typename InterruptBase<config>::MsgRecv MsgRecv;
		/** \brief The delegate that holds the possible receive callback **/
		Delegate<MsgRecv&> receiveFun;
		
	public:
		/** \brief Registers a callback for the Receive-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * \param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(MsgRecv&)>
		void enableReceiveInterrupt(T& obj)
		{
			receiveFun.template bind<T, Fxn>(&obj);
		}

		/**\copydoc ReceiveHandler::enableReceiveInterrupt(T&)**/

		template<class T, void (T::*Fxn)(MsgRecv&) const>
		void enableReceiveInterrupt(const T& obj)
		{
			receiveFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Receive-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(MsgRecv&)>
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

	protected:
		/** \brief Calls the delegate if set
		 *
		 *  This function abstracts the delegate, enabling (de)activating the
		 *  delegate completely
		 **/

		void receiveCallback(MsgRecv &msg)
		{
			if(!receiveFun.isEmpty())
				receiveFun(msg);
		}
};

template<typename config>
class ReceiveInterrupt<false, config> : public InterruptBase<config>
{
	private:
		typedef typename InterruptBase<config>::MsgRecv MsgRecv;

	public:
		void receiveCallback(MsgRecv &msg){}
};

/** \brief Interrupt handler class for transmission
 *  \tparam useTransmitInterrupt defines the usage of interrupts for transmission
 *  \tparam config the supplied configuration
 
 *  This class handles the registration and the interface to user supplied
 *  interrupt callbacks for transmission complete events.  If transmission complete interrupts are disables
 *  this class will not provide any interface to register or deregister
 *  callbacks.  The calling of the callbacks is done automatically by the
 *  polling function, which is registered as the hardware interrupt handler, if
 *  useTransmitInterrupt is true.
 **/

template<bool useTransmitInt, typename config>
class TransmitInterrupt : public ReceiveInterrupt<config::useReceiveInterrupt, config>
{
	private:
		/** \brief Forward declaration **/
		typedef typename InterruptBase<config>::RTRSend RTRSend;
		/** \brief The delegate that holds the possible transmit callback **/
		Delegate<RTRSend&> transmitFun;
		
	public:
		/** \brief Registers a callback for the Transmit-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * \param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(RTRSend&)>
		void enableTransmitInterrupt(T& obj)
		{
			transmitFun.template bind<T, Fxn>(&obj);
		}

		/**\copydoc TransmitHandler::enableTransmitInterrupt(T&)**/

		template<class T, void (T::*Fxn)(RTRSend&) const>
		void enableTransmitInterrupt(const T& obj)
		{
			transmitFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Transmit-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(RTRSend&)>
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

	protected:
		/** \brief Calls the delegate if set
		 *
		 *  This function abstracts the delegate, enabling (de)activating the
		 *  delegate completely
		 **/

		void transmitCallback(RTRSend &msg)
		{
			if(!transmitFun.isEmpty())
				transmitFun(msg);
		}
};

template<typename config>
class TransmitInterrupt<false, config> : public ReceiveInterrupt<config::useReceiveInterrupt, config>
{
	private:
		typedef typename InterruptBase<config>::RTRSend RTRSend;

	public:
		void transmitCallback(RTRSend&){}
};

/** \brief Interrupt handler class for error reporting
 *  \tparam useTransmitInterrupt defines the usage of interrupts for error reporting
 *  \tparam config the supplied configuration
 
 *  This class handles the registration and the interface to user supplied
 *  interrupt callbacks for error reproting events.  If error reporting interrupts are disables
 *  this class will not provide any interface to register or deregister
 *  callbacks.  The calling of the callbacks is done automatically by the
 *  polling function, which is registered as the hardware interrupt handler, if
 *  useErrorInterrupt is true.
 **/

template<bool useErrorInt, typename config>
class ErrorInterrupt : public TransmitInterrupt<config::useTransmitInterrupt, config>
{
	private:
		/** \brief Forward declaration **/
		typedef typename InterruptBase<config>::Error Error;
		/** \brief The delegate that holds the possible error callback **/
		Delegate<Error&> errorFun;
		
	public:
		/** \brief Registers a callback for the Error-Interrupt.
		 *
		 * \tparam T class which contains the method
		 * \tparam Fxn the method of class T, which should be used
		 *
		 * \param obj the instance of T which method should be called
		 **/

		template<class T, void (T::*Fxn)(Error&)>
		void enableErrorInterrupt(T& obj)
		{
			errorFun.template bind<T, Fxn>(&obj);
		}

		/**\copydoc ErrorHandler::enableErrorInterrupt(T&)**/

		template<class T, void (T::*Fxn)(Error&) const>
		void enableErrorInterrupt(const T& obj)
		{
			errorFun.template bind<T, Fxn>(&obj);
		}

		/** \brief Registers a callback for the Error-Interrupt.
		 *
		 * \tparam Fxn the function, that is to be called
		 **/

		template<void (*Fxn)(Error&)>
		void enableErrorInterrupt()
		{
			errorFun.template bind<Fxn>();
		}

		/** \brief Disables the registered callback for the Error-Interrupt
		 **/
		void disableErrorInterrupt()
		{
			errorFun.reset();
		}

	protected:
		/** \brief Calls the delegate if set
		 *
		 *  This function abstracts the delegate, enabling (de)activating the
		 *  delegate completely
		 **/

		void errorCallback(Error &error)
		{
			if(!errorFun.isEmpty())
				errorFun(error);
		}
};

template<typename config>
class ErrorInterrupt<false, config> : public TransmitInterrupt<config::useTransmitInterrupt, config>
{
	private:
		typedef typename InterruptBase<config>::Error Error;

	public:
		void errorCallback(Error &msg){}
};

/** \brief Unification class
 *  \tparam config supplied configuration
 
 *  This class unificates the configured functionality of all interrupt
 *  handlers for the CAN driver.
 **/

template<typename config>
class Interrupts : public ErrorInterrupt<config::useErrorInterrupt, config>
{};

}

}
