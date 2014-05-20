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

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib config */
namespace config
{
    /** AVR-HaLib config helpers */
    namespace helpers
    {
        using namespace std;

        /** \brief TODO \todo
         *
         * \tparam T TODO \todo
         */
        template <typename T>
        class numeric_limits_base
        {
            public:
                static const bool               is_specialized    = false;              /**< TODO \todo */
                static const int                digits            = 0;                  /**< TODO \todo */
                static const int                digits8           = 0;                  /**< TODO \todo */
                static const int                digits10          = 0;                  /**< TODO \todo */
                static const int                digits16          = 0;                  /**< TODO \todo */
                static const bool               is_signed         = false;              /**< TODO \todo */
                static const bool               is_integer        = false;              /**< TODO \todo */
                static const bool               is_exact          = false;              /**< TODO \todo */
                static const int                radix             = 0;                  /**< TODO \todo */
                static const int                min_exponent      = 0;                  /**< TODO \todo */
                static const int                min_exponent10    = 0;                  /**< TODO \todo */
                static const int                max_exponent      = 0;                  /**< TODO \todo */
                static const int                max_exponent10    = 0;                  /**< TODO \todo */
                static const bool               has_infinity      = false;              /**< TODO \todo */
                static const bool               has_quiet_NaN     = false;              /**< TODO \todo */
                static const bool               has_signaling_NaN = false;              /**< TODO \todo */
                static const float_denorm_style has_denorm        = denorm_absent;      /**< TODO \todo */
                static const bool               has_denorm_loss   = false;              /**< TODO \todo */
                static const bool               is_iec559         = false;              /**< TODO \todo */
                static const bool               is_bounded        = false;              /**< TODO \todo */
                static const bool               is_modulo         = false;              /**< TODO \todo */
                static const bool               traps             = false;              /**< TODO \todo */
                static const bool               tinyness_before   = false;              /**< TODO \todo */
                static const float_round_style  round_style       = round_toward_zero;  /**< TODO \todo */

                /** \brief TODO \todo */
                static T min()           throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T max()           throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T lowest()        throw() { return static_cast<T>(0); }

                /** \brief TODO \todo */
                static T epsilon()       throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T round_error()   throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T infinity()      throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T quiet_NaN()     throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T signaling_NaN() throw() { return static_cast<T>(0); }
                /** \brief TODO \todo */
                static T denorm_min()    throw() { return static_cast<T>(0); }
        };

        /** \brief TODO \todo
         *
         * \tparam IntType TODO \todo
         * \tparam minValue TODO \todo
         * \tparam maxValue TODO \todo
         * \tparam digitNr TODO \todo
         */
        template <typename IntType, IntType minValue, IntType maxValue, int digitNr = -1>
        class integer_limits : public numeric_limits_base<IntType>
        {
            /** \brief TODO \todo */
            static const int calcDigits2   = (int)(sizeof(IntType) * 8) - (minValue == 0 ? 0 : 1);
            /** \brief TODO \todo */
            static const int selectDigits2 = (digitNr < 0) ? calcDigits2 : digitNr;
            /** \brief TODO \todo */
            static const int calcDigits8   = selectDigits2/ 3 + (selectDigits2 % 3) ? 1 : 0;
            /** \brief TODO \todo */
            static const int calcDigits10  = selectDigits2 * 301 / 1000 + (selectDigits2 * 301 % 1000) ? 1 : 0;
            /** \brief TODO \todo */
            static const int calcDigits16  = selectDigits2 / 4 + (selectDigits2 % 4) ? 1 : 0;

            public:
                /** \brief TODO \todo */
                static const bool is_specialized = true;

                /** \brief TODO \todo */
                static const int digits      = selectDigits2;
                /** \brief TODO \todo */
                static const int digits8     = calcDigits8;
                /** \brief TODO \todo */
                static const int digits10    = calcDigits10;
                /** \brief TODO \todo */
                static const int digits16    = calcDigits16;

                /** \brief TODO \todo */
                static const bool is_signed  = (minValue != 0);
                /** \brief TODO \todo */
                static const bool is_integer = true;
                /** \brief TODO \todo */
                static const bool is_exact   = true;
                /** \brief TODO \todo */
                static const int  radix      = 2;

                /** \brief TODO \todo */
                static const bool is_bounded = true;
                /** \brief TODO \todo */
                static const bool is_modulo  = true;

                /** \brief TODO \todo */
                static IntType min() throw() { return minValue; }
                /** \brief TODO \todo */
                static IntType max() throw() { return maxValue; }
        };

    }
}
}

namespace std
{
    /** \brief TODO \todo
     *
     * \tparam T TODO \todo
     */
    template<class T>
    class numeric_limits : public avr_halib::config::helpers::numeric_limits_base<T> {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<bool>
      : public avr_halib::config::helpers::integer_limits<bool, false, true, 1>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<signed char>
      : public avr_halib::config::helpers::integer_limits<signed char, 0x80, 0x7F>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<unsigned char>
      : public avr_halib::config::helpers::integer_limits<unsigned char, 0x0, 0xFF>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<char>
      : public numeric_limits<signed char>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<short>
      : public avr_halib::config::helpers::integer_limits<short, 0x8000, 0x7FFF>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<unsigned short>
      : public avr_halib::config::helpers::integer_limits<unsigned short, 0x0, 0xFFFFU>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<int>
      : public avr_halib::config::helpers::integer_limits<int, 0x8000, 0x7FFF>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<unsigned int>
      : public avr_halib::config::helpers::integer_limits<unsigned int, 0x0, 0xFFFFU>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<long>
      : public avr_halib::config::helpers::integer_limits<long, 0x80000000L, 0x7FFFFFFFL>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<unsigned long>
      : public avr_halib::config::helpers::integer_limits<unsigned long, 0x0UL, 0xFFFFFFFFUL>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<long long>
      : public avr_halib::config::helpers::integer_limits<long long, 0x8000000000000000LL, 0x7FFFFFFFFFFFFFFFLL>
    {};

    /** \brief TODO \todo */
    template<>
    class numeric_limits<unsigned long long>
      : public avr_halib::config::helpers::integer_limits<unsigned long long, 0x0ULL, 0xFFFFFFFFFFFFFFFFULL>
    {};
}
