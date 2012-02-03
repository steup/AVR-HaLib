/** \addtogroup atmega32 */
/*@{*/
/**
 *	\file	include/avr-halib/avr/interrupts/atmega32.h
 *	\brief	Defines atmega32 interrupt classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

// This file is part of avr-halib. See COPYING for copyright details.
 
#pragma once

#include "avr-halib/avr/interrupt.h"
/* External Interrupt Request 0 */
DefineInterrupt(  INT0_vect );
/* External Interrupt Request 1 */
DefineInterrupt(  INT1_vect );
/* External Interrupt Request 2 */
DefineInterrupt(  INT2_vect );
/* Timer/Counter2 Compare Match */
DefineInterrupt(  TIMER2_COMP_vect );
/* Timer/Counter2 Overflow */
DefineInterrupt(  TIMER2_OVF_vect );
/* Timer/Counter1 Capture Event */
DefineInterrupt(  TIMER1_CAPT_vect );
/* Timer/Counter1 Compare Match A */
DefineInterrupt(  TIMER1_COMPA_vect );
/* Timer/Counter1 Compare Match B */
DefineInterrupt(  TIMER1_COMPB_vect );
/* Timer/Counter1 Overflow */
DefineInterrupt(  TIMER1_OVF_vect );
/* Timer/Counter0 Compare Match */
DefineInterrupt(  TIMER0_COMP_vect );
/* Timer/Counter0 Overflow */
DefineInterrupt(  TIMER0_OVF_vect );
/* Serial Transfer Complete */
DefineInterrupt(  SPI_STC_vect );
/* USART, Rx Complete */
DefineInterrupt(  USART_RXC_vect );
/* USART Data Register Empty */
DefineInterrupt(  USART_UDRE_vect );
/* USART, Tx Complete */
DefineInterrupt(  USART_TXC_vect );
/* ADC Conversion Complete */
DefineInterrupt(  ADC_vect );
/* EEPROM Ready */
DefineInterrupt(  EE_RDY_vect );
/* Analog Comparator */
DefineInterrupt(  ANA_COMP_vect );
/* 2-wire Serial Interface */
DefineInterrupt(  TWI_vect );
/* Store Program Memory Ready */
DefineInterrupt(  SPM_RDY_vect );
/*@}*/
