//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "lcassert.h"
#include "Round.h"
#include <limits>
#include <type_traits>
#include <math.h>
#include <intrin.h>

namespace lc::util {

//! Returns the square of the argument
//!
//! \param   x   Argument
//! \return  Square of argument
template <typename T>
constexpr T square(T x)
{
    return x * x;
}

//! Returns the remainder of the division x / y
//!
//! \note The result is always non-negative
//!
//! \param   x   Numerator (can be either positive or negative)
//! \param   y   Denominator (must be positive)
//! \return  The remainder of dividing the arguments
template <typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type modulo(T x, T y)
{
    return (x >= T(0)) ? ((x < y) ? x : ::fmod(x, y)) : (::fmod(x, y) + y);
}

//! Returns the remainder of the division x / y
//!
//! \note The result is always non-negative
//!
//! \param   x   Numerator (can be either positive or negative)
//! \param   y   Denominator (must be positive)
//! \return  The remainder of dividing the arguments
template <typename T>
constexpr typename std::enable_if<std::is_integral<T>::value, T>::type modulo(T x, T y)
{
    return (x >= T(0)) ? ((x < y) ? x : (x % y)) : ((x % y) + y);
}

//! Get GCD (greatest common integer divider)
template <typename T>
T gcd(T x, T y)
{
    T a = std::abs(x);
    T b = std::abs(y);
    T tmp = b;

    // GCD
    while (tmp != 0)
    {
        b = tmp;
        tmp = a % tmp;
        a = b;
    }

    return a;
}

//! Cast 64-bit int to 32-bit int (asserts integer overflows)
//!
//! \param   n   Input value
//! \return  Cast result
constexpr long cast32(int64_t n)
{
    return static_cast<int32_t>(n);
}

//! Calculate integer log2 of positive number
//!
//! \param   value   Input value
//! \return  log_2 of the input value, rounded to the next lower integer value.
template <typename T>
T log2(T value)
{
    ASSERT(value);
    T n = 0;
    while (value)
    {
        value >>= 1;
        ++n;
    }

    return n - 1;
}

//! Round positive value up to next power of two
//!
//! \param   value   Positive input value
//! \return  Next larger power of two
template <typename T>
T nextPowerOf2(T value)
{
    T n = 1;
    while (value > n)
    {
        n <<= 1;
    }

    return n;
}

//! Round to nearest multiple of \c granularity
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Nearest multiple of \c granularity
template <typename T>
constexpr typename std::enable_if<std::is_unsigned<T>::value, T>::type roundNearest(T value,
                                                                                    T granularity)
{
    return ((value + (granularity / 2)) / granularity) * granularity;
}

//! Round to nearest multiple of \c granularity
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Nearest multiple of \c granularity
template <typename T>
constexpr typename std::enable_if<std::is_signed<T>::value, T>::type roundNearest(T value,
                                                                                  T granularity)
{
    return value >= 0 ? ((value + (granularity / 2)) / granularity) * granularity
                      : (((-value) + ((granularity - 1) / 2)) / granularity) * -granularity;
}

//! Round to nearest multiple of \c granularity
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Nearest multiple of \c granularity
template <typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type roundNearest(
    T value, T granularity)
{
    return ceil((value - (granularity / T(2.0)) + T(0.5)) / granularity) * granularity;
}

//! Round up to next larger multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next larger multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_unsigned<T>::value, T>::type roundUp(T value,
                                                                               T granularity)
{
    return ((value + granularity - 1) / granularity) * granularity;
}

//! Round up to next larger multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next larger multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_signed<T>::value, T>::type roundUp(T value, T granularity)
{
    return value >= 0 ? ((value + granularity - 1) / granularity) * granularity
                      : ((-value) / granularity) * -granularity;
}

//! Round up to next larger multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next larger multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type roundUp(T value,
                                                                                     T granularity)
{
    return ceil(value / granularity) * granularity;
}

//! Round down to next smaller multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next smaller multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_unsigned<T>::value, T>::type roundDown(T value,
                                                                                 T granularity)
{
    return ((value) / granularity) * granularity;
}

//! Round down to next smaller multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next smaller multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_signed<T>::value, T>::type roundDown(T value,
                                                                               T granularity)
{
    return value >= 0 ? ((value) / granularity) * granularity
                      : ((-value + granularity - 1) / granularity) * -granularity;
}

