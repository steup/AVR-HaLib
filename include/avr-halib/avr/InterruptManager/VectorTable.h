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
 * $Id: VectorTable.h 755 2011-12-06 23:14:21Z mschulze $
 *
 ******************************************************************************/
#ifndef __VECT_TAB_HPP_0859D507E24FCE__
#define __VECT_TAB_HPP_0859D507E24FCE__

#include "avr-halib/avr/InterruptManager/VectorTableEntry.h"

#include "linker_stubs.h"


namespace Interrupt {

/*! \brief Within the vector_table function the vector table is generated by
 *         the help of the vector table entry (iterator).
 */
template <typename SlotConfig, typename _DefaultSlot, uint16_t Size>
void vector_table() __attribute__((section(".vectortable"), naked, used));
template <typename SlotConfig, typename _DefaultSlot, uint16_t Size>
void vector_table() {
    VectorTableEntry<SlotConfig, _DefaultSlot, 1, Size>::iterate();
}

// primary template
/*! \brief The VectorTable is calculate and aranged in dependence of the given
 *         SlotConfig that hold the users interrupt vector configuration
 *
 *  \tparam SlotConfig is a sequence containing slots
 *  \tparam _DefaultSlot is used for this slot if it is not given within
 *          the SlotConfig, meaning the slot is not configured by the user
 *  \tparam Size is the size of the vector table in entries
 */
template <typename SlotConfig, typename _DefaultSlot, uint16_t Size>
struct VectorTable {
    typedef VectorTable type;

    /*! \brief The Generator class template is a helper used for the
     *         instantiation of the vector_table function template. This is the
     *         reason why it is defined locally.
     */
    template< void (*T)()>
    struct Generator {
        typedef Generator type;
    };

    /*! \brief The VectorTable is generated by instantiating the vector_table
     *         function template. At this point calling this function is not an
     *         option. As the name states, it is a vector_table and not really
     *         a function because  that function consists of a kind of lookup
     *         table having a series of jmps, refering to vector service
     *         routines for each supported vector. However, instantiation is
     *         needed to enable the creation of the vector table. The taking of
     *         the address of the vector_table function template by giving it
     *         to a respective class template does the trick.
     */
    static void generate() __attribute__((always_inline)) {
        typedef Generator<vector_table<SlotConfig, _DefaultSlot, Size> >  __t;
    }
};


/*! \brief specializations of VectorTable without entries
 *
 * \copydoc VectorTableEntry
 */
template <typename SlotConfig, typename _DefaultSlot>
struct VectorTable<SlotConfig, _DefaultSlot, 0> {
    typedef VectorTable type;
    static void generate() __attribute__((always_inline)) {}
};

} /* namespace Interrupt */

#endif
