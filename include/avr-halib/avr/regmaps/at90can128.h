/** \addtogroup at90can128 */
/*@{*/
/**
 *	\file	include/avr-halib/avr/regmaps/at90can128.h
 *	\brief	Contains at90can128 specific stuff
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/interrupt.h"

#include <stdint.h>

/**
 *	\brief	Namespace containing at90can128 specific stuff
 *
 *	Content in global namespace as well if compiling for at90can128.
 */
namespace at90can128
{

/**
 *	\brief		Register map for Timer0 of at90can128
 *	\ingroup	at90can128
 */
class Timer0
{
public:
	/// Counter register width
	enum { counterWidth = 8 };
	
	/// Clock sources of Timer0
	enum ClockSelect {
		stop = 0,	///< no clock source
		ps1 = 1,	///< internal clock
		ps8 = 2,	///< internal clock, prescaler 8
		ps64 = 3,	///< internal clock, prescaler 64
		ps256 = 4,	///< internal clock, prescaler 256
		ps1024 = 5,	///< internal clock, prescaler 1024
		extFalling = 6,	///< external clock on falling edge
		extRising = 7	///< external clock on rising edge
	};
	void setCS(ClockSelect cs)
	{
		 _clockSelect = cs;
	}
	
	/// Waveform generation modes of Timer0
	enum WaveformGenerationMode {
		normal = 0,		///< normal mode
		phaseCorrectPwm = 1,	///< phase correct pwm
		ctc = 2,		///< clear timer on compare match
		fastPwm = 3		///< fast pwm
	};
	void setWGM(WaveformGenerationMode i)
	{
		_waveformGenerationMode0 = i;
		_waveformGenerationMode1 = i >> 1;
	}
	WaveformGenerationMode getWGM()
	{
		return (WaveformGenerationMode)((_waveformGenerationMode1 << 1) | _waveformGenerationMode0);
	}

	/// Compare match output mode (controls OC0A on compare match)
	enum CompareMatchOutputMode
	{
		disconnected = 0,	///< no change of OC0A
		toggle = 1,		///< toggle OC0A on compare match
		clear = 2,		///< clear OC0A on compare match
		set = 3			///< set OC0A on compare match
	};
	void setCOMA(CompareMatchOutputMode com)
	{
		_compareMatchOutputMode = com;
	}
	

	
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
	
private:	
	uint8_t __base [0x44];
// TCCR0A (0x44) {
	uint8_t _clockSelect : 3;
	uint8_t _waveformGenerationMode1 : 1;
	uint8_t _compareMatchOutputMode : 2;
	uint8_t _waveformGenerationMode0 : 1;
	uint8_t : 1;
// }
	uint8_t __pad0 [0x47 - 0x44 - 1];

public:
// OCR0A (0x47) {
	/// Output compare register A
	uint8_t outputCompareA : 8;
// }

private:
	uint8_t __pad1 [0x6e - 0x47 - 1];

public:
// TIMSK0 (0x6e) {
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable \c SIG_OVERFLOW0 and \c SIG_OUTPUT_COMPARE0
		im_overflowEnable = 1,		///< Enable \c SIG_OVERFLOW0
		im_outputCompareAEnable = 2	///< Enable \c SIG_OUTPUT_COMPARE0
	};
	/// Interrupt mask (enables/disables Timer0's interrupts)
	uint8_t interruptMask : 2;

	bool : 6;
// }
};



/**
 *	\brief		Register map for Timer1 of at90can128
 *	\ingroup	at90can128
 *
 *	\todo		Add input capture support
 */
class Timer1
{
public:
	/// Counter register width
	enum { counterWidth = 16 };
	
	/// Clock sources of Timer1
	enum ClockSelect {
		stop = 0,	///< no clock source
		ps1 = 1,	///< internal clock
		ps8 = 2,	///< internal clock, prescaler 8
		ps64 = 3,	///< internal clock, prescaler 64
		ps256 = 4,	///< internal clock, prescaler 256
		ps1024 = 5,	///< internal clock, prescaler 1024
		extFalling = 6,	///< external clock on falling edge
		extRising = 7	///< external clock on rising edge
	};
	void setCS(ClockSelect cs)
	{
		 _clockSelect = cs;
	}
	
