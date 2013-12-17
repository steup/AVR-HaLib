#pragma once

#include <avr-halib/common/mpl.h>
#include <boost/mpl/list.hpp>

namespace avr_halib
{
namespace power
{
    struct Morpheus
    {
        private:
            struct DoSync
            {
                template<typename Syncer>
                void operator()(Syncer& s)
                {
                    s.sync();
                }
            };

        public:
            struct DefaultConfig
            {
                typedef boost::mpl::list<> SyncList;
            };

            struct SleepModes
            {
                /**\brief Possible sleep modes of the AVR
                 **/
                enum SleepModeType
                {
                    idle=0,      /**<Deactivates \f$clk_{CPU}\f$ and \f$clk_{Flash}\f$**/
                    noiseReduce=1,  /**<Deactivates \f$clk_{CPU},clk_{Flash},clk_{IO}\f$, reduces noise for ADC$**/
                    powerDown=2,  /**<Deactivates all Clocks, only asynchonous interrupts can occur**/
                    powerSave=3,  /**<same as PowerDown, but activates Timer2 in async mode**/
                    standby=6    /**<same as PowerSave, but faster wakeup time, then powerDown and powerSave**/
                };
            };

            typedef SleepModes::SleepModeType SleepModeType;

            template<typename Config = DefaultConfig>
            struct configure
            {
                struct type
                {
                    typedef avr_halib::power::Morpheus::SleepModes    SleepModes;
                    typedef avr_halib::power::Morpheus::SleepModeType SleepModeType;

                    /**\brief sleep the mcu
                     * \param mode the sleep mode to use
                     *
                     * this call puts the cpu to sleep. The operation will be resumed when an
                     * interrupt occurs, however dependant on the selected sleep mode some
                     * interrupt sources may not trigger.
                     **/
                    static void sleep(SleepModeType mode)
                    {
                        if(mode > SleepModes::idle)
                        {
                            DoSync sync;
                            mpl::for_each<typename Config::SyncList>(sync);
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

                    template<SleepModeType mode>
                    static void sleep()
                    {
                        sleep(mode);
                    }
                };
            };
    };
}
}
