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

#include "avr-halib/avr/InterruptManager/InterruptBinding.h"
#include <avr-halib/common/delegate.h>
#include <boost/mpl/int.hpp>
#include <stdint.h>

#define DEFAULT_vect    -1

namespace Interrupt {

// definition of a function pointer that has to have signal semantic
typedef void (*fnc_ptr_signal_sematic)(::Interrupt::Binding::SignalSemanticFunction*);

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
    typename bindType=::Interrupt::Binding::Invalid
>
struct Slot {
    typedef Slot type;
    typedef typename ::boost::mpl::int_<nr>::type number;
    typedef bindType   bindTag;
};

/*! \brief specialisation of %Slot
 * \copydoc Slot
 */
template<uint16_t nr>
struct Slot <nr, ::Interrupt::Binding::SignalSemanticFunction> {
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
        void (*f)(::Interrupt::Binding::SignalSemanticFunction*)
    >
    struct Bind {
        typedef Bind type;
        typedef typename ::boost::mpl::int_<nr>::type number;
        typedef ::Interrupt::Binding::SignalSemanticFunction bindTag;

        static const fnc_ptr_signal_sematic target;
    };
};
template<uint16_t nr>
template<
    void (*f)(::Interrupt::Binding::SignalSemanticFunction*)
> const fnc_ptr_signal_sematic Slot <
    nr,
    ::Interrupt::Binding::SignalSemanticFunction
  >::Bind<f>::template target=f;

/*! \brief specialisation of %Slot
 * \copydoc Slot
 */
template<uint16_t nr>
struct Slot <nr, ::Interrupt::Binding::FixedPlainFunction> {
    typedef Slot type;

    /*! \brief Interrupt binding mechanism for interrupt functions, having
     *         non signal semantic and are allowed to call other function
     *         internally.
     */
    template<typename StaticCallback>
    struct setCallback {
        typedef setCallback type;
        typedef typename ::boost::mpl::int_<nr>::type number;
        typedef ::Interrupt::Binding::FixedPlainFunction bindTag;

        static const trampoline_ptr target;
      private:
        static void trampoline() __attribute__((naked)) {
            asm volatile (
                "push r31       \n"
                "push r30       \n"
                "ldi r30, lo8(%[Function])\n"
                "ldi r31, hi8(%[Function])\n"
                "jmp redir_func     \n"
                :
                : [Function] "i" (StaticCallback::invoke)
            );
        }
    };

};

template<uint16_t nr>
template<typename StaticCallback>
const trampoline_ptr
Slot< nr, ::Interrupt::Binding::FixedPlainFunction>::setCallback<StaticCallback>::
target=&Slot <nr, ::Interrupt::Binding::FixedPlainFunction>::setCallback<StaticCallback>::trampoline;

/*! \brief specialisation of %Slot
 * \copydoc Slot
 */
template<uint16_t nr>
struct Slot <nr, ::Interrupt::Binding::DynamicPlainFunction> {
    typedef Slot type;
    typedef Delegate<void> CallbackType;
    typedef typename ::boost::mpl::int_<nr>::type number;
    typedef ::Interrupt::Binding::DynamicPlainFunction bindTag;

    /*! \brief Interrupt binding mechanism for interrupt functions, having non
     *         signal semantic and are allowed to call other function
     *         internally, furtermore it allows for binding and rebinding at
     *         run-time.
     */
    typedef Slot    Bind;
    static const trampoline_ptr target;

    static char delegate[sizeof(CallbackType)];
private:
    static void trampoline() __attribute__((naked))
    {
        Delegate<void>* d = reinterpret_cast<CallbackType*>(delegate);
        asm volatile (
            "push r31       \n"
            "push r30       \n"
            "lds r30, %[Function]\n"
            "lds r31, %[Function]+1\n"
            "jmp redir_func     \n"
            :
            : [Function] "m" (*d->stub_ptr_)
        );
    }
public:
    static void setCallback(const CallbackType& cb)
    {
        *reinterpret_cast<CallbackType*>(delegate) = cb;
    }

    static const CallbackType& getCallback()
    {
        return *reinterpret_cast<CallbackType*>(delegate);
    }

    static bool isEmpty()
    {
        return reinterpret_cast<CallbackType*>(delegate)->isEmpty();
    }

    static void reset()
    {
        return reinterpret_cast<CallbackType*>(delegate)->reset();
    }

};
template<uint16_t nr>
const trampoline_ptr
Slot< nr, ::Interrupt::Binding::DynamicPlainFunction>::Bind::
target=&Slot <nr, ::Interrupt::Binding::DynamicPlainFunction>::Bind::trampoline;

template<uint16_t nr>
char Slot< nr, ::Interrupt::Binding::DynamicPlainFunction>::delegate[sizeof(Slot<nr, ::Interrupt::Binding::DynamicPlainFunction>::CallbackType)];

/*! \brief The DefaultSlot struct is used to bind an interrupt function to all
 *         not specified interrupt vectors of the interrupt config
 *
 * \tparam bindType describes how the bind should work e.g. static or
 *         dynamic \ref ::Interrupt::Binding
 */
template <
    typename bindType=::Interrupt::Binding::Invalid
>
struct DefaultSlot : Slot<DEFAULT_vect, bindType> {};


} /* namespace Interrupt */


#endif // __SLOT_H_2E2122A1762A14__
