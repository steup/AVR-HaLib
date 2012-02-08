#pragma once

namespace std
{
	enum float_round_style {
	  round_indeterminate       = -1,
	  round_toward_zero         =  0,
	  round_to_nearest          =  1,
	  round_toward_infinity     =  2,
	  round_toward_neg_infinity =  3
	};

	enum float_denorm_style {
	  denorm_indeterminate      = -1,
	  denorm_absent             =  0,
	  denorm_present            =  1
	};

}

namespace avr_halib
{
namespace config
{
namespace helpers
{
	using namespace std;

	template <typename T>
	class numeric_limits_base {
		public:
		  static const bool               is_specialized    = false;
		  static const int                digits            = 0;
		  static const int                digits8           = 0;
		  static const int                digits10          = 0;
		  static const int                digits16          = 0;
		  static const bool               is_signed         = false;
		  static const bool               is_integer        = false;
		  static const bool               is_exact          = false;
		  static const int                radix             = 0;
		  static const int                min_exponent      = 0;
		  static const int                min_exponent10    = 0;
		  static const int                max_exponent      = 0;
		  static const int                max_exponent10    = 0;
		  static const bool               has_infinity      = false;
		  static const bool               has_quiet_NaN     = false;
		  static const bool               has_signaling_NaN = false;
		  static const float_denorm_style has_denorm        = denorm_absent;
		  static const bool               has_denorm_loss   = false;
		  static const bool               is_iec559         = false;
		  static const bool               is_bounded        = false;
		  static const bool               is_modulo         = false;
		  static const bool               traps             = false;
		  static const bool               tinyness_before   = false;
		  static const float_round_style  round_style       = round_toward_zero;

		  static T min()           throw() { return static_cast<T>(0); }
		  static T max()           throw() { return static_cast<T>(0); }
		  static T lowest()        throw() { return static_cast<T>(0); }

		  static T epsilon()       throw() { return static_cast<T>(0); }
		  static T round_error()   throw() { return static_cast<T>(0); }
		  static T infinity()      throw() { return static_cast<T>(0); }
		  static T quiet_NaN()     throw() { return static_cast<T>(0); }
		  static T signaling_NaN() throw() { return static_cast<T>(0); }
		  static T denorm_min()    throw() { return static_cast<T>(0); }

	};

	template <typename IntType, IntType minValue, IntType maxValue, int digitNr = -1>
	class integer_limits : public numeric_limits_base<IntType> 
	{
		  static const int calcDigits2      = (int)(sizeof(IntType) * 8) - (minValue == 0 ? 0 : 1);
		  static const int selectDigits2    = (digitNr < 0) ? calcDigits2 : digitNr;
		  static const int calcDigits8      = selectDigits2/ 3 + (selectDigits2 % 3) ? 1 : 0;
		  static const int calcDigits10     = selectDigits2 * 301 / 1000 + (selectDigits2 * 301 % 1000) ? 1 : 0; 
		  static const int calcDigits16     = selectDigits2 / 4 + (selectDigits2 % 4) ? 1 : 0;
		public:
		  static const bool is_specialized = true;

		  static const int digits          = selectDigits2;
		  static const int digits8         = calcDigits8;
		  static const int digits10        = calcDigits10;
		  static const int digits16        = calcDigits16;

		  static const bool is_signed      = (minValue != 0);
		  static const bool is_integer     = true;
		  static const bool is_exact       = true;
		  static const int  radix          = 2;

		  static const bool is_bounded     = true;
		  static const bool is_modulo      = true;

		  static IntType min()    throw() { return minValue; }
		  static IntType max()    throw() { return maxValue; }
	};

}
}
}

namespace std
{

	template<class T> 
	class numeric_limits : public avr_halib::config::helpers::numeric_limits_base<T> {};

	template<>
	class numeric_limits<bool>
	  : public avr_halib::config::helpers::integer_limits<bool, false, true, 1>
	{};

	template<>
	class numeric_limits<signed char>
	  : public avr_halib::config::helpers::integer_limits<signed char, 0x80, 0x7F>
	{};

	template<>
	class numeric_limits<unsigned char>
	  : public avr_halib::config::helpers::integer_limits<unsigned char, 0x0, 0xFF>
	{};

	template<>
	class numeric_limits<char>
	  : public numeric_limits<signed char>
	{};

	template<>
	class numeric_limits<short>
	  : public avr_halib::config::helpers::integer_limits<short, 0x8000, 0x7FFF>
	{};

	template<>
	class numeric_limits<unsigned short>
	  : public avr_halib::config::helpers::integer_limits<unsigned short, 0x0, 0xFFFFU>
	{};

	template<>
	class numeric_limits<int>
	  : public avr_halib::config::helpers::integer_limits<int, 0x8000, 0x7FFF>
	{};

	template<>
	class numeric_limits<unsigned int>
	  : public avr_halib::config::helpers::integer_limits<unsigned int, 0x0, 0xFFFFU>
	{};

	template<>
	class numeric_limits<long>
	  : public avr_halib::config::helpers::integer_limits<long, 0x80000000L, 0x7FFFFFFFL>
	{};

	template<>
	class numeric_limits<unsigned long>
	  : public avr_halib::config::helpers::integer_limits<unsigned long, 0x0UL, 0xFFFFFFFFUL>
	{};

	template<>
	class numeric_limits<long long>
	  : public avr_halib::config::helpers::integer_limits<long long, 0x8000000000000000LL, 0x7FFFFFFFFFFFFFFFLL>
	{};

	template<>
	class numeric_limits<unsigned long long>
	  : public avr_halib::config::helpers::integer_limits<unsigned long long, 0x0ULL, 0xFFFFFFFFFFFFFFFFULL>
	{};
}
