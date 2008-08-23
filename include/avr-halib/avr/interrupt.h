/** \addtogroup avr */
/*@{*/
/**
 *	\file	include/avr-halib/avr/interrupt.h
 *	\brief	Defines macros #UseInterrupt, #redirectISRM and #redirectISRF
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 *
 *	\example isr.cpp
 */

#pragma once


#include <avr/interrupt.h>


// defining of redirection memory variables and 
// the redirection stub for the certain vector

// extern "C" void _redir_func();

// defines interrupt class (delegate)
#define DefineInterrupt(X)			__DefineInterrupt(X)

#define __DefineInterrupt(X)						\
	class X##_REDIR {								\
	public:								\
	typedef void (*invoke_stub)();					\
	static void const	*obj_ptr;					\
	static invoke_stub stub_ptr;					\
									\
	template<void (*Fxn)()>						\
		static void from_function()						\
		{									\
	obj_ptr = 0;							\
	stub_ptr = Fxn;							\
		}									\
									\
	template<typename T, void (T::*Fxn)()>				\
		struct mem_fn_stub						\
		{									\
	static void invoke()						\
	{								\
		T * obj = static_cast<T *>( const_cast<void *>(obj_ptr ) );	\
		(obj->*Fxn)();						\
	}								\
		};								\
									\
	template<typename T, void (T::*Fxn)() const>			\
		struct mem_fn_const_stub						\
		{									\
	static void invoke()						\
	{								\
		T const * obj = static_cast<T const *>(obj_ptr );		\
		(obj->*Fxn)();						\
	}								\
		};								\
									\
	template<typename T, void (T::*Fxn)()>				\
		static void from_function(T * obj)					\
		{									\
	obj_ptr = const_cast<T const *>(obj );				\
	stub_ptr = &mem_fn_stub<T, Fxn>::invoke;			\
		}									\
									\
	template<typename T, void (T::*Fxn)() const>			\
		static void from_function(T const * obj)					\
		{									\
	obj_ptr = obj;							\
	stub_ptr = &mem_fn_const_stub<T, Fxn>::invoke;			\
		}									\
									\
	}


/**
 *	\brief	Macro that generates ISR delegate storage
 *	\param	X	Interrupt vector symbol
 *
 *	Use this macro exactly once for every Interrupt you use in your source code (whole project).
 *
 *	\see	\ref doc_interrupts, redirectISRF, redirectISRM
 */
#define UseInterrupt(X)			__UseInterrupt(X)

 //mögliche lösung für Use Interupt problem weak obj_ptr symbol

#define __UseInterrupt(X)				\
	void const	*X##_REDIR::obj_ptr;			\
	void (*X##_REDIR::stub_ptr)();\
extern "C" void X (void) __attribute__ ((naked)); 	\
extern "C" void X (void) {				\
		asm volatile (				\
			"push r31 		\n"	\
			"push r30 		\n"	\
			"lds r30, %[Function] 	\n"	\
			"lds r31, %[Function]+1	\n"	\
			"jmp redir_func 	\n"		\
			:					\
			:[Function] "m" (*X##_REDIR::stub_ptr)\
			);				\
		}


/**
 *	\brief	Redirects interrupt handling to a method
 *	\param	vector	Interrupt vector symbol
 *	\param	func	Method pointer
 *	\param	obj	Object whose method should be called
 *
 *	This macro can be used multiple times in your code (like a function). It replaces the interrupt service
 *	routine, which will be called when interrupt \p vector occurs, with method \p func of object \p obj.
 *
 *	\see	\ref doc_interrupts, UseInterrupt, redirectISRF
 */
#define redirectISRM(vector,func,obj) __redirectISRM(vector,func,obj)
#define __redirectISRM(vector,func,obj)	\
	do {							\
	vector##_REDIR::from_function<typeof(obj), func>(&obj);		\
	} while(0)
// do-while(0) -> forces ";" after use, makes define a single command

/**
 *	\brief	Redirects interrupt handling to a function
 *	\param	vector	Interrupt vector symbol
 *	\param	func	Function pointer
 *
 *	This macro can be used multiple times in your code (like a function). It replaces the interrupt service
 *	routine, which will be called when interrupt \p vector occurs, with function \p func.
 *
 *	\see	\ref doc_interrupts, UseInterrupt, redirectISRM
 */
#define redirectISRF(vector,func)		__redirectISRF(vector,func)
#define __redirectISRF(vector,func)		\
	do {						\
	vector##_REDIR::from_function<func>();			\
	} while(0)



#if defined(__AVR_AT90CAN128__)
#	include "avr-halib/avr/interrupts/at90can128.h"
#elif defined(__AVR_ATMEGA32__)
#	include "avr-halib/avr/interrupts/atmega32.h"
#else
#	error "Library not ported to this platform yet."
#endif

/*@}*/
