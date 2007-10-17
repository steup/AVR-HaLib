/**
 *	\file halib/atmega32/timer.h
 *
 *	\brief	Stellt Klassen zur Verwendung der ATmega32-Timer (Overrrun-Interrupt mit Prescalern) bereit.
 *
 *	\see \ref halibtimer
 */

#pragma once

#include "interrupt.h"


#define TIMER_NONE	0
#define TIMER_PS_1	1
#define TIMER_PS_8	2
#define TIMER_PS_64	3
#define TIMER_PS_256	4
#define TIMER_PS_1024	5
#define TIMER_PS_32	6
#define TIMER_PS_128	7




/// Abstrakte Interrupt-Klasse f&uuml;r den Timer/Counter2-Overflow-Interrrupt.
DECLARE_INTERRUPT_CLASS(InterruptOverflow2);

/// Abstrakte Interrupt-Klasse f&uuml;r den Timer/Counter1-Overflow-Interrrupt.
DECLARE_INTERRUPT_CLASS(InterruptOverflow1);

/// Abstrakte Interrupt-Klasse f&uuml;r den Timer/Counter0-Overflow-Interrrupt.
DECLARE_INTERRUPT_CLASS(InterruptOverflow0);

/*!
 *	\class Timer0
 *	\brief Implementiert ATmega32-Timer0 (8-Bit) mit Prescaler in einfachem Overflow-Interrupt-Modus.
 *	\see \ref halibtimer
 *
 */
class Timer0 : public Timer, public InterruptOverflow0
{
public:
	
	///	Destruktor. Stoppt den Timer.
	~Timer0()
	{
		stop();
	}

	/*! \brief Startet den Timer.
	*	
	*	Started den Timer mit dem angegebenen Prescaler bzw. &auml;ndert den Prescaler.
	*	\param	prescalerId	Prescaler, unterst&uuml;tzt werden hier: \c TIMER_NONE, \c TIMER_PS_1, \c TIMER_PS_8, \c TIMER_PS_64, \c TIMER_PS_256, \c TIMER_PS_1024
	*	\return	true, wenn erfolgreich gestartet.
	*/
	bool start(uint8_t prescalerId);
	
	/// Stoppt den Timer.
	void stop();
	
	/// Interrupt-Service-Routine f&uuml;r Overflow-Interrupt. Ruft abstrakte Methode <tt>void onTimer()</tt> auf.
	void onInterruptOverflow0()
	{
		onTimer();
	}
};

/*!
 *	\class Timer1
 *	\brief Implementiert ATmega32-Timer1 (16-Bit) mit Prescaler in einfachem Overflow-Interrupt-Modus.
 *	\see \ref halibtimer
 *
 */
class Timer1 : public Timer, public InterruptOverflow1
{
public:

	///	Destruktor. Stoppt den Timer.
	~Timer1()
	{
		stop();
	}

	/*! \brief Startet den Timer.
	*	
	*	Started den Timer mit dem angegebenen Prescaler bzw. &auml;ndert den Prescaler.
	*	\param	prescalerId	Prescaler, unterst&uuml;tzt werden hier: \c TIMER_NONE, \c TIMER_PS_1, \c TIMER_PS_8, \c TIMER_PS_64, \c TIMER_PS_256, \c TIMER_PS_1024
	*	\return	true, wenn erfolgreich gestartet.
	*/
	bool start(uint8_t prescalerId);

	/// Stoppt den Timer.
	void stop();
	
	/// Interrupt-Service-Routine f&uuml;r Overflow-Interrupt. Ruft abstrakte Methode <tt>void onTimer()</tt> auf.
	void onInterruptOverflow1()
	{
		onTimer();
	}
};

/*!
 *	\class Timer2
 *	\brief Implementiert ATmega32-Timer2 (8-Bit) mit Prescaler in einfachem Overflow-Interrupt-Modus.
 *	\warning Die Motor-Klasse reserviert sich Timer2 f&uuml;r die Software-PWM. Verwendet man also den vorimplentierten Motor, ist dieser Timer nicht frei verwendbar!
 *	\see \ref halibtimer
 */
class Timer2 : public Timer, public InterruptOverflow2
{
public:

	///	Destruktor. Stoppt den Timer.
	~Timer2()
	{
		stop();
	}


	/*! \brief Startet den Timer.
	*	
	*	Started den Timer mit dem angegebenen Prescaler bzw. &auml;ndert den Prescaler.
	*	\param	prescalerId	Prescaler, unterst&uuml;tzt werden hier: \c TIMER_NONE, \c TIMER_PS_1, \c TIMER_PS_8, \c TIMER_PS_32, \c TIMER_PS_64, \c TIMER_PS_128, \c TIMER_PS_256, \c TIMER_PS_1024
	*	\return	true, wenn erfolgreich gestartet.
	*/
	bool start(uint8_t prescalerId);
 
	/// Stoppt den Timer.
	void stop();
	
	/// Interrupt-Service-Routine f&uuml;r Overflow-Interrupt. Ruft abstrakte Methode <tt>void onTimer()</tt> auf.
	void onInterruptOverflow2()
	{
		onTimer();
	}
};

