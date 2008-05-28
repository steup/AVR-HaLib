/**
 *	\file	avr-halib/at90can128/uart.cpp
 *	\brief	Implementiert die Uart-Schnittstelle (nicht-template-Bestandteile) des ATmega32.
 *
 *
 */


IMPLEMENT_INTERRUPT_CLASS(SIG_UART0_RECV, InterruptUartRecv)
IMPLEMENT_INTERRUPT_CLASS(SIG_UART0_DATA, InterruptUartData)
