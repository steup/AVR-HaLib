/** \addtogroup share */
/*@{*/
/**
 *	\file	avr-halib/share/coutdevice.h
 *	\brief	Defines COutDevice
 *	\author	Philipp Werner, Karl Fessel
 *	\see 	doc_cdevices
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include <stdint.h>

template <class BaseClass> class COutDeviceHex: public BaseClass
{};

/**
 *	\class	COutDevice coutdevice.h include/avr-halib/share/coutdevice.h
 *	\brief	Extends BaseClass with ability to write strings and integers
 *	\see	doc_cdevices
 *	\param	BaseClass	A Class with a method <tt>void put(const char c)</tt> that writes a character.
 */
template <class BaseClass>
		class COutDevice : public BaseClass
{
	
public:

	/// Write a string
	void writeString(const char * c)
	{
		while (*c)
			BaseClass::put(*(c++));
	}
	
	/// Write an integer
	void writeInt(int32_t d)
	{
		if (d == 0)
		{
			BaseClass::put('0');
		}
		else
		{
			bool neg = false;
			if (d < 0)
			{
				neg = true;
				d = -d;
			}
			
			char buffer [12];
	
			uint8_t i = 11;		// position in buffer
			buffer[11] = '\0';
			while (d)
			{
				buffer[--i] = '0' + (d % 10);
				d /= 10;
			}
	
			if (neg)
				buffer[--i] = '-';
			writeString(buffer+i);
		}
	}

	/// Write an integer
	void writeInt(uint32_t d)
	{
		if (d == 0)
		{
			BaseClass::put('0');
		}
		else
		{
			char buffer [12];
	
			uint8_t i = 11;		// position in buffer
			buffer[11] = '\0';
			while (d)
			{
				buffer[--i] = '0' + (d % 10);
				d /= 10;
			}
			writeString(buffer+i);
		}
	}

	/// Write a newline
	void writeNewline()
	{
		BaseClass::put('\n');
		BaseClass::put('\r');
	}

	/// Streaming operator for string output
	COutDevice & operator<<(const char * c)
	{
		writeString(c);
		return *this;
	}

	/// Streaming operator for char output
	COutDevice & operator<<(const char c)
	{
		BaseClass::put(c);
		return *this;
	}


	/// Streaming operator for integer output
	COutDevice & operator<<(const int32_t d)
	{
		writeInt(d);
		return *this;
	}


	/// Streaming operator for integer output
	COutDevice & operator<<(const uint32_t d)
	{
		writeInt(d);
		return *this;
	}


	/// Streaming operator for integer output
	COutDevice & operator<<(const int16_t d)
	{
		writeInt(d);
		return *this;
	}


	/// Streaming operator for integer output
	COutDevice & operator<<(const uint16_t d)
	{
		writeInt(d);
		return *this;
	}


	/// Streaming operator for integer output
	COutDevice & operator<<(const uint8_t d)
	{
		writeInt(d);
		return *this;
	}

	COutDeviceHex< COutDevice<BaseClass> > & HEX()
	{
		return * dynamic_cast< COutDeviceHex< COutDevice< BaseClass > > * > (this);
	}
};



template <class BaseClass> class SecOut: public BaseClass
{
	public:
	void put(const char c)__attribute__ ((always_inline))
	{
		while(!BaseClass::ready());
		BaseClass::put(c);
	}
};

template <class Base,class T> class COutDeviceTypeWrap:public Base
{
	public:
	void put(const T c){Base::put((char)(c) );}
};

/*@}*/
