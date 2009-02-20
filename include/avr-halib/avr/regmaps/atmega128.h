/** \addtogroup atmega128 */
/*@{*/
/**
 *	\file	include/avr-halib/avr/regmaps/atmega128.h
 *	\brief	Contains atmega128 specific stuff
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/interrupt.h"

#include <stdint.h>

/**
 *	\brief	Namespace containing atmega128 specific stuff
 *
 *	Content in global namespace as well if compiling for at90can128.
 */
namespace atmega128
{
class __DefineController
{
	public:
		enum
	{
		controllerClk=CPU_FREQUENCY
	};

};
//Begin UART (not tested)
template <class _Controller_Configuration = __DefineController> class _Uart0w
{
public:
	typedef _Controller_Configuration Controller_Configuration;
union{
	struct{
			uint8_t __base_ucsra [0x2b];
			union{
				uint8_t ucsra;
				struct{
					bool	mpcm:1;
					bool	u2x:1;
					bool	pe:1;
					bool	dor:1;
					bool	fe:1;
					bool	udre:1;
					bool	txc:1;
					bool	rxc:1;
				};
			};
	};
	struct{
			uint8_t __base_ucsrb [0x2a];
			union{
				uint8_t ucsrb;
				struct{
					bool	txb8:1;
					bool	rxb8:1;
					bool	ucsz2:1;
					bool	txen:1;
					bool	rxen:1;
					bool	udrie:1;
					bool	txcie:1;
					bool	rxcie:1;
				};
			};
	};
	struct{
			uint8_t __base_ucsrc [0x95];
			union{
				uint8_t ucsrc;
				struct {
					bool	ucpol:1;
					bool	ucsz0:1;
					bool	ucsz1:1;
					bool	usbs:1;
					uint8_t	upm:2;
					bool	umsel:1;
					bool	:1;//reserved
				};
			};
	};
	
	struct{
			uint8_t __base_ubbrl [0x29];
			uint8_t ubrrl;
	};
	struct{
			uint8_t __base_ubbrh [0x90];
			uint8_t ubrrh;
	};
	struct{
			uint8_t __base_udr [0x2c];
			uint8_t udr;
	};
};
		
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
template <class _Controller_Configuration = __DefineController> class _Uart1w
{
public:
	typedef _Controller_Configuration Controller_Configuration;
union{
	struct{
			uint8_t __base_ucsra [0x9b];
			union{
				uint8_t ucsra;
				struct{
					bool	mpcm:1;
					bool	u2x:1;
					bool	pe:1;
					bool	dor:1;
					bool	fe:1;
					bool	udre:1;
					bool	txc:1;
					bool	rxc:1;
				};
			};
	};
	struct{
			uint8_t __base_ucsrb [0x9a];
			union{
				uint8_t ucsrb;
				struct{
					bool	txb8:1;
					bool	rxb8:1;
					bool	ucsz2:1;
					bool	txen:1;
					bool	rxen:1;
					bool	udrie:1;
					bool	txcie:1;
					bool	rxcie:1;
				};
			};
	};
	struct{
			uint8_t __base_ucsrc [0x9d];
			union{
				uint8_t ucsrc;
				struct {
					bool	ucpol:1;
					bool	ucsz0:1;
					bool	ucsz1:1;
					bool	usbs:1;
					uint8_t	upm:2;
					bool	umsel:1;
					bool	:1;//reserved
				};
			};
	};
	
	struct{
			uint8_t __base_ubbrl [0x99];
			uint8_t ubrrl;
	};
	struct{
			uint8_t __base_ubbrh [0x98];
			uint8_t ubrrh;
	};
	struct{
			uint8_t __base_udr [0x9c];
			uint8_t udr;
	};
};
		
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
template< class _Uartw = _Uart0w<> > class _Uartw_commons: public _Uartw
{	
	public:
	enum{noParity=0x00,evenParity=0x2,oddParity=0x3};
	typedef class _Uartw::Controller_Configuration Controller_Configuration;
			
	template<uint8_t databits,char parity,uint8_t stopbits, bool syncronous> void configure()
	{
		_Uartw::ucsrc = 0;
		_Uartw::umsel = syncronous;
		_Uartw::ucsz2  = (databits==9);
		_Uartw::ucsz1 = (databits>6);
		_Uartw::ucsz0 = (databits != 5 && databits != 7);
		_Uartw::usbs = (stopbits==2);
		_Uartw::upm = parity=='N'?(noParity):(parity=='E'?(evenParity):(parity=='O'?(oddParity):parity));
	}
	
	template<uint8_t databits,char parity,uint8_t stopbits> void configure()
	{
		_Uartw::ucsrc = 0;
		_Uartw::umsel = false;
		_Uartw::ucsz2  = (databits==9);
		_Uartw::ucsz1 = (databits>6);
		_Uartw::ucsz0 = (databits != 5 && databits != 7);
		_Uartw::usbs = (stopbits==2);
		_Uartw::upm = parity=='N'?(noParity):(parity=='E'?(evenParity):(parity=='O'?(oddParity):parity));
	}
	
	
	void setbaudrate(uint32_t baudrate)
	{
		union{
				uint16_t ubrr;
				struct{
					uint8_t ubrrl;
					uint8_t ubrrh;
				};
			}ub;
		ub.ubrr=(Controller_Configuration::controllerClk/16/baudrate)-1;
		_Uartw::ubrrh=ub.ubrrh;
		_Uartw::ubrrl=ub.ubrrl;
		
	}
};

template  <class _CC = __DefineController> class Uart0w: public _Uartw_commons<_Uart0w<_CC> >{};
template  <class _CC = __DefineController> class Uart1w: public _Uartw_commons<_Uart1w<_CC> >{};

struct Uart0:public Uart0w<>
{
// 	typedef RBoardController Controller_Configuration;
	enum{baudrate=19200};
};
struct Uart1:public Uart1w<>
{
// 	typedef RBoardController Controller_Configuration;
	enum{baudrate=19200};
};
//End UART
	
	
//Begin ADconv
/**
 *	\brief		Register map for ADConv of atmega128
 *	\param		Controller_Configuration	TODO
 *	\ingroup	atmega128
 *
 */

template <class Controller_Configuration> struct ADConv
{
private:
	uint8_t __base[0x24] ;
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
// 	union{
// 		bool	adfr: 1;
		bool	adate:1;// adate is the new name used in at90can128 which support more freerunning modes
// 	};
	bool	adsc : 1;
	bool	aden : 1;
	union
	{
		uint8_t  admux	:8;
		struct{
			uint8_t  mux	:5;
			bool	 adlra	:1;
			uint8_t  refs	:2;
		};
	};
	enum {ps2 = 1, ps4 = 2, ps8 = 3, ps16 = 4, ps32 = 5, ps64 = 6, ps128 = 7,ps_none };
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

//End ADconv

/**
 *	\brief		Register map for usage of external interrupts
 *	\ingroup	atmega128
 */
class ExternalInterrupts
{
	union{
		struct
		{
			//EIFR TODO
				uint8_t __base_efir [0x58];
				uint8_t efir; 
		};
		struct
		{
				uint8_t __base_eimsk [0x59];
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
		};
		struct{
				uint8_t __base_eicra [0x6A]; 
			// EICRA (0x6A) {
				uint8_t senseInt0 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt1 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt2 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt3 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
			// }
		
		};
		struct{
				uint8_t __base_eicrb [0x5A];
			// EICRB (0xA) {
				uint8_t senseInt4 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt5 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt6 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
				uint8_t senseInt7 : 2;	///< Write 0 for int reqest on low level, 2 for int reqest on falling edge and 3 for int reqest on rising edge
			// }
		};
	};	
};


}// end of namespace 

#if defined(__AVR_ATmega128__)
using namespace atmega128;
#endif

/*@}*/

