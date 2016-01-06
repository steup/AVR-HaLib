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
 * $Id: InterruptManager.h 755 2011-12-06 23:14:21Z mschulze $
 *
 ******************************************************************************/

#ifndef __INTERRUPTMANAGER_H_3D3808AB33ADEB__
#define __INTERRUPTMANAGER_H_3D3808AB33ADEB__

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/empty.hpp>
#include <avr/io.h>

#include "avr-halib/interrupts/InterruptManager/Slot.h"
#include "avr-halib/interrupts/InterruptManager/VectorTable.h"
#include "avr-halib/interrupts/InterruptManager/CalculateSlotEntry.h"
#include "avr-halib/interrupts/InterruptManager/OpcodeReti.h"

namespace avr_halib
{
namespace interrupts
{
namespace interrupt_manager
{
    /*! \brief The InterruptManager is used for initialization of the vector table
     *         and further, it provides the bind interface for the runtime
     *         replacable interrupt vectors
     *
     *  \tparam SlotConfig is a sequence containing slots
     *  \tparam debug effects how big is the vector table created and what will
     *          be default slot entry for unconfigured slots
     *  \tparam vts is the maximal vector table size of the platform, which is
     *          usually determined automatically.
     */
    template <
        typename SlotConfig=::boost::mpl::vector<>,
        bool debug = false,
        uint16_t vts = (_VECTORS_SIZE/4)-1
        > struct InterruptManager
    {
        /*! \brief less_pred is used within a boost::mpl::max_element call in
         *         order to find the highest slot number
         *
         *  \tparam A first slot
         *  \tparam B second slot
         */
        template<typename A, typename B>
        struct less_pred
        {
            static const uint32_t value = (A::number::value < B::number::value);
        };

        /*! \brief BogusSlot is used in case of an empty SlotConfig, indicating the
         *         generation of an empty vector table is needed.
         */
        struct BogusSlot
        {
            typedef BogusSlot type;
            typedef ::boost::mpl::int_<0>::type number;
        };

        /*! holds the maximum number of acquired slots.  */
        enum
        {
            HighestSlotNumber = ::boost::mpl::if_<
                ::boost::mpl::empty< SlotConfig >,
                BogusSlot,
                typename ::boost::mpl::deref<
                typename ::boost::mpl::max_element<
                SlotConfig,
                less_pred<
                ::boost::mpl::_,
                ::boost::mpl::_
                >
                >::type
                >::type
                >::type::number::value
        };

        // Check whether the highest slot number greater than the vector table can be.
        // If yes we have an error
        BOOST_MPL_ASSERT_MSG(!(HighestSlotNumber>vts), a_configured_slot_exceeds_vector_table_size, ());

        /*! \todo Check whether slots are defined more than once, which is an error
         *        needs to be implemented. transform_view with predicate than sort
         *        and unique.  the result of sort and unique must be the same if
         *        not, we have an error.
         */

        /*! \brief Calculate the default slot entry through searching the
         *         SlotConfig after a specific slot. If it is found, it will be
         *         used. Otherwise an inline vector is choosen. The inline vector
         *         is implemented as direct return from interrupt using the iret
         *         machine instruction.
         */
        typedef typename CalculateSlotEntry<
            SlotConfig,
            avr_halib::interrupts::interrupt_manager::Slot<DEFAULT_vect>,
            OpcodeReti<>
            >::type _DefaultSlot;

				typedef VectorTable<
					SlotConfig, _DefaultSlot,
					debug ? vts : HighestSlotNumber
					> VTable;

        /*! \brief Rebind a dynamic slot.
         *
         * \tparam S is the interrupt that should be bound with f
         * \tparam f is the function that is bound to the Slot S
         *
         * \todo If the slot is not dynamically bound, it will throw an error. This
         *       error message is somehow cryptic and we could provide a better
         *       one. For this we have to check if the slot is allowed to be bound
         *       dynamically and if not give the user a descriptive message
         */
        template<typename S,void (*f)()>
        static void bind()
        {
            ::boost::mpl::deref<typename FindSlot<SlotConfig,S>::type >::type::template bind<f>();
        }

        /*! \brief Rebind a dynamic slot.
         *
         * \tparam S is the interrupt that should be bound with f
         * \tparam T is class holding the member function given by f
         * \tparam f is the member function that is bound to the Slot S
         *
         * \param obj is of type T
         */
        template <typename S, typename T, void (T::*f)() >
        static void bind(T const * obj)
        {
            ::boost::mpl::deref<typename FindSlot<SlotConfig,S>::type >::type::template bind<T,f>(obj);
        }

        /*! \brief Rebind a dynamic slot.
         *
         * \tparam S is the interrupt that should be bound with f
         * \tparam T is class holding the const member function given by f
         * \tparam f is the const member function that is bound to the Slot S
         *
         * \param obj is of type T
         */
         template <typename S, typename T, void (T::*f)() const >
        static void bind(T const * obj)
        {
            ::boost::mpl::deref<typename FindSlot<SlotConfig,S>::type >::type::template bind<T,f>(obj);
        }
    };
}
}
}

#define BIND_INTERRUPTS(x) void __attribute__((section(".vectortable"), naked)) imInit() { x::VTable::generate(); }

/*!\example interruptManager.cpp
 *
 * This example shows the usage of the InterruptManager. It shows the three
 * ways an interrupt may be bound (SignalSemantic, FixedPlain,
 * DynamicPlain). For the example we choose the Timer1 with comperator A
 * and B interrupt as well as the overflow interrupt. The timer is
 * configured to be overflowed approximately each four second and the
 * comparators are set to 1/3 and 2/3 of the whole interval. Each interrupt
 * outputs its type in short by using the logging framework.
 *
 * The first interrupt is bound with SignalSemanticFunction and it prints
 * "SSF". The second one prints "FPF", meening FixedPlainFunction. The
 * third is bound DynamicPlainFunktion and it prints alternating "tick" and
 * "tack".
 *
 * How the configuration has to performed is exemplarily shown in the
 * example and also how binding and rebinding of dynamically configured
 * slots work.
 *
 * If there are questions regarding the framework contact the author.
 */

#endif // __INTERRUPTMANAGER_H_3D3808AB33ADEB__
