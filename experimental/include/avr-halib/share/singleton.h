#pragma once

#include <stdint.h>
#include <stddef.h>
#include <boost/type_traits/is_base_of.hpp>

namespace avr_halib
{
namespace object
{

struct SingletonType{};

/**\brief A general implementation of the singleton concept
 * \tparam T the type to declare singleton
 *
 * This template class allows one to declare another class singleton. This
 * class cannot be constructed afterwards, but must be referenced by a call to
 * getInstance(). The concept allows to create single objects of classes, that
 * exist exactly once.
 **/
template<typename T>
class Singleton : public T, public SingletonType
{

	public:
		typedef T noSingleton;

	private:
		void* operator new(size_t, void* buffer)
		{
			return buffer;
		}

	protected:
		/**\brief no constructor**/
		Singleton(){};
		/**\brief no copy constructor**/
		Singleton(const Singleton&);
	public:

		/**\brief Return an instance of this singleton class
		 * \return an instance of the class T
		 *
		 * This function will construct the object of the class T the first
		 * time it is called. Afterwards it will always return this single
		 * instance.
		 **/
		static Singleton& getInstance()
		{
			static char storage[sizeof(Singleton)];
			static bool constructed=false;
			char *ptr=storage;
			if(!constructed)
			{
				new(ptr) Singleton;
				constructed=true;
			}
			return *(reinterpret_cast<Singleton*>(ptr));
		}
};

template<typename T>
struct isSingleton
{
	typedef typename boost::is_base_of<T, SingletonType>::type type;
};

template<typename T>
struct removeSingleton
{
	typedef typename T::noSingleton type;
};

}
}
