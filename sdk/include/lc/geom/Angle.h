//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include <lc/util/lcassert.h>
#include <lc/util/lcmath.h>
#include <math.h>
#include <iostream>

namespace lc::geom {

//! Wrapper class for angle
class Angle final
{
public:
    //! Angle representing 0 (0°)
    static const Angle zero;

    //! Angle representing 2*Pi (360°)
    static const Angle twoPi;

    //! Angle representing Pi (180°)
    static const Angle pi;

    //! Angle representing Pi/2 (90°)
    static const Angle piHalf;

    //! Angle representing 3*Pi/2 (270°)
    static const Angle threePiHalf;

    //! Epsilon value for angles
    static const Angle epsilon;

    //! Default constructor (initializes angle at 0°)
    constexpr Angle()
        : radians_(0.0)
    {}

    //! Construct angle from radians
    static constexpr Angle fromRadians(double radians);

    //! Construct angle from degrees
    static constexpr Angle fromDegrees(double degrees);

    //! Get angle in radians
    constexpr double radians() const;

    //! Get angle in degrees
    constexpr double degrees() const;

    //! Compare angle to within specified tolerance
    constexpr bool equals(Angle rhs, Angle tolerance = Angle::epsilon) const;

    //! Compare angle for equivalence
    bool equivalentTo(Angle rhs, Angle tolerance = Angle::epsilon) const;

    //!@{
    //! Overloaded operators
    constexpr Angle operator-() const { return Angle(-radians_); }
    constexpr Angle operator+() const { return *this; }
    Angle& operator+=(const Angle& angle)
    {
        radians_ += angle.radians();
        return *this;
    }
    Angle& operator-=(const Angle& angle)
    {
        radians_ -= angle.radians();
        return *this;
    }
    Angle& operator*=(double multiplier)
    {
        radians_ *= multiplier;
        return *this;
    }
    Angle& operator/=(double divider)
    {
        radians_ /= divider;
        return *this;
    }
    //!@}

private:
    //! Private constructor from radians
    explicit constexpr Angle(double radians)
        : radians_(radians)
    {}

