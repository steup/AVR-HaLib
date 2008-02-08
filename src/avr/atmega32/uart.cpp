/**
 *	\file	halib/atmega32/uart.cpp
 *	\brief	Implementiert die Uart-Schnittstelle (nicht-template-Bestandteile) des ATmega32.
 *
 *
 */


IMPLEMENT_INTERRUPT_CLASS(SIG_UART_RECV, InterruptUartRecv)
IMPLEMENT_INTERRUPT_CLASS(SIG_UART_DATA, InterruptUartData)
