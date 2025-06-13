//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "lcassert.h"
#include <limits>
#include <emmintrin.h>
#include <cstdint>
#include <cmath>

namespace lc::util {

//! Class for fast rounding of floating point values to integer
template <typename T>
class Round
{
public:
    //! Default constructor.
    Round();

    //! Destructor.
    ~Round();

    //! Arithmetically round value to nearest integer (round half towards +infinity)
    //!
    //! \note Half-way values are always rounded up towards +infinity
    //!
    //! \param   value   Argument
    //!
    //! \return  Argument rounded to nearest integer value
    T nearest(double value) const;

private:
    unsigned int savedMode_;
};

//------------------------------------------------------------------------------
template <typename T>
__forceinline Round<T>::Round()
{
#ifndef _MANAGED
#ifndef _WIN64
    unsigned int fpword;
    __asm
    {
        fstcw fpword;  // store FP control word in 'fpword'
        fwait;  // wait until completed
    }

    savedMode_ = fpword;
    fpword = (fpword & 0xf3ff) | 0x0400;  // RC_DOWN

    __asm
    {
        fldcw  fpword;
    }
#endif
#endif
}

//------------------------------------------------------------------------------
template <typename T>
__forceinline Round<T>::~Round()
{
#ifndef _MANAGED
#ifndef _WIN64
    unsigned int fpword = savedMode_;
    __asm
    {
        fldcw fpword;
    }
#endif
#endif
}

//------------------------------------------------------------------------------
template <typename T>
__forceinline

//! Arithmetically round value to nearest integer (round half towards +infinity)
//! 
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value 
T Round<T>::nearest(double value) const
{
#ifdef _DEBUG
    ASSERT(value <= static_cast<double>(std::numeric_limits<T>::max()) - 0.5);
#endif

#ifndef _MANAGED
#ifdef _WIN64
    return static_cast<T>(floor(value + 0.5));
#else
    static const double half = 0.5;

    T result;
    __asm
    {
        fld value;
        fadd half;
        fistp result
    }

    return result;
#endif
#else
    return (T)System::Math::Floor(value + 0.5);
#endif
}

//------------------------------------------------------------------------------
#ifdef _WIN64
//! Arithmetically round value to nearest integer (round half towards +infinity)
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value
template <>
__forceinline int Round<int>::nearest(double value) const
{
    if (value > 0)
    {
        value += 0.5;
        return _mm_cvttsd_si32(_mm_load_sd(&value));
    }
    else
    {
        value = floor(value + 0.5);
        return _mm_cvttsd_si32(_mm_load_sd(&value));
    }
}

//! Arithmetically round value to nearest integer (round half towards +infinity)
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value
template <>
__forceinline long Round<long>::nearest(double value) const
{
    if (value > 0)
    {
        value += 0.5;
        return _mm_cvttsd_si32(_mm_load_sd(&value));
    }
    else
    {
        value = floor(value + 0.5);
        return _mm_cvttsd_si32(_mm_load_sd(&value));
    }
}
#endif

#ifndef _MANAGED
//------------------------------------------------------------------------------
template <>
class Round<double>
{
public:
    //! Arithmetically round value to nearest integer (round half towards +infinity)
    //!
    //! \note Half-way values are always rounded up towards +infinity
    //!
    //! \param   value   Argument
    //!
    //! \return  Argument rounded to nearest integer value
    __forceinline double nearest(double value) const { return floor(value + 0.5); }
};

//------------------------------------------------------------------------------
template <>
class Round<float>
{
public:
    //! Arithmetically round value to nearest integer (round half towards +infinity)
    //!
    //! \note Half-way values are always rounded up towards +infinity
    //!
    //! \param   value   Argument
    //!
    //! \return  Argument rounded to nearest integer value
    __forceinline float nearest(float value) const { return (float)floor(value + 0.5f); }
};

#endif

//! Arithmetically round value to nearest integer (round half towards +infinity)
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value
template <typename T>
__forceinline T round(double value)
{
#pragma warning(push)
#pragma warning(disable : 4244)
#ifndef _MANAGED
    return static_cast<T>(Round<T>().nearest(value));
#else
    return (T)System::Math::Floor(value + 0.5);
#endif
#pragma warning(pop)
}

//! Arithmetically round value to nearest integer (round half towards +infinity)
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value
template <typename T>
__forceinline T round(long double value)
{
#pragma warning(push)
#pragma warning(disable : 4244)
#ifndef _MANAGED
    return static_cast<T>(Round<T>().nearest(value));
#else
    return (T)System::Math::Floor(value + 0.5);
#endif  // !_MANAGED
}

//! Arithmetically round value to nearest integer (round half towards +infinity)
//!
//! \note Half-way values are always rounded up towards +infinity
//!
//! \param   value   Argument
//!
//! \return  Argument rounded to nearest integer value
template <typename T>
__forceinline T round(float value)
{
#pragma warning(push)
#pragma warning(disable : 4244)
#ifndef _MANAGED
    return static_cast<T>(Round<T>().nearest(value));
#else
    return (T)System::Math::Floor(value + 0.5);
#endif
#pragma warning(pop)
}

template <class... T>
constexpr bool always_false = false;

//! Cast double value to integer, using current rounding mode
//!
//! \param   value   Value to cast
//!
//! \return  Value cast to integer
template <typename T>
__forceinline T fastCast(double value)
{
#ifndef _MANAGED
#ifdef _WIN64
    static_assert(always_false<T>, "Unsupported template type");
    return (T)value;
#else
    T result;
    __asm
    {
        fld value;
        fistp result
    }

    return result;
#endif
#else
    return (T)value;
#endif
}

#ifdef _WIN64
template <>
__forceinline int fastCast<int>(double value)
{
    return _mm_cvttsd_si32(_mm_load_sd(&value));
}

template <>
__forceinline long fastCast<long>(double value)
{
    return _mm_cvttsd_si32(_mm_load_sd(&value));
}

template <>
__forceinline int64_t fastCast<std::int64_t>(double value)
{
    return _mm_cvttsd_si64(_mm_load_sd(&value));
}
#endif

}  // namespace lc::util
