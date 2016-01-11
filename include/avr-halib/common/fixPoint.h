#pragma once

#include <stdint.h>

#include <avr-halib/config/limits.h>

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/promote.hpp>

#include <math.h>

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
          FixPointValue(float num)
          {
              BaseType preDot = ::truncf(num);
              num -= preDot;
              this->value=preDot<<offset;
              this->value|=(BaseType)::truncf(num*(1<<offset));
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
              typename boost::promote<BaseType>::type temp;
              temp = this->value;
              temp *= a.value;
              this->value = temp>>offset;
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
              uint8_t dynOffset=offset;
              while(this->value < std::numeric_limits<BaseType>::max()/2  && dynOffset-- > 0) {
                this->value*=2;
              }
                
              this->value/=a.value;
              this->value<<=dynOffset;
              return *this;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator<(const FixPointValue& a) const
          {
              return this->value < a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator>(const FixPointValue& a) const
          {
              return this->value > a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator<=(const FixPointValue& a) const
          {
              return this->value <= a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator>=(const FixPointValue& a) const
          {
              return this->value >= a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator==(const FixPointValue& a) const
          {
              return this->value == a.value;
          }

          /** \brief TODO \brief
           *
           * \param a TODO \todo
           */
          bool operator!=(const FixPointValue& a) const
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
          
          /** \brief TODO \todo */
          FixPointValue abs() const
          {
            if((*this)<0)
              return -(*this);
             else
              return *this;
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

              BaseType value = this->value;
              if(value < 0) {
                  value = -value; 
                  log << "-";
              }

              log << (value>>offset) << ".";

              value=value&postDotMask;
              if(!value)
                log << '0';
              while(value) {
                value *= 10;
                BaseType digit = value >> offset;
                log << digit;
                value -= digit << offset;
              }
          }

          template<typename> friend class avr_halib::logging::OutputStreamExtension;
    };
}
}
