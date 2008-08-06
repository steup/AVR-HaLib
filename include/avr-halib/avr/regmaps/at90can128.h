// This file is part of avr-halib. See COPYING for copyright details.
 
#pragma once
#include "avr-halib/avr/interrupt.h"
#include <stdint.h>

class Timer0
{
//	enum { counterWidth = 8 };
private:	
	uint8_t __base [0x44];

public:
// TCCR0A (0x44) {
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	uint8_t clockSelect : 3;

	uint8_t waveformGenerationMode1 : 1;
	
	uint8_t compareMatchOutputMode : 2;

	uint8_t waveformGenerationMode0 : 1;

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

	uint8_t : 1;
// }

private:
	uint8_t __pad0 [0x47 - 0x44 - 1];

public:
// OCR0A (0x47) {
	uint8_t outputCompareA : 8;
// }

private:
	uint8_t __pad1 [0x6e - 0x47 - 1];

public:
// TIMSK0 (0x6e) {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2 };
	uint8_t interruptMask : 2;

	bool : 6;
// }

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE0, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW0, Fxn, obj);
	}
	
};



class Timer1
{
//	enum { counterWidth = 16 };
private:	
	uint8_t __base [0x6f];

public:	
// TIMSK1 (0x6f) {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2, im_outputCompareBEnable = 4, im_outputCompareCEnable = 8 };
	uint8_t interruptMask : 4;

	bool : 4;
// }
	
private:
	uint8_t __pad0 [0x80 - 0x6f - 1];

public:	
// TCCR1A (0x80) {
	uint8_t waveformGenerationMode01 : 2;
	uint8_t compareMatchOutputModeC : 2;
	uint8_t compareMatchOutputModeB : 2;
	uint8_t compareMatchOutputModeA : 2;
// }

public:	
// TCCR1B (0x81) {

	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	uint8_t clockSelect : 3;
	
	uint8_t waveformGenerationMode23 : 2;
	
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

	uint8_t : 3;
// }

private:
// TCCR1C (0x82)
// TCNT1L (0x84)
// TCNT1H (0x85)
// ICR1L (0x86)
// ICR1H (0x87)
	uint8_t __pad1 [0x88 - 0x81 - 1];

public:	
// OCR1AL (0x88) {
	uint8_t outputCompareA : 8;
// OCR1AH (0x89) {
	uint8_t outputCompareAH : 8;
// OCR1BL (0x8a) {
	uint8_t outputCompareB : 8;
// OCR1BH (0x8b) {
	uint8_t outputCompareBH : 8;
// OCR1CL (0x8c) {
	uint8_t outputCompareC : 8;
// OCR1CH (0x8d) {
	uint8_t outputCompareCH : 8;
	
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE1A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW0, Fxn, obj);
	}
};



class Timer2
{
//	enum { counterWidth = 8 };
	
private:
	uint8_t __base [0x70];
	
public:
// TIMSK2 (0x70) {
	// Interrupt Mask
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2 };
	uint8_t interruptMask : 4;

	bool : 4;
// }
	
private:
	uint8_t __pad0 [0xb0 - 0x70 - 1];

public:
// TCCR2A (0xb0) {
	
	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps32 = 3, cs_ps64 = 4, cs_ps128 = 5, cs_ps256 = 6, cs_ps1024 = 7 };
	uint8_t clockSelect : 3;
	
	uint8_t waveformGenerationMode1 : 1;
	
	uint8_t compareMatchOutputMode : 2;

	uint8_t waveformGenerationMode0 : 1;

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

	bool : 1;
// }

private:
// TCNT2 (0xb2)
	uint8_t __pad1 [0xb3 - 0xb0 - 1];

public:	
// OCRA2 (0xb3) {
	uint8_t outputCompareA : 8;
// }
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE2, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW2, Fxn, obj);
	}
};



class Timer3
{
//	enum { counterWidth = 16 };
	
private:
	uint8_t __base [0x71];

public:
// TIMSK3 (0x71) {
	enum { im_disable = 0, im_overflowEnable = 1, im_outputCompareAEnable = 2, im_outputCompareBEnable = 4, im_outputCompareCEnable = 8 };
	uint8_t interruptMask : 4;

