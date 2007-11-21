/**
 *	\file	halib/at90can128/timer.cpp
 *	\brief	Implementiert die ATmega32-Timer in einigen der unterst&uuml;tzten Modi.
 *	\see \ref halibtimer
 *
 *
 */




IMPLEMENT_INTERRUPT_CLASS(SIG_OVERFLOW2, InterruptOverflow2);
IMPLEMENT_INTERRUPT_CLASS(SIG_OVERFLOW1, InterruptOverflow1);
IMPLEMENT_INTERRUPT_CLASS(SIG_OVERFLOW0, InterruptOverflow0);


bool Timer0::start(uint8_t prescalerId)
{
	if (prescalerId > 5)		// Prescaler 32 und 128 nicht unerstützt
		return false;

//	TCCR0 = prescalerId;		// Prescaler setzen
//	TIMSK |= _BV(TOIE0);		// Overrun Interrupt Enable
//	sei();

	return true;
}

void Timer0::stop()
{
//	TIMSK &= ~_BV(TOIE0);		// Overrun Interrupt Disable
}


bool Timer1::start(uint8_t prescalerId)
{
	if (prescalerId > 5)		// Prescaler 32 und 128 nicht unerstützt
		return false;

//	TCCR1B = prescalerId;		// Prescaler setzen
//	TIMSK |= _BV(TOIE1);		// Overrun Interrupt Enable
//	sei();

	return true;
}

void Timer1::stop()
{
//	TIMSK &= ~_BV(TOIE1);			// Overrun Interrupt Disable
}


bool Timer2::start(uint8_t prescalerId)
{
	if (prescalerId > 7)
		return false;

	// Prescaler 1/x                        -  1  8 64 256 1024 32 128
//	const uint8_t timer2prescalers [] = { 0, 1, 2, 4,  6,  7, 3,  5};
//	if (prescalerId > 7)
//		TCCR2 = 0;
//	else
//		TCCR2 = timer2prescalers[prescalerId];
//	TIMSK |= _BV(TOIE2);				// Overrun Interrupt Enable
	sei();

	return true;
}

void Timer2::stop()
{
//	TIMSK &= ~_BV(TOIE2);			// Overrun Interrupt Disable
}


