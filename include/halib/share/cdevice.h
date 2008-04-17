/**	\file	halib/share/cdevice.h
 *
 *	\brief	Defines CDevice
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include "halib/share/cindevice.h"
#include "halib/share/coutdevice.h"

/**
 *	\class	CDevice cdevice.h include/halib/share/cdevice.h
 *	\brief	Extends BaseClass with ability to read and write strings and integers
 *	\see	doc_cdevices
 *	\param	BaseClass	A Class with a method <tt>void put(const char c)</tt> that writes a character and a method
 *				<tt>bool get(char & c)</tt> that returns true if a character c was read.
 */
template <class BaseClass>
		class CDevice : public CInDevice< COutDevice<BaseClass> >
{

public:

};
