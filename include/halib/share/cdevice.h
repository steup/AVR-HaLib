/**	\file	halib/share/cdevice.h
 *
 *	\brief	Defines a base class for character devices
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include "halib/share/cindevice.h"
#include "halib/share/coutdevice.h"

/**
 *	\class	CDevice cdevice.h include/halib/share/cdevice.h
 *	\brief	Base class for character devices
 *	\see	doc_cdevices
 */
class CDevice : public CInDevice, public COutDevice
{
	
public:

};
