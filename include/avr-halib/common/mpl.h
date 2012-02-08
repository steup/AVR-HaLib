#pragma once

#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>

using boost::mpl::begin;
using boost::mpl::end;
using boost::mpl::next;
using boost::mpl::deref;

namespace avr_halib
{
/**\brief Extensions to boost::mpl**/
namespace mpl
{
	using namespace boost::mpl;
	/**\brief helper metafunctions**/
	namespace helpers
	{
		/**\brief for_each helper metafunction
		 * \tparam curr the current iterator of the type list
		 * \tparam end the end iterator of the type list
		 * \tparam the functor to execute
		 *
		 * This helper calls the functor on every element of the list.
		 **/
		template<typename curr, typename end, typename F>
		struct For_each
		{
			/**\brief define current type**/
			typedef For_each type;

			/**\brief next for_each metafunction to use**/
			typedef For_each<typename next<curr>::type, end, F> next;

			/**\brief execution function, calling the functor
			 * \param f an instance of the functor
			 *
			 * this function creates an instance of the current type node
			 * passes it to the functor during execution and calls the
			 * execution function of the next for_each metafunction
			 **/
			static void run(F& f)
			{
				typename deref<curr>::type node;
				f(node);
				next::run(f);
			}
		};

		/**\brief For_each recursion end condition
		 * \tparam end the itertaor pointing to the end of the type-list
		 * \tparam F the type of the functor to use
		 *
		 * This struct ends the recursive traversal of the type-list
		 **/
		template<typename end, typename F>
		struct For_each<end, end, F>
		{
			/**\brief current type**/
			typedef For_each type;

			/**\brief empty execution function
			 * \param f an instance of the functor
			 **/
			static void run(F& f){}
		};
		
		/**\brief helper metafunction, checking wether to call a functor on a node of a type list
		 * \tparam F the functor to use
		 */
		template<typename F>
		struct Caller
		{
			/**\brief Reference to the instance of the functor to use**/
			F& f;
			/**\brief the number of the node, on which the functor should be executed**/
			unsigned int i;
			/**\brief return value, if execution was successfull**/
			bool result;

			/**\brief Constructor initializing the values**/
			Caller(F &f, unsigned int i): f(f), i(i), result(false){}

			/**\brief Functor function, to be used by for_each
			 * \tparam node the current node of the type list
			 * \param n an instance of the node
			 *
			 * This function checks if this node is the one, on which the
			 * supplied functor f should be executed, if this is the case, the
			 * functor will be called with the current node as parameter,
			 * otherwise the next node is checked.
			 **/
			template<typename node>
			void operator()(node& n)
			{
				if(!i--)
				{
					f(n);
					result=true;
				}
			}
		};
	}

	/**\brief Runtime algorithm on every node of a type list
	 * \tparam sequence the type list
	 * \tparam F the functor to be executed
	 * \param f an instance of the functor
	 *
	 * This function calls the functor f on every node of the type list.
	 * Therefore the functor must provide an templated<typename T> void
	 * operator()(T& t).
	 **/
	template<typename sequence, typename F>
	void for_each(F& f)
	{
		typedef typename begin<sequence>::type start;
		typedef typename end<sequence>::type end;
		typedef typename helpers::For_each<start, end, F>::type ForEach;
		ForEach::run(f);
	}

	/**\brief Runtime algorithm on one node of a type list
	 * \tparam sequence the type list
	 * \tparam F the functor to be executed
	 * \param f an instance of the functor
	 * \param i the number of the node, the functor should be called on
	 * \return true if node was found, false otherwise
	 *
	 * This function calls the functor f on the i-th node of the type list.
	 * Therefore the functor must therefor provide an templated<typename T>
	 * void operator()(T& t).
	 **/
	template<typename sequence, typename F>
	bool call_on(F& f, unsigned int i)
	{
		helpers::Caller<F> c(f, i);
		for_each<sequence>(c);
		return c.result;
	};
}
}

/**\example mplExt.cpp
 *
 * This example shows the usage of the boost::mpl extensions, provided by this
 * library.  There is a definition of a list of types, which is parsed by a
 * for_each call and a call_on call, to print output.
 **/
