namespace atmega128rfa1
{
/**
 * TODO check this (copy from at90can128)
 *	\brief		Register map for Timer0 of atmega128rfa1
 *	\ingroup	atmega128
 */
class Timer0
{
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
	/// Interrupt mask (enables/disables Timer0's interrupts)
	uint8_t interruptMask : 2;

	bool : 6;
// }
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
	
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable \c TIMER0_OVF_vect and \c TIMER0_COMP_vect
		im_overflowEnable = 1,		///< Enable \c TIMER0_OVF_vect
		im_outputCompareAEnable = 2	///< Enable \c TIMER0_COMP_vect
	};
	
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(TIMER0_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER0_OVF_vect, Fxn, obj);
	}
	

}__attribute__((packed));


/**
 * TODO check this  (copy from at90can128)
 *	\brief		Register map for Timer2 of atmega128rfa1
 *	\ingroup	atmega128rfa1
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
		redirectISRM(TIMER2_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER2_OVF_vect, Fxn, obj);
	}
	
private:
	uint8_t __base [0x70];
	
public:
// TIMSK2 (0x70) {
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable \c TIMER2_OVF_vect and \c TIMER2_COMP_vect
		im_overflowEnable = 1,		///< Enable \c TIMER2_OVF_vect
		im_outputCompareAEnable = 2	///< Enable \c TIMER2_COMP_vect
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
}__attribute__((packed));



/**
 *	\brief		Register map for Timer1,3 and 4 of atmega128rfa1
 *	\ingroup	atmega128rfa1
 *
 *	\todo		Add input capture support
 */
template<typename desc> class TimerC
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
		fastPwm10 = 7,		///< fast pwm, 10 bit
				phaseFrequencyCorrectPwmICRn = 8, ///<pwm phase and frequency correct
		phaseFrequencyCorrectPwmOCRnA = 9, ///<pwm phase and frequency correct
		phaseCorrectPwmICRn = 10,  ///<pwm phase correct
		phaseCorrectPwmOCRnA = 11, ///<pwm phase correct
		ctcICRn = 12, ///<CRC
		reserved = 13, ///<reserved
		fastPwmICRn = 14, ///<fast pwm
		fastPwmOCRnA = 15 ///<fast pwm
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
	
	/// Interrupt mask bits
	enum
	{
		im_disable = 0,			///< Disable Timer1's interrupts
		im_overflowEnable = 1,		///< Enable \c TIMER1_OVF_vect
		im_outputCompareAEnable = 2,	///< Enable \c TIMER1_COMPA_vect
		im_outputCompareBEnable = 4,	///< Enable \c TIMER1_COMPB_vect
		im_outputCompareCEnable = 8,	///< Enable \c TIMER1_COMPC_vect
		im_inputCaptureEnable = 32	///< Enable \c TIMER1_CAPT_vect
	};
	
	
	union
	{
		struct
			{
				uint8_t __pad0[desc::tifr];
				union
				{
					struct
					{
						uint8_t tovf : 1;
						uint8_t ocaf : 1;
						uint8_t ocbf : 1;
						uint8_t occf : 1;
						uint8_t      : 1;
						uint8_t icf  : 1;
					};
					uint8_t tifr;
				};
			};
			struct
			{
				uint8_t __pad1[desc::timsk];
				union
				{
					struct
					{
						uint8_t toie  : 1;
						uint8_t ociea : 1;
						uint8_t ocieb : 1;
						uint8_t ociec : 1;
						uint8_t       : 1;
						uint8_t icie  : 1;
					};
					struct
					{
						// TIMSK (0x6f) 
					
						/// Interrupt mask (enables/disables Timer1's interrupts)
						uint8_t interruptMask : 6;
						bool : 2;
					};
					uint8_t timsk;
				};
			};
			struct
			{
				uint8_t __pad2[desc::tccr];
				union
				{
					struct
					{
						uint8_t wgm0 : 2;
						uint8_t comc : 2;
						uint8_t comb : 2;
						uint8_t coma : 2;
						uint8_t cs   : 3;
						uint8_t wgm1 : 2;
						uint8_t      : 1;
						uint8_t ices : 1;
						uint8_t icnc : 1;
						uint8_t      : 5;
						uint8_t focc : 1;
						uint8_t focb : 1;
						uint8_t foca : 1;
					};
					struct
					{
						// TCCRxA
						uint8_t _waveformGenerationMode01 : 2;
						uint8_t _compareMatchOutputModeC : 2;
						uint8_t _compareMatchOutputModeB : 2;
						uint8_t _compareMatchOutputModeA : 2;
						// TCCRxB (0x81)
						uint8_t _clockSelect : 3;
						uint8_t _waveformGenerationMode23 : 2;
						uint8_t : 3;
					};
					struct
					{
						uint8_t tccra;
						uint8_t tccrb;
						uint8_t tccrc;
					};
				};
			};
			struct
			{
				uint8_t __pad3[desc::tcnt];
				union
				{
					uint16_t tcnt;
					struct
					{
						uint8_t tcntl;
						uint8_t tcnth;
					};
				};
			};
			struct
			{
				uint8_t __pad4[desc::icr];
				union
				{
					uint16_t icr;
					struct
					{
						uint8_t icrl;
						uint8_t icrh;
					};
				};
			};
			struct
			{
				uint8_t __pad5[desc::ocr];
				union
				{
					uint16_t ocra;
					struct
					{
						uint8_t ocral;
						uint8_t ocrah;
					};
					struct{
						// OCR1AL (0x88)
						/// Output compare register A (low byte)
						uint8_t outputCompareA : 8;
						// OCR1AH (0x89)
						/// Output compare register A (high byte)
						uint8_t outputCompareAH : 8;
					};
				};
				union
				{
					uint16_t ocrb;
					struct
					{
						uint8_t ocrbl;
						uint8_t ocrbh;
					};
					struct{
						// OCR1BL (0x8a)
						/// Output compare register B (low byte)
						uint8_t outputCompareB : 8;
						// OCR1BH (0x8b)
						/// Output compare register B (high byte)
						uint8_t outputCompareBH : 8;
					};
				};
				union
				{
					uint16_t ocrc;
					struct
					{
						uint8_t ocrcl;
						uint8_t ocrch;
					};
					struct{
						// OCR1CL (0x8c)
						/// Output compare register C (low byte)
						uint8_t outputCompareC : 8;
						// OCR1CH (0x8d)
						/// Output compare register C (high byte)
						uint8_t outputCompareCH : 8;
					};
				};
			};
		};
