#pragma once

#include <avr/sleep.h>

namespace avr_halib
{
namespace power
{
	/**\brief Possible sleep modes of the AVR
	 **/
	enum SleepModes
	{
		idle=0,			/**<Deactivates \f$clk_{CPU}\f$ and \f$clk_{Flash}\f$**/
		noiseReduce=1,	/**<Deactivates \f$clk_{CPU},clk_{Flash},clk_{IO}\f$, reduces noise for ADC$**/
		powerDown=2,	/**<Deactivates all Clocks, only asynchonous interrupts can occur**/
		powerSave=3,	/**<same as PowerDown, but activates Timer2 in async mode**/
		standby=6		/**<same as PowerSave, but faster wakeup time, then powerDown and powerSave**/
	};

	/**\brief sleep the mcu
	 * \param mode the sleep mode to use
	 *
	 * this call puts the cpu to sleep. The operation will be resumed when an
	 * interrupt occurs, however dependant on the selected sleep mode some
	 * interrupt sources may not trigger.
	 **/
	void sleep(SleepModes mode)
	{
		asm volatile
		("ldi r18, 0xF0\t\n"
		 "out 0x33, %0\t\n"
		 "sleep\t\n"
		 "out 0x33, r18\t\n"
		:
		:"r"((mode<<1)|0xF1)
		:"r18"
		);
	}
}
}
