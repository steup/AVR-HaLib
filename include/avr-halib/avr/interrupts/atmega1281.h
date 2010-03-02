/*! \addtogroup atmega1281 */
/*@{*/
/*! \file   include/avr-halib/avr/interrupts/atmega1281.h
 *  \brief  Defines atmega1281 interrupt classes
 *
 *  This file is part of avr-halib. See COPYING for copyright details.
 */
#pragma once

#include "avr-halib/avr/interrupt.h"

DefineInterrupt( SIG_INTERRUPT0 );          // External Interrupt 0
DefineInterrupt( SIG_INTERRUPT1 );          // External Interrupt 1
DefineInterrupt( SIG_INTERRUPT2 );          // External Interrupt 2
DefineInterrupt( SIG_INTERRUPT3 );          // External Interrupt 3
DefineInterrupt( SIG_INTERRUPT4 );          // External Interrupt 4
DefineInterrupt( SIG_INTERRUPT5 );          // External Interrupt 5
DefineInterrupt( SIG_INTERRUPT6 );          // External Interrupt 6
DefineInterrupt( SIG_INTERRUPT7 );          // External Interrupt 7
DefineInterrupt( SIG_PIN_CHANGE0 );         // Pin Change Interrupt Request 0
DefineInterrupt( SIG_PIN_CHANGE1 );         // Pin Change Interrupt Request 1
DefineInterrupt( SIG_PIN_CHANGE2 );         // Pin Change Interrupt Request 2
DefineInterrupt( SIG_ADC );                 // ADC Conversion Complete
DefineInterrupt( SIG_COMPARATOR );          // Analog Comparator
DefineInterrupt( SIG_EEPROM_READY );        // EEPROM Ready
DefineInterrupt( SIG_SPI );                 // Serial Transfer Complete
DefineInterrupt( SIG_SPM_READY );           // Store Program Memory Read
DefineInterrupt( SIG_2WIRE_SERIAL );        // 2-wire Serial Interface
DefineInterrupt( SIG_UART0_RECV );          // USART0 Rx Complete
DefineInterrupt( SIG_UART0_TRANS );         // USART0 Tx Complete
DefineInterrupt( SIG_UART0_DATA );          // USART0 Data Register Empty
DefineInterrupt( SIG_UART1_RECV );          // USART1 Rx Complete
DefineInterrupt( SIG_UART1_TRANS );         // USART1 Tx Complete
DefineInterrupt( SIG_UART1_DATA );          // USART1 Data Register Empty
DefineInterrupt( SIG_USART2_RECV );         // USART2 Rx Complete
DefineInterrupt( SIG_USART2_TRANS );        // USART2 Tx Complete
DefineInterrupt( SIG_USART2_DATA );         // USART2 Data register Empty
DefineInterrupt( SIG_USART3_RECV );         // USART3 Rx Complete
DefineInterrupt( SIG_USART3_TRANS );        // USART3 Tx Complete
DefineInterrupt( SIG_USART3_DATA );         // USART3 Data register Empty
DefineInterrupt( SIG_WATCHDOG_TIMEOUT );    // Watchdog Timeout Interrupt
DefineInterrupt( SIG_OUTPUT_COMPARE0A );    // Timer/Counter0 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE0B );    // Timer/Counter0 Compare Match B
DefineInterrupt( SIG_OVERFLOW0 );           // Timer/Counter0 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE1 );      // Timer/Counter1 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE1A );    // Timer/Counter1 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE1B );    // Timer/Counter1 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE1C );    // Timer/Counter1 Compare Match C
DefineInterrupt( SIG_OVERFLOW1 );           // Timer/Counter1 Overflow
DefineInterrupt( SIG_OUTPUT_COMPARE2A );    // Timer/Counter2 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE2B );    // Timer/Counter2 Compare Match A
DefineInterrupt( SIG_OVERFLOW2 );           // Timer/Counter2 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE3 );      // Timer/Counter3 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE3A );    // Timer/Counter3 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE3B );    // Timer/Counter3 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE3C );    // Timer/Counter3 Compare Match C
DefineInterrupt( SIG_OVERFLOW3 );           // Timer/Counter3 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE4 );      // Timer/Counter4 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE4A );    // Timer/Counter4 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE4B );    // Timer/Counter4 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE4C );    // Timer/Counter4 Compare Match C
DefineInterrupt( SIG_OVERFLOW4 );           // Timer/Counter4 Overflow
DefineInterrupt( SIG_INPUT_CAPTURE5 );      // Timer/Counter5 Capture Event
DefineInterrupt( SIG_OUTPUT_COMPARE5A );    // Timer/Counter5 Compare Match A
DefineInterrupt( SIG_OUTPUT_COMPARE5B );    // Timer/Counter5 Compare Match B
DefineInterrupt( SIG_OUTPUT_COMPARE5C );    // Timer/Counter5 Compare Match C
DefineInterrupt( SIG_OVERFLOW5 );           // Timer/Counter5 Overflow
/*@}*/
