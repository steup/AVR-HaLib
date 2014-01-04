#pragma once

#include <stdint.h>

#include <avr-halib/config/limits.h>
#include "promote.h"

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_unsigned.hpp>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib object */
namespace object
{
    /** \brief TODO \todo
     *
     * \tparam BaseType TODO \todo
     * \tparam offset TODO \todo
     */
    template<typename BaseType, uint8_t offset>
    class FixPointValue
    {
        /** \brief TODO \todo */
        BaseType value;

        public:
          /** \brief TODO \todo */
          typedef BaseType ValueType;
          /** \brief TODO \todo */
          static const uint8_t exponent=offset;

          /** \brief TODO \todo */
          static FixPointValue min()
          {
              FixPointValue temp;
              temp.value=std::numeric_limits<BaseType>::min();
              return temp;
          }

          /** \brief TODO \todo */
          static FixPointValue max()
          {
              FixPointValue temp;
              temp.value=std::numeric_limits<BaseType>::min();
              return temp;
          }

          /** \brief TODO \todo */
          static FixPointValue epsilon()
          {
              FixPointValue temp;
              temp.value=1;
              return temp;
          }

          /** \brief TODO \todo */
          explicit FixPointValue() : value(0){}

          /** \brief TODO \todo */
          FixPointValue(BaseType preDot, BaseType postDot=0)
          {
              this->value=0;
              this->value|=postDot&((1<<offset)-1);
              this->value|=preDot<<offset;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          FixPointValue& operator+=(const FixPointValue& a)
          {
              this->value+=a.value;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param value TODO \todo
           */
          FixPointValue& operator+=(const BaseType& value)
          {
              this->value+=((BaseType)value<<offset);
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          FixPointValue& operator-=(const FixPointValue& a)
          {
              this->value-=a.value;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param value TODO \todo
           */
          FixPointValue& operator-=(const BaseType& value)
          {
              this->value-=((BaseType)value<<offset);
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          FixPointValue& operator*=(const FixPointValue& a)
          {
              this->value>>=offset/2;
              this->value*=a.value>>(offset-offset/2);
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param value TODO \todo
           */
          FixPointValue& operator*=(const BaseType& value)
          {
              this->value*=value;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          FixPointValue& operator/=(const FixPointValue& a)
          {
              this->value/=a.value;
              this->value<<=offset;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator<(const FixPointValue& a)
          {
              return this->value < a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator>(const FixPointValue& a)
          {
              return this->value > a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator<=(const FixPointValue& a)
          {
              return this->value <= a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator>=(const FixPointValue& a)
          {
              return this->value >= a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator==(const FixPointValue& a)
          {
              return this->value == a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator!=(const FixPointValue& a)
          {
              return this->value !=a.value;
          }

          /** \brief TODO \brief
           *
           * \tparam T TODO \todo
           * \param value TODO \todo
           */
          template<typename T>
          FixPointValue& operator/=(const T& value)
          {
              this->value/=value;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \tparam T TODO \todo
           * \param value TODO \todo
           */
          template<typename T>
          FixPointValue operator+(const T value) const
          {
              FixPointValue temp(*this);
              temp+=value;
              return temp;
          }

          /** \brief TODO \brief
           *
           * \tparam T TODO \todo
           * \param value TODO \todo
           */
          template<typename T>
          FixPointValue operator-(const T value) const
          {
              FixPointValue temp(*this);
              temp-=value;
              return temp;
          }

          /** \brief TODO \brief
           *
           * \tparam T TODO \todo
           * \param value TODO \todo
           */
          template<typename T>
          FixPointValue operator*(const T value) const
          {
              FixPointValue temp(*this);
              temp*=value;
              return temp;
          }

          /** \brief TODO \brief
           *
           * \tparam T TODO \todo
           * \param value TODO \todo
           */
          template<typename T>
          FixPointValue operator/(const T value) const
          {
              FixPointValue temp(*this);
              temp/=value;
              return temp;
          }

          /** \brief TODO \brief */
          FixPointValue operator-() const
          {
              FixPointValue temp;
              temp.value=-this->value;
              return temp;
          }

          /** \brief TODO \todo */
          BaseType trunc() const
          {
              return this->value>>offset;
          }

      private:
          /** \brief TODO \brief
           *
           * \tparam Logger TODO \todo
           * \param log TODO \todo
           * \param base TODO \todo
           * \param precision TODO \todo
           */
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
