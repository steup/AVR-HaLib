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

/*$Id: mcu_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file mcu_drv_can128.h
 * \brief MCU specific mcu driver.
 * 
 * MCU driver for the AT90CAN128 mcu.
 */

#ifndef __MCU_DRV_CAN128_H
#define __MCU_DRV_CAN128_H

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>

#include "irq_drv.h"
using namespace irq;


namespace mcu
{
  /*!
   * \def GET_APPL_SECTION_SIZE_W()
   *
   * Computes the size of application section with the settings of fuse bits.
   */
#define GET_APPL_SECTION_SIZE_W()					\
  (									\
   MAX_BOOT_SIZE >> (( boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS) &	\
		       (( 1 << BOOTSZ0 ) | ( 1 << BOOTSZ1 )) )		\
		     >> 1)						\
   )
    
  /*!
   * \def GET_BL_SECTION_START()
   *
   * Computes start address of bootloader section with the settings of fuse 
   * bits.
   */
#define GET_BL_SECTION_START()						\
    (									\
     ((uint32_t)FLASHEND >> 1) - ((uint32_t)GET_APPL_SECTION_SIZE_W())	\
    )
    
  /*!
   * \brief MCU constants.
   *
   * Identifier of MCU constants.
   */
  enum {
    FOSC = 16000, /*!< Clock frequency in kHz. */
    MAX_BOOT_SIZE = 4096, /*!< Maximal bootloader section size. */
  };

  /*!
   * \brief Memory areas.
   * 
   * Identifier of memory areas available.
   */
  enum MEMORY_AREA{
    FLASH, /*!< Flash memory area. */
    EEPROM, /*!< Eeprom memory area. */
  };

  /*!
   * \brief High Fuse byte register bits.
   * 
   * Identifier of bits of high fuse byte, according to documentation.
   */
  enum FUSE_HIGH_BYTE{BOOTRST = 0,
		      BOOTSZ0,
		      BOOTSZ1,
		      EESAVE,
		      WDTON,
		      SPIEN,
		      JTAGEN,
		      OCDEN
  };

  /*!
   * \brief Get reset status.
   *
   * Get reset status from the appropriate register.
   *
   * \return Reset status setting.
   */
  static inline uint8_t get_reset_status(){
    return ( MCUSR & 0x1F );
  }

  /*!
   * \brief Clear reset status.
   *
   * Clears reset status.
   */
  static inline void clear_reset_status(){
    MCUSR = 0;
  }

  /*!
   * \brief Jump to zero.
   *
   * Performs a jump to address zero. 
   */
  static inline void jump_to_zero(){
    asm volatile ("jmp 0");
  }

  /*!
   * \brief Start application.
   *
   * Prepares everything to start an application by another one. 
   *
   * \note Moves irq vector table to application space.
   */
  static inline void appl_start(){
    cli();
    change_irq_vector_to_appl();
    jump_to_zero();
  }

} // namespace mcu

#endif /* __MCU_DRV_CAN128_H */
