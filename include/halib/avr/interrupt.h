

#pragma once

//#include <avr/io.h>
#include <avr/interrupt.h>
// defining of redirection memory variables and 
// the redirection stub for the certain vector

// extern "C" void _redir_func();

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
 *	\brief	Makro that generates ISR delegate storage
 *	\param	X	Interrupt vector
 *	Use this macro exactly once for every Interrupt you use in your source code.
 */
#define UseInterrupt(X)			__UseInterrupt(X)


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


#define redirectISRMF(vector,func, obj) __redirectISRMF(vector,func, obj)
#define __redirectISRMF(vector,func, obj)	\
	do {							\
	vector##_REDIR::from_function<typeof(obj), func>(&obj);		\
	} while(0)
// do-while(0) -> forces ";" after use, makes define a single command

#define redirectISRF(vector,func)		__redirectISRF(vector,func)
#define __redirectISRF(vector,func)		\
	do {						\
	vector##_REDIR::from_function<func>();			\
	} while(0)



#if defined(__AVR_AT90CAN128__)
#	include "halib/avr/interrupts/at90can128.h"
#elif defined(__AVR_ATMEGA32__)
#	include "halib/avr/interrupts/atmega32.h"
#else
#	error "Library not ported to this platform yet."
#endif

