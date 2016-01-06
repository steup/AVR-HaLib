/*******************************************************************************
 *
 * Copyright (c) 2010-2011 Michael Schulze <mschulze@ivs.cs.uni-magdeburg.de>
 * All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions
 *    are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *
 *    * Neither the name of the copyright holders nor the names of
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id: Slot.h 755 2011-12-06 23:14:21Z mschulze $
 *
 ******************************************************************************/

#ifndef __SLOT_H_2E2122A1762A14__
#define __SLOT_H_2E2122A1762A14__

#include "avr-halib/interrupts/InterruptManager/InterruptBinding.h"
#include <boost/mpl/int.hpp>
#include <stdint.h>

#define DEFAULT_vect -1

namespace avr_halib
{
namespace interrupts
{
namespace interrupt_manager
{
    // definition of a function pointer that has to have signal semantic
    typedef void (*fnc_ptr_signal_sematic)();

    // definition of a function pointer that has usual function semantic
    typedef void (*trampoline_ptr)();
    typedef trampoline_ptr  fnc_ptr;

    /*! \brief The Slot struct is used to bound an interrupt function to a specific
     *         interrupt vector with the number given as template argument.
     *
     * \tparam nr the interrupt vector number on which the interrupt function
     *         is bound
     * \tparam bindType describes how the bind should work e.g. static or
     *         dynamic \ref ::Interrupt::Binding
     */
    template <
        uint16_t nr,
        typename bindType=avr_halib::interrupts::interrupt_manager::Binding::Invalid
        >
    struct Slot
    {
        typedef Slot type;
        typedef typename ::boost::mpl::int_<nr>::type number;
        typedef bindType   bindTag;
    };

    //! @cond Doxygen_Suppress
    /*! \brief specialisation of %Slot
     * \copydoc Slot
     */
    template<uint16_t nr>
    struct Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::SignalSemanticFunction>
    {
        typedef Slot type;

        /*! \brief Interrupt binding mechanism for non calling plain interrupt
         *         functions, having signal semantic.
         *
         *  It can also be used for interrupt functions which call internally other
         *  function, however, in this case the compiler generates additional code.
         *  If it is used multiple times with such kind of interrupts, the code
         *  size increase due to multiple generation of such additional code. This
         *  can be avoided using FixedPlainFunction that uses a more sophisticate and
         *  elegant mechanism
         */
        template<
            void (*f)()
            >
        struct Bind
        {
            typedef Bind type;
            typedef typename ::boost::mpl::int_<nr>::type number;
            typedef avr_halib::interrupts::interrupt_manager::Binding::SignalSemanticFunction bindTag;

            static const fnc_ptr_signal_sematic target;
        };
    };

    template<uint16_t nr>
    template<
        void (*f)()
        > const fnc_ptr_signal_sematic Slot <
        nr,
        avr_halib::interrupts::interrupt_manager::Binding::SignalSemanticFunction
        >::Bind<f>::template target=f;

    /*! \brief specialisation of %Slot
     * \copydoc Slot
     */
    template<uint16_t nr>
    struct Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction>
    {
        typedef Slot type;

        /*! \brief Interrupt binding mechanism for interrupt functions, having
         *         non signal semantic and are allowed to call other function
         *         internally.
         */
        template<void (*f)()>
        struct Bind
        {
            typedef Bind type;
            typedef typename ::boost::mpl::int_<nr>::type number;
            typedef avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction bindTag;

            static const trampoline_ptr target;

        private:
            static void trampoline() __attribute__((naked))
            {
                asm volatile (
                    "push r31       \n"
                    "push r30       \n"
                    "ldi r30, lo8(%[Function])\n"
                    "ldi r31, hi8(%[Function])\n"
                    "jmp redir_func     \n"
                    :
                    : [Function] "i" (f)
                );
            }
        };

    };

