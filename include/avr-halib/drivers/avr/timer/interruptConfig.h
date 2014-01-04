#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>

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
     * \tparam TimerConfig Configuration
     */
    template<typename TimerConfig>
    struct InterruptConfig
    {
        /** \brief TODO \todo
         *
         * \tparam Int TODO \todo
         * \tparam usage TODO \todo
         */
        template<uint8_t Int, bool usage>
        struct SlotUsage
        {
            /** \brief TODO \todo */
            typedef typename avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction IntType;

            /** \brief TODO \todo */
            struct type
            {
                /** \brief TODO \todo */
                typedef typename avr_halib::interrupts::interrupt_manager::Slot<Int, IntType> Slot;
                /** \brief TODO \todo */
                static const bool use=usage;
            };
        };

        struct copyPred
        {
            /** \brief TODO \todo
             *
             * \tparam SlotUsage TODO \todo
             */
            template<typename SlotUsage>
            struct apply
            {
                /** \brief TODO \todo */
                typedef boost::mpl::bool_<SlotUsage::use> type;
            };
        };

        struct transformOp
        {
            /** \brief TODO \todo
             *
             * \tparam SlotUsage TODO \todo
             */
            template<typename SlotUsage>
            struct apply
            {
                /** \brief TODO \todo */
                typedef typename SlotUsage::Slot type;
            };
        };

        /** \brief TODO \todo */
        typedef typename SlotUsage<TimerConfig::RegMap::IntMap::overflow,
                                   TimerConfig::overflowInt
                                   >::type OverflowSlot;

        /** \brief TODO \todo */
        typedef typename boost::mpl::vector<OverflowSlot>::type SlotList;

        // typedef typename boost::mpl::copy_if<SlotList, copyPred>::type ClearedSlotList;

        /** \brief TODO \todo */
        typedef typename boost::mpl::transform</*Cleared*/SlotList, transformOp>::type type;
    };
}
}
}
}
