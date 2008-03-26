/**	\file halib/share/cbuffer.h
 *
 *	\brief	Defines and implements CBuffer, a virtual character devices for temporary storage
 *	\author	Karl Fessel, Philipp Werner
 *	\see 	doc_cdevices
 *
 */

#pragma once

#include "halib/share/cdevice.h"
#include "halib/share/queuebuffer.h"

/**	\brief Virtual device: temporary character queue
 *	\param length_t	Type used for indexing and size of buffer
 *	\param bufLen	Size of buffer
 * 
 * 	Input can be written into a queue of fixed length and be read and parsed.
 *
 */
template <class length_t, length_t bufLen>
		class CBuffer : public CDevice
{
protected:

	QueueBuffer<char, length_t, bufLen> buffer;
	
public:
	/// Constructor
	CBuffer(){}

	/// Write one character
	void putc (const char c)
	{
		buffer.put(c);
	}

	/// Read one character
	char getc()
	{
		return buffer.get();
	}

};
