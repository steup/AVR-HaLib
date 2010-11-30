#pragma once

namespace avr_halib
{
namespace config
{
	template<typename TimerRegMap>
	struct TimerDefaultConfig
	{
		typedef TimerRegMap RegMap;

		enum Parameters
		{
			overflowInt		= false,
			ocmAInt			= false,
			ocmBInt			= false,
			ocmCInt			= false,
			async			= false,
			asyncExtClock	= false,
			dynamicPrescaler= false
		};

		typedef typename RegMap::WaveForms  WF;
		typedef typename RegMap::Prescalers PS;
		typedef typename RegMap::CompareMatchModes CM;

		static const WF waveForm = RegMap::normal;
		static const PS ps		 = RegMap::noClock;
		static const CM ocmAMode = RegMap::noOutput;
		static const CM ocmBMode = RegMap::noOutput;
		static const CM ocmCMode = RegMap::noOutput;
	};
}
}
