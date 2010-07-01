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
 * \file servers.cpp
 * \brief Server definitions.
 *
 * Definitions of all servers used in the current configuration.
 * Together with the declaration file, changing
 * the configuration can be made just here. In fact, the separation in this 
 * two files is only needed as there are problems using global static 
 * variables in avr-c++ (GCC Bugzilla Bug 29141).
 *
 * \sa servers.h
 *
 * \note Look in the documentation of the included files for their 
 * functionality.
 */

// static S_Uart_Phy _s_uart_phy; 
static S_Can_Phy _s_can_phy; 
static S_Id _s_id; 
static S_Isp _s_isp; 
