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

/*$Id: eeprom_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file eeprom_drv_can128.h
 * \brief MCU specific eeprom driver.
 * 
 * Eeprom driver for the AT90CAN128 mcu.
 */

#ifndef __EEPROM_DRV_CAN128_H
#define __EEPROM_DRV_CAN128_H

#include <avr/io.h>

#include "eeprom.h"


namespace eeprom
{
  /*!
   * \brief Read buffer from eeprom memory.
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
  bool read_buffer(const uint16_t& addr, 
		   uint8_t* buffer, 
		   const uint16_t& buffer_len);


  bool init_async(void (*cb)(void));


  /*!
   * \brief Full erase by irq.
   *
   * Erase of all eeprom by using eeprom interrupt. 
   *
   * \param[in] cb Callback function pointer to call if finished.
   *
   * \return False If successfully started. \n
   * True In case of an error.
   */
  bool full_erase_async(void (*cb)(void));


  /*!
   * \brief Writes buffer to eeprom by irq.
   *
   * Writes a buffer to eeprom memory using eeprom interrupt. 
   *
   * \param[in] addr Reference to address where to start write.
   * \param[in] buffer Buffer with data to write
   * \param[in] buffer_len Size of buffer. 
   * \param[in] cb Callback function pointer to call if finished.
   *
   * \return False If successfully started. \n
   * True In case of an error.
   */
  bool write_buffer_async(const uint16_t& addr, 
			  uint8_t* buffer, 
			  const uint16_t& buffer_len,
			  void (*cb)(void));

  /*!
   * \brief Read byte.
   *
   * Read content from eeprom at given address.. 
   *
   * \param[in] addr Reference to address.
   *
   * \return Content of eeprom. \n
   */
  static inline uint8_t read_byte(const uint16_t& addr){
    EEAR = addr;
    EECR |= (1<<EERE);
    return EEDR;
  }

} // namespace eeprom
#endif /* __EEPROM_DRV_CAN128_H */
