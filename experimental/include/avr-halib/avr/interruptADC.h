#pragma once

#include <avr-halib/avr/basicADC.h>
#include <avr-halib/avr/sleep.h>
#include <avr-halib/interrupts/interrupt.h>

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
	template<typename RM, bool disableAfterConversion=false>
	class InterruptADC : public BasicADC<RM, disableAfterConversion>,
						 public interrupts::InterruptRegistration<typename RM::InterruptMap>
	{
		public:
			/**\brief Shortcut to the RegMap**/
			typedef RM RegMap;
			typedef typename RegMap::InterruptMap InterruptMap;
			typedef typename InterruptMap::Slots InterruptSlotList;
		private:
			/**\brief the configured BasicADC class to use as basis**/
			typedef BasicADC<RM, disableAfterConversion> Base;


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
				{
					UseRegmap(rm, RegMap);
					rm.aden=true;
					SyncRegMap(rm);
					
					do
					{
						Morpheus::template sleep<power::noiseReduce>();
					}while(!this->isDone());
				}
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
