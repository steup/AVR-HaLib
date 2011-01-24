#pragma once

namespace avr_halib
{
namespace object
{
	/**\brief Special delegate calling functions of childs
	 * \tparam parameter the parameter of the delegate call
	 *
	 * This delegate is only usefull in certain situations, especally if one wants polymorphic behaviour in classes, but without the overhead of the virtual keyword.
	 * This delegate costs exactly 2 bytes of ram and an indirect call.
	 **/
	template < typename parameter=void >
	class ThisDelegate
	{
		/**\brief Define stub to be void pointer**/
        typedef void (*invoke_stub)(void const *, parameter);
		/**\brief Storage of stub pointer**/
        volatile invoke_stub stub_ptr_;

		/**\brief Helper class to invoke stubbed function
		 * \tparam T the type of the object
		 * \tparam Fxn the type of the function
		 *
		 * This helper calls the stub pointer after casting it to the
		 * appropriate type
		 **/
        template < typename T, void (T::*Fxn)(parameter) >
        struct mem_fn_stub
		{
			/**\brief invoke the stub pointer
			 * \param obj_ptr the pointer to the object of type T
			 * \param a0 the parameter of the call to the function
			 **/
            static void invoke(void const * obj_ptr, parameter a0)
			{
                T * obj = static_cast<T *>(const_cast<void *>(obj_ptr));
                (obj->*Fxn)(a0);
            }
        };

        template < void (*Fxn)(parameter) >
        struct function_stub {
            static void invoke(void const *, parameter a0) {
                (*Fxn)(a0);
            }
        };

	protected:
		/**\brief Constructor
		 *
		 * Sets function pointer to 0
		 **/
        ThisDelegate() : stub_ptr_(0) {}

		/**\brief Call the registered function
		 * \param a0 the parameter of the call
		 **/
		void operator()(parameter a0) const
		{
            (*stub_ptr_)(this, a0);
        }

	public:
		/**\brief reset function pointer to 0**/
        void reset()
		{
            stub_ptr_ = 0;
        }

		/**\brief check if function was registered
		 * \return true if there is one, false otherwise
		 **/
        bool isEmpty()
		{
            return stub_ptr_ == 0;
        }

		/**\brief bind a new function
		 * \tparam T the type containing the function
		 * \tparam Fxn the function of type T
		 **/
        template < typename T, void (T::*Fxn)(parameter) >
        void bind()
		{
            stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
        }

		/**
         * \brief Assigns a function to this delegate object
         * \tparam Fxn Function pointer
         *
         * \see setDelegateFunction
         */
        template < void (*Fxn)(parameter) >
        void bind()
		{
            stub_ptr_ = &function_stub<Fxn>::invoke;
        }

		/**\brief overloaded cast to bool
		 * 
		 * Can be used as another way for isEmpty
		 **/
        operator bool() const
		{
            return stub_ptr_;
        }
	};

	/**\brief Special delegate calling functions of childs, version without parameters
	 *
	 * This delegate is only usefull in certain situations, especally if one wants polymorphic behaviour in classes, but without the overhead of the virtual keyword.
	 * This delegate costs exactly 2 bytes of ram and an indirect call.
	 **/
	template<>
	class ThisDelegate<void>
	{
		/**\brief Define stub to be void pointer**/
        typedef void (*invoke_stub)(void const *);
		/**\brief Storage of stub pointer**/
        volatile invoke_stub stub_ptr_;

		/**\brief Helper class to invoke stubbed function
		 * \tparam T the type of the object
		 * \tparam Fxn the type of the function
		 *
		 * This helper calls the stub pointer after casting it to the
		 * appropriate type
		 **/
        template < typename T, void (T::*Fxn)() >
        struct mem_fn_stub {
			/**\brief invoke the stub pointer
			 * \param obj_ptr the pointer to the object of type T
			 **/
            static void invoke(void const * obj_ptr)
			{
                T * obj = static_cast<T *>(const_cast<void *>(obj_ptr));
                (obj->*Fxn)();
            }
        };

		template < void (*Fxn)() >
        struct function_stub {
            static void invoke(void const *) {
                (*Fxn)();
            }
        };

	protected:
		/**\brief Constructor
		 *
		 * Sets function pointer to 0
		 **/
        ThisDelegate() : stub_ptr_(0) {}

		/**\brief Call the registered function**/
		void operator()() const
		{
            (*stub_ptr_)(this);
        }

	public:
		/**\brief reset function pointer to 0**/
        void reset()
		{
            stub_ptr_ = 0;
        }

		/**\brief check if function was registered
		 * \return true if there is one, false otherwise
		 **/
        bool isEmpty()
		{
            return stub_ptr_ == 0;
        }

		/**\brief bind a new function
		 * \tparam T the type containing the function
		 * \tparam Fxn the function of type T
		 **/
        template < typename T, void (T::*Fxn)() >
        void bind()
		{
            stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
        }

		/**
         * \brief Assigns a function to this delegate object
         * \tparam Fxn Function pointer
         *
         * \see setDelegateFunction
         */
        template < void (*Fxn)() >
        void bind()
		{
            stub_ptr_ = &function_stub<Fxn>::invoke;
        }

		/**\brief overloaded cast to bool
		 * 
		 * Can be used as another way for isEmpty
		 **/
        operator bool() const
		{
            return stub_ptr_;
        }
	};
}
}
