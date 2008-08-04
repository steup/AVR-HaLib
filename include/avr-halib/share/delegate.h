/**
 *	\file	include/avr-halib/share/delegate.h
 *	\brief	Defines class Delegate and macros setDelegateMethod and setDelegateFunction
 *
 */


#pragma once

/**
 *	\class	Delegate delegate.h "avr-halib/share/delegate.h"
 *	\brief	A class which represents a method or function
 */
class Delegate
{
	typedef void (*invoke_stub)(void const *);
	void const  *obj_ptr_;
	invoke_stub stub_ptr_;

	template<typename T, void (T::*Fxn)()>
	struct mem_fn_stub
	{
		static void invoke(void const * obj_ptr)
		{
			T * obj = static_cast<T *>( const_cast<void *>( obj_ptr ) );
			(obj->*Fxn)();
		}
	};

	template<typename T, void (T::*Fxn)() const>
	struct mem_fn_const_stub
	{
		static void invoke(void const * obj_ptr)
		{
			T const * obj = static_cast<T const *>( obj_ptr );
			(obj->*Fxn)();
		}
	};

	template<void (*Fxn)()>
	struct function_stub
	{
		static void invoke(void const *)
		{
			(*Fxn)( );
		}
	};

public:
	Delegate() : obj_ptr_(0), stub_ptr_(0) { }

	/**
	 *	\brief	Assigns a method to this delegate object
	 *	\tparam	T	Class which contains the method
	 *	\tparam	T::*Fxn	Method pointer
	 *	\param	obj	Pointer to the instance of \c T
	 *
	 *	\see setDelegateMethod
	 */
	template<typename T, void (T::*Fxn)()>
	void fromMethod(T * obj)
	{
		obj_ptr_ = const_cast<T const *>( obj );
		stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
	}

	template<typename T, void (T::*Fxn)() const>
	void fromMethod(T const * obj)
	{
		obj_ptr_ = obj;
		stub_ptr_ = &mem_fn_const_stub<T, Fxn>::invoke;
	}

	/**
	 *	\brief	Assigns a function to this delegate object
	 *	\tparam Fxn	Function pointer
	 *
	 *	\see setDelegateFunction
	 */
	template<void (*Fxn)()>
	void fromFunction()
	{
		obj_ptr_ = 0;
		stub_ptr_ = &function_stub<Fxn>::invoke;
	}

	/**
	 *	\brief	Calls associated function or method
	 *	\pre	\ref fromFunction, \ref fromMethod, \ref setDelegateMethod or \ref setDelegateFunction have to be called before
	 */
	void operator ()() const
	{
		( *stub_ptr_ )( obj_ptr_);
	}
};


/**
 *	\brief	Assigns a method to a delegate object
 *	\param delegate	Delegate object
 *	\param objType	Class which contains the method
 *	\param func	Method
 *	\param obj	Object, instance of \c objType
 *
 *	\see Delegate
 */
#define setDelegateMethod(delegate, objType, func, obj)		__setDelegateMethod(delegate, objType, func, obj)
#define __setDelegateMethod(delegate, objType, func, obj)	\
	delegate.fromMethod<objType, & func>(& obj)


/**
 *	\brief	Assigns a function to a delegate object
 *	\param delegate	Delegate object
 *	\param func	Function
 *
 *	\see Delegate
 */
#define setDelegateFunction(delegate, func)        __setDelegateFunction(delegate, func)
#define __setDelegateFunction(delegate, func)		\
	delegate.fromFunction< & func>();