	// TODO: add other supported modes
	/// Waveform generation modes of Timer1
	enum WaveformGenerationMode {
		normal = 0,		///< normal mode
		phaseCorrectPwm = 1,	///< phase correct pwm, 8 bit
		phaseCorrectPwm9 = 2,	///< phase correct pwm, 9 bit
		phaseCorrectPwm10 = 3,	///< phase correct pwm, 10 bit
		ctc = 4,		///< clear timer on compare match
		fastPwm = 5,		///< fast pwm, 8 bit
		fastPwm9 = 6,		///< fast pwm, 9 bit
		fastPwm10 = 7		///< fast pwm, 10 bit
	};
	void setWGM(WaveformGenerationMode i)
	{
		_waveformGenerationMode01 = i;
		_waveformGenerationMode23 = i >> 2;
	}
	WaveformGenerationMode getWGM()
	{
		return (WaveformGenerationMode)((_waveformGenerationMode23 << 2) | _waveformGenerationMode01);
	}

	/// Compare match output mode (controls OC1A, OC1B, OC1C on compare match)
	enum CompareMatchOutputMode
	{
		disconnected = 0,	///< no change
		toggle = 1,		///< toggle on compare match
		clear = 2,		///< clear on compare match
		set = 3			///< set on compare match
	};
	void setCOMA(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeA = com;
	}
	void setCOMB(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeB = com;
	}
	void setCOMC(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeC = com;
	}
	

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE1A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE1B, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE1C, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW1, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(SIG_INPUT_CAPTURE1, Fxn, obj);
	}


private:	
	uint8_t __base [0x6f];

public:	
// TIMSK1 (0x6f) {
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable Timer1's interrupts
		im_overflowEnable = 1,		///< Enable \c SIG_OVERFLOW1
		im_outputCompareAEnable = 2,	///< Enable \c SIG_OUTPUT_COMPARE1A
		im_outputCompareBEnable = 4,	///< Enable \c SIG_OUTPUT_COMPARE1B
		im_outputCompareCEnable = 8,	///< Enable \c SIG_OUTPUT_COMPARE1C
		im_inputCaptureEnable = 32	///< Enable \c SIG_INPUT_CAPTURE1
	};
	/// Interrupt mask (enables/disables Timer1's interrupts)
	uint8_t interruptMask : 6;

	bool : 2;
// }
	
private:
	uint8_t __pad0 [0x80 - 0x6f - 1];
// TCCR1A (0x80) {
	uint8_t _waveformGenerationMode01 : 2;
	uint8_t _compareMatchOutputModeC : 2;
	uint8_t _compareMatchOutputModeB : 2;
	uint8_t _compareMatchOutputModeA : 2;
// }

public:	
// TCCR1B (0x81) {
	uint8_t _clockSelect : 3;
	uint8_t _waveformGenerationMode23 : 2;
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
	/// Output compare register A (low byte)
	uint8_t outputCompareA : 8;
// OCR1AH (0x89) {
	/// Output compare register A (high byte)
	uint8_t outputCompareAH : 8;
// OCR1BL (0x8a) {
	/// Output compare register B (low byte)
	uint8_t outputCompareB : 8;
// OCR1BH (0x8b) {
	/// Output compare register B (high byte)
	uint8_t outputCompareBH : 8;
// OCR1CL (0x8c) {
	/// Output compare register C (low byte)
	uint8_t outputCompareC : 8;
// OCR1CH (0x8d) {
	/// Output compare register C (high byte)
	uint8_t outputCompareCH : 8;
};


/**
 *	\brief		Register map for Timer2 of at90can128
 *	\ingroup	at90can128
 *
 */
class Timer2
{
public:
	/// Counter register width
	enum { counterWidth = 8 };
	
	/// Clock sources of Timer2
	enum ClockSelect {
		stop = 0,	///< no clock source
		ps1 = 1,	///< internal clock
		ps8 = 2,	///< internal clock, prescaler 8
		ps32 = 3,	///< internal clock, prescaler 32
		ps64 = 4,	///< internal clock, prescaler 64
		ps128 = 5,	///< internal clock, prescaler 128
		ps256 = 6,	///< internal clock, prescaler 256
		ps1024 = 7	///< internal clock, prescaler 1024
	};
	void setCS(ClockSelect cs)
	{
		 _clockSelect = cs;
	}
	
