 



#pragma once

struct Timer0
{
//	enum { counterWidth = 8 };
	
	volatile uint8_t : 0x44 * 8;

// TCCR0 {
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

// TIMSK {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareEnable = 2 };
	volatile uint8_t interruptMask : 2;

	volatile uint8_t : 6;
// }

	volatile uint8_t : (0x5c - 0x5a) * 8;

// OCR0 {
	volatile uint8_t outputCompare : 8;
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
