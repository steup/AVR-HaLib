/**
 *	\file	avr-halib/avr/canary/can_debug.h
 *	\brief	Debug helpers for CAN driver
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "can_enums.h"

namespace avr_halib
{
namespace canary
{
	/** \brief Converts a CAN driver event into a readeable message.
	 *
	 * This function exists mainly for debugging purpouses, because 
	 * it will occupy e rather bit amount of flash memory when used.
	 *
	 * @tparam T should be avr_can_driver::Events, only exists to 
	 * only create code for the function when called at least once.
	 * @param event the CAN driver event that should be converted
	 * @return the message describing the CAN driver event
	 **/
	template<typename T>
	const char* eventToString(T event)
	{
			switch((Events)event)
			{
				case(INVALIDID):	return "invalid id";
				case(INVALIDMSG):	return "Msg not valid";
				case(ACKERROR):		return "no Ack";
				case(CRCERROR):		return "crc error";
				case(BITERROR):		return "bit transmission error";
				case(STUFFERROR):	return "bit stuffing error";
				case(FORMERROR):	return "format error";
				case(NOFREEMOB):	return "no MOb free";
				case(NOTHING):		return "nothing happend";
				case(RECEIVEOK):	return "package received";
				case(TRANSMITOK):	return "package sended";
				case(SUCCESS):		return "ok";
				default:		return "unknown";
			}
	}
}
}
