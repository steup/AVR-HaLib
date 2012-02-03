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

/*$Id: isp_drv_can128.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file isp_drv_can128.h
 * \brief MCU specific isp driver.
 * 
 * In-system-programming driver for the AT90CAN128 mcu.
 *
 * \note It's just intended (and tested) for the mcu spoken of. It should 
 * be possible to use it for other AVR's too, as they provide the same 
 * interface.
 */

#ifndef __ISP_DRV_CAN128_H
#define __ISP_DRV_CAN128_H

#include <stdint.h>

#include "isp.h"
#include "mcu_drv.h"
using namespace mcu;
#include "flash_drv.h"
#include "eeprom_drv.h"


namespace isp
{
  /*!
   * \brief Full erase by irq.
   *
   * Erase of given memory area by using interrupts. 
   *
   * \param[in] mem_area Identifier of memory area.
   * \param[in] cb Callback function to call if finished.
   *
   * \return True If memory area is not supported. \n
   * Return code of called memory driver otherwise.
   * 
   * \todo Erase of EEPROM not supported, only flash memory supported.
   */
  static inline bool full_erase_async(const MEMORY_AREA& mem_area, 
				      void (*cb)(void)){
    if ( mem_area == FLASH ){
      return flash::full_erase_async(cb);
    }
    if ( mem_area == EEPROM ){
      return eeprom::full_erase_async(cb);
    }
    return true;
  }

  /*!
   * \brief Write buffer by irq.
   *
   * Writes a buffer to given memory area using interrupts. 
   *
   * \param[in] mem_area Identifier of memory area.
   * \param[in] addr Reference to address where to start write.
   * \param[in] buffer Buffer with data to write
   * \param[in] buffer_len Size of buffer. 
   * \param[in] cb Callback function to call if finished.
   *
   * \return True If memory area is not supported. \n
   * Return code of called memory driver otherwise.
   * 
   * \todo Write of EEPROM not supported, only flash memory supported.
   */
  static inline bool write_buffer_async(const MEMORY_AREA& mem_area,
					const uint32_t& addr, 
					uint8_t* buffer, 
					uint8_t buffer_len,
					void (*cb)(void)){
    if ( mem_area == FLASH ){
      return flash::write_buffer_async(addr, 
				       buffer, 
				       buffer_len, 
				       cb);
    }
    if ( mem_area == EEPROM ){
      return eeprom::write_buffer_async((const uint16_t&)addr, 
					buffer, 
					buffer_len, 
					cb);
    }
    return true;
  }
 
  /*!
   * \brief Read buffer.
   *
   * Fills a given buffer with memory conent. 
   *
   * \param[in] mem_area Identifier of memory area.
   * \param[in] addr Reference to address where to start write.
   * \param[in] buffer Buffer with data to write
   * \param[in] buffer_len Size of buffer. 
   *
   * \return True If memory area is not supported. \n
   * Return code of called memory driver otherwise.
   * 
   * \todo Read of EEPROM not supported, only flash memory supported.
   */
  static inline bool read_buffer(const MEMORY_AREA& mem_area,
				 const uint32_t& addr, 
				 uint8_t* buffer, 
				 uint8_t buffer_len){
    if ( mem_area == FLASH ){
      return flash::read_buffer(addr, 
				buffer, 
				buffer_len);
    }
    if ( mem_area == EEPROM ){
      return eeprom::read_buffer((const uint16_t&)addr, 
				 buffer, 
				 buffer_len);
    }
    return true;
  }

} // namespace isp

#endif /* __ISP_DRV_CAN128_H */
