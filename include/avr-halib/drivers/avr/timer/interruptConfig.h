#pragma once

#include <avr-halib/interrupts/InterruptManager/InterruptBinding.h>
#include <avr-halib/interrupts/InterruptManager/Slot.h>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>

namespace avr_halib
{
namespace drivers
{
namespace avr
{
namespace timer
{
    template<typename TimerConfig>
    struct InterruptConfig
    {
        template<uint8_t Int, bool usage>
        struct SlotUsage
        {
            typedef typename avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction IntType;

            struct type
            {
                typedef typename avr_halib::interrupts::interrupt_manager::Slot<Int, IntType> Slot;
                static const bool use=usage;
            };
        };

        struct copyPred
        {
            template<typename SlotUsage>
            struct apply
            {
                typedef boost::mpl::bool_<SlotUsage::use> type;
            };
        };

        struct transformOp
        {
            template<typename SlotUsage>
            struct apply
            {
                typedef typename SlotUsage::Slot type;
            };
        };

        typedef typename SlotUsage<TimerConfig::RegMap::IntMap::overflow,
                                   TimerConfig::overflowInt
                                   >::type OverflowSlot;

        typedef typename boost::mpl::vector<OverflowSlot>::type SlotList;

        // typedef typename boost::mpl::copy_if<SlotList, copyPred>::type ClearedSlotList;

        typedef typename boost::mpl::transform</*Cleared*/SlotList, transformOp>::type type;
    };
}
}
}
}
