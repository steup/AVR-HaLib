 
#pragma once

#include "halib/avr/interrupt.h"

DefineInterrupt( SIG_INTERRUPT0 );		// External Interrupt Request 0
DefineInterrupt( SIG_INTERRUPT1 );		// External Interrupt Request 1
DefineInterrupt( SIG_INTERRUPT2 );		// External Interrupt Request 2
DefineInterrupt( SIG_INTERRUPT3 );		// External Interrupt Request 3
DefineInterrupt( SIG_INTERRUPT4 );		// External Interrupt Request 4
DefineInterrupt( SIG_INTERRUPT5 );		// External Interrupt Request 5
DefineInterrupt( SIG_INTERRUPT6 );		// External Interrupt Request 6
DefineInterrupt( SIG_INTERRUPT7 );		// External Interrupt Request 7
DefineInterrupt( SIG_OUTPUT_COMPARE2 );		// Timer/Counter2 Compare Match
DefineInterrupt( SIG_OVERFLOW2 );		// Timer/Counter2 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE1 );		// Timer/Counter1 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE1A );	// Timer/Counter1 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE1B );	// Timer/Counter1 Compare Match B
DefineInterrupt( SIG_OVERFLOW1 );		// Timer/Counter1 Overflow
DefineInterrupt( SIG_OUTPUT_COMPARE0 );		// Timer/Counter0 Compare Match
DefineInterrupt( SIG_OVERFLOW0 );		// Timer/Counter0 Overflow
DefineInterrupt( SIG_SPI );			// Serial Transfer Complete
DefineInterrupt( SIG_UART0_RECV );		// USART0 Rx Complete
DefineInterrupt( SIG_UART0_DATA );		// USART0 Data Register Empty
DefineInterrupt( SIG_USART0_TRANS );		// USART0 Tx Complete
DefineInterrupt( SIG_UART1_RECV );		// USART1 Rx Complete
DefineInterrupt( SIG_UART1_DATA );		// USART1 Data Register Empty
DefineInterrupt( SIG_USART1_TRANS );		// USART1 Tx Complete
DefineInterrupt( SIG_ADC );			// ADC Conversion Complete
DefineInterrupt( SIG_EEPROM_READY );		// EEPROM Ready
DefineInterrupt( SIG_COMPARATOR );		// Analog Comparator
DefineInterrupt( SIG_2WIRE_SERIAL );		// 2-wire Serial Interface
DefineInterrupt( SIG_SPM_READY );		// Store Program Memory Ready
DefineInterrupt( SIG_CAN_INTERRUPT1 );		// CAN Transfer Complete or Error
DefineInterrupt( SIG_CAN__OVERFLOW1 );		// CAN Timer Overrun
