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

	TCCR0A = prescalerId;		// Prescaler setzen
	TIMSK0 = 1<<TOIE0;		// Overrun Interrupt Enable
	sei();

	return true;
}

void Timer0::stop()
{
	TIMSK0 = 0;		// Overrun Interrupt Disable
}


bool Timer1::start(uint8_t prescalerId)
{
	if (prescalerId > 5)		// Prescaler 32 und 128 nicht unerstützt
		return false;

	TCCR1B = prescalerId;		// Prescaler setzen
	TIMSK1 = 1 << TOIE1;		// Overrun Interrupt Enable
	sei();

	return true;
}

void Timer1::stop()
{
	TIMSK1 = 0;			// Overrun Interrupt Disable
}


bool Timer2::start(uint8_t prescalerId)
{
	if (prescalerId > 7)
		return false;

	// Prescaler 1/x                        -  1  8 64 256 1024 32 128
	const uint8_t timer2prescalers [] = { 0, 1, 2, 4,  6,  7, 3,  5};
	if (prescalerId > 7)
		TCCR2A = 0;
	else
		TCCR2A = timer2prescalers[prescalerId];
	TIMSK2 = (1 << TOIE2);				// Overrun Interrupt Enable
	sei();

	return true;
}

void Timer2::stop()
{
	TIMSK2 = 0;			// Overrun Interrupt Disable
}


