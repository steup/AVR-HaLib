#pragma once

#include <stdint.h>
#include <stddef.h>
#include <boost/type_traits/is_base_of.hpp>
#include <avr-halib/common/placement_new.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib common */
namespace common
{
    /** \brief TODO \todo */
    struct SingletonTag{};

    /** \brief A general implementation of the singleton concept
     * \tparam T the type to declare singleton
     *
     * This template class allows one to declare another class singleton. This
     * class cannot be constructed afterwards, but must be referenced by a call to
     * getInstance(). The concept allows to create single objects of classes, that
     * exist exactly once.
     */
    template<typename T>
    class Singleton : public T, public SingletonTag
    {
        public:
            /** \brief TODO \todo */
            typedef T noSingleton;
            /** \brief TODO \todo */
            typedef Singleton type;

        private:
            /** \brief no constructor */
            Singleton(){};
            /** \brief no copy constructor */
            Singleton(const Singleton&);
            /** \brief no assignement operator */
            Singleton& operator=(const Singleton&);

        public:

            /** \brief Return an instance of this singleton class
             *
             * \return an instance of the class Singleton<T>
             *
             * This function will construct the object of the class Singleton<T>
             * the first time it is called. Afterwards it will always return this
             * single instance. On gcc this function is declared to be put into the
             * initalization area, that is executed before main() is entered.
             * However this will only happen if the instantiation of the singleton
             * is used at all.
             *
             */
            static Singleton& getInstance() // __attribute__((constructor))
            {
                static bool guard=false;
                static uint8_t storage[sizeof(Singleton)];
                if(!guard)
                {
                    new(storage) Singleton();
                    guard=true;
                }
                return *((Singleton*)(storage));
            }
    };

    /** \brief TODO \todo
     *
     * \tparam T TODO \todo
     */
    template<typename T>
    struct isSingleton
    {
        /** \brief TODO \todo */
        typedef typename boost::is_base_of<T, SingletonTag>::type type;
    };

    /** \brief TODO \todo
     *
     * \tparam T TODO \todo
     */
    template<typename T>
    struct removeSingleton
    {
        /** \brief TODO \todo */
        typedef typename T::noSingleton type;
    };
}
}
