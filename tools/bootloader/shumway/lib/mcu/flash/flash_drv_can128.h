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

/*$Id$*/

/*!
 * \file flash_drv_can128.h
 * \brief MCU specific flash driver.
 * 
 * Flash driver for the AT90CAN128 mcu.
 */

#ifndef __FLASH_DRV_CAN128_H
#define __FLASH_DRV_CAN128_H

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "flash.h"
#include "mcu_drv.h"
using namespace mcu;

/*!
 * \def boot_rww_enable_irq(__dest_addr)
 *
 * Mapping of common flash macros to mcu specific macro.
 */
#define boot_rww_enable_irq(__dest_addr)		\
  __boot_rww_enable_irq(__dest_addr)		

/*!
 * \def boot_page_erase_irq(__dest_addr)
 *
 * Mapping of common flash macros to mcu specific macro.
 */
#define boot_page_erase_irq(__dest_addr)		\
  __boot_page_erase_extended_irq(__dest_addr)		

/*!
 * \def boot_page_write_irq(__dest_addr)
 *
 * Mapping of common flash macros to mcu specific macro.
 */
#define boot_page_write_irq(__dest_addr)		\
  __boot_page_write_extended_irq(__dest_addr)		


namespace flash
{
  /*!
   * \brief Read byte from flash memory.
   *
   * Read byte at the address given.
   *
   * \param[in] addr Reference to address.
   *
   * \return Byte with flash memory content.
   */
  static inline uint8_t read_byte(const uint32_t& addr){
    // this is for a thinkable extension on adresses lower 0xFFFF.
    // than, a new adress type has to be defined instead of uint32_t
    return pgm_read_byte_far(addr);
  }

  /*!
   * \brief Read buffer from flash memory.
   *
   * Read memory, beginning at a given address. Fill given buffer with
   * bytes read.
   *
   * \param[in] addr Reference to start address.
   * \param[in] buffer Buffer to fill with bytes read.
   * \param[in] buffer_len Size of buffer. Buffer will be filled until this 
   * size is reached.
   *
   * \return False.
   */
  bool read_buffer(const uint32_t& addr, 
		   uint8_t* buffer, 
		   const uint16_t& buffer_len);

  /*!
   * \brief Page blank check.
   *
   * Checks, if page is blank or not. Page selected is specified by
   * the given address, which could be anywhere in the page.
   *
   * \param[in] addr Reference to address, which specifies the page to check.
   *
   * \return False On blank page read. \n
   * True If page is not blank.
   */
  bool page_blank_check(const uint32_t& addr);

  /*!
   * \brief Full erase by irq.
   *
   * Erase of all flash pages by using spm interrupt. 
   *
   * \param[in] cb Callback function pointer to call if finished.
   *
   * \return False If successfully started. \n
   * True In case of an error.
   */
  bool full_erase_async(void (*cb)(void));

  /*!
   * \brief Writes buffer to flash by irq.
   *
   * Writes a buffer to flash memory using spm interrupt. 
   *
   * \param[in] addr Reference to address where to start write.
   * \param[in] buffer Buffer with data to write
   * \param[in] buffer_len Size of buffer. 
   * \param[in] cb Callback function pointer to call if finished.
   *
   * \return False If successfully started. \n
   * True In case of an error.
   */
  bool write_buffer_async(const uint32_t& addr, 
			  uint8_t* buffer, 
			  const uint16_t& buffer_len,
			  void (*cb)(void));

} // namespace flash
#endif /* __FLASH_DRV_CAN128_H */
