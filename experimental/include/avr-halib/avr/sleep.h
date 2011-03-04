#pragma once

#include <avr/sleep.h>
#include <avr-halib/share/mplExt.h>
#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/share/delay.h>

namespace avr_halib
{
namespace power
{
	template<typename RegMap>
	struct UartSleepSynchronizer
	{
		void sync()
		{
			delay_us(12*1000000ULL/RegMap::baudrate);
		}
	};

	struct DoSync
	{
		template<typename Syncer>
		void operator()(Syncer& s)
		{
			s.sync();
		}
	};

	typedef boost::mpl::list<> EmptySyncList;

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

	template<typename SyncList=EmptySyncList>
	struct Morpheus
	{
		/**\brief sleep the mcu
		 * \param mode the sleep mode to use
		 *
		 * this call puts the cpu to sleep. The operation will be resumed when an
		 * interrupt occurs, however dependant on the selected sleep mode some
		 * interrupt sources may not trigger.
		 **/
		
		static void sleep(SleepModes mode)
		{
			if(mode>idle)
			{
				DoSync sync;
				mplExt::for_each<SyncList>(sync);
			}

			asm volatile
			("in r0, 0x3f\n\t"
			 "push r0\n\t"
			 "sei\n\t"
			 "ldi r18, 0xF0\t\n"
			 "out 0x33, %0\t\n"
			 "sleep\t\n"
			 "out 0x33, r18\t\n"
			 "pop r0\n\t"
			 "out 0x3f, r0"
			:
			:"r"((mode<<1)|0xF1)
			:"r18", "r0"
			);
		}

		template<SleepModes mode>
		static void sleep()
		{
			Morpheus::sleep(mode);
		}
	};
}
}
