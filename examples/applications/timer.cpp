#if defined(__AVR_AT90CAN128__)
#	define F_CPU 16000000UL
#	include "avr-halib/portmaps/robbyboard.h"
#elif defined(__AVR_ATMEGA32__)
#	define F_CPU 8000000UL
#	include "avr-halib/portmaps/bobbyboard.h"
#else
#	error "Example program not ported to this platform yet."
#endif

#include "avr-halib/avr/regmaps/at90can128.wip.h"
#define UseRegmap(var,map) map & var = (*(map*)0x0)


// #include "avr-halib/avr/timer.h"
#include "avr-halib/ext/led.h"

// Some methods might be useless for some timers because the do not support such a feature
template <class TimerRegmap>
	class Timer
{
public:
	typedef TimerRegmap Regmap;
	
	void selectClock(enum TimerRegmap::ClockSelect cs)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCS(cs);
	}
	
	void setWaveformGenerationMode(enum TimerRegmap::WaveformGenerationMode wgm)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setWGM(wgm);
	}
	
	void setCompareMatchOutputModeA(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMA(com);
	}
	
	void setCompareMatchOutputModeB(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMB(com);
	}

	void setCompareMatchOutputModeC(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMC(com);
	}

	
	uint8_t getInterruptMask()
	{
		UseRegmap(timer, TimerRegmap);
		return timer.interruptMask;
	}
	void setInterruptMask(uint8_t i)
	{
		UseRegmap(timer, TimerRegmap);
		timer.interruptMask = i;
	}
	
};




Led<Led0> led0;

void toggle()
{
	static int i = 0;
	i++;
	if (i % 10 == 0)
		led0.toggle();
}

UseInterrupt(SIG_OVERFLOW3);

int main()
{
	Timer<Timer3> timer;
	timer.selectClock(Timer3::ps64);
	
	timer.setInterruptMask(Timer3::im_overflowEnable);
	redirectISRF(SIG_OVERFLOW3, &toggle);
	
	sei();
	
	while (0);
	return 0;
}
