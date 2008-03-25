

#pragma once

#include "halib/avr/interrupt.h"


// Todo: PWM
GenInterrupt(SIG_OUTPUT_COMPARE0);
GenInterrupt(SIG_OVERFLOW0);


struct Timer0
{
//	enum { counterWidth = 8 };
	
	volatile uint8_t : 0x44 * 8;

// TCCR0A {
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;

	volatile uint8_t compareMatchOutputMode : 2;

	// Waveform generation mode
	enum { wgm_normal = 0, wgm_phaseCorrectPwm = 1, wgm_ctc = 2, wgm_fastPwm = 3 };
	volatile uint8_t waveformGenerationMode : 2;

	volatile uint8_t : 1;
// }

	volatile uint8_t : (0x47 - 0x45) * 8;

// OCR0A {
	volatile uint8_t outputCompare : 8;
// }

	volatile uint8_t : (0x6e - 0x48) * 8;

// TIMSK0 {
	// Interrupt Mask
	enum { im_disable = 0, im_outputCompareEnable = 1, im_overflowEnable = 2 };
	volatile uint8_t interruptMask : 2;

	volatile uint8_t : 6;
// }


	template<class T, void (T::*Fxn)()>
	static void setOutputCompareInterrupt(T & obj)
	{
		redirectISRMF(SIG_OUTPUT_COMPARE0, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRMF(SIG_OVERFLOW0, Fxn, obj);
	}
	
};

struct Timer1
{
//	enum { counterWidth = 16 };
	
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;
};

struct Timer2
{
//	enum { counterWidth = 8 };
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps32 = 3, cs_ps64 = 4, cs_ps128 = 5, cs_ps256 = 6, cs_ps1024 = 7 };
	volatile uint8_t clockSelect : 3;
};

struct Timer3
{
//	enum { counterWidth = 16 };
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;
};

/**
 *	\brief	Calls a function after x milliseconds
 *	\param TimerRegmap	Register map of the timer to use
 */
template <class TimerRegmap = Timer0>
	class EggTimer
{
#define timer (*((TimerRegmap *)0x0))
	uint32_t counter;

	void nextStep();

public:
	void startEggTimer(uint16_t ms);
	void stopEggTimer();

	virtual void onEggTimer() {}
};
#define  CPU_FREQUENCY F_CPU
#include "halib/avr/uart.wip.h"
template <class TimerRegmap>
void EggTimer<TimerRegmap>::startEggTimer(uint16_t ms)
{
Uart<Uart0> uart;
// in vielfaches von 256 takten teilen
// (F_CPU >> 8) * 256 Takte pro sek. (so viele OI für eine Sek.)
// (F_CPU >> 8) / 1024 * 256 Takte pro ms (so viele OI für eine Sek.)
// rund (F_CPU >> 18) * 256 Takte pro ms
	counter = ms * (F_CPU >> 18);
	uart << "counter: " << counter;
	//TIMSK0 = 1;
	timer.interruptMask = TimerRegmap::im_outputCompareEnable;
	timer.template setOutputCompareInterrupt< EggTimer<TimerRegmap>, & EggTimer<TimerRegmap>::nextStep > (*this);
	//OCR0A = 0xff;
	timer.outputCompare = 0xff;
//	TCCR0A = (1 << WGM01) | (1 << CS02) | (1 << CS00);
	timer.waveformGenerationMode = TimerRegmap::wgm_ctc;	// Clear timer on compare match
	nextStep();
}

template <class TimerRegmap>
void EggTimer<TimerRegmap>::stopEggTimer()
{
	timer.clockSelect = TimerRegmap::cs_stop;
}
#include <avr/io.h>

template <class TimerRegmap>
void EggTimer<TimerRegmap>::nextStep()
{
//	DDRA = 0x0f;
// static int i = 0;
// if (i % 1000 == 0)
// 	onEggTimer();
// i++;
// 	timer.clockSelect = 7;

	if (!counter)
	{
		stopEggTimer();
		onEggTimer();
	}
	else
	{
		// select highest possible prescaler
		uint16_t prescaler;
		uint8_t prescalerId;
	
		if (counter >= 256)
		{
			PORTA |= 8;
			prescaler = 256;
			prescalerId = TimerRegmap::cs_ps256;
		}
		else  if (counter >= 64)
		{
			PORTA |= 4;
			prescaler = 64;
			prescalerId = TimerRegmap::cs_ps64;
		}
		else if (counter >= 8)
		{
			PORTA |= 2;
			prescaler = 8;
			prescalerId = TimerRegmap::cs_ps8;
		}
		else
		{
			PORTA |= 1;
			prescaler = 1;
			prescalerId = TimerRegmap::cs_ps1;
		}	
		
		counter -= prescaler;
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

