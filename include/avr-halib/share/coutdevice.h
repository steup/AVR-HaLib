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

template< int val> class Num 
{public:enum{value=val};};

template <class BaseClass,int base=0>	class COutDevice;

template <class BaseClass> class COutDeviceBase: public BaseClass
{
public: 
	/// Write a string
	void writeString(const char * c)
	{
		while (*c)
			BaseClass::put(*(c++));
	}
	
	/// Write an integer hexadecimal
	template<class T>void writeHInt(T d)
	{
		uint8_t i = 0;
		uint8_t * da = (uint8_t *)(void *)& d;
		BaseClass::put('0');
		BaseClass::put('x');
		while(i < sizeof(d))
		{
			
			//higherhalfbyte
			uint8_t n = 0xf &(da[i]>>4);
			if (n > 9)
			BaseClass::put('a'+ n - 10);
			else
			BaseClass::put('0'+ n);
			//lowererhalfbyte
			n = 0xf & da[i];
			if (n > 9)
			BaseClass::put('a'+ n - 10);
			else
			BaseClass::put('0'+ n);
			i++;
		}
		
	}
	/// Write an integer charcterwise
	template<class T>void writeCInt(T d)
	{
		uint8_t i = 0;
		uint8_t * da = (uint8_t *)(void *)& d;
		while(i < sizeof(d))
		{
			BaseClass::put(da[i]);
			i++;
		}
	}
	
	/// Write an integer decimal
	template<class T> void writeInt(T d)
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
			
			const uint8_t bsize = sizeof(T) * 3 + 2;
			char buffer [bsize];
	
			uint8_t i = bsize - 1 ;		// position in buffer
			buffer[i] = '\0';
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

	/// Write an usigned integer decimal
	template<class T> void writeUInt(T d)
	{
		if (d == 0)
		{
			BaseClass::put('0');
		}
		else
		{
			const uint8_t bsize = sizeof(T) * 3 + 1;
			char buffer [bsize];
	
			uint8_t i = bsize - 1 ;		// position in buffer
			buffer[i] = '\0';
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

};


/**
 *	\class	COutDevice coutdevice.h include/avr-halib/share/coutdevice.h
 *	\brief	Extends BaseClass with ability to write strings and integers chars will be written as chars
 *	\see	doc_cdevices
 *	\param	BaseClass	A Class with a method <tt>void put(const char c)</tt> that writes a character.
 */

template <class BaseClass> 
		class COutDevice<BaseClass,0> : public COutDeviceBase<BaseClass>
{
	
public:

		/// Streaming operator for char output
		COutDevice & operator<<(const char c)
		{
			this->put(c);
			return *this;
		}

		/// Streaming operator for integer output
		COutDevice & operator<<(const int16_t d)
		{
			this->writeInt(d);
			return *this;
		}

	/// Streaming operator for integer output
		COutDevice & operator<<(const int32_t d)
		{
			this->writeInt(d);
			return *this;
		}

		/// Streaming operator for integer output
		COutDevice & operator<<(const uint8_t d)
		{
			this->writeUInt(d);
			return *this;
		}

		/// Streaming operator for integer output
		COutDevice & operator<<(const uint16_t d)
		{
			this->writeUInt(d);
			return *this;
		}

		/// Streaming operator for integer output
		COutDevice & operator<<(const uint32_t d)
		{
			this->writeUInt(d);
			return *this;
		}

		/// Streaming operator for string output
		COutDevice & operator<<(const char * c)
		{
			this->writeString(c);
			return *this;
		}

		/// Streaming operator for pointer output
		COutDevice & operator<<(const void * c)
		{
			this->writeHInt(c);
			return *this;
		}


};

template <class BaseClass> class COutDevice<BaseClass,10>:
		public COutDeviceBase<BaseClass>
{
	public:
		template<class T>COutDevice & operator<<(T d)
		{
			this->writeInt(d);
			return *this;
		}

		/// Streaming operator for string output
		COutDevice & operator<<(const char * c)
		{
			this->writeString(c);
			return *this;
		}
		
		/// Streaming operator for pointer output
		COutDevice & operator<<(const void * c)
		{
			this->writeHInt(c);
			return *this;
		}
		
};

template <class BaseClass> class COutDevice<BaseClass,16> : public COutDeviceBase<BaseClass>
{
	public:
		template<class T>COutDevice & operator<<(T d)
		{
			this->writeHInt(d);
			return *this;
		}
		/// Streaming operator for string output
		COutDevice & operator<<(const char * c)
		{
			this->writeString(c);
			return *this;
		}
		
		/// Streaming operator for pointer output
		COutDevice & operator<<(const void * c)
		{
			this->writeHInt(c);
			return *this;
		}
		
};

template <class BaseClass> class COutDevice<BaseClass,256>: public COutDeviceBase<BaseClass>
{
	public:
		template<class T> COutDevice & operator<<(T d)
		{
			this->writeCInt(d);
			return *this;
		}
		/// Streaming operator for string output
		COutDevice & operator<<(const char * c)
		{
			this->writeString(c);
			return *this;
		}
		/// Streaming operator for pointer output
		COutDevice & operator<<(const void * c)
		{
			this->writeHInt(c);
			return *this;
		}

};


template<typename B, class T> COutDevice< COutDeviceBase<T>,B::value > & BASE(COutDeviceBase<T> & cout)
{
	return * static_cast< COutDevice< COutDeviceBase<T>,B::value> * > (&cout);
}
template<class T> COutDevice< COutDeviceBase<T>,10 > & DEC(COutDeviceBase<T> & cout)
{
	return * static_cast< COutDevice< COutDeviceBase<T>,10> * > (&cout);
}
template<class T> COutDevice< COutDeviceBase<T>,16 > & HEX(COutDeviceBase<T> & cout)
{
	return * static_cast< COutDevice< COutDeviceBase<T>,16> * > (&cout);
}
template<class T> COutDevice< COutDeviceBase<T>,256 > & CHAR(COutDeviceBase<T> & cout)
{
	return * static_cast< COutDevice< COutDeviceBase<T>,256> * > (&cout);
}


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