    static constexpr double abs(double value) { return value >= 0.0 ? value : -value; }

private:
    double radians_;
};

#ifdef PI
#undef PI
#endif

//! PI
constexpr double PI = 3.141592653589793238462643383280;

//! 2 * PI
constexpr double TWO_PI = 6.283185307179586476925286766559;

//! PI / 2
constexpr double PI_HALF = 1.570796326794896619231321691640;

__declspec(selectany) const Angle Angle::zero(0.0);
__declspec(selectany) const Angle Angle::twoPi(TWO_PI);
__declspec(selectany) const Angle Angle::pi(PI);
__declspec(selectany) const Angle Angle::piHalf(PI_HALF);
__declspec(selectany) const Angle Angle::threePiHalf(3 * PI_HALF);
// epsilon defined as tan(1/(2^32))
__declspec(selectany) const Angle Angle::epsilon(2.3283064365386962891045725816118e-10);

//! Convert degrees to radians
constexpr double degreesToRadians(double degrees)
{
    return degrees * PI / 180.0;
}

//! Convert radians to degrees
constexpr double radiansToDegrees(double radians)
{
    return radians * 180.0 / PI;
}

//! Normalize angle to range [lowerBound, lowerBound + 2*PI]
inline Angle normalizeInclusive(Angle angle, Angle lowerBound = Angle::zero)
{
    auto rad = angle.radians();
    while (rad < lowerBound.radians())
    {
        rad += TWO_PI;
    }
    while (rad > (lowerBound.radians() + TWO_PI))
    {
        rad -= TWO_PI;
    }
    return Angle::fromRadians(rad);
}

//! Normalize angle to range [lowerBound, lowerBound + 2*PI)
inline Angle normalize(Angle angle, Angle lowerBound = Angle::zero)
{
    auto rad = angle.radians();
    while (rad < lowerBound.radians())
    {
        rad += TWO_PI;
    }
    while (rad >= (lowerBound.radians() + TWO_PI))
    {
        rad -= TWO_PI;
    }
    return Angle::fromRadians(rad);
}

//------------------------------------------------------------------------------
constexpr Angle Angle::fromRadians(double radians)
{
    return Angle(radians);
}

//------------------------------------------------------------------------------
constexpr Angle Angle::fromDegrees(double degrees)
{
    return Angle(degreesToRadians(degrees));
}

//------------------------------------------------------------------------------
constexpr double Angle::radians() const
{
    return radians_;
}

//------------------------------------------------------------------------------
constexpr double Angle::degrees() const
{
    return radiansToDegrees(radians_);
}

//------------------------------------------------------------------------------
constexpr bool Angle::equals(Angle rhs, Angle tolerance /*= Angle::epsilon*/) const
{
    return abs(radians_ - rhs.radians()) < tolerance.radians();
}

//------------------------------------------------------------------------------
inline bool Angle::equivalentTo(Angle rhs, Angle tolerance /*= Angle::epsilon*/) const
{
    return abs(normalize(*this).radians() - normalize(rhs).radians()) < tolerance.radians();
}

//------------------------------------------------------------------------------
constexpr bool operator==(const Angle& lhs, const Angle& rhs)
{
    return lhs.radians() == rhs.radians();
}

//------------------------------------------------------------------------------
constexpr bool operator!=(const Angle& lhs, const Angle& rhs)
{
    return lhs.radians() != rhs.radians();
}

//------------------------------------------------------------------------------
constexpr Angle operator+(Angle lhs, const Angle& rhs)
{
    return Angle::fromRadians(lhs.radians() + rhs.radians());
}

//------------------------------------------------------------------------------
constexpr Angle operator-(Angle lhs, const Angle& rhs)
{
    return Angle::fromRadians(lhs.radians() - rhs.radians());
}

//------------------------------------------------------------------------------
constexpr Angle operator*(double multiplier, Angle angle)
{
    return Angle::fromRadians(angle.radians() * multiplier);
}

//------------------------------------------------------------------------------
constexpr Angle operator*(Angle angle, double multiplier)
{
    return Angle::fromRadians(angle.radians() * multiplier);
}

//------------------------------------------------------------------------------
inline Angle operator/(Angle angle, double divisor)
{
    return Angle::fromRadians(angle.radians() / divisor);
}

//------------------------------------------------------------------------------
inline double operator/(Angle lhs, Angle rhs)
{
    return lhs.radians() / rhs.radians();
}

//------------------------------------------------------------------------------
inline bool operator>(Angle lhs, Angle rhs)
{
    return lhs.radians() > rhs.radians();
}

//------------------------------------------------------------------------------
inline bool operator>=(Angle lhs, Angle rhs)
{
    return lhs.radians() >= rhs.radians();
}

//------------------------------------------------------------------------------
inline bool operator<(Angle lhs, Angle rhs)
{
    return lhs.radians() < rhs.radians();
}

//------------------------------------------------------------------------------
inline bool operator<=(Angle lhs, Angle rhs)
{
    return lhs.radians() <= rhs.radians();
}

//! Stream output operator
inline std::ostream& operator<<(std::ostream& os, const Angle& angle)
{
    return os << angle.degrees() << "°";
}

//! Fast approximation of tangent (tan)
//!
//! \note The maximum absolute error is 8.0613e-04 for an input angle in range
//!       [0, Pi/4] radians
//!
//! \param   angle  Angle in range [0, Pi/4] radians
//! \return  Tangent of argument
inline double fasttan(Angle angle)
{
    ASSERT(0.0 <= angle.radians() && angle.radians() <= PI / 4.0);

    auto sq = util::square(angle.radians());
    auto res = 2.033e-01;
    res *= sq;
    res += 3.1755e-01;
    res *= sq;
    res += 1.0;
    res *= angle.radians();
    return res;
}

//!@{
//! Overloaded functions
inline double cos(Angle angle)
{
    return ::cos(angle.radians());
}
inline double sin(Angle angle)
{
    return ::sin(angle.radians());
}
inline double tan(Angle angle)
{
    return ::tan(angle.radians());
}
inline Angle abs(Angle angle)
{
    return Angle::fromRadians(::abs(angle.radians()));
}
inline Angle fmod(Angle angle, Angle denom)
{
    return Angle::fromRadians(::fmod(angle.radians(), denom.radians()));
}
//!@}

}  // namespace lc::geom

constexpr lc::geom::Angle operator"" _deg(long double deg)
{
    return lc::geom::Angle::fromDegrees(static_cast<double>(deg));
}

constexpr lc::geom::Angle operator"" _rad(long double rad)
{
    return lc::geom::Angle::fromRadians(static_cast<double>(rad));
}
