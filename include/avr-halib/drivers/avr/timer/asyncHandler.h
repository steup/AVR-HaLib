#pragma once

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
     * \tparam RegMap
     * \tparam asyncEnabled
     */
    template<typename RegMap, bool asyncEnabled>
    struct AsyncHandler;

    /** \brief TODO \todo
     *
     * \tparam config Configuration
     */
    template<typename config>
    struct AsyncHandler<config, true>
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;

        protected:
            /** \brief TODO \todo */
            AsyncHandler()
            {
                UseRegMap(rm, RegMap);
                rm.as=!config::asyncExtClock;
                rm.extclk=config::asyncExtClock;
            }

            /** \brief TODO \todo */
            void sync() const
            {
                UseRegMap(rm, RegMap);
                while(rm.tcrub || rm.tcnub)
                    SyncRegMap(rm);
            }
    };

    /** \brief TODO \todo
     *
     * \tparam config Configuration
     */
    template<typename config>
    struct AsyncHandler<config, false>
    {
        protected:
            /** \brief TODO \todo */
            void sync(){}
    };
}
}
}
}
