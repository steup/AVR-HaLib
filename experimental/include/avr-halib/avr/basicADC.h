#pragma once

#include <avr-halib/regmaps/regmaps.h>
#include <stdint.h>

namespace avr_halib
{
namespace drivers
{
	/**\brief Basic ADC class
	 * \tparam cfg Configuration
	 *
	 * This class provides an polled interface to the ADC of the AVR.  It
	 * provides support for compile time configurations and selection of these
	 * during run and compile time.
	 **/

	template<typename RM, bool disableAfterConversion=false>
	class BasicADC
	{
		private:
			/**\brief Value fetch helper**/
			template<typename regmap>
			struct Fetcher
			{
				/** fetch an 8-bit value
				 * \param value the storage to copy value to
				 **/
				static void fetch(uint8_t& value)
				{
					UseRegMap(rm, regmap);
					rm.aden=false;
					SyncRegMap(rm);
					value=rm.adch;
				}

				/** fetch an 16-bit value
				 * \param value the storage to copy value to
				 **/
				static void fetch(uint16_t& value)
				{
					UseRegMap(rm, regmap);
					rm.aden=false;
					SyncRegMap(rm);
					value=rm.adc;
				}
			};

			/**\brief Applying the regmap to the value fetcher**/
			typedef Fetcher<RM> Fetch;

			/**\brief Shortcut to ADC multiplexer enumeration**/
			typedef typename RM::MuxChannels Mux;
			/**\brief Shortcut to ADC reference sources enumeration**/
			typedef typename RM::References Ref;
			/**\brief Shortcut to ADC prescaler enumeration**/
			typedef typename RM::Prescalers Ps;

		public:
			/**\brief Shortcut to the RegMap**/
			typedef RM RegMap;

		private:
			/**\brief helper function, checks for ongoing adc operation
			 * \return true if busy, false otherwise
			 **/
			bool checkBusy()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.adsc;
			}

		public:
			/**\brief Basic Constructor
			 *
			 * enables ADC, disables interrupts and time trigger operation
			 **/
			BasicADC()
			{
				UseRegMap(rm, RegMap);
				rm.adie=false;
				rm.aden=true;
				rm.adate=false;
			}

			/**\brief Basic Destructor
			 *
			 * disables ADC and resets ADC channels pins to GPIO mode
			 *
			 **/
			~BasicADC()
			{
				UseRegMap(rm, RegMap);
				rm.aden=false;
				rm.didr0=0;
			}

			/**\brief start the conversion**/
			void startConversion()
			{
				UseRegMap(rm, RegMap);
				rm.aden=true;
				rm.adsc=true;
				SyncRegMap(rm);
			}

			/**\brief configure the adc for a conversion
			 * \param mux the adc channel to use
			 * \param ref the reference voltage to use
			 * \param ps the prescaler for the conversion, if omitted the adc runs at 400 kHz
			 * \return true if every thing went well, false otherwise
			 *
			 * Uses the supplied index i to find a configuration in the passed
			 * cfg structure, to set up the adc.
			 * Run-time version of this function.
			 **/
			template<typename ReturnType>
			bool configure(Mux mux, Ref ref, Ps ps=RM::recommendedPrescaler)
			{
				if(checkBusy())
					return false;

				UseRegMap(rm, RM);
				rm.adps=ps;
				rm.mux=mux;
				rm.refs=ref;
				if(sizeof(ReturnType)==1)
					rm.adlra=true;
				else
					rm.adlra=false;
				SyncRegMap(rm);

				return true;
			}

			/**\brief Fetch the result of a conversion
			 * \tparam T the type of the conversion value
			 * \param value a reference to store the value to
			 * \return Result of operation, see ADCResults
			 *
			 * run-time version.
			 **/
			template<typename T>
			void fetchValue(T &value)
			{
				Fetch::fetch(value);
			}

			/**\brief Check for finished conversions
			 * \return true if finished, false otherwise
			 **/
			bool isDone()
			{
				return !checkBusy();
			}
	};

	/**\example basicADC.cpp
 	*
 	* This example uses 3 ADC channels connected to the Robbyboard.  The
 	* application periodically starts ADC conversions and print the output to the
 	* LCD.
 	**/
}
}
