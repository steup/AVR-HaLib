#pragma once

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib common */
namespace common
{
    /** \brief TODO \todo */
    extern "C" void emptyFunction(){}

    /** \brief A class which represents a method or function and implements anonymous callback functionality
     *
     * \tparam parameter TODO \todo
     */
    template < typename parameter = void >
    class Delegate
    {
        /** \brief TODO \todo */
        typedef void (*invoke_stub)(void const *, parameter);
        /** \brief TODO \todo */
        void const  *obj_ptr_;
        /** \brief TODO \todo */
        volatile invoke_stub stub_ptr_;

        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         * \tparam Fxn TODO \todo
         */
        template<typename T, void (T::*Fxn)(parameter)>
        struct mem_fn_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const * obj_ptr, parameter a0)
            {
                T * obj = static_cast<T *>(const_cast<void *>(obj_ptr));
                (obj->*Fxn)(a0);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         * \tparam Fxn TODO \todo
         */
        template<typename T, void (T::*Fxn)(parameter) const>
        struct mem_fn_const_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const * obj_ptr, parameter a0)
            {
                T const * obj = static_cast<T const *>(obj_ptr);
                (obj->*Fxn)(a0);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam Fxn TODO \todo
         */
        template<void (*Fxn)(parameter)>
        struct function_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const *, parameter a0)
            {
                (*Fxn)(a0);
            }
        };

        public:
            Delegate() : obj_ptr_(0), stub_ptr_(reinterpret_cast<invoke_stub>(&emptyFunction)) { }

            /** \brief TODO \todo */
            void reset()
            {
                stub_ptr_ = reinterpret_cast<invoke_stub>(&emptyFunction);
            }
            /** \brief TODO \todo */
            bool isEmpty()
            {
                return stub_ptr_ == reinterpret_cast<invoke_stub>(&emptyFunction);
            }
            /** \brief Assigns a method to this delegate object
             *
             * \tparam T Class which contains the method
             * \tparam T::*Fxn Method pointer
             * \param obj Pointer to the instance of \c T
             *
             * \see setDelegateMethod
             */
            template<typename T, void (T::*Fxn)(parameter)>
            void bind(T * obj)
            {
                obj_ptr_ = const_cast<T const *>(obj);
                stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
            }

            /** \brief TODO \todo
             *
             * \tparam T TODO \todo
             * \tparam Fxn TODO \todo
             */
            template<typename T, void (T::*Fxn)(parameter) const>
            void bind(T const * obj)
            {
                obj_ptr_ = obj;
                stub_ptr_ = &mem_fn_const_stub<T, Fxn>::invoke;
            }

            /** \brief Assigns a function to this delegate object
             *
             * \tparam Fxn Function pointer
             *
             * \see setDelegateFunction
             */
            template<void (*Fxn)(parameter)>
            void bind()
            {
                obj_ptr_ = 0;
                stub_ptr_ = &function_stub<Fxn>::invoke;
            }

            /** \brief Calls associated function or method
             *
             * \pre \ref bind,
             *      \ref setDelegateMethod or
             *      \ref setDelegateFunction have to be called before
             */
            void operator()(parameter a0)
            {
                (*stub_ptr_)(obj_ptr_, a0);
            }

            /** \brief TODO \todo */
            operator bool() const
            {
                return stub_ptr_;
            }
    };

    /** \brief TODO \todo */
    template<>
    class Delegate<void>
    {
        /** \brief TODO \todo */
        typedef void (*invoke_stub)(void const *);
        /** \brief TODO \todo */
        void const  *obj_ptr_;
        /** \brief TODO \todo */
        volatile invoke_stub stub_ptr_;

        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         * \tparam Fxn TODO \todo
         */
        template <typename T, void (T::*Fxn)()>
        struct mem_fn_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const * obj_ptr)
            {
                T * obj = static_cast<T *>(const_cast<void *>(obj_ptr));
                (obj->*Fxn)();
            }
        };

        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         * \tparam Fxn TODO \todo
         */
        template <typename T, void (T::*Fxn)() const>
        struct mem_fn_const_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const * obj_ptr)
            {
                T const * obj = static_cast<T const *>(obj_ptr);
                (obj->*Fxn)();
            }
        };

        /** \brief TODO \todo
         *
         * \tparam Fxn TODO \todo
         */
        template<void (*Fxn)()>
        struct function_stub
        {
            /** \brief TODO \todo */
            static void invoke(void const *)
            {
                (*Fxn)();
            }
        };

        public:
            Delegate() : obj_ptr_(0), stub_ptr_(reinterpret_cast<invoke_stub>(&emptyFunction)) { }

            /** \brief TODO \todo */
            void reset()
            {
                stub_ptr_ = reinterpret_cast<invoke_stub>(&emptyFunction);
            }
            /** \brief TODO \todo */
            bool isEmpty()
            {
                return stub_ptr_ == reinterpret_cast<invoke_stub>(&emptyFunction);
            }
            /** \brief Assigns a method to this delegate object
             *
             * \tparam T Class which contains the method
             * \tparam T::*Fxn Method pointer
             * \param obj Pointer to the instance of \c T
             *
             * \see setDelegateMethod
             */
            template<typename T, void (T::*Fxn)()>
            void bind(T * obj)
            {
                obj_ptr_ = const_cast<T const *>(obj);
                stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
            }

            /** \brief TODO \todo
             *
             * \tparam T TODO \todo
             * \tparam Fxn TODO \todo
             */
            template<typename T, void (T::*Fxn)() const>
            void bind(T const * obj)
            {
                obj_ptr_ = obj;
                stub_ptr_ = &mem_fn_const_stub<T, Fxn>::invoke;
            }

            /** \brief Assigns a function to this delegate object
             *
             * \tparam Fxn Function pointer
             *
             * \see setDelegateFunction
             */
            template<void (*Fxn)()>
            void bind() {
                obj_ptr_ = 0;
                stub_ptr_ = &function_stub<Fxn>::invoke;
            }

            /** \brief Calls associated function or method
             *
             * \pre \ref bind,
             *      \ref setDelegateMethod or
             *      \ref setDelegateFunction have to be called before
             */
            void operator()()
            {
                (*stub_ptr_)(obj_ptr_);
            }

            /** \brief TODO \todo */
            operator bool() const
            {
                return stub_ptr_;
            }
    };

    /** \brief Assigns a method to a delegate object
     *
     * \param delegate Delegate object
     * \param objType Class which contains the method
     * \param func Method
     * \param obj Object, instance of \c objType
     *
     * \see Delegate
     */
    #define setDelegateMethod(delegate, objType, func, obj)  __setDelegateMethod(delegate, objType, func, obj)
    #define __setDelegateMethod(delegate, objType, func, obj) delegate.bind<objType, & func>(& obj)

    /** \brief Assigns a function to a delegate object
     *
     * \param delegate Delegate object
     * \param func Function
     *
     * \see Delegate
     */
    #define setDelegateFunction(delegate, func) __setDelegateFunction(delegate, func)
    #define __setDelegateFunction(delegate, func) delegate.bind< & func>();
}
}
