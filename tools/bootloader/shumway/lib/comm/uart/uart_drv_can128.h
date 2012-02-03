/* 
 * Shumway, a CAN-bootloader for Atmel AVR microcontoller. 
 * Copyright (C) 2006 Joerg Diederich, Dipl. Inform. Michael Schulze 
 * Embedded Systems and Operating Systems 
 * Department of Distributed Systems (IVS)
 * Faculty of Computer Science
 * Otto-von-Guericke-University Magdeburg, Germany
 *
 * joerg.diederich@graffiti.net
 * mschulze@ivs.cs.uni-magdeburg.de 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2 
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA. 
 */

/*$Id: uart_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file uart_drv_can128.h
 * \brief MCU specific UART driver.
 * 
 * UART driver for the AT90CAN128 mcu.
 */

#ifndef __UART_DRV_CAN128_H
#define __UART_DRV_CAN128_H

#include <avr/io.h>

#include "mcu.h"
using namespace mcu;


namespace uart
{
/*!
 * \def GET_BAUD_RATE(fosc,baud_rate)
 *
 * Computes the register setting for a desired baud rate with a given 
 * mcu clock.
 */
#define GET_BAUD_RATE(fosc,baud_rate)			\
  (((((((uint32_t)fosc*1000)<<1)/			\
      ((uint32_t)((uint32_t)baud_rate)*8))+1)>>1)-1)

  /*!
   * \brief UART ports.
   *
   * Identifier constants for UART ports.
   */
  enum PORT{
    UART0,
    UART1,
  };

  /*!
   * \brief UART modes.
   *
   * Identifier for UART mode register settings.
   */
  enum MODE{
    RX_ONLY = 0x10, /*!< Receive-only mode. */
    TX_ONLY = 0x08, /*!< transmit-only mode. */
    RXTX = 0x18, /*!< Receive and transmit mode. */
  };


  /*!
   * \brief UART baud rates.
   *
   * Identifier for UART baud rate register settings. In bit/s.
   */
  enum BAUD_RATE{
    BAUD_RATE_19200 = GET_BAUD_RATE(mcu::FOSC,19200),
    BAUD_RATE_115200 = GET_BAUD_RATE(mcu::FOSC,115200)
  };

  /*!
   * \brief UART parity settings.
   *
   * Identifier for UART parity register settings. 
   */
  enum PARITY{
    PARITY_NONE = 0x00,
    PARITY_EVEN = 0x20,
    PARITY_ODD = 0x30,
  };

  /*!
   * \brief UART stop bit settings.
   *
   * Identifier for UART stop bit register settings. 
   */
  enum STOP_BITS{
    STOP_BITS_1 = 0x00,
    STOP_BITS_2 = 0x80
  };

  /*!
   * \brief UART data bit settings.
   *
   * Identifier for UART data bit register settings. 
   */
  enum DATA_BITS{
    DATA_BITS_5 = 0x00,
    DATA_BITS_6 = 0x02,
    DATA_BITS_7 = 0x04,
    DATA_BITS_8 = 0x0C,
    DATA_BITS_9 = 0x04
  };

  /*!
   * \brief Transmit character.
   *
   * Transmit single character via UART port.
   *
   * \param[in] p UART port to use.
   * \param[in] c Character to transmit.
   */
  static inline void transmit(PORT p, char c){
    if ( p == UART0 ){
      loop_until_bit_is_set(UCSR0A, UDRE0);
      UDR0 = c;
    }
    else{
      loop_until_bit_is_set(UCSR1A, UDRE1);
      UDR1 = c;
    }
  }

  /*!
   * \brief Transmit string.
   *
   * Transmit string via UART port.
   *
   * \param[in] p UART port to use.
   * \param[in] s String to transmit. Have to be null-terminated.
   * 
   * \note Overloaded. Not for pure C, but there namespaces are not available 
   * either.
   */
  static inline void transmit(PORT p, char *s){
    while(*s){
      transmit(p, *s++);
    }
  }
  
  /*!
   * \brief Clear UART port.
   *
   * Clears UART port. Disables irq and transfer modes. Resets transmit and 
   * receive flags. Enables 8 bit parity, 1 stop bit, async operation.
   * Diables baudrate setting.
   *
   * \param[in] p UART port.
   */
  static inline void clear_port(PORT p){
    if ( p == UART0 ){
      UCSR0B = 0;
      UCSR0A = 0x40;
      UCSR0C = 0x06;
      UBRR0H = 0;
      UBRR0L = 0;
    }
    else{
      UCSR1B = 0;
      UCSR1A = 0x40;
      UCSR1C = 0x06;
      UBRR1H = 0;
      UBRR1L = 0;
    }
  }

  /*!
   * \brief Set baudrate.
   *
   * Sets baudrate of UART port according to the given argument.
   *
   * \param[in] p UART port.
   * \param[in] br Baudrate.
   */
  static inline void set_baudrate(PORT p, BAUD_RATE br){
    if ( p == UART0 ){
      UBRR0H = (uint8_t)br >> 8;
      UBRR0L = (uint8_t)br;
      UCSR0A |= 0x02;
    }
    else{
      UBRR1H = (uint8_t)br >> 8;
      UBRR1L = (uint8_t)br;
      UCSR1A |= 0x02;
    }
  }

  /*!
   * \brief Enable UART port.
   *
   * Enables UART port in the given mode.
   *
   * \param[in] p UART port.
   * \param[in] mode UART mode.
   */
  static inline void enable(PORT p, MODE mode){
    if ( p == UART0 ){
      UCSR0B |= (uint8_t) mode;
    }
    else{
      UCSR1B |= (uint8_t) mode;
    }
  }

  /*!
   * \brief Inittialise UART port.
   *
   * Initialise UART port. Enables default baudrate of 19200 bit/s and 
   * receive and transmit mode.
   *
   * \param[in] p UART port.
   */
  static inline void init(PORT p){
    clear_port(p);
    set_baudrate(p, BAUD_RATE_19200);
    enable(p, RXTX);
  }

} // namespace uart 

#endif /* __UART_DRV_CAN128_H */
