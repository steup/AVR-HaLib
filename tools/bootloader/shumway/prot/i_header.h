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

/*$Id: i_header.h 484 2010-07-01 13:25:33Z mschulze $*/

/*!
 * \file i_header.h
 * \brief Declarations of internal packet headers.
 *
 * Using a shared memory, meta informations for the belonging data are stored 
 * in a header before.
 */

#ifndef __I_HEADER_H
#define __I_HEADER_H

#include <stdint.h>

#include "protocol.h"

/*
 * \struct i_hdr_phy_t
 * 
 * Header for data coming from a physical interface.
 */
struct i_hdr_phy_t{
  uint32_t id; /*!< Identifier of sender of data.*/
  uint8_t len; /*!< Length of header and data together.*/
};

/*
 * \struct i_hdr_action_t
 * 
 * Header for data for a related action
 */
struct i_hdr_action_t{
  uint8_t len;  /*!< Length of header and data together.*/
  ACTION action; /*!< Action related with data. */
};


#endif /* __I_HEADER_H */
