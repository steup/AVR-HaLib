 
#pragma once

#include <stdint.h>

struct Timer0
{
//	enum { counterWidth = 8 };
	
	volatile bool : 0x44 * 8;

// TCCR0A {
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;

	volatile uint8_t waveformGenerationMode1 : 1;
	
	volatile uint8_t compareMatchOutputMode : 2;

	volatile uint8_t waveformGenerationMode0 : 1;

	// Waveform generation mode
	enum { wgm_normal = 0, wgm_phaseCorrectPwm = 1, wgm_ctc = 2, wgm_fastPwm = 3 };
	void setWaveformGenerationMode(uint8_t i)
	{
		waveformGenerationMode0 = i;
		waveformGenerationMode1 = i >> 1;
	}
	uint8_t getWaveformGenerationMode()
	{
		return (waveformGenerationMode1 << 1) | waveformGenerationMode0;
	}

	volatile bool : 1;
// }

	volatile bool : (0x47 - 0x45) * 8;

// OCR0A {
	volatile uint8_t outputCompareA : 8;
// }

	volatile bool : (0x6e - 0x48) * 8;

// TIMSK0 {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2 };
	volatile uint8_t interruptMask : 2;

	volatile bool : 6;
// }


	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
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
	
	volatile bool : 0x6f * 8;
	
// TIMSK1 (0x6f) {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2, im_outputCompareBEnable = 4, im_outputCompareCEnable = 8 };
	volatile uint8_t interruptMask : 4;

	volatile bool : 4;
// }
	
	volatile bool : (0x80 - 0x6f - 1) * 8;
	
// TCCR1A (0x80) {
	volatile uint8_t waveformGenerationMode01 : 2;
	volatile uint8_t compareMatchOutputModeC : 2;
	volatile uint8_t compareMatchOutputModeB : 2;
	volatile uint8_t compareMatchOutputModeA : 2;
// }
	
// TCCR1B (0x81) {

	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;
	
	volatile uint8_t waveformGenerationMode23 : 2;
	
	// Waveform generation mode
	// TODO: add other supported modes
	enum { wgm_normal = 0, wgm_phaseCorrectPwm = 1, wgm_ctc = 4, wgm_fastPwm = 5 };
	void setWaveformGenerationMode(uint8_t i)
	{
		waveformGenerationMode01 = i;
		waveformGenerationMode23 = i >> 2;
	}
	uint8_t getWaveformGenerationMode()
	{
		return (waveformGenerationMode23 << 2) | waveformGenerationMode01;
	}

	volatile uint8_t : 3;
// }
	
	
// TCCR1C (0x82)
// TCNT1L (0x84)
// TCNT1H (0x85)
// ICR1L (0x86)
// ICR1H (0x87)
	volatile bool : (0x88 - 0x81 - 1) * 8;
	
// OCR1AL (0x88) {
	volatile uint8_t outputCompareA : 8;
// OCR1AH (0x89) {
	volatile uint8_t outputCompareAH : 8;
// OCR1BL (0x8a) {
	volatile uint8_t outputCompareB : 8;
// OCR1BH (0x8b) {
	volatile uint8_t outputCompareBH : 8;
// OCR1CL (0x8c) {
	volatile uint8_t outputCompareC : 8;
// OCR1CH (0x8d) {
	volatile uint8_t outputCompareCH : 8;
	
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRMF(SIG_OUTPUT_COMPARE1A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRMF(SIG_OVERFLOW0, Fxn, obj);
	}
};



struct Timer2
{
//	enum { counterWidth = 8 };
	
	volatile bool : 0x70 * 8;
	
// TIMSK2 (0x70) {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2 };
	volatile uint8_t interruptMask : 4;

	volatile bool : 4;
// }
	
	volatile bool : (0xb0 - 0x70 - 1) * 8;

// TCCR2A (0xb0) {
	
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps32 = 3, cs_ps64 = 4, cs_ps128 = 5, cs_ps256 = 6, cs_ps1024 = 7 };
	volatile uint8_t clockSelect : 3;
	
	volatile uint8_t waveformGenerationMode1 : 1;
	
	volatile uint8_t compareMatchOutputMode : 2;

	volatile uint8_t waveformGenerationMode0 : 1;

	// Waveform generation mode
	enum { wgm_normal = 0, wgm_phaseCorrectPwm = 1, wgm_ctc = 2, wgm_fastPwm = 3 };
	void setWaveformGenerationMode(uint8_t i)
	{
		waveformGenerationMode0 = i;
		waveformGenerationMode1 = i >> 1;
	}
	uint8_t getWaveformGenerationMode()
	{
		return (waveformGenerationMode1 << 1) | waveformGenerationMode0;
	}

	volatile bool : 1;
// }

// TCNT2 (0xb2)
	volatile bool : (0xb3 - 0xb0 - 1) * 8;
	
// OCRA2 (0xb3) {
	volatile uint8_t outputCompareA : 8;
// }
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRMF(SIG_OUTPUT_COMPARE2, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRMF(SIG_OVERFLOW2, Fxn, obj);
	}
};



struct Timer3
{
//	enum { counterWidth = 16 };
	
	volatile bool : 0x71 * 8;

// TIMSK3 (0x71) {
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2, im_outputCompareBEnable = 4, im_outputCompareCEnable = 8 };
	volatile uint8_t interruptMask : 4;

	volatile bool : 4;
// }
	
	volatile bool : (0x90 - 0x71 - 1) * 8;
	
// TCCR1A (0x90) {
	volatile uint8_t waveformGenerationMode01 : 2;
	volatile uint8_t compareMatchOutputModeC : 2;
	volatile uint8_t compareMatchOutputModeB : 2;
	volatile uint8_t compareMatchOutputModeA : 2;
// }
	
// TCCR1B (0x91) {

	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	volatile uint8_t clockSelect : 3;
	
	volatile uint8_t waveformGenerationMode23 : 2;
	
	// Waveform generation mode
	// TODO: add other supported modes
	enum { wgm_normal = 0, wgm_phaseCorrectPwm = 1, wgm_ctc = 4, wgm_fastPwm = 5 };
	void setWaveformGenerationMode(uint8_t i)
	{
		waveformGenerationMode01 = i;
		waveformGenerationMode23 = i >> 2;
	}
	uint8_t getWaveformGenerationMode()
	{
		return (waveformGenerationMode23 << 2) | waveformGenerationMode01;
	}

	volatile uint8_t : 3;
// }
	
	
// TCCR1C (0x92)
// TCNT1L (0x94)
// TCNT1H (0x95)
// ICR1L (0x96)
// ICR1H (0x97)
	volatile bool : (0x88 - 0x81 - 1) * 8;
	
// OCR1AL (0x98) {
	volatile uint8_t outputCompareA : 8;
// OCR1AH (0x99) {
	volatile uint8_t outputCompareAH : 8;
// OCR1BL (0x9a) {
	volatile uint8_t outputCompareB : 8;
// OCR1BH (0x9b) {
	volatile uint8_t outputCompareBH : 8;
// OCR1CL (0x9c) {
	volatile uint8_t outputCompareC : 8;
// OCR1CH (0x9d) {
	volatile uint8_t outputCompareCH : 8;

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRMF(SIG_OUTPUT_COMPARE3A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRMF(SIG_OVERFLOW3, Fxn, obj);
	}
};
