

#pragma once

#include "halib/avr/interrupt.h"

// Fehler: Faktor 2!?
// Todo: PWM, unexakteren weniger häufig Interruptenden EggTimer (in 10 Sekunden, uint8_t)
// Todo: aufsplitten in jeweils 1 datei pro komponente
// Todo: EggTimer optimieren für Ausnutzung 16-Bit-Timer (weniger Interrupts)
// Todo: Stoppuhr
GenInterrupt(SIG_OUTPUT_COMPARE0);
GenInterrupt(SIG_OVERFLOW0);


/**
 *	\brief	Calls a function after x milliseconds
 *	\param TimerRegmap	Register map of the timer to use
 *
 */
template <class TimerRegmap = Timer0>
	class ExactEggTimer
{
#define timer (*((TimerRegmap *)0x0))

	// Number of interrupts to wait when using prescaler 1
	uint32_t counter;

	// Select biggest possible prescaler for minimizing number of interrupts
	void nextStep();

public:
	void startEggTimer(uint16_t ms);
	void stopEggTimer();

	virtual void onEggTimer() {}
};
#define  CPU_FREQUENCY F_CPU
//#include "halib/avr/uart.wip.h"
//Uart<Uart1> uart;

template <class TimerRegmap>
void ExactEggTimer<TimerRegmap>::startEggTimer(uint16_t ms)
{
// in vielfaches von 256 takten teilen
// (F_CPU >> 8) * 256 Takte pro sek. (so viele OI für eine Sek.)
// (F_CPU >> 8) / 1000 * 256 Takte pro ms (so viele OI für eine Sek.)
// rund (F_CPU >> 18) * 256 Takte pro ms
	counter = ms * (F_CPU / 256000);
//	uart << "starteggTimer(): counter: " << counter;
//	uart.newline();
//	for (volatile uint32_t c = 15000; c; c--)
//		;
	timer.interruptMask = TimerRegmap::im_outputCompareEnable;
	timer.template setOutputCompareInterrupt< ExactEggTimer<TimerRegmap>, & ExactEggTimer<TimerRegmap>::nextStep > (*this);
	timer.outputCompare = 0xff;
	timer.waveformGenerationMode = TimerRegmap::wgm_ctc;	// Clear timer on compare match
	nextStep();
}

template <class TimerRegmap>
void ExactEggTimer<TimerRegmap>::stopEggTimer()
{
	timer.clockSelect = TimerRegmap::cs_stop;
}
//#include <avr/io.h>

template <class TimerRegmap>
void ExactEggTimer<TimerRegmap>::nextStep()
{
// static int i = 0;
// i++;

	if (!counter)
	{
	//	uart << "eggTimer: #IR=" << i;
	//	uart.newline();
	//	i = 0;
		stopEggTimer();
		onEggTimer();
	}
	else
	{
		// select highest possible prescaler
		uint16_t prescaler;
		uint8_t prescalerId;
	
		if (counter >= 1024)
		{
			prescaler = 1024;
			prescalerId = TimerRegmap::cs_ps1024;
		}
		else if ((uint16_t)counter >= (uint16_t)256)
		{
			prescaler = 256;
			prescalerId = TimerRegmap::cs_ps256;
		}
		else  if ((uint8_t)counter >= (uint8_t)64)
		{
			prescaler = 64;
			prescalerId = TimerRegmap::cs_ps64;
		}
		else if ((uint8_t)counter >= (uint8_t)8)
		{
			prescaler = 8;
			prescalerId = TimerRegmap::cs_ps8;
		}
		else
		{
			prescaler = 1;
			prescalerId = TimerRegmap::cs_ps1;
		}	
		
		counter -= prescaler;
		timer.clockSelect = prescalerId;
	}
}
#undef timer






/**
 *	\brief	Calls a function after x tenth of seconds
 *	\param TimerRegmap	Register map of the timer to use
 *	Less resolution and less code / interrupts, but exact timing in this resolution 
 * 	(maximal timing error: 16384 / CPU_FREQ s, e.g. 0.001 s with 16 MHz)
 */
template <class TimerRegmap = Timer0>
	class EggTimer
{
#define timer (*((TimerRegmap *)0x0))
	// Number of interrupts to wait when using prescaler 64
	uint16_t counter;

	// Select biggest possible prescaler for minimizing number of interrupts
	void nextStep();

public:
	void startEggTimer(uint8_t ts);
	void stopEggTimer();

	virtual void onEggTimer() {}
};
#define  CPU_FREQUENCY F_CPU
//#include "halib/avr/uart.wip.h"
//Uart<Uart1> uart;

template <class TimerRegmap>
void EggTimer<TimerRegmap>::startEggTimer(uint8_t ts)
{
// geringster verwendeter Prescaler: 64 -> kleinste gemessene Zeiteinheit: 256 * 64 = 16384 Takte
// in vielfaches von 16384 Takten teilen
// (F_CPU / 16384) * 16384 Takte pro sek. (so viele OI für eine Sek.)
// (F_CPU / 163840) * 16384 Takte pro s/10 (so viele OI für eine Sek.)
// rund (F_CPU >> 18) * 256 Takte pro ms
	counter = ts * (F_CPU / 163840);
//	uart << "starteggTimer(): counter: " << counter;
//	uart.newline();
//	for (volatile uint32_t c = 15000; c; c--)
//		;
	timer.interruptMask = TimerRegmap::im_outputCompareEnable;
	timer.template setOutputCompareInterrupt< EggTimer<TimerRegmap>, & EggTimer<TimerRegmap>::nextStep > (*this);
	timer.outputCompare = 0xff;
	timer.waveformGenerationMode = TimerRegmap::wgm_ctc;	// Clear timer on compare match
	nextStep();
}

template <class TimerRegmap>
void EggTimer<TimerRegmap>::stopEggTimer()
{
	timer.clockSelect = TimerRegmap::cs_stop;
}
//#include <avr/io.h>

template <class TimerRegmap>
void EggTimer<TimerRegmap>::nextStep()
{
//  static int i = 0;
//  i++;

	if (!counter)
	{
//		uart << "eggTimer: #IR=" << i;
//		uart.newline();
	//	i = 0;
		stopEggTimer();
		onEggTimer();
	}
	else
	{
		// select highest possible prescaler
		uint8_t diff;
		uint8_t prescalerId;
	
		if (counter >= 16)
		{
			diff = 16;
			prescalerId = TimerRegmap::cs_ps1024;
		}
		else if ((uint8_t)counter >= (uint8_t)4)
		{
			diff = 4;
			prescalerId = TimerRegmap::cs_ps256;
		}
		else
		{
			diff = 1;
			prescalerId = TimerRegmap::cs_ps64;
		}	
		
		counter -= diff;
		timer.clockSelect = prescalerId;
	}
}
#undef timer


/*

differences:
- counter register width
- clock source: extern, different prescalers
- overflow and compare match interrupt
- pwm (phase correct)
- clear timer on compare match (auto reload)
- external event counter
- frequncy generator

*/

