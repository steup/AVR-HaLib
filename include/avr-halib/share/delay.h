/** \addtogroup share */
/*@{*/
/**
 *	\file	avr-halib/share/delay.h
 *	\brief	Busy waiting functions
 *	\author	Philipp Werner
 *
 *	\todo	At the moment this file can be only included once in all of your files! Solve this problem! Replace F_CPU define with enum...
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once
#include <stdint.h>


// #if defined ALWAYS_INLINE_DELAY
// void delay_ms(uint16_t ms) __attribute__ ((always_inline));
// #elif defined NO_INLINE_DELAY
// void delay_ms(uint16_t ms) __attribute__ ((naked,noinline));
// #else
void delay_ms(uint16_t ms);
// #endif
#ifndef DELAYNOIMPL
/**	\brief	Busy waiting for x milliseconds
 *	\param	ms	Time to wait in ms
 *	Produces only 20 Byte maschine code!
 *
 */
void delay_ms(uint16_t ms)
{
//  	for (; ms; ms--)
//  		for (register uint16_t c = (uint16_t)(F_CPU / 5000); c; c--)	// wait for one ms
// 			;
	asm volatile
	(
		"rjmp    .+12           	; jump to l3			\n"
/* l1: */	"ldi     r18, lo8(%1)   	; load loop count		\n"
		"ldi     r19, hi8(%1)    					\n"
/* l2: */	"subi    r18, 0x01		; decrement (16 bit)		\n"
		"sbci    r19, 0x00						\n"
		"brne    .-6			; jump to l2 if not zero	\n"
		"sbiw    %0, 0x01		; decrement (16 bit)		\n"
/* l3: */	"sbiw    %0, 0x00						\n"
		"brne    .-16			; jump to l1 if not zero	\n"
// #if defined NO_INLINE_DELAY		
//		"ret"
// #endif
		:				// no output
		: "w" (ms), "i" (F_CPU / 4000)	// input: ms to wait and loop runs per ms
		: "r18", "r19"
	);
}
#endif
// #if defined ALWAYS_INLINE_DELAY
// void delay_us(uint16_t ms) __attribute__ ((always_inline));
// #elif defined NO_INLINE_DELAY
// void delay_us(uint16_t ms) __attribute__ ((naked,noinline));
// #else
void delay_us(uint16_t ms);
// #endif
#ifndef DELAYNOIMPL
/**	\brief	Busy waiting for x mircoseconds
 *	\param	us	Time to wait in us
 *	Produces only 20 Byte maschine code!
 */
void delay_us(uint16_t us)
{
//  	for (; us; us--)
//  		for (register uint16_t c = (uint16_t)(F_CPU / 5000); c; c--)	// wait for one us
// 			;
	asm volatile
	(
		"rjmp    .+12           	; jump to l3			\n"
/* l1: */	"ldi     r18, lo8(%1)   	; load loop count		\n"
		"ldi     r19, hi8(%1)    					\n"
/* l2: */	"subi    r18, 0x01		; decrement (16 bit)		\n"
		"sbci    r19, 0x00						\n"
		"brne    .-6			; jump to l2 if not zero	\n"
		"sbiw    %0, 0x01		; decrement (16 bit)		\n"
/* l3: */	"sbiw    %0, 0x00						\n"
		"brne    .-16			; jump to l1 if not zero	\n"
// #if defined NO_INLINE_DELAY
//		"ret"
// #endif
		:				// no output
		: "w" (us), "i" (F_CPU / 4000 / 1000)	// input: ms to wait and loop runs per ms
		: "r18", "r19"
	);
}
#endif
/*@}*/
