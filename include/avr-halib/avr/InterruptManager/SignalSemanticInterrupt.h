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
 * $Id: SignalSemanticInterrupt.h 518 2010-10-04 14:00:46Z mschulze $
 *
 ******************************************************************************/

#ifndef __SIGNALSEMANTICINTERRUPT_H_B93A6568892B59__
#define __SIGNALSEMANTICINTERRUPT_H_B93A6568892B59__

#define __STRINGIFICATION__(x) #x
#define __TOSTR__(x) __STRINGIFICATION__(x)

#include "avr-halib/avr/InterruptManager/InterruptBinding.h"

#define IMPLEMENT_INTERRUPT_SIGNALSEMANTIC_FUNCTION(name) \
    extern "C" void __vector_ ## name (::Interrupt::Binding::SignalSemanticFunction*) __attribute__((signal, used, externally_visible)); \
    void name (::Interrupt::Binding::SignalSemanticFunction*) __attribute__((alias(__TOSTR__(__vector_ ## name)))); \
    extern "C" void __vector_ ## name (::Interrupt::Binding::SignalSemanticFunction*)


#endif // __SIGNALSEMANTICINTERRUPT_H_B93A6568892B59__

