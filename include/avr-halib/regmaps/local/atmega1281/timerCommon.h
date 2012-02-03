#pragma once

namespace avr_halib
{
namespace regmaps
{
namespace local
{
namespace atmega1281
{
namespace helpers
{
/** \brief Common definitions of register content for timers **/
struct CommonTimerDefinitions
{
		/** \brief behaviours on compare match **/
		enum CompareMatchModes
		{
			noOutput,			/**< no output on compare match **/
			toggle,				/**< toggle output on compare match, not applicable for pwm modes **/
			clear,				/**< clear output on compare match **/
			set					/**< set output on compare match **/
		};
};
}
}
}
}
}
