#pragma once

namespace avr_halib
{
namespace config
{
	/**\brief Helper template to configure frequency parameters
	 * \tparam freq the frequency
	 *
	 *  Due to compiler limitations the enumeration values can not exceed
	 *  16bit, therefore the cpu speed must be broken down into components.
	 *  This library uses the Hz kHz and mHz components, whuch are available as
	 *  enumeration values of this template.
	 **/
	template<unsigned long long freq>
	struct Frequency
	{
		/**\brief the frequency value**/
		static const long long value=freq;
		/**\brief Additional declaration for compatibility with AVR-Halib**/
		static const long long controllerClk=freq;
		/**\brief Compil-time constant values**/
		enum Constants
		{
			mHz=freq%1000000000ULL,	/**< MHz component**/
			kHz=freq%1000000ULL,	/**< kHz component**/
			hz =freq%1000			/**<  Hz component**/
		};
	};
}
}
