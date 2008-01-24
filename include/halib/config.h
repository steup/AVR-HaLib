/**	\file	halib/config.h
 *	\brief	Konfiguration der Hardware-Bibliothek, Registerbreiten und andere Hardwarespzifika
 *
 */

#pragma once
#include <stdint.h>

//#define CYGARC_ALIGNMENT 4
//#define CYGARC_P2ALIGNMENT 2



// #define UART_BAUDRATE 2400
// #define UART_BAUDRATE 4800
// #define UART_BAUDRATE 9600
// #define UART_BAUDRATE 14400
#define UART_BAUDRATE 19200
// #define UART_BAUDRATE 28800
// #define UART_BAUDRATE 38400
// #define UART_BAUDRATE 57600
// #define UART_BAUDRATE 115200


// Ermï¿½licht das zeitweilige ï¿½erschreiben der Interrupt-Handler
// (merkt sich den ISR-Verweis beim construct von Interrupt-Handler-Objekten
// und stellt ihn beim destruct wieder her)
//#define USE_DYNAMIC_INTERRUPT_HANDLING


// Schaltet Klasse Led frei
#define USE_LED

// Schaltet Klasse LedBlock frei
#define USE_LEDBLOCK

// Schaltet Klasse Button frei
#define USE_BUTTON

// Schaltet Klasse Buttonblock frei
//#define USE_BUTTONBLOCK



#ifdef __AVR_ATmega32__
	typedef uint8_t port_t;
	typedef uint8_t register_t;
#	define CPU_FREQUENCY 8000000
#else 
#ifdef __AVR_AT90CAN128__
	typedef uint8_t port_t;
	typedef uint8_t register_t;
#	define CPU_FREQUENCY 16000000
#else	
#	error "device type not defined"
#endif
#endif

