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

namespace avr_halib
{
namespace mplExt
{
	namespace helpers
	{
		template<typename T>
		struct basicPromote;

		template<>
		struct basicPromote<uint8_t>
		{
			typedef uint16_t type;
		};

		template<>
		struct basicPromote<uint16_t>
		{
			typedef uint32_t type;
		};

		template<>
		struct basicPromote<uint32_t>
		{
			typedef uint64_t type;
		};

		template<>
		struct basicPromote<uint64_t>
		{
			typedef uint64_t type;
		}; 
	}

	template<typename T>
	struct promote
	{
		private:
		enum{
			isSigned   = boost::is_signed<T>::type::value,
			isConst    = boost::is_const<T>::type::value,
			isVolatile = boost::is_volatile<T>::type::value
		};

		typedef typename boost::make_unsigned< typename boost::remove_cv< T >::type 
											  >::type BType;

		typedef typename helpers::basicPromote<BType>::type EType;

		typedef typename boost::make_signed<EType>::type SEType;

		typedef typename boost::mpl::if_c<isSigned, SEType, EType>::type CSEType;

		typedef typename boost::add_const<CSEType>::type CEType;

		typedef typename boost::mpl::if_c<isConst, CEType, CSEType>::type CCEType;

		typedef typename boost::add_volatile<CCEType>::type VEType;
	public:
		typedef typename boost::mpl::if_c<isVolatile, VEType, CCEType>::type type;
	};

}
}
