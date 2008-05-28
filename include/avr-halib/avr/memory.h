/**
 *	\file	include/avr-halib/avr/memory.h
 *	\brief	Defines SyncMem and SyncObj macro
 *
 */
 
#pragma once

/**
 *	\brief	Synchronize memory
 *
 *	Use this macro to set a point of synchronization which will be also hold with optimizations turned on.
 *	The compiler will finish all actions that are in your code before this line and write back changed variables to RAM.
 *	Behind this macro the compiler has no assumptions about the memory content any more. In this way optimzations by the
 *	compiler are only done between two synchronization points and not beyond them.
 *
 *	\see	SyncPortmap
 *	\see	SyncRegmap
 */
#define SyncMem asm("":::"memory")


/**
 *	\brief	Synchronize object
 *
 *	Use this macro to set a point of synchronization which will be also hold with optimizations turned on.
 *	The compiler will finish all actions concerning \p var that are in your code before this line.
 *	Behind this macro the compiler has no assumptions about the portmap content any more. In this way optimzations by the
 *	compiler are only done between two synchronization points and not beyond them.
 *
 *	Use it before you read data that may have changed without compilers knowledge, like by interrupts.
 *	Otherwise the compiler may optimize your code away because he does not
 *	know that some memory can be changed outside the operational sequence of your program. Using this synchronization
 *	method does not generate extra code, but it allows the compiler to do more optimizations than making the whole portmap
 *	volatile.
 *
 *	\see	SyncMem
 */	
#define SyncObj(var) SyncMem
//#define SyncObj(var) asm("":"=m" (var):)
