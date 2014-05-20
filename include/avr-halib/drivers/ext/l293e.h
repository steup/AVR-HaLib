#pragma once

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** External-Drivers */
namespace ext
{
    /** \brief TODO \todo
     *
     * \tparam PortMap TODO \todo
     * \tparam inverted TODO \todo
     */
    template<typename PortMap, bool inverted=false>
    class L293E
    {
        /** \brief TODO \todo */
        static const uint8_t ddrMask=0x3;

        public:
            /** \brief TODO \todo */
            enum States
            {
                freeRunning=0,
                forward=(inverted?2:1),
                backward=(inverted?1:2),
                halt=3
            };

            /** \brief TODO \todo */
            L293E()
            {
                UsePortmap(pm, PortMap);
                pm.state.setDdr(ddrMask);
                SyncPortmap(pm);
            }

            /** \brief TODO \todo
             *
             * \param state TODO \todo
             */
            void state(States state)
            {
                UsePortmap(pm, PortMap);
                pm.state.setPort(state);
                SyncPortmap(pm);
            }

            /** \brief TODO \todo */
            States state() const
            {
                UsePortmap(pm, PortMap);
                SyncPortmap(pm);
                return (States)pm.state.getPort();
            }
    };
}
}
}
