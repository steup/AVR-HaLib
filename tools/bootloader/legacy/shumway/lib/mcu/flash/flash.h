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

/*$Id: flash.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file flash.h
 * \brief Flash specific declarations.
 *
 * Contains common and always valid flash declarations.
 */

#ifndef __FLASH_H
#define __FLASH_H

  /*!
   * \def __boot_page_erase_extended_irq(__dest_addr)
   *
   * Macro for erase of flash page in flash > 64k..
   *
   * \note The avr-libc macros for this purpose violate the interrupt enable 
   * flag in spm status register.
   */
#define __boot_page_erase_extended_irq(__dest_addr)	\
  ({							\
    __asm__ __volatile__				\
      (							\
       "movw r30, %A3\n\t"				\
       "sts  %1, %C3\n\t"				\
       "lds  __tmp_reg__, %0\n\t"			\
       "or __tmp_reg__, %2\n\t"				\
       "sts %0, __tmp_reg__\n\t"			\
       "spm\n\t"					\
       :						\
       : "i" (_SFR_MEM_ADDR(__SPM_REG)),		\
	 "i" (_SFR_MEM_ADDR(RAMPZ)),			\
	 "r" ((uint8_t)__BOOT_PAGE_ERASE),		\
	 "r" ((uint32_t)__dest_addr)			\
       : "r30", "r31"					\
       );						\
  })

  /*!
   * \def __boot_page_write_extended_irq(__dest_addr)
   *
   * Macro for write of flash page in flash > 64k..
   *
   * \note The avr-libc macros for this purpose violate the interrupt enable 
   * flag in spm status register.
   */
#define __boot_page_write_extended_irq(__dest_addr)	\
  ({							\
    __asm__ __volatile__				\
      (							\
       "movw r30, %A3\n\t"				\
       "sts  %1, %C3\n\t"				\
       "lds  __tmp_reg__, %0\n\t"			\
       "or __tmp_reg__, %2\n\t"				\
       "sts %0, __tmp_reg__\n\t"			\
       "spm\n\t"					\
       :						\
       : "i" (_SFR_MEM_ADDR(__SPM_REG)),		\
	 "i" (_SFR_MEM_ADDR(RAMPZ)),			\
	 "r" ((uint8_t)__BOOT_PAGE_WRITE),		\
	 "r" ((uint32_t)__dest_addr)			\
       : "r30", "r31"					\
       );						\
  })

  /*!
   * \def __boot_rww_enable_irq()
   *
   * Macro for re-enable rww acccess of flash.
   *
   * \note The avr-libc macros for this purpose violate the interrupt enable 
   * flag in spm status register.
   */
#define __boot_rww_enable_irq()			\
  ({						\
    __asm__ __volatile__			\
      (						\
       "lds  __tmp_reg__, %0\n\t"		\
       "or __tmp_reg__, %1\n\t"			\
       "sts %0, __tmp_reg__\n\t"		\
       "spm\n\t"				\
       :					\
       : "i" (_SFR_MEM_ADDR(__SPM_REG)),	\
	 "r" ((uint8_t)__BOOT_RWW_ENABLE)	\
       );					\
  })

/*!
 * \namespace flash
 * \brief Flash functions.
 * 
 * Encapsulates all flash related code. Prevents possible name clashing.
 */
namespace flash
{

  /*!
   * \brief Flash constants.
   *
   * Identifier of flash constants.
   */
  enum{
    BLANK_BYTE = 0xFF, /*!< Content of Blank byte. */
  };

} // namespace flash

#endif /* __FLASH_H */
