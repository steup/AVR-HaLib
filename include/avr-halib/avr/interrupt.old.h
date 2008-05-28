#ifndef REDIRVEC_H_
#define REDIRVEC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
// defining of redirection memory variables and 
// the redirection stub for the certain vector

// extern "C" void _redir_func();

#define REDIR_ISR(vector)	__REDIR_ISR(vector)
#define __REDIR_ISR(vector) 				\
	void (vector ## _TYPE::*vector ## _REDIR_FUNCTION)() =0;			\
	vector ## _TYPE * vector ## _REDIR_OBJECT=0;		\
	void vector (void) __attribute__ ((naked)); 	\
	void vector (void) {				\
		asm volatile (				\
			"push r31 		\n"	\
			"push r30 		\n"	\
			"lds r30, %[Function] 	\n"	\
			"lds r31, %[Function]+1	\n"	\
			"push r25 		\n"	\
			"push r24 		\n"	\
			"lds r24, %[Obj] 		\n"	\
			"lds r25, %[Obj]+1		\n"	\
			"jmp redir_func 	\n"  	\
			:  				\
			:[Function] "m" (vector ## _REDIR_FUNCTION),[Obj] "m" (vector ## _REDIR_OBJECT)\
			);				\
		}

// macro for setting the redirection function 
// for a specific vector
#define redirectISR(vector,func,object)	__redirectISR(vector,func,object)
#define __redirectISR(vector,func,object)			\
{							\
	extern  void (vector ## _TYPE::*vector ## _REDIR_FUNCTION)() ;		\
	extern vector ## _TYPE * vector ## _REDIR_OBJECT;\
	uint8_t tmp=SREG;				\
	cli();						\
	vector ## _REDIR_FUNCTION=(void (vector ## _TYPE::*)())func;		\
	vector ## _REDIR_OBJECT=(vector ## _TYPE *)object;	\
	SREG=tmp;					\
}








#endif
