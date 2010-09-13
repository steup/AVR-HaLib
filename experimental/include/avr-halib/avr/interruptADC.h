#pragma once

#include <avr-halib/avr/basicADC.h>
#include <avr-halib/avr/sleep.h>

namespace avr_halib
{
namespace drivers
{
	/**\brief Interrupt based ADC driver
	 * \tparam adcConfig Configuration containing channel list and RegMap
	 *
	 * This class provides an driver for the ADC of Atmel AVRs, that is capable
	 * of using interrupt mechanisms to tell the application, that a conversion
	 * is finished.
	 **/
	template<typename RM>
	class InterruptADC : public BasicADC<RM>
	{
		public:
			/**\brief Shortcut to the RegMap**/
			typedef RM RegMap;
		private:
			/**\brief the conigured BasicADC class to use as basis**/
			typedef BasicADC<RM> Base;


		public:
			/**\brief Constructor calling the basis constructor**/
			InterruptADC() : Base()
			{
				UseRegMap(rm, RegMap);
				rm.adie=true;
			}

			/**\brief Start a conversion
			 * \param lowNoise if true, start the conversion using low noise mode
			 *
			 * Similar to the call fo the BasicADC, but enables the interrupt
			 * flag of the ADC and waits for termination if low noise is used.
			 **/
			void startConversion(bool lowNoise)
			{
				if(!lowNoise)
					this->Base::startConversion();
				else
					do
					{
						power::sleep(power::noiseReduce);
					}while(!this->isDone());
			}

			/**\brief Register a member function as callback for ADC conversion complete interrupt
			 * \tparam T the type of the object
			 * \tparam the member function of T
			 * \param obj the instance of T
			 *
			 * This function registers the provided member function to be
			 * called upon triggering of the interrupt
			 **/
			template<typename T, void (T::*Fxn)()>
			void registerCallback(T& obj)
			{
				RegMap::template setADCInterrupt<T, Fxn>(obj);
			}

			/**\brief Register a const-member function as callback for ADC conversion complete interrupt
			 * \tparam T the type of the object
			 * \tparam the const member function of T
			 * \param obj the constant instance of T
			 *
			 * This function registers the provided constant member function to
			 * be called upon triggering of the interrupt
			 **/
			template<typename T, void (T::*Fxn)() const>
			void registerCallback(const T& obj)
			{
				RegMap::template setADCInterrupt<T, Fxn>(obj);
			}

			/**\brief Register a static member function or a C-function as callback for ADC conversion complete interrupt
			 * \tparam the static member function or C-function
			 *
			 * This function registers the provided function to be
			 * called upon triggering of the interrupt
			 **/
			template<void (*Fxn)()>
			void registerCallback()
			{
				RegMap::template setADCInterrupt<Fxn>();
			}
	};

	/**\example interruptADC.cpp
	 *
	 * This example shows the usage of the ADC in an event driven way.  The ADC
	 * is set up using the compile-time interface, afterwards the MCU is sent
	 * to sleep, to reduce the impact of digital noise ond the conversion.  Up
	 * on finishing the conversion, the ADC triggers an interrupt, which causes
	 * the onConversionComplete function to be called, which then sets
	 * conversionComplete flag to true. This couses the while loop to
	 * terminate, so the value is fetched and displayed.
	 **/
}
}