	bool : 4;
// }

private:	
	uint8_t __pad0 [0x90 - 0x71 - 1];

public:	
// TCCR1A (0x90) {
	uint8_t waveformGenerationMode01 : 2;
	uint8_t compareMatchOutputModeC : 2;
	uint8_t compareMatchOutputModeB : 2;
	uint8_t compareMatchOutputModeA : 2;
// }

// TCCR1B (0x91) {

	// Clock select
	enum { cs_stop = 0, cs_ps1 = 1, cs_ps8 = 2, cs_ps64 = 3, cs_ps256 = 4, cs_ps1024 = 5, cs_extFalling = 6, cs_extRising = 7 };
	uint8_t clockSelect : 3;
	
	uint8_t waveformGenerationMode23 : 2;
	
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

	uint8_t : 3;
// }
	
private:	
// TCCR1C (0x92)
// TCNT1L (0x94)
// TCNT1H (0x95)
// ICR1L (0x96)
// ICR1H (0x97)
	uint8_t __pad1 [0x88 - 0x81 - 1];

public:	
// OCR1AL (0x98) {
	uint8_t outputCompareA : 8;
// OCR1AH (0x99) {
	uint8_t outputCompareAH : 8;
// OCR1BL (0x9a) {
	uint8_t outputCompareB : 8;
// OCR1BH (0x9b) {
	uint8_t outputCompareBH : 8;
// OCR1CL (0x9c) {
	uint8_t outputCompareC : 8;
// OCR1CH (0x9d) {
	uint8_t outputCompareCH : 8;

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE3A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW3, Fxn, obj);
	}
};


class Uart0
{
private:
	uint8_t __base [0xc0];
public:
	uint8_t ucsra;
	uint8_t ucsrb;
	uint8_t ucsrc;
private:
	uint8_t __pad0;
public:
	uint8_t ubrrl;
	uint8_t ubrrh;
	uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRM(SIG_UART0_RECV, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRM(SIG_UART0_DATA, Fxn, obj);
	}
};

class Uart1
{
private:
	uint8_t __base [0xc8];
public:
	uint8_t ucsra;
	uint8_t ucsrb;
	uint8_t ucsrc;
private:
	uint8_t __pad0;
public:
	uint8_t ubrrl;
	uint8_t ubrrh;
	uint8_t udr;
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
	static void setRecvInterrupt(T & obj)
	{
		redirectISRM(SIG_UART1_RECV, Fxn, obj);
	}
	template<class T, void (T::*Fxn)()>
	static void setDataInterrupt(T & obj)
	{
		redirectISRM(SIG_UART1_DATA, Fxn, obj);
	}
};

template <class Controller_Configuration> struct ADConv
{
private:
	uint8_t __base[0x78];
public:
	union
	{
		struct
		{
			uint8_t adcl:8;
			uint8_t adch:8;
		};
		struct
		{
			uint16_t adc:16;
		};
	};
	
	uint8_t adps : 3;
	bool	adie : 1;
	bool	adif : 1;
	bool	adate: 1;
	bool	adsc : 1;
	bool	aden : 1;
	uint8_t adts : 3;
	uint8_t  : 3;
	bool	acme : 1;
	uint8_t  : 1;
	union
	{
		uint8_t  admux	:8;
		struct{
			uint8_t  mux	:5;
			bool	 adlra	:1;
			uint8_t  refs	:2;
		};
	};
private:	
	uint8_t __pad0 :8;
public:
	uint8_t didr0;
	
	enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7, ps_none };
	enum {ref_aref = 0, ref_avcc = 1, ref_internal2_56 = 3};
	
	enum
	{	
		__recommendedPrescaler = Controller_Configuration::controllerClk/200000UL,
		recommendedPrescalar = __recommendedPrescaler > 64? (ps128) : __recommendedPrescaler > 32? (ps64) :__recommendedPrescaler > (ps32)? 5:__recommendedPrescaler > 8? (ps16):__recommendedPrescaler > 4? (ps8):__recommendedPrescaler > 2? (ps4):(ps2)
	};
	
	
	// a way to encapsulate interrupt symbol to use in device specific structure
	// mainly for internal use, syntax not nice at all 
	template<class T, void (T::*Fxn)()>
		static void setADCInterrupt(T & obj)
	{
		redirectISRM(SIG_ADC, Fxn, obj);
	}
	
	
};
