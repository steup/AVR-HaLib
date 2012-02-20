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

/*$Id: irq_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file irq_drv_can128.h
 * \brief MCU specific irq driver.
 * 
 * Irq driver for the AT90CAN128 mcu.
 */

#ifndef __IRQ_DRV_CAN128_H
#define __IRQ_DRV_CAN128_H

#include <avr/io.h>


/*!
 * \namespace irq
 * \brief Irq functions.
 * 
 * Encapsulates all irq related code. Prevents possible name clashing.
 *
 * \todo Put namespace declaration in other file (irq.h). Ignored for now
 * because it was forgotten, but would result in empty file.
 */
namespace irq
{
  /*!
   * \brief Enable irq vector change.
   *
   * Enables interrupt vector change by setting the appropriate bits.
   */
  static inline void enable_irq_vector_change(){
    MCUCR |= (1<<IVCE);
  }
  
  /*!
   * \brief Switch irq vector table to bootloader section.
   *
   * Switch interrupt vector table to bootloader section. 
   * Performs actions as described at page 64 of documentation.
   */
  static inline void change_irq_vector_to_bl(){
    uint8_t mcucr_save = MCUCR;
    enable_irq_vector_change();
    MCUCR = mcucr_save | (1<<IVSEL);
  }

  /*!
   * \brief Switch irq vector table to application section.
   *
   * Switch interrupt vector table to application section. 
   * Performs actions as described at page 64 of documentation.
   */
  static inline void change_irq_vector_to_appl(){
    enable_irq_vector_change();
    MCUCR &= ~( (1<<IVSEL)|(1<<IVCE) );
  }

} // namespace irq

#endif /* __IRQ_DRV_CAN128_H */
