/**	\file halib/share/cindevice.h
 *
 *	\brief	Defines a base class for character input devices
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include <stdint.h>

/**
 *	\class	CInDevice cindevice.h include/halib/share/cindevice.h
 *	\brief	Base class for character input devices
 *	\see	doc_cdevices
 */
class CInDevice
{
	
public:

	/**	\brief Read a character
	 *	\param	c	Reference to variable to store the char
	 *	\returns	false, if there is nothing to get
	 */
	virtual bool getc(char & c) = 0;

	/**
	*	\brief	Reads a word (string with no whitespaces inside)
	*	\param	s	String buffer (string read will be null-terminated)
	*	\param	maxLength	Length of the string buffer
	*	\returns	Number of characters read (less than maxLength)
	*
	*	This function removes leading whitespaces and the whitespace behind the word.
	*	
	*
	*/
	uint8_t readString(char * s, uint8_t maxLength);

	/**
	*	\brief	Read a number
	*	\param	val	Reference to a variable to store the parsed number
	*	\returns	true, if a number was found
	*
	*	This function removes leading whitespaces.
	*
	*	\attention The first character behind the number or (if there is no number) behind the last
	*	of leading whitespaces is lost! Hint: use a clear syntax for data transmitted though interfaces
	*	(e.g. a keyword in front of numbers and a whitespace behind a number).
	*/
	bool readInt(int32_t & val);
};
