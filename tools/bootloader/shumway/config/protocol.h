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

/*!
 * \file protocol.h
 * \brief Protocol commands.
 *
 * Shumway protocol commands are declared here. Have to be the same as on
 * host side, e.g. avrdude.
 */

/*$Id: protocol.h 484 2010-07-01 13:25:33Z mschulze $*/

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/*!
 * \brief Protocol action commands.
 *
 * Enumeration of Shumway action commands.
 */
enum ACTION{
  ACTION_INVALID = 0x00, /*!< Invalid action */
  ACTION_SELECT_OPEN, /*!< Open Shumway programmer for further actions.*/
  ACTION_SELECT_CLOSE,/*!< Close Shumway programmer, finish actions. */
  ACTION_MEM_SELECT, /*!< Select memory area. */
  ACTION_ADDR, /*!< Select destination address in selected memory area. */
  ACTION_WRITE, /*!< Write data, starting at destination address. */
  ACTION_READ, /*!< Read data from destination address until given address. */
  ACTION_FULL_ERASE, /*!< Erase all data in selected memory area. */
};

/*!
 * \brief Protocol memory selection commands.
 *
 * Enumeration of Shumway memory selection commands.
 */
enum MEMORY_SELECTION{
  MS_MEM_INVALID = 0x00, /*!< Invalid memory selection. */
  MS_MEM_FLASH, /*!< Selection of flash memory. */
  MS_MEM_EEPROM, /*!< Selection of eeprom memory. */
};

/*!
 * \brief Protocol replies from programmer.
 *
 * Enumeration of Shumway programmer replies.
 */
enum REPLY{ 
  REPLY_ERROR = 0x00, /*!< Error reply. */
  REPLY_OK, /*!< OK reply. */
};

#endif /* PROTOCOL_H */
