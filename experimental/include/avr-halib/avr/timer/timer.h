#pragma once

#include <avr-halib/avr/timer/base.h>
#include <avr-halib/avr/timer/dynamicPrescaler.h>
#include <boost/mpl/if.hpp>

namespace avr_halib
{
namespace drivers
{
namespace timer
{
	using boost::mpl::if_c;

	template<typename config>
	struct Timer : public if_c< config::dynamicPrescaler,
								DynamicPrescaler<config, Base>,
								Base<config>
							>::type
	{
		private:
			typedef typename if_c< config::dynamicPrescaler, 
								   DynamicPrescaler<config, Base>,
								   Base<config>
							   >::type BaseDriver;

		public:
			typedef typename config::RegMap RegMap;
			typedef typename BaseDriver::OutputCompareUnits OutputCompareUnits;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;
			typedef typename RegMap::WaveForms WaveForms;
	};
}
}
}