//! Round down to next smaller multiple of \c granularity
//!
//! \param   value       Value to round
//! \param   granularity Multiple to round to
//! \return  Next smaller multiple of granularity
template <typename T>
constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type roundDown(
    T value, T granularity)
{
    return floor(value / granularity) * granularity;
}

#define BITSPERLONG (8 * sizeof(uint64_t))
#define TOP2BITS(x) ((x & (INT64_C(3) << (BITSPERLONG - 2))) >> (BITSPERLONG - 2))

//! Calculate the integer square root
//!
//! \param   x   Argument
//! \return  Integer square root of argument
inline __declspec(deprecated) uint64_t usqrt(uint64_t x)
{
    uint64_t a = INT64_C(0);  // accumulator
    uint64_t r = INT64_C(0);  // remainder

    for (auto i = 0u; i < BITSPERLONG; i++)
    {
        r = (r << 2) + TOP2BITS(x);
        x <<= 2;
        a <<= 1;
        auto e = (a << 1) + 1;
        if (r >= e)
        {
            r -= e;
            a++;
        }
    }

    // round
    if (a & (INT64_C(1) << (BITSPERLONG / 2 - 1)))
    {
        a >>= (BITSPERLONG / 2);
        a++;
    }
    else
    {
        a >>= (BITSPERLONG / 2);
    }
    return a;
}

//! Return 10^exp

template <typename T>
typename std::enable_if<std::is_integral<T>::value && (!std::is_signed<T>::value) &&
                            (sizeof(T) <= 8),
                        T>::type
pow10(int exp)
{
    ASSERT(exp >= 0 && exp <= 19);
    static const T powers[] = {
        static_cast<T>(1),  // 10^0
        static_cast<T>(10),  // 10^1
        static_cast<T>(100),  // 10^2
        static_cast<T>(1000),  // 10^3
        static_cast<T>(10000),  // 10^4
        static_cast<T>(100000),  // 10^5
        static_cast<T>(1000000),  // 10^6
        static_cast<T>(10000000),  // 10^7
        static_cast<T>(100000000),  // 10^8
        static_cast<T>(1000000000),  // 10^9
        static_cast<T>(10000000000),  // 10^10
        static_cast<T>(100000000000),  // 10^11
        static_cast<T>(1000000000000),  // 10^12
        static_cast<T>(10000000000000),  // 10^13
        static_cast<T>(100000000000000),  // 10^14
        static_cast<T>(1000000000000000),  // 10^15
        static_cast<T>(10000000000000000),  // 10^16
        static_cast<T>(100000000000000000),  // 10^17
        static_cast<T>(1000000000000000000),  // 10^18
        static_cast<T>(10000000000000000000u)  // 10^19
    };

    return powers[exp];
}

//! Return 10^exp

template <typename T>
typename std::
    enable_if<std::is_integral<T>::value && (std::is_signed<T>::value) && (sizeof(T) == 8), T>::type
    pow10(int exp)
{
    ASSERT(exp >= 0 && exp <= 18);
    static const T powers[] = {
        static_cast<T>(1),  // 10^0
        static_cast<T>(10),  // 10^1
        static_cast<T>(100),  // 10^2
        static_cast<T>(1000),  // 10^3
        static_cast<T>(10000),  // 10^4
        static_cast<T>(100000),  // 10^5
        static_cast<T>(1000000),  // 10^6
        static_cast<T>(10000000),  // 10^7
        static_cast<T>(100000000),  // 10^8
        static_cast<T>(1000000000),  // 10^9
        static_cast<T>(10000000000),  // 10^10
        static_cast<T>(100000000000),  // 10^11
        static_cast<T>(1000000000000),  // 10^12
        static_cast<T>(10000000000000),  // 10^13
        static_cast<T>(100000000000000),  // 10^14
        static_cast<T>(1000000000000000),  // 10^15
        static_cast<T>(10000000000000000),  // 10^16
        static_cast<T>(100000000000000000),  // 10^17
        static_cast<T>(1000000000000000000)  // 10^18
    };

    return powers[exp];
}

//! Return 10^exp
template <typename T>
constexpr typename std::enable_if<std::is_integral<T>::value && (sizeof(T) == 4), T>::type pow10(
    int exp)
{
    ASSERT(exp >= 0 && exp <= 9);
    constexpr T powers[] = {
        1,  // 10^0
        10,  // 10^1
        100,  // 10^2
        1000,  // 10^3
        10000,  // 10^4
        100000,  // 10^5
        1000000,  // 10^6
        10000000,  // 10^7
        100000000,  // 10^8
        1000000000  // 10^9
    };

    return powers[exp];
}

