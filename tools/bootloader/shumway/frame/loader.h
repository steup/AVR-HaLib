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
 * \file loader.h
 * \brief Application Shumway entry declaration.
 *
 * In order to enable applications the jump into the Shumway programmer,
 * they have to include this file and make use of it. 
 */

#ifndef __LOADER_H
#define __LOADER_H

/*!
 * \brief Shumway foreign entry adresses.
 *
 * Enumaration of Shumway entry address for applications. 
 * 
 * \note Have to be the same as on Shumway programmer side.
 */
enum BL_ENTRY{
  BL_ENTRY_ADDR_H = 0x0001, /*!< High bytes of entry point address. */
  BL_ENTRY_ADDR_L = 0xFFD0, /*!< Low bytes of entry point address. */
};

/*!
 * \brief Jump to Shumway programmer.
 *
 * Simply a jump to a specified address.
 *
 * \sa BL_ENTRY
 */
void 
loader_run()
{
  __asm__ __volatile__				 \
    (                                            \
     "jmp %A0\n\t"                               \
     :						 \
     : "i" ((uint16_t)BL_ENTRY_ADDR_L)		 \
     );                      
}


#endif /* __LOADER_H */
