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
 * $Id$
 *
 ******************************************************************************/

#ifndef __VECTORTABLEENTRY_H_12400BC7EECD95__
#define __VECTORTABLEENTRY_H_12400BC7EECD95__

#include "avr-halib/avr/InterruptManager/CalculateSlotEntry.h"

namespace Interrupt {

/*! \brief Provides the structure of a vector table entry
 *
 *  The entry is implemented as an interator. It starts on vector one and ends
 *  in dependence of the given debug flag at the HighestSlotNumber or at the
 *  maximal usable vector table size. For each entry a lookup in the SlotConfig
 *  is executed. If it finds a configured slot for the current entry, it uses
 *  this otherwise it takes the default slot. Subsequently, it creates the
 *  entry and proceeds the next entry.
 */
template<typename SlotConfig, typename _DefaultSlot, int S, int E, bool b=true>
struct VectorTableEntry{
    static void iterate() __attribute__((always_inline)) {
        typedef typename CalculateSlotEntry<SlotConfig, ::Interrupt::Slot< S >, _DefaultSlot>::type  _SlotEntry;
        _SlotEntry::create();
        VectorTableEntry<SlotConfig, _DefaultSlot, S+1, E, S!=E>::iterate();
    }
};

/*! \brief specialisation of %VectorTableEntry that acts as end condition for
 *         the interation
 *
 * \copydoc VectorTableEntry
 */
template<typename SlotConfig, typename _DefaultSlot, int S, int E>
struct VectorTableEntry<SlotConfig, _DefaultSlot, S, E, false>{
    static void iterate()__attribute__((always_inline)) {}
};

} /* namespace Interrupt */

#endif // __VECTORTABLEENTRY_H_12400BC7EECD95__

