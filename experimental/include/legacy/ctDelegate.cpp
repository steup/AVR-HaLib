#include "src/config.h"

#include <halib_ext/ctDelegate.h>

using halib_ext::object::CTDelegate;

typedef CTDelegate<int> ThisDelegate;

struct A
{
	void getName(int in)
	{
		log::emit() << "Member " << in++ << log::endl;
	}
};

struct B
{
	void getName(int in) const
	{
		log::emit() << "Const Member " << in++ << log::endl;
	}
};

struct C
{
	static void getName(int in)
	{
		log::emit() << "Static " << in++ << log::endl;
	}
};

A a;
B b;
ThisDelegate::Bind<A, &A::getName> c;
ThisDelegate::ConstBind<B, &B::getName> d;
ThisDelegate::StaticBind<&C::getName> e;

int main()
{
	c.bind(a);
	d.bind(b);
	c(10);
	d(11);
	e(12);
	return 0;
}
