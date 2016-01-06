#pragma once

#include <avr-halib/regmaps/local.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
    /** AVR-Driver-Helpers */
    namespace helpers
    {
        /** \brief ExtIntBase class
        *
        * TODO \todo
        */
        template<typename ExtIntRegMap, typename ExtIntRegMap::Types type>
        class ExtIntBase
        {
            public:
                /** \brief TODO \todo */
                typedef typename ExtIntRegMap::InterruptMap InterruptMap;

            protected:
                /** \brief TODO \todo */
                typedef ExtIntRegMap RegMap;

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                ExtIntBase()
                {
                    UseRegMap(rm, RegMap);
                    rm.flag =false;
                    rm.mask =true;
                    rm.type =type;
                    SyncRegMap(rm);
                }

            public:
                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                bool getState() const
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    return rm.flag;
                }

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                void enable()
                {
                    UseRegMap(rm, RegMap);
                    rm.mask =true;
                    SyncRegMap(rm);
                }

                /** \brief TODO \todo
                 *
                 * TODO \todo
                 */
                void disable()
                {
                    UseRegMap(rm, RegMap);
                    rm.mask =false;
                    SyncRegMap(rm);
                }
        };
    }

    /** \brief ExternalInterrupt class
     *
     * TODO \todo
     */
    template<typename ExtIntRegMap, typename ExtIntRegMap::Sensitivity sens>
    class ExternalInterrupt
    {
        /** \brief TODO \todo */
        typedef ExtIntRegMap RegMap;

        public:
						typedef typename ExtIntRegMap::InterruptMap InterruptMap;

            ExternalInterrupt()
            {
                this->setSensitivity(sens);
            }

            /** \brief TODO \todo
             *
             * \param value TODO \todo
             */
            void setSensitivity(typename RegMap::Sensitivity value)
            {
                UseRegMap(rm, RegMap);
                rm.sensitivity =value;
                SyncRegMap(rm);
            }

            /** \brief TODO \todo
             *
             * \param value TODO \todo
             */
            void setPullUp(bool value)
            {
                UseRegMap(rm, RegMap);
                rm.trigger =value;
                SyncRegMap(rm);
            }
    };

    /** \brief SoftwareInterrupt class
     *
     * TODO \todo
     */
    template<typename ExtIntRegMap>
    class SoftwareInterrupt
    {
        /** \brief TODO \todo */
        typedef ExtIntRegMap RegMap;

        public:
						typedef typename ExtIntRegMap::InterruptMap InterruptMap;

            SoftwareInterrupt()
            {
                this->setSensitivity(RegMap::rising);
            }

            /** \brief TODO \todo
             *
             * TODO \todo
             */
            void trigger()
            {
                UseRegMap(rm, RegMap);
                rm.trigger =true;
                SyncRegMap(rm);
                rm.trigger =false;
                SyncRegMap(rm);
            }
    };
}
}
}