public:	
}__attribute__((packed));

/**
 *	\brief		Register map for Timer1 of atmega128rfa1
 *	\ingroup	atmega128rfa1
 *
 *	\todo		Add input capture support
 */
struct Timer1Desc
{
// 	typedef interrupts::atmega128rfa1::Timer1IntMap IntMap;
	enum Addresses
	{
		tifr=0x36,
		timsk=0x6F,
		tccr=0x80,
		tcnt=0x84,
		icr=0x86,
		ocr=0x88,
	};
};
struct Timer1:public TimerC<Timer1Desc>
{
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(TIMER1_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(TIMER1_COMPB_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(TIMER1_COMPC_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER1_OVF_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(TIMER1_CAPT_vect, Fxn, obj);
	}
};


/**
 *	\brief		Register map for Timer3 of atmega128rfa1
 *	\ingroup	atmega128rfa1
 *
 *	\todo		Add input capture support
 */
struct Timer3Desc
{
// 	typedef interrupts::atmega128rfa1::Timer3IntMap IntMap;
	
	enum Addresses
	{
		tifr=0x38,
		timsk=0x71,
		tccr=0x90,
		tcnt=0x94,
		icr=0x96,
		ocr=0x98,
	};
};
struct Timer3:public TimerC<Timer3Desc>
{

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(TIMER3_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(TIMER3_COMPB_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(TIMER3_COMPC_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER3_OVF_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(TIMER3_CAPT_vect, Fxn, obj);
	}

}__attribute__((packed));

struct Timer4Desc
{
// 	typedef interrupts::atmega128rfa1::Timer4IntMap IntMap;
	enum Addresses
	{
		tifr=0x39,
		timsk=0x72,
		tccr=0xA0,
		tcnt=0xA4,
		icr=0xA6,
		ocr=0xA8,
	};
};
struct Timer4:public TimerC<Timer4Desc>
{

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(TIMER4_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(TIMER4_COMPB_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(TIMER4_COMPC_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER4_OVF_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(TIMER4_CAPT_vect, Fxn, obj);
	}

}__attribute__((packed));

struct Timer5Desc
{
// 	typedef interrupts::atmega128rfa1::Timer5IntMap IntMap;
	
	enum Addresses
	{
		tifr=0x3A,
		timsk=0x73,
		tccr=0x120,
		tcnt=0x124,
		icr=0x126,
		ocr=0x128,
	};
};
struct Timer5:public TimerC<Timer5Desc>
{

	template<class T, void (T::*Fxn)()>
	static void setOutputCompareAInterrupt(T & obj)
	{
		redirectISRM(TIMER5_COMPA_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareBInterrupt(T & obj)
	{
		redirectISRM(TIMER5_COMPB_vect, Fxn, obj);
	}
	
	template<class T, void (T::*Fxn)()>
	static void setOutputCompareCInterrupt(T & obj)
	{
		redirectISRM(TIMER5_COMPC_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setOverflowInterrupt(T & obj)
	{
		redirectISRM(TIMER5_OVF_vect, Fxn, obj);
	}

	template<class T, void (T::*Fxn)()>
	static void setInputCaptureInterrupt(T & obj)
	{
		redirectISRM(TIMER5_CAPT_vect, Fxn, obj);
	}

}__attribute__((packed));
}