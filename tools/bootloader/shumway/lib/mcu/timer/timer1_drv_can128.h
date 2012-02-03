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

/*$Id: timer1_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file timer1_drv_can128.h
 * \brief MCU specific driver for timer 1.
 * 
 * Timer driver for the AT90CAN128 mcu timer 1.
 */

#ifndef __TIMER1_DRV_CAN128_H
#define __TIMER1_DRV_CAN128_H

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>

/*!
 * \namespace timer1
 * \brief Timer 1 functions.
 * 
 * Encapsulates all related code for timer 1. Prevents possible name clashing.
 *
 * \todo Put namespace declaration in other file (timer1.h). Ignored for now
 * because it was forgotten, but would result in empty file.
 */
namespace timer1
{
  /*!
   * \brief Modes of operations.
   *
   * Timer operations modes identifier.
   * 
   * \todo Incomplete.
   */
  enum MOOP{
    MOOP_0 = 0x00,
    MOOP_12 = 0x0C,
  };

  /*!
   * \brief Clock selections.
   *
   * Clock selection identifier. Another word for prescaler settings.
   */
  enum CLOCK_SELECT{CS_STOP,
		    CS_NONE,
		    CS_8,
		    CS_64,
		    CS_256,
		    CS_1024,
		    CS_EXT_FALLIN,
		    CS_EXT_RISIN,
  };

  /*!
   * \brief Enable timer 1.
   *
   * Enables timer 1. 
   * 
   * \param[in] cs Clock select mode. Defaults to none.
   * \param[in] mode Mode of operation. Defaults to mode 0.
   * 
   * \note Disables all output capture operations.
   */
  void enable(CLOCK_SELECT cs = CS_NONE, MOOP mode = MOOP_0);

  /*!
   * \brief Register for input capture.
   *
   * Register for input capture events.
   * 
   * \param[in] cb Callback function in case of input capture.
   *
   * \return False If successfully registered. \n
   * True otherwise.
   */
  bool register_input_capture(void (*cb)());

  /*!
   * \brief unregister from input capture.
   *
   * Unregister from input capture events.
   * 
   * \note Just disables callback call. Leaves input capture unchanged.
   */
  void unregister_input_capture();

  /*!
   * \brief Set prescaler.
   *
   * Sets prescaler of timer 1.
   *
   * \param[in] cs Clock select mode, specifying prescaler value.
   */
  static inline void set_prescaler(CLOCK_SELECT cs)
  {
    uint8_t __tccr_save;
    uint8_t __value = cs &0x07;
    __tccr_save = TCCR1B & ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TCCR1B =  __tccr_save | __value;
  }

  /*!
   * \brief Set operation mode.
   *
   * Sets mode of operation of  of timer 1.
   *
   * \param[in] mode Mode of operation.
   */
  static inline void set_mode_of_operation(MOOP mode)
  {
    // somewhat hard because shared across two registers.
    uint8_t __tccr_save;
    uint8_t __lmode = mode & 0x03;
    uint8_t __hmode = (mode & 0x0C) << 1;
    __tccr_save = TCCR1A & ~((1 << WGM11) | (1 << WGM10));
    TCCR1A = __tccr_save | __lmode;
    __tccr_save = TCCR1B & ~((1 << WGM13) | (1 << WGM12));
    TCCR1B = __tccr_save | __hmode;
  }

  /*!
   * \brief Disable all output comare.
   *
   * Disconnect all output compare pins OC1A, OC1B and OC1C of timer 1.
   */
  static inline void disable_output_compare_all(){
    // clear all bits except the wgbits
    TCCR1A &= ((1 << WGM11) | (1 << WGM10));
  }

  /*!
   * \brief Set input capture value.
   *
   * Sets value for usage at input capture.
   *
   * \param[in] value Reference to value.
   */
  static inline void set_input_capture_val(const uint16_t& value){
      ICR1H = value >> 8;
      ICR1L = value;
  }

  /*!
   * \brief Enable input cature irq.
   *
   * Enables interrupt generation at input capture events.
   */
  static inline void enable_input_capture_irq(){
      TIMSK1 |= ( 1 << ICIE1);
  }

  /*!
   * \brief Disable input cature irq.
   *
   * Disables interrupt generation at input capture events.
   */
  static inline void disable_input_capture_irq(){
      TIMSK1 &= ~( 1 << ICIE1);
  }

} // namespace timer1

#endif /* __TIMER1_DRV_CAN128_H */
