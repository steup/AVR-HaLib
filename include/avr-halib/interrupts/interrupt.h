#pragma once

#include "avr-halib/interrupts/InterruptManager/InterruptManager.h"
#include "avr-halib/interrupts/InterruptManager/InterruptBinding.h"

#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/if.hpp>

inline void sei() __attribute__((always_inline));
inline void sei()
{
    asm volatile("sei");
}

inline void cli() __attribute__((always_inline));
inline void cli()
{
    asm volatile("cli");
}

namespace avr_halib
{
namespace interrupts
{
    namespace helpers
    {
        template<uint8_t intNumber>
        struct Slot : public avr_halib::interrupts::interrupt_manager::Slot<intNumber, avr_halib::interrupts::interrupt_manager::Binding::DynamicPlainFunction>{};

        template<typename IM>
        struct MultipleInterrupts
        {
            typedef typename IM::Interrupts Int;

            template<Int i, typename T, void (T::*F)(void)>
            static void registerCallback(T& obj)
            {
              Slot<i>::template bind<T, F>(&obj);
            }

            template<Int i, typename T, void (T::*F)(void)>
            static void registerCallback(const T& obj)
            {
              Slot<i>::template bind<T, F>(&obj);
            }

            template<Int i, void (*F)(void)>
            static void registerCallback()
            {
              Slot<i>::template bind<F>();
            }
        };

        template<typename IM>
        struct SingleInterrupt
        {
            template<typename T, void (T::*F)(void)>
            static void registerCallback(T& obj)
            {
                boost::mpl::front< typename IM::Slots >::type::template bind<T, F>(&obj);
            }

            template<typename T, void (T::*F)(void)>
            static void registerCallback(const T& obj)
            {
                boost::mpl::front< typename IM::Slots >::type::template bind<T, F>(&obj);
            }

            template<void (*F)(void)>
            static void registerCallback()
            {
                boost::mpl::front< typename IM::Slots >::type::template bind<F>();
            }
        };
    }

    template<typename IM>
    struct InterruptRegistration:
        public boost::mpl::if_c< boost::mpl::size< typename IM::Slots >::type::value == 1,
                helpers::SingleInterrupt<IM>,
                helpers::MultipleInterrupts<IM>
            >::type
        {};
}
}
