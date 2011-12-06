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

#ifndef __CALCULATESLOTENTRY_H_CC16286CEF1E3B__
#define __CALCULATESLOTENTRY_H_CC16286CEF1E3B__

#include "avr-halib/avr/InterruptManager/OpcodeJmp.h"
#include "avr-halib/avr/InterruptManager/FindSlot.h"

#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>

namespace Interrupt {

/*! \brief CalculateSlotEntry calculates the used opcode and target for the
 *         entry
 *
 *  \tparam SlotConfig is a sequence containing slots
 *  \tparam _Slot is the slot for which the opcode is determined
 *  \tparam _DefaultSlot is used for this slot if it is not given within the
 *          SlotConfig, meaning the slot is not configured by the user
 */
template<typename SlotConfig, typename _Slot, typename _DefaultSlot>
struct CalculateSlotEntry {
    // search for the slot
    typedef typename FindSlot<SlotConfig, _Slot >::type __t;

    // in dependence of the search result the opcode is determined
    typedef typename ::boost::mpl::eval_if<
                         ::boost::is_same<
                             __t,
                             typename ::boost::mpl::end<SlotConfig>::type
                         >,
                         _DefaultSlot,
                         OpcodeJmp<
                             typename ::boost::mpl::deref<__t>::type
                         >
                     >::type type;
};

} /* namespace Interrupt */

#endif // __CALCULATESLOTENTRY_H_CC16286CEF1E3B__

