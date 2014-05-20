#pragma once

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_volatile.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/mpl/if.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** Extensions to boost::mpl */
namespace mplExt
{
    /** Extensions to boost::mpl */
    namespace helpers
    {
        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         */
        template<typename T>
        struct basicPromote;

        /** \brief TODO \todo */
        template<>
        struct basicPromote<uint8_t>
        {
            /** \brief TODO \todo */
            typedef uint16_t type;
        };

        /** \brief TODO \todo */
        template<>
        struct basicPromote<uint16_t>
        {
            /** \brief TODO \todo */
            typedef uint32_t type;
        };

        /** \brief TODO \todo */
        template<>
        struct basicPromote<uint32_t>
        {
            /** \brief TODO \todo */
            typedef uint64_t type;
        };

        /** \brief TODO \todo */
        template<>
        struct basicPromote<uint64_t>
        {
            /** \brief TODO \todo */
            typedef uint64_t type;
        };
    }

    /** \brief TODO \todo
     *
     * \tparam T TODO \todo
     */
    template<typename T>
    struct promote
    {
        private:
            /** \brief TODO \todo */
            enum
            {
              isSigned   = boost::is_signed<T>::type::value,
              isConst    = boost::is_const<T>::type::value,
              isVolatile = boost::is_volatile<T>::type::value
            };

            /** \brief TODO \todo */
            typedef typename boost::make_unsigned< typename boost::remove_cv< T >::type
                                >::type BType;

            /** \brief TODO \todo */
            typedef typename helpers::basicPromote<BType>::type EType;

            /** \brief TODO \todo */
            typedef typename boost::make_signed<EType>::type SEType;

            /** \brief TODO \todo */
            typedef typename boost::mpl::if_c<isSigned, SEType, EType>::type CSEType;

            /** \brief TODO \todo */
            typedef typename boost::add_const<CSEType>::type CEType;

            /** \brief TODO \todo */
            typedef typename boost::mpl::if_c<isConst, CEType, CSEType>::type CCEType;

            /** \brief TODO \todo */
            typedef typename boost::add_volatile<CCEType>::type VEType;

        public:
            /** \brief TODO \todo */
            typedef typename boost::mpl::if_c<isVolatile, VEType, CCEType>::type type;
    };
}
}