	/// Waveform generation modes of Timer2
	enum WaveformGenerationMode {
		normal = 0,		///< normal mode
		phaseCorrectPwm = 1,	///< phase correct pwm
		ctc = 2,		///< clear timer on compare match
		fastPwm = 3		///< fast pwm
	};
	void setWGM(WaveformGenerationMode i)
	{
		_waveformGenerationMode0 = i;
		_waveformGenerationMode1 = i >> 1;
	}
	WaveformGenerationMode getWGM()
	{
		return (WaveformGenerationMode)((_waveformGenerationMode1 << 1) | _waveformGenerationMode0);
	}

	/// Compare match output mode (controls OC2A on compare match)
	enum CompareMatchOutputMode
	{
		disconnected = 0,	///< no change
		toggle = 1,		///< toggle on compare match
		clear = 2,		///< clear on compare match
		set = 3			///< set on compare match
	};
	void setCOMA(CompareMatchOutputMode com)
	{
		_compareMatchOutputMode = com;
	}
	
	
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
	
private:
	uint8_t __base [0x70];
	
public:
// TIMSK2 (0x70) {
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable \c SIG_OVERFLOW2 and \c SIG_OUTPUT_COMPARE2
		im_overflowEnable = 1,		///< Enable \c SIG_OVERFLOW2
		im_outputCompareAEnable = 2	///< Enable \c SIG_OUTPUT_COMPARE2
	};
	/// Interrupt mask (enables/disables Timer2's interrupts)
	uint8_t interruptMask : 2;

	bool : 6;
// }
	
private:
	uint8_t __pad0 [0xb0 - 0x70 - 1];

// TCCR2A (0xb0) {
	
	uint8_t _clockSelect : 3;
	uint8_t _waveformGenerationMode1 : 1;
	uint8_t _compareMatchOutputMode : 2;
	uint8_t _waveformGenerationMode0 : 1;
	bool : 1;
// }

// TCNT2 (0xb2)
	uint8_t __pad1 [0xb3 - 0xb0 - 1];

public:	
// OCRA2 (0xb3) {
	/// Output compare register A
	uint8_t outputCompareA : 8;
// }
};



/**
 *	\brief		Register map for Timer3 of at90can128
 *	\ingroup	at90can128
 *
 *	\todo		Add input capture support
 */
class Timer3
{
public:
	/// Counter register width
	enum { counterWidth = 16 };
	
	/// Clock sources of Timer3
	enum ClockSelect {
		stop = 0,	///< no clock source
		ps1 = 1,	///< internal clock
		ps8 = 2,	///< internal clock, prescaler 8
		ps64 = 3,	///< internal clock, prescaler 64
		ps256 = 4,	///< internal clock, prescaler 256
		ps1024 = 5,	///< internal clock, prescaler 1024
		extFalling = 6,	///< external clock on falling edge
		extRising = 7	///< external clock on rising edge
	};
	void setCS(ClockSelect cs)
	{
		 _clockSelect = cs;
	}
	
	// TODO: add other supported modes
	/// Waveform generation modes of Timer3
	enum WaveformGenerationMode {
		normal = 0,		///< normal mode
		phaseCorrectPwm = 1,	///< phase correct pwm, 8 bit
		phaseCorrectPwm9 = 2,	///< phase correct pwm, 9 bit
		phaseCorrectPwm10 = 3,	///< phase correct pwm, 10 bit
		ctc = 4,		///< clear timer on compare match
		fastPwm = 5,		///< fast pwm, 8 bit
		fastPwm9 = 6,		///< fast pwm, 9 bit
		fastPwm10 = 7		///< fast pwm, 10 bit
	};
	void setWGM(WaveformGenerationMode i)
	{
		_waveformGenerationMode01 = i;
		_waveformGenerationMode23 = i >> 2;
	}
	WaveformGenerationMode getWGM()
	{
		return (WaveformGenerationMode)((_waveformGenerationMode23 << 2) | _waveformGenerationMode01);
	}

