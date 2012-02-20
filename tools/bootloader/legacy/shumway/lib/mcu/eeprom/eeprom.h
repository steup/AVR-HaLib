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

/*$Id: eeprom.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file eeprom.h
 * \brief Eeprom specific declarations.
 *
 * Contains common and always valid eeprom declarations.
 */

#ifndef __EEPROM_H
#define __EEPROM_H

/*!
 * \namespace eeprom
 * \brief Eeprom functions.
 * 
 * Encapsulates all eeprom related code. Prevents possible name clashing.
 */
namespace eeprom
{

  /*!
   * \brief Eeprom constants.
   *
   * Identifier of eeprom constants.
   */
  enum{
    BLANK_BYTE = 0xFF, /*!< Content of Blank byte. */
  };

} // namespace eeprom

#endif /* __EEPROM_H */
