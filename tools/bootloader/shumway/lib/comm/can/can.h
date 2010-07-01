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
 * \file can.h
 * \brief CAN specific declarations.
 *
 * Contains common and always valid CAN declarations.
 */

#ifndef __CAN_H
#define __CAN_H


/*!
 * \namespace can
 * \brief CAN functions.
 * 
 * Encapsulates all CAN related code. Prevents possible name clashing.
 */
namespace can
{
  /*!
   * \brief Enumeration of CAN constants.
   *
   * CAN constants.
   */
  enum {
    DATA_LEN = 8, /*!< Max number of data bytes in CAN message. */ 
  };

} // namespace can

#endif /* __CAN_H */
