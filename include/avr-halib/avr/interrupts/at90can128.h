/** \addtogroup at90can128 */
/*@{*/
/**
 *	\file	include/avr-halib/avr/interrupts/at90can128.h
 *	\brief	Defines at90can128 interrupt classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once
#include "avr-halib/avr/interrupt.h"
/* External Interrupt Request 0 */
DefineInterrupt(  INT0_vect );
/* External Interrupt Request 1 */
DefineInterrupt(  INT1_vect );
/* External Interrupt Request 2 */
DefineInterrupt(  INT2_vect );
/* External Interrupt Request 3 */
DefineInterrupt(  INT3_vect );
/* External Interrupt Request 4 */
DefineInterrupt(  INT4_vect );
/* External Interrupt Request 5 */
DefineInterrupt(  INT5_vect );
/* External Interrupt Request 6 */
DefineInterrupt(  INT6_vect );
/* External Interrupt Request 7 */
DefineInterrupt(  INT7_vect );
/* Timer/Counter2 Compare Match */
DefineInterrupt(  TIMER2_COMP_vect );
/* Timer/Counter2 Overflow */
DefineInterrupt(  TIMER2_OVF_vect );
/* Timer/Counter1 Capture Event */
DefineInterrupt(  TIMER1_CAPT_vect );
/* Timer/Counter1 Compare Match A */
DefineInterrupt(  TIMER1_COMPA_vect );
/* Timer/Counter Compare Match B */
DefineInterrupt(  TIMER1_COMPB_vect );
/* Timer/Counter1 Compare Match C */
DefineInterrupt(  TIMER1_COMPC_vect );
/* Timer/Counter1 Overflow */
DefineInterrupt(  TIMER1_OVF_vect );
/* Timer/Counter0 Compare Match */
DefineInterrupt(  TIMER0_COMP_vect );
/* Timer/Counter0 Overflow */
DefineInterrupt(  TIMER0_OVF_vect );
/* CAN Transfer Complete or Error */
DefineInterrupt(  CANIT_vect );
/* CAN Timer Overrun */
DefineInterrupt(  OVRIT_vect );
/* SPI Serial Transfer Complete */
DefineInterrupt(  SPI_STC_vect );
/* USART0, Rx Complete */
DefineInterrupt(  USART0_RX_vect );
/* USART0 Data Register Empty */
DefineInterrupt(  USART0_UDRE_vect );
/* USART0, Tx Complete */
DefineInterrupt(  USART0_TX_vect );
/* Analog Comparator */
DefineInterrupt(  ANALOG_COMP_vect );
/* ADC Conversion Complete */
DefineInterrupt(  ADC_vect );
/* EEPROM Ready */
DefineInterrupt(  EE_READY_vect );
/* Timer/Counter3 Capture Event */
DefineInterrupt(  TIMER3_CAPT_vect );
/* Timer/Counter3 Compare Match A */
DefineInterrupt(  TIMER3_COMPA_vect );
/* Timer/Counter3 Compare Match B */
DefineInterrupt(  TIMER3_COMPB_vect );
/* Timer/Counter3 Compare Match C */
DefineInterrupt(  TIMER3_COMPC_vect );
/* Timer/Counter3 Overflow */
DefineInterrupt(  TIMER3_OVF_vect );
/* USART1, Rx Complete */
DefineInterrupt(  USART1_RX_vect );
/* USART1, Data Register Empty */
DefineInterrupt(  USART1_UDRE_vect );
/* USART1, Tx Complete */
DefineInterrupt(  USART1_TX_vect );
/* 2-wire Serial Interface */
DefineInterrupt(  TWI_vect );
/* Store Program Memory Read */
DefineInterrupt(  SPM_READY_vect );
/*@}*/
