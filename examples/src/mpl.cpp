#include "config.h"

#include <avr-halib/common/mpl.h>

#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>

using avr_halib::mpl::for_each;
using avr_halib::mpl::call_on;
using boost::mpl::list;
using boost::mpl::size;

struct A
{
	enum{ name='A' };
};

struct B
{
	enum{ name='B' };
};

struct C
{
	enum{ name='C' };
};

typedef list<A, B, C>::type List;

enum{ listSize = size< List >::value };

struct Printer
{
	template<typename Node>
	void operator()(Node& n)
	{
		log::emit() << (char)Node::name << " ";
	}
};

int main()
{
	Printer p;

	for_each<List>(p);
	log::emit() << log::endl;
	while(true)
	{
		for(unsigned int i=0;i<listSize;i++)
			if(call_on<List>(p, listSize-i))
				log::emit() << "X" << log::endl;
			else
				log::emit() << "O" << log::endl;
	}

	return 0;
}