    template<uint16_t nr>
    template<void (*f)()>
    const trampoline_ptr
    Slot< nr, avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction>::Bind<f>::
    target=&Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::FixedPlainFunction>::Bind<f>::trampoline;

/*! \brief specialisation of %Slot
     * \copydoc Slot
     */
    template<uint16_t nr>
    struct Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::SingletonFunction>
    {
        typedef Slot type;

        /*! \brief Interrupt binding mechanism for interrupt functions, having
         *         non signal semantic and are allowed to call other function
         *         internally.
         */
        template<typename T, void (T::*f)(), T** t>
        struct Bind
        {
            typedef Bind type;
            typedef typename ::boost::mpl::int_<nr>::type number;
            typedef avr_halib::interrupts::interrupt_manager::Binding::SingletonFunction bindTag;

            static const trampoline_ptr target;

        private:
						static void forward() {
							if(*t)
							((*t)->*f)();
						}

            static void trampoline() __attribute__((naked))
            {
                asm volatile (
                    "push r31       \n"
                    "push r30       \n"
                    "ldi r30, lo8(%[Function])\n"
                    "ldi r31, hi8(%[Function])\n"
                    "jmp redir_func     \n"
                    :
                    : [Function] "i" (forward)
                );
            }
        };

    };

    template<uint16_t nr>
    template<typename T, void (T::*f)(), T** t>
    const trampoline_ptr
    Slot< nr, avr_halib::interrupts::interrupt_manager::Binding::SingletonFunction>::Bind<T, f, t>::
    target=&Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::SingletonFunction>::Bind<T, f, t>::trampoline;

/*! \brief specialisation of %Slot
     * \copydoc Slot
     */
    template<uint16_t nr>
    struct Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::FixedObjectFunction>
    {
        typedef Slot type;

        /*! \brief Interrupt binding mechanism for interrupt functions, having
         *         non signal semantic and are allowed to call other function
         *         internally.
         */
        template<typename T, void (T::*f)(), T& t>
        struct Bind
        {
            typedef Bind type;
            typedef typename ::boost::mpl::int_<nr>::type number;
            typedef avr_halib::interrupts::interrupt_manager::Binding::FixedObjectFunction bindTag;

            static const trampoline_ptr target;

        private:
						static void forward() {
							(t.*f)();
						}

            static void trampoline() __attribute__((naked))
            {
                asm volatile (
                    "push r31       \n"
                    "push r30       \n"
                    "ldi r30, lo8(%[Function])\n"
                    "ldi r31, hi8(%[Function])\n"
                    "jmp redir_func     \n"
                    :
                    : [Function] "i" (forward)
                );
            }
        };

    };

    template<uint16_t nr>
    template<typename T, void (T::*f)(), T& t>
    const trampoline_ptr
    Slot< nr, avr_halib::interrupts::interrupt_manager::Binding::FixedObjectFunction>::Bind<T, f, t>::
    target=&Slot <nr, avr_halib::interrupts::interrupt_manager::Binding::FixedObjectFunction>::Bind<T, f, t>::trampoline;

    //! @endcond

    /*! \brief Transform a function pointer to a type holding it, providing the
     *         possibility to forward it to other types as template parameter.
     */
    template<fnc_ptr f>
    struct Function
    {
        static const fnc_ptr target;
    };
    template<fnc_ptr f>
    const fnc_ptr Function<f>::target=f;

    /*! \brief The DefaultSlot struct is used to bind an interrupt function to all
     *         not specified interrupt vectors of the interrupt config
     *
     * \tparam bindType describes how the bind should work e.g. static or
     *         dynamic \ref avr_halib::interrupts::interrupt_manager::Binding
     */
    template <
        typename bindType=avr_halib::interrupts::interrupt_manager::Binding::Invalid
        >
    struct DefaultSlot : Slot<DEFAULT_vect, bindType> {};
}
}
}

#endif // __SLOT_H_2E2122A1762A14__
