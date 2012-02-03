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
 * $Id: FindSlot.h 518 2010-10-04 14:00:46Z mschulze $
 *
 ******************************************************************************/

#ifndef __FINDSLOT_H_67BB41A546AC2F__
#define __FINDSLOT_H_67BB41A546AC2F__

#include <boost/mpl/find_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

/*! \brief SlotContained is a predicate used for ::boost::mpl::find_if. It
 *         evaluates to true if Slot1 and Slot2 having the same number, meaning
 *         they refer to the same slot.
 */
template<typename Slot1, typename Slot2 >
struct SlotContained {
        static const int value= ::boost::is_same<
                                    typename Slot1::number,
                                    typename Slot2::number
                                >::value;
};

/*! \brief FindSlot searchs the _Slot within the SlotConfig sequence
 *
 *  \tparam SlotConfig is a sequence containing slots
 *  \tparam _Slot is the slot that is searched for
 *
 *  \returns if found an iterator to that slot else a past end interator
 */
template<typename SlotConfig, typename _Slot>
struct FindSlot : ::boost::mpl::find_if<
                        SlotConfig,
                        SlotContained<
                            ::boost::mpl::_1,
                            _Slot
                        >
                     > {};

#endif // __FINDSLOT_H_67BB41A546AC2F__