namespace detail {

// default to false
template <typename T, typename U, typename Enable = void>
struct TypeHelper
{
    static bool test(U) { return false; }
};

// if identical signedness, smaller fits into same or larger type
template <typename T, typename U>
struct TypeHelper<
    T,
    U,
    typename std::enable_if<std::is_signed<T>::value == std::is_signed<U>::value &&
                            !std::is_enum<U>::value && (sizeof(U) <= sizeof(T))>::type>
{
    static bool test(U) { return true; }
};

// if identical signedness, larger fits into smaller if within its limits
template <typename T, typename U>
struct TypeHelper<T,
                  U,
                  typename std::enable_if<std::is_signed<T>::value == std::is_signed<U>::value &&
                                          !std::is_enum<U>::value && (sizeof(U) > sizeof(T))>::type>
{
    static bool test(U value)
    {
        return (value >= static_cast<U>(std::numeric_limits<T>::lowest())) &&
               (value <= static_cast<U>(std::numeric_limits<T>::max()));
    }
};

// signed fits into unsigned if within its limits
template <typename T, typename U>
struct TypeHelper<
    T,
    U,
    typename std::enable_if<std::is_unsigned<T>::value && std::is_signed<U>::value>::type>
{
    static bool test(U value)
    {
        return (value >= 0) &&
               (sizeof(U) <= sizeof(T) ||
                (sizeof(U) == sizeof(T) &&
                 static_cast<T>(value) <= std::numeric_limits<T>::max()) ||
                (sizeof(U) > sizeof(T) && value <= static_cast<U>(std::numeric_limits<T>::max())));
    }
};

// smaller unsigned always fits into larger signed
template <typename T, typename U>
struct TypeHelper<T,
                  U,
                  typename std::enable_if<std::is_signed<T>::value && std::is_unsigned<U>::value &&
                                          (sizeof(U) < sizeof(T))>::type>
{
    static bool test(U) { return true; }
};

// unsigned fits into smaller or same-sized signed if within its limits
template <typename T, typename U>
struct TypeHelper<T,
                  U,
                  typename std::enable_if<std::is_signed<T>::value && std::is_unsigned<U>::value &&
                                          (sizeof(U) >= sizeof(T))>::type>
{
    static bool test(U value) { return (value <= static_cast<U>(std::numeric_limits<T>::max())); }
};


// unsigned fits into smaller or same-sized signed if within its limits
template <typename T, typename U>
struct TypeHelper<T, U, typename std::enable_if<std::is_enum<U>::value>::type>
{
    static bool test(U value)
    {
        using V = typename std::underlying_type<U>::type;
        return detail::TypeHelper<T, V>::test(static_cast<V>(value));
    }
};

}  // namespace detail

//! Test if source \p value fits into (possibly smaller sized) target type
//!
//! \param value     Value to test
//! \return          `true` if `value` fits into target type, `false` otherwise
//! \tparam T        Target type
//! \tparam U        Source type
template <typename T, typename U>
bool fitsIntoType(U value)
{
    return detail::TypeHelper<T, U>::test(value);
}

//! Find first bit set in a word
//!
//! The ffs() function returns the position of the first (least significant) bit
//! set in the word i. The least significant bit is position 1 and the most
//! significant position is, for example, 32 or 64.
//!
//! \param value     Value to test
//! \return          Position of the first bit set, or 0 if no bits are set.
//@{

#pragma intrinsic(_BitScanForward)

inline uint8_t ffs(uint32_t value)
{
    unsigned long index;
    if (_BitScanForward(&index, value))
        return static_cast<uint8_t>(index + 1);
    else
        return 0;
}

#ifdef _WIN64

#pragma intrinsic(_BitScanForward64)

inline uint8_t ffs(uint64_t value)
{
    unsigned long index;
    if (_BitScanForward64(&index, value))
        return static_cast<uint8_t>(index + 1);
    else
        return 0;
}

#else

inline uint8_t ffs(uint64_t value)
{
    unsigned long index;
    if (_BitScanForward(&index, static_cast<uint32_t>(value)))
        return static_cast<uint8_t>(index + 1);
    else if (_BitScanForward(&index, value >> 32))
        return static_cast<uint8_t>(index + 33);
    else
        return 0;
}

#endif

inline uint8_t ffs(int32_t value)
{
    return ffs(static_cast<uint32_t>(value));
}
inline uint8_t ffs(int64_t value)
{
    return ffs(static_cast<uint64_t>(value));
}

//@}

}  // namespace lc::util
