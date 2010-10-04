#ifndef BOOST_PP_IS_ITERATING

#ifndef __VECT_TAB_HPP_0859D507E24FCE__
#define __VECT_TAB_HPP_0859D507E24FCE__

#include "avr-halib/avr/InterruptManager/VectorTableEntry.h"
#include "avr-halib/avr/InterruptManager/CalculateSlotEntry.h"

// includes that are needed for preprocessor metaprogramming
#   include <boost/preprocessor/repetition.hpp>
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/arithmetic/sub.hpp>
#   include <boost/preprocessor/punctuation/comma_if.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>
#   include <boost/preprocessor/iteration/local.hpp>
#   include <boost/preprocessor/control/if.hpp>
#   include <boost/preprocessor/facilities/empty.hpp>

#   ifndef VECTTAB_MAX_SIZE
#       define VECTTAB_MAX_SIZE 40  // default maximum size is 3
#   endif

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
struct VectorTable;


// generate specializations in the form of
//
//template <typename SlotConfig, typename _DefaultSlot>
//struct VectorTable<SlotConfig, _DefaultSlot, n> {
//    static void init() {
//        typedef typename CalculateSlotEntry<SlotConfig, ::Interrupt::Slot< 1>, _DefaultSlot>::type  t1;
//        typedef typename CalculateSlotEntry<SlotConfig, ::Interrupt::Slot< 2>, _DefaultSlot>::type  t2;
//        typedef typename CalculateSlotEntry<SlotConfig, ::Interrupt::Slot< 3>, _DefaultSlot>::type  t3;
//        ....
//        static const VectEntry __attribute__((section(".vectortable"),used)) VectTable[15] = {
//            {t1::opcode,t1::value},
//            {t2::opcode,t2::value},
//            {t3::opcode,t3::value},
//             ....
//        };
//    }
//};
//
// by means of preprocessor metaprogramming
#    define BOOST_PP_ITERATION_LIMITS (1, VECTTAB_MAX_SIZE)
#    define BOOST_PP_FILENAME_1       "avr-halib/avr/InterruptManager/VectorTable.h" // this file
#    include BOOST_PP_ITERATE()

#  endif // __VECT_TAB_HPP_0859D507E24FCE__

#else // BOOST_PP_IS_ITERATING

#  define n BOOST_PP_ITERATION()


#  define VECTTAB_typedef(__S) \
    typedef typename CalculateSlotEntry<SlotConfig, ::Interrupt::Slot< __S>, _DefaultSlot>::type  BOOST_PP_CAT(t,__S);

#  define VECTTAB_entry(__S) \
    {BOOST_PP_CAT(t,__S)::opcode,BOOST_PP_CAT(t,__S)::value}BOOST_PP_COMMA_IF(BOOST_PP_SUB(n,__S))
// specialization pattern
template <typename SlotConfig, typename _DefaultSlot>
struct VectorTable<SlotConfig, _DefaultSlot, n> {
    static void init() {
#       define BOOST_PP_LOCAL_MACRO(n)  VECTTAB_typedef(n)
#       define BOOST_PP_LOCAL_LIMITS    (1, n)
#       include BOOST_PP_LOCAL_ITERATE()
        static const VectorTableEntry __attribute__((section(".vectortable"),used)) VectTable[n] = {
#       define BOOST_PP_LOCAL_MACRO(n)  VECTTAB_entry(n)
#       define BOOST_PP_LOCAL_LIMITS    (1, n)
#       include BOOST_PP_LOCAL_ITERATE()
        };
    }
};

#  undef VECTTAB_typedef
#  undef VECTTAB_entry
#  undef n

#endif // BOOST_PP_IS_ITERATING
