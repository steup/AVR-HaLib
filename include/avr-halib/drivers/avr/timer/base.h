#pragma once

#include <avr-halib/drivers/avr/timer/halSelector.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
/** Timer */
namespace timer
{
    /** \brief TODO \todo
     *
     * \tparam config Configuration
     */
    template<typename config>
    class Base : public HALSelector<config>::type
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::WaveForms WaveForms;
            /** \brief TODO \todo */
            typedef typename RegMap::ValueType ValueType;

        public:
            /** \brief TODO \todo */
            Base()
            {
                UseRegMap(rm, RegMap);
                rm.tcnt  = 0;
                rm.tifr  = 0;
                rm.cs    = RegMap::noClock;
                rm.toie  = config::overflowInt;
                rm.setWaveForm(config::waveForm);

            }

            /** \brief TODO \todo
             *
             * \param newWaveForm TODO \todo
             */
            void setWaveForm(WaveForms newWaveForm)
            {
                UseRegMap(rm, RegMap);
                rm.setWaveForm(newWaveForm);
                SyncRegMap(rm);
                this->sync();
            }

            /** \brief TODO \todo */
            WaveForms getWaveForm() const
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.getWaveForm();
            }

            /** \brief TODO \todo
             *
             * \param value TODO \todo
             */
            void setCounter(ValueType value)
            {
                UseRegMap(rm, RegMap);
                rm.tcnt=value;
                SyncRegMap(rm);
                this->sync();
            }

            /** \brief TODO \todo */
            ValueType getCounter() const
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.tcnt;
            }

            /** \brief TODO \todo
             *
             * \param value TODO \todo
             */
            static void setOverflowInt(bool value)
            {
                UseRegMap(rm, RegMap);
                rm.toie=value;
                SyncRegMap(rm);
            }

            /** \brief TODO \todo */
            bool getOverflowInt() const
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.toie;
            }

            /** \brief TODO \todo */
            void start()
            {
                UseRegMap(rm, RegMap);
                rm.cs=config::ps;
                SyncRegMap(rm);
                this->sync();
            }

            /** \brief TODO \todo */
            void stop()
            {
                UseRegMap(rm, RegMap);
                rm.cs=RegMap::noClock;
                SyncRegMap(rm);
                this->sync();
            }

            /** \brief TODO \todo */
            bool isStarted() const
            {
                UseRegMap(rm, RegMap);
                return rm.cs != RegMap::noClock;
            }
    };
}
}
}
}
