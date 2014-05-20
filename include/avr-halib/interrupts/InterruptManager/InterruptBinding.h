/*******************************************************************************
 *
 * Copyright (c) 2010 Michael Schulze <mschulze@ivs.cs.uni-magdeburg.de>
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
 * $Id: InterruptBinding.h 518 2010-10-04 14:00:46Z mschulze $
 *
 ******************************************************************************/

#ifndef __INTERRUPTBINDING_H_A194A0D2CA6BE6__
#define __INTERRUPTBINDING_H_A194A0D2CA6BE6__

namespace avr_halib
{
namespace interrupts
{
namespace interrupt_manager
{
    /*! \brief The struct contains identifier for describing the different
     *         interrupt bind mechanism and they are used to configure/tag
     *         the interrupt Slot.
     */
    struct Binding
    {
        /*! \brief Interrupts bound as SignalSemanticFunction has to have signal
         *         semantic. To enforce this, the macro
         *         IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION should be used.
         *
         *  This struct is also used to enforce compile time errors if a wrong
         *  function sould be bound SignalSemantic because only functions with
         *  signal semantic are allowed to be bind SignalSemantic.
         */
        struct SignalSemanticFunction;

        /*! \brief Interrupts bound as FixedPlainFunction are not allowed to have
         *         signal semantic.  A usual static function or static member
         *         function can be bound and the way how it is activated ensures
         *         the signal semantic.
         */
        struct FixedPlainFunction;

        /*! \brief Interrupts bound as DynamicPlainFunction are not allowed to have
         *         signal semantic. All function types like plain functions, static
         *         function, static member function, member functions or const
         *         member function can be bound and the way how the bounded entity
         *         is activated ensures the signal semantic.
         */
        struct DynamicPlainFunction;

        /*! \brief Invalid is used as default parameter of the Slot struct */
        struct Invalid;
    };
}
}
}

#endif // __INTERRUPTBINDING_H_A194A0D2CA6BE6__
