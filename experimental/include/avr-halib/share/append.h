#pragma once

#include <boost/mpl/end.hpp>
#include <boost/mpl/insert_range.hpp>

namespace avr_halib
{
namespace mplExt
{
	using namespace boost::mpl;

	template<typename ListA, typename ListB>
	struct append
	{
		private:
			typedef typename end<ListA>::type ListAEndIterator;
		public:
			typedef typename insert_range<ListA, ListAEndIterator, ListB>::type type;
	};
}
}
