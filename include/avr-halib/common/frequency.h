#pragma once

#include <stdint.h>

namespace avr_halib
{
namespace config
{
	/**\brief Helper template to configure frequency parameters
	 * \tparam numerator the numerator of the frequency
	 * \tparam denominator the numerator of the frequency
	 *
	 *  Due to compiler limitations the enumeration values can not exceed
	 *  16bit, therefore the cpu speed must be broken down into components.
	 *  This library uses the Hz kHz and mHz components, whuch are available as
	 *  enumeration values of this template.
	 **/
	template<unsigned long long freqNum, unsigned long long freqDenom=1>
	struct Frequency
	{
		/**\brief the frequency value**/
		static const long long numerator=freqNum;
		static const long long denominator=freqDenom;
		
		public:
			static const long long value=numerator/denominator;
			/**\brief Additional declaration for compatibility with AVR-Halib**/
			static const long long controllerClk=value;

			/**\brief Compil-time constant values**/
			enum Constants
			{
				MHz=value%1000000000ULL,	              /**< MHz component**/
				kHz=value%1000000ULL,	              /**< kHz component**/
				hz =value%1000,			              /**<  Hz component**/
				mHz=(1000*value)%1000 /**< mHz component**/
			};

			template<typename FreqB>
			struct div
			{
				typedef Frequency<numerator*FreqB::denominator, denominator*FreqB::numerator> type;
			};

			template<typename FreqB>
			struct mult
			{
				typedef Frequency<numerator*FreqB::numerator, denominator*FreqB::denominator> type;
			};
	};
}
}
