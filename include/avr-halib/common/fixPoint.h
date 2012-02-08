#pragma once

#include <stdint.h>

#include <avr-halib/avr/limits.h>
#include "promote.h"

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_unsigned.hpp>


namespace avr_halib
{
namespace object
{
	template<typename BaseType, uint8_t offset>
	class FixPointValue
	{
		BaseType value;

		public:
			typedef BaseType ValueType;
			static const uint8_t exponent=offset;
			
			static FixPointValue min()
			{
				FixPointValue temp;
				temp.value=std::numeric_limits<BaseType>::min();
				return temp;
			}

			static FixPointValue max()
			{
				FixPointValue temp;
				temp.value=std::numeric_limits<BaseType>::min();
				return temp;
			}

			static FixPointValue epsilon()
			{
				FixPointValue temp;
				temp.value=1;
				return temp;
			}

			explicit FixPointValue() : value(0){}

			FixPointValue(BaseType preDot, BaseType postDot=0)
			{
				this->value=0;
				this->value|=postDot&((1<<offset)-1);
				this->value|=preDot<<offset;
			}

			FixPointValue& operator+=(const FixPointValue& a)
			{
				this->value+=a.value;
				return *this;
			}

			FixPointValue& operator+=(const BaseType& value)
			{
				this->value+=((BaseType)value<<offset);
				return *this;
			}

			FixPointValue& operator-=(const FixPointValue& a)
			{
				this->value-=a.value;
				return *this;
			}

			FixPointValue& operator-=(const BaseType& value)
			{
				this->value-=((BaseType)value<<offset);
				return *this;
			}

			FixPointValue& operator*=(const FixPointValue& a)
			{
				this->value>>=offset/2;
				this->value*=a.value>>(offset-offset/2);
				return *this;
			}

			FixPointValue& operator*=(const BaseType& value)
			{
				this->value*=value;
				return *this;
			}

			FixPointValue& operator/=(const FixPointValue& a)
			{
				this->value/=a.value;
				this->value<<=offset;
				return *this;
			}

			bool operator<(const FixPointValue& a)
			{
				return this->value < a.value;
			}

			bool operator>(const FixPointValue& a)
			{
				return this->value > a.value;
			}

			bool operator<=(const FixPointValue& a)
			{
				return this->value <= a.value;
			}

			bool operator>=(const FixPointValue& a)
			{
				return this->value >= a.value;
			}

			bool operator==(const FixPointValue& a)
			{
				return this->value == a.value;
			}

			bool operator!=(const FixPointValue& a)
			{
				return this->value !=a.value;
			}

			template<typename T>
			FixPointValue& operator/=(const T& value)
			{
				this->value/=value;
				return *this;
			}


			template<typename T>
			FixPointValue operator+(const T value) const
			{
				FixPointValue temp(*this);
				temp+=value;
				return temp;
			}

			template<typename T>
			FixPointValue operator-(const T value) const
			{
				FixPointValue temp(*this);
				temp-=value;
				return temp;
			}

			template<typename T>
			FixPointValue operator*(const T value) const
			{
				FixPointValue temp(*this);
				temp*=value;
				return temp;
			}

			template<typename T>
			FixPointValue operator/(const T value) const
			{
				FixPointValue temp(*this);
				temp/=value;
				return temp;
			}

			FixPointValue operator-() const
			{
				FixPointValue temp;
				temp.value=-this->value;
				return temp;
			}

			BaseType trunc() const
			{
				return this->value>>offset;
			}

		private:

			template<typename Logger>
			void log(Logger& log, unsigned char base, uint8_t precision) const
			{
				static const BaseType postDotMask=(1ULL<<offset)-1;
				static const BaseType preDotMask=~(0LL)&~postDotMask;

				typedef typename boost::make_unsigned<BaseType>::type OutType;
				typedef typename avr_halib::mplExt::promote<OutType>::type CalcType;

				BOOST_STATIC_ASSERT( boost::is_unsigned<CalcType>::type::value && sizeof(CalcType)==2*sizeof(BaseType));

				CalcType value;
				if(this->value<0)
				{
					value=-this->value;
					log << "-";
				}
				else
					value=this->value;

				log << (OutType)((value&preDotMask)>>offset) << ".";

				value=value&postDotMask;

				OutType one=1ULL<<offset;
				for(uint8_t i=0;i<precision;i++)
				{
					value*=base;
					if(value<one)
						log << '0';
				}

				value/=(1ULL<<offset);

				log << (OutType)value;
			}

			template<typename> friend class avr_halib::logging::OutputStreamExtension;
	};
}
}
