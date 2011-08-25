#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace at90can128
{
	struct pwm
	{
		enum Type
		{
			fast,
			phaseCorrect,
			phaseFreqCorrect
		};

		enum Cycle
		{
			static8,
			static9,
			static10,
			dynamicICR,
			dynamicOCRA
		};

		enum OutputMode
		{
			normal   =2,
			inverted
		};

		/** \brief clock prescaler for this timer **/
		enum Prescalers
		{
			noClock=0,			/**< clock input disabled, no counting **/
			ps1,				/**< \f$ f_{t}=f_{cpu} \f$ **/
			ps8,				/**< \f$ f_{t}=\frac{f_{cpu}}{8} \f$ **/
			ps64,				/**< \f$ f_{t}=\frac{f_{cpu}}{64} \f$ **/
			ps256,				/**< \f$ f_{t}=\frac{f_{cpu}}{256} \f$ **/
			ps1024,				/**< \f$ f_{t}=\frac{f_{cpu}}{1024} \f$ **/
			extClkFalling,		/**< external clock input on falling edge **/
			extClkRising		/**< external clock input on rising edge **/
		};
	};
}
}
}
}
