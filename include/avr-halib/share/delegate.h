/**
 *	\file	include/avr-halib/share/delegate.h
 *	\brief	Defines class Delegate and macros setDelegateMethod and setDelegateFunction
 *
 */


#pragma once

/**
 *	\class	Delegate delegate.h "avr-halib/share/delegate.h"
 *	\brief	A class which represents a method or function
 *
 *
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
	 *	\param	T	Class of the method
	 *	\param	T::*Fxn	Method
	 *	\param	obj	Pointer the instance of the object
	 *
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

	template<void (*Fxn)()>
	void fromFunction()
	{
		obj_ptr_ = 0;
		stub_ptr_ = &function_stub<Fxn>::invoke;
	}

	void operator ()() const
	{
		( *stub_ptr_ )( obj_ptr_);
	}
};

#define setDelegateMethod(delegate, objType, func, obj)		__setDelegateMethod(delegate, objType, func, obj)
#define __setDelegateMethod(delegate, objType, func, obj)	\
	delegate.fromMethod<objType, & func>(& obj)


#define setDelegateFunction(delegate, func)        __setDelegateFunction(delegate, func)
#define __setDelegateFunction(delegate, func)		\
	delegate.fromFunction< & func>();


