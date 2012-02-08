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

template <class BaseClass,uint16_t base=0>	class COutDevice;

template <class BaseClass_b> class COutDeviceBase: public BaseClass_b
{
	enum{savespace=1};
public: 
	/// Write a string
	void writeString(const char * c)
	{
		while (*c)
			BaseClass_b::put(*(c++));
	}
	
private:
		/// Write an integer hexadecimal
	void _write1HInt(const uint8_t b)
	{
		{//higherhalfbyte
			uint8_t h = 0xf & (b>>4);
			if (h > 9)
				BaseClass_b::put('a'+ h - 10);
			else
				BaseClass_b::put('0'+ h);
		}	
		{// lowererhalfbyte
			uint8_t l = 0xf & b;
			if (l > 9)
				BaseClass_b::put('a'+ l - 10);
			else
				BaseClass_b::put('0'+ l);
		}
	}
	
	void writeHInt(const uint32_t d, uint8_t length)
	{
		uint8_t * da = (uint8_t *)& d;
		BaseClass_b::put('0');
		BaseClass_b::put('x');
		for(uint8_t i = length;i;i--)
		{
			uint8_t b = da[i-1];
			_write1HInt(b);
		}
	}
	
public:
	template<class T>void writeHInt(const T d)
	{
		if(savespace) //evaluated at compiletime
		{
			writeHInt(d,sizeof(d));
		}else
		{
			uint8_t * da = (uint8_t *)& d;
			BaseClass_b::put('0');
			BaseClass_b::put('x');
			for(uint8_t i = sizeof(d);i;i--)
			{
				uint8_t b = da[i-1];
				_write1HInt(b);
			}
		}
	}
private:
	/// Write an integer characterwise
	void writeCInt(const uint32_t d, uint8_t length)
	{
		uint8_t * da = (uint8_t *)& d;
		for(uint8_t i = length;i;i--)
		{
			uint8_t b = da[i-1];
			BaseClass_b::put(b);
		}
	}
public:
	/// Write an integer characterwise
	template<class T>void writeCInt(const T d)
	{
		if(savespace) //evaluated at compiletime
		{
			writeCInt(d,sizeof(d));
		}else
		{
			uint8_t * da = (uint8_t *)& d;
			for(uint8_t i = sizeof(d);i;i--)
			{
				uint8_t b = da[i-1];
				BaseClass_b::put(b);
			}
		}
	}

	/// Write an integer decimal
	template<class T> void writeInt(const T din)
	{
		if(savespace && !(sizeof(T)==4)) //evaluated at compiletime
		{
			writeInt((const int32_t) din);
		}else
		{
			T d = din;
			if (d == 0)
			{
				BaseClass_b::put('0');
			}
			else
			{
				bool neg = false;
				if (d < 0)
				{
					neg = true;
					d = -d;
				}
				
				const uint8_t bsize = sizeof(d) * 3 + 2;
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
	}

	/// Write an usigned integer decimal
	template<class T> void writeUInt(const T din)
	{
		if(savespace && !(sizeof(T)==4)) //evaluated at compiletime TODO: find a way to do the sizeof thing by using the type of din
		{
			writeInt((const int32_t) din);
		}else
		{
			T d = din;
			if (d == 0)
			{
				BaseClass_b::put('0');
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
	}
	/// Write a newline
	void writeNewline()
	{
		BaseClass_b::put('\n');
		BaseClass_b::put('\r');
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


template<uint16_t B, class T> COutDevice< COutDeviceBase<T>,B > & BASE(COutDeviceBase<T> & cout)
{
	return * static_cast< COutDevice< COutDeviceBase<T>,B> * > (&cout);
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
