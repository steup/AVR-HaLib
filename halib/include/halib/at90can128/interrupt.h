/*!
 *	\file	halib/at90can128/interrupt.h
 *	\brief	Stellt Makros zur Deklaration und Implementierung der Interrupt-Klassen
 *			bereit.
 *	
 *	\see \ref halibinterrupt "Interrupt-Behandlung"
 *
 */


#pragma once

#include "halib/config.h"
#include <avr/interrupt.h>

/*! \def DECLARE_INTERRUPT_CLASS(classname)
    \brief Definiert Interrupt-Klasse.
    
    Makro, das Interrupt-Klasse mit Namen \a classname definiert.
	\see \ref halibinterrupt "Interrupt-Behandlung"
*/
#if defined(USE_DYNAMIC_INTERRUPT_HANDLING)
#	define DECLARE_INTERRUPT_CLASS(classname) \
class classname \
{ \
	classname * oldHandler; \
public: \
	classname(); \
	~classname(); \
	virtual void on##classname() = 0; \
};
#else

#	define DECLARE_INTERRUPT_CLASS(classname) \
class classname \
{ \
public: \
	classname(); \
	virtual void on##classname() = 0; \
};
#endif

/*! \def IMPLEMENT_INTERRUPT_CLASS(sig,classname)
    \brief Implementiert Interrupt-Klasse.
   
    Makro, das Interrupt-Klasse mit Namen \a classname implementiert. Dabei wird
	der Interrupt-Klasse das Signal \a sig zugeordnet.

	\see \ref halibinterrupt "Interrupt-Behandlung"
*/
#if defined(USE_DYNAMIC_INTERRUPT_HANDLING)
#define IMPLEMENT_INTERRUPT_CLASS(sig,classname) \
static classname * instOf##classname = 0; \
classname::classname() \
{ \
	oldHandler = instOf##classname; \
	instOf##classname = this; \
} \
classname::~classname() \
{ \
	instOf##classname = oldHandler; \
} \
SIGNAL(sig) \
{ \
	instOf##classname->on##classname(); \
}
#else
#define IMPLEMENT_INTERRUPT_CLASS(sig,classname) \
static classname * instOf##classname = 0; \
classname::classname() \
{ \
	instOf##classname = this; \
} \
SIGNAL(sig) \
{ \
	instOf##classname->on##classname(); \
}
#endif





