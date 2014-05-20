#pragma once

#include <avr-halib/interrupts/interrupt.h>

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
     * \tparam IntMap TODO \todo
     */
    template<typename IntMap>
    struct Interrupts
    {
        /** \brief TODO \todo
         *
         * \tparam i TODO \todo
         * \param obj TODO \todo
         */
        template<typename IntMap::Interrupts i, typename T, void (T::*F)(void)>
        void registerCallback(T& obj)
        {
            interrupts::Interrupt<IntMap>::template setInt<i, T, F>(obj);
        }
    };
}
}
}
}