	/// Compare match output mode (controls OC3A, OC3B, OC3C on compare match)
	enum CompareMatchOutputMode
	{
		disconnected = 0,	///< no change
		toggle = 1,		///< toggle on compare match
		clear = 2,		///< clear on compare match
		set = 3			///< set on compare match
	};
	void setCOMA(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeA = com;
	}
	void setCOMB(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeB = com;
	}
	void setCOMC(CompareMatchOutputMode com)
	{
		_compareMatchOutputModeC = com;
	}
	

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE3A, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE3B, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(SIG_OUTPUT_COMPARE3C, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(SIG_OVERFLOW3, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(SIG_INPUT_CAPTURE3, Fxn, obj);
	}

private:
	uint8_t __base [0x71];

public:
// TIMSK3 (0x71) {
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable Timer1's interrupts
		im_overflowEnable = 1,		///< Enable \c SIG_OVERFLOW3
		im_outputCompareAEnable = 2,	///< Enable \c SIG_OUTPUT_COMPARE3A
		im_outputCompareBEnable = 4,	///< Enable \c SIG_OUTPUT_COMPARE3B
		im_outputCompareCEnable = 8,	///< Enable \c SIG_OUTPUT_COMPARE3C
		im_inputCaptureEnable = 32	///< Enable \c SIG_INPUT_CAPTURE3
	};
	/// Interrupt mask (enables/disables Timer3's interrupts)
	uint8_t interruptMask : 6;

	bool : 2;
// }

private:	
	uint8_t __pad0 [0x90 - 0x71 - 1];

// TCCR3A (0x90) {
	uint8_t _waveformGenerationMode01 : 2;
	uint8_t _compareMatchOutputModeC : 2;
	uint8_t _compareMatchOutputModeB : 2;
	uint8_t _compareMatchOutputModeA : 2;
// }

// TCCR3B (0x91) {
	uint8_t _clockSelect : 3;

	uint8_t _waveformGenerationMode23 : 2;
	uint8_t : 3;
// }
	
private:	
// TCCR3C (0x92)
// TCNT3L (0x94)
// TCNT3H (0x95)
// ICR3L (0x96)
// ICR3H (0x97)
	uint8_t __pad1 [0x98 - 0x91 - 1];

public:	
// OCR3AL (0x98) {
	/// Output compare register A (low byte)
	uint8_t outputCompareA : 8;
// OCR3AH (0x99) {
	/// Output compare register A (high byte)
	uint8_t outputCompareAH : 8;
// OCR3BL (0x9a) {
	/// Output compare register B (low byte)
	uint8_t outputCompareB : 8;
// OCR3BH (0x9b) {
	/// Output compare register B (high byte)
	uint8_t outputCompareBH : 8;
// OCR3CL (0x9c) {
	/// Output compare register C (low byte)
	uint8_t outputCompareC : 8;
// OCR3CH (0x9d) {
	/// Output compare register C (high byte)
	uint8_t outputCompareCH : 8;
};


/**
 *	\brief		Register map for Uart0 of at90can128
 *	\ingroup	at90can128
 *
 */
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

/**
 *	\brief		Register map for Uart1 of at90can128
 *	\ingroup	at90can128
 *
 */
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

/**
 *	\brief		Register map for ADConv of at90can128
 *	\param		Controller_Configuration	TODO
 *	\ingroup	at90can128
 *
 */
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


/**
 *	\brief		Register map for usage of external interrupts
 *	\ingroup	at90can128
 */
class ExternalInterrupts
{
private:
	uint8_t __base [0x3C];

// EIFR (0x3C) {
	uint8_t __pad0; 
// }

public:
// EIMSK (0x3D) {
	bool enableInt0 : 1;	///< Enable external interrupt 0
	bool enableInt1 : 1;	///< Enable external interrupt 1
	bool enableInt2 : 1;	///< Enable external interrupt 2
	bool enableInt3 : 1;	///< Enable external interrupt 3
	bool enableInt4 : 1;	///< Enable external interrupt 4
	bool enableInt5 : 1;	///< Enable external interrupt 5
	bool enableInt6 : 1;	///< Enable external interrupt 6
	bool enableInt7 : 1;	///< Enable external interrupt 7
// }
private:
	uint8_t __pad1 [0x69 - 0x3D - 1]; 
public:	
// EICRA (0x69) {
	uint8_t senseInt0 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt1 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt2 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt3 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
// }

// EICRB (0x6A) {
	uint8_t senseInt4 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt5 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt6 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
	uint8_t senseInt7 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
// }
};


}

#if defined(__AVR_AT90CAN128__)
using namespace at90can128;
#endif

/*@}*/
