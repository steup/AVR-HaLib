// This file is part of avr-halib. See COPYING for copyright details.
 
#pragma once

#include "avr-halib/avr/interrupt.h"

DefineInterrupt( SIG_INTERRUPT0 );		// External Interrupt Request 0
DefineInterrupt( SIG_INTERRUPT1 );		// External Interrupt Request 1
DefineInterrupt( SIG_INTERRUPT2 );		// External Interrupt Request 2
DefineInterrupt( SIG_OUTPUT_COMPARE2 );		// Timer/Counter2 Compare Match
DefineInterrupt( SIG_OVERFLOW2 );		// Timer/Counter2 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE1 );		// Timer/Counter1 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE1A );	// Timer/Counter1 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE1B );	// Timer/Counter1 Compare Match B
DefineInterrupt( SIG_OVERFLOW1 );		// Timer/Counter1 Overflow
DefineInterrupt( SIG_OUTPUT_COMPARE0 );		// Timer/Counter0 Compare Match
DefineInterrupt( SIG_OVERFLOW0 );		// Timer/Counter0 Overflow
DefineInterrupt( SIG_SPI );			// Serial Transfer Complete
DefineInterrupt( SIG_UART_RECV );		// USART Rx Complete
DefineInterrupt( SIG_UART_DATA );		// USART Data Register Empty
DefineInterrupt( SIG_USART_TRANS );		// USART Tx Complete
DefineInterrupt( SIG_ADC );			// ADC Conversion Complete
DefineInterrupt( SIG_EEPROM_READY );		// EEPROM Ready
DefineInterrupt( SIG_COMPARATOR );		// Analog Comparator
DefineInterrupt( SIG_2WIRE_SERIAL );		// 2-wire Serial Interface
DefineInterrupt( SIG_SPM_READY );		// Store Program Memory Ready

