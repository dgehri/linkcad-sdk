//-------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// May not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//-------------------------------------------------------------------------------
#pragma once
#include "geom.h"
#include "Angle.h"
#include "Tolerance.h"

#if !defined(__GNUC__) && !defined(__clang__)
#include <boost/multiprecision/cpp_int.hpp>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include <iostream>

namespace lc::geom {

#if defined(__GNUC__) || defined(__clang__)
using coord_128 = __int128;  // Use built-in 128-bit integer for GCC/Clang
#elif defined(_MSC_VER)
using coord_128 = boost::multiprecision::int128_t;  // Use Boost Multiprecision for MSVC
#endif

//! 2D Vector
//!
//! \paramT  Coordinate type
template <typename T>
class Vector2dT final
{
public:
    using dist = T;
    using coord = T;
    using PointT = Point2dT<T>;

    //! Null-vector
    static constexpr Vector2dT zeroVector() { return Vector2dT(T(0), T(0)); }

    //! x-axis unit vector
    static constexpr Vector2dT ux() { return Vector2dT(T(1), T(0)); }

    //! y-axis unit vector
    static constexpr Vector2dT uy() { return Vector2dT(T(0), T(1)); }

    //! Default constructor
    constexpr Vector2dT()
        : x()
        , y()
    {}

    //! Parameter constructor
    constexpr Vector2dT(T x, T y)
        : x(x)
        , y(y)
    {}

    //! Explicit conversion
    template <typename U>
    constexpr explicit Vector2dT(const Vector2dT<U>& other)
        : x(static_cast<T>(other.x))
        , y(static_cast<T>(other.y))
    {}

    //! Implicit conversion
    template <typename U>
    explicit constexpr operator Vector2dT<U>() const
    {
        return Vector2dT<U>(this);
    }

    //! Construct from Point
    constexpr explicit Vector2dT(const PointT& pt)
        : x(pt.x)
        , y(pt.y)
    {}

    //! Assignment operator
    template <typename U>
    Vector2dT& operator=(const Vector2dT<U>& other);

    //! Set components
    void set(T x, T y);

    //! Construct from elliptical polar coordinates
    static Vector2dT fromPolar(T a, T b, Angle angle);

    //! Construct from polar coordinates
    static Vector2dT fromPolar(T r, Angle angle);

    //! Vector addition: u += rhs
    Vector2dT& operator+=(const Vector2dT& v);

    //! Vector difference: u -= rhs
    Vector2dT& operator-=(const Vector2dT& v);

    //! Division by scalar: u /= s
    Vector2dT& operator/=(T d);

    //! Multiplication by scalar: u *= s
    Vector2dT& operator*=(T m);

    //! Unary negation
    constexpr Vector2dT operator-() const;

    //! Unary +
    constexpr Vector2dT operator+() const;

    //! Equality operator
    constexpr bool operator==(const Vector2dT& v) const;

    //! Inequality operator
    constexpr bool operator!=(const Vector2dT& v) const;

    //! Less than
    constexpr bool operator<(const Vector2dT& v) const;

    //! less than or equal
    constexpr bool operator<=(const Vector2dT& v) const;

    //! greater than
    constexpr bool operator>(const Vector2dT& v) const;

    //! greater than or equal
    constexpr bool operator>=(const Vector2dT& v) const;

    //! Indexed-based vector component extraction
    constexpr T operator[](int index) const;

    //! Return perpendicular vector (rotated 90deg CCW)
    constexpr Vector2dT perp() const;

    //! Return vector with negated x-axis
    constexpr Vector2dT negatedX() const;

    //! Return vector with negated y-axis
    constexpr Vector2dT negatedY() const;

    //! Swap x- and y-coordinates
    void swap();

    //! Return squared norm of vector
    constexpr double squareNorm() const;

    //! Return infinity norm of vector
    T infinityNorm() const;

    //! Normalize this vector
    void normalize();

    //! Return vector length
    double length() const;

    //! Return value d = [length, 106% * length]
    double fastLength() const;

    //! Return vector angle (measured counter-clockwise from positive x-axis)
    Angle angle() const;

    //! Return vector slope
    double slope() const;

    //! Test if null vector
    constexpr bool isNull() const;

    //! Test if argument vector is co-directional to this vector
    bool isCodirectionalTo(const Vector2dT& other, const Tolerance& tol = Tolerance::global) const;

    //! Test if this vector is equal to other vector within specified tolerance
    bool equals(const Vector2dT& other, const Tolerance& tol = Tolerance::global) const;

    //! Test if this vector's length is equal to other vector's length within specified tolerance
    bool lengthEquals(const Vector2dT& other, const Tolerance& tol = Tolerance::global) const;

    //! Return rotated vector about z-axis (CCW)
    Vector2dT rotated(Angle angleCcw) const;

public:
    T x;  //!< x component
    T y;  //!< y component

private:
    template <typename U>
    friend class Vector2dT;
    template <typename U>
    constexpr Vector2dT(const Vector2dT<U>* other)
        : x(static_cast<T>(other->x))
        , y(static_cast<T>(other->y))
    {}
};

//------------------------------------------------------------------------------
using Vector2d = Vector2dT<double>;

//------------------------------------------------------------------------------
// Assignment
template <typename T>
template <typename U>
Vector2dT<T>& Vector2dT<T>::operator=(const Vector2dT<U>& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

// Set components
template <typename T>
void Vector2dT<T>::set(T x, T y)
{
    this->x = x;
    this->y = y;
}

// Construct from elliptical polar coordinates
template <>
inline Vector2dT<double> Vector2dT<double>::fromPolar(double a, double b, Angle angle)
{
    return Vector2dT<double>(a * cos(angle), b * sin(angle));
}

template <>
inline Vector2dT<float> Vector2dT<float>::fromPolar(float a, float b, Angle angle)
{
    return Vector2dT(static_cast<float>(a * cos(angle)), static_cast<float>(b * sin(angle)));
}

template <>
inline Vector2dT<int32_t> Vector2dT<int32_t>::fromPolar(int32_t a, int32_t b, Angle angle)
{
    util::Round<int32_t> r;
    return Vector2dT(r.nearest(a * cos(angle)), r.nearest(b * sin(angle)));
}

template <>
inline Vector2dT<int64_t> Vector2dT<int64_t>::fromPolar(int64_t a, int64_t b, Angle angle)
{
    util::Round<int64_t> r;
    return Vector2dT(r.nearest(a * cos(angle)), r.nearest(b * sin(angle)));
}

template <typename T>
Vector2dT<T> Vector2dT<T>::fromPolar(T r, Angle angle)
{
    return fromPolar(r, r, angle);
}

// Vector sum
template <typename T>
Vector2dT<T>& Vector2dT<T>::operator+=(const Vector2dT<T>& v)
{
    x += v.x;
    y += v.y;
    return (*this);
}

// Vector sum
template <typename T>
Vector2dT<T> operator+(Vector2dT<T> lhs, const Vector2dT<T>& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

// Vector subtraction
template <typename T>
Vector2dT<T>& Vector2dT<T>::operator-=(const Vector2dT<T>& v)
{
    x -= v.x;
    y -= v.y;
    return (*this);
}

// Vector subtraction
template <typename T>
constexpr Vector2dT<T> operator-(Vector2dT<T> lhs, const Vector2dT<T>& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

// Division by scalar
template <typename T>
Vector2dT<T>& Vector2dT<T>::operator/=(T d)
{
    x /= d;
    y /= d;
    return (*this);
}

// Division by scalar
template <typename T, Scalar U>
constexpr Vector2dT<T> operator/(Vector2dT<T> v, U d)
{
    return {v.x / d, v.y / d};
}

// Multiplication with scalar
template <typename T>
Vector2dT<T>& Vector2dT<T>::operator*=(T m)
{
    x *= m;
    y *= m;
    return (*this);
}

// Multiplication with scalar
template <typename T, Scalar U>
constexpr Vector2dT<T> operator*(Vector2dT<T> v, U m)
{
    return {v.x * m, v.y * m};
}

// Multiplication with scalar
template <typename T, Scalar U>
constexpr Vector2dT<T> operator*(U m, Vector2dT<T> v)
{
    return v *= m;
}

// Dot product
template <typename T>
constexpr T dotProduct(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

// Dot product
template <typename T>
constexpr T operator*(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    return dotProduct(lhs, rhs);
}

// Cross product
template <typename T>
constexpr T crossProduct(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

// Cross product
template <typename T>
constexpr T operator^(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    return crossProduct(lhs, rhs);
}

// Unary negation
template <typename T>
constexpr Vector2dT<T> Vector2dT<T>::operator-() const
{
    return Vector2dT(-x, -y);
}

// Unary +
template <typename T>
constexpr Vector2dT<T> Vector2dT<T>::operator+() const
{
    return *this;
}

// Equality operator
template <typename T>
constexpr bool Vector2dT<T>::operator==(const Vector2dT<T>& v) const
{
    return (x == v.x) && (y == v.y);
}

// Inequality operator
template <typename T>
constexpr bool Vector2dT<T>::operator!=(const Vector2dT<T>& v) const
{
    return !(*this == v);
}

// Less than
template <typename T>
constexpr bool Vector2dT<T>::operator<(const Vector2dT<T>& v) const
{
    return (x != v.x) ? (x < v.x) : (y < v.y);
}

// Less than or equal
template <typename T>
constexpr bool Vector2dT<T>::operator<=(const Vector2dT<T>& v) const
{
    return (x != v.x) ? (x < v.x) : (y <= v.y);
}

// Greater than
template <typename T>
constexpr bool Vector2dT<T>::operator>(const Vector2dT<T>& v) const
{
    return !(*this <= v);
}

// Greater than or equal
template <typename T>
constexpr bool Vector2dT<T>::operator>=(const Vector2dT<T>& v) const
{
    return !(*this < v);
}

// Indexed-based vector component extraction
template <typename T>
constexpr T Vector2dT<T>::operator[](int index) const
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    default: throw std::invalid_argument("index out of range");
    }
}

// Return perpendicular vector (rotated 90deg CCW)
template <typename T>
constexpr Vector2dT<T> Vector2dT<T>::perp() const
{
    return Vector2dT(-y, x);
}

// Return vector perpendicular to input vector (rotated 90deg CCW)
template <typename T>
constexpr Vector2dT<T> perp(const Vector2dT<T>& l)
{
    return l.perp();
}

// Return vector with negated x-axis
template <typename T>
constexpr Vector2dT<T> Vector2dT<T>::negatedX() const
{
    return Vector2dT(-x, y);
}

// Return vector with negated y-axis
template <typename T>
constexpr Vector2dT<T> Vector2dT<T>::negatedY() const
{
    return Vector2dT(x, -y);
}

// Swap x- and y-coordinates
template <typename T>
void Vector2dT<T>::swap()
{
    std::swap(x, y);
}

// Return squared norm of vector
template <typename T>
constexpr double Vector2dT<T>::squareNorm() const
{
    return static_cast<double>(x) * static_cast<double>(x) +
           static_cast<double>(y) * static_cast<double>(y);
}

// Return normalized vector
template <typename T>
constexpr double squareNorm(const Vector2dT<T>& l)
{
    return l.squareNorm();
}

// Return infinity norm of vector
template <typename T>
T Vector2dT<T>::infinityNorm() const
{
    T x0 = x > 0 ? x : -x;
    T y0 = y > 0 ? y : -y;
    return x0 > y0 ? x0 : y0;
}

// Normalized vector
template <typename T>
void Vector2dT<T>::normalize()
{
    T len = length();
    if (len > T())
    {
        (*this) /= len;
    }
}

// Return normalized vector
template <typename T>
Vector2dT<T> normalize(Vector2dT<T> v)
{
    v.normalize();
    return v;
}

// Return vector length
template <typename T>
double Vector2dT<T>::length() const
{
    return _hypot(static_cast<double>(x), static_cast<double>(y));
}

// Return vector length
template <typename T>
double length(const Vector2dT<T>& l)
{
    return l.length();
}

// Return value d = [length, 106% * length]
template <typename T>
double Vector2dT<T>::fastLength() const
{
    auto xAbs = std::abs(static_cast<double>(x));
    auto yAbs = std::abs(static_cast<double>(y));
    if (xAbs < yAbs)
        return yAbs + xAbs / 2.0;
    else
        return xAbs + yAbs / 2.0;
}

// Return value in range [length, 106% * length]
template <typename T>
double fastLength(const Vector2dT<T>& l)
{
    return l.fastLength();
}

// Return vector angle (measured counter-clockwise from positive x-axis)
template <typename T>
Angle Vector2dT<T>::angle() const
{
    return Angle::fromRadians(atan2(static_cast<double>(y), static_cast<double>(x)));
}

// Return slope
template <typename T>
double Vector2dT<T>::slope() const
{
    return static_cast<double>(y) / static_cast<double>(x);
}

//  Return vector angle
template <typename T>
Angle angle(const Vector2dT<T>& l)
{
    return l.angle();
}

// Check if null vector
template <typename T>
constexpr bool Vector2dT<T>::isNull() const
{
    return x == T(0) && y == T(0);
}

// Test if this vector is equal to other vector within specified tolerance
template <typename T>
bool Vector2dT<T>::equals(const Vector2dT& other,
                          const Tolerance& tol /* = Tolerance::global */) const
{
    return (*this - other).length() < tol.maximumError();
}

// Test if this vector length is equal to other vector within specified tolerance
template <typename T>
bool Vector2dT<T>::lengthEquals(const Vector2dT& other,
                                const Tolerance& tol /* = Tolerance::global */) const
{
    return std::abs(length() - other.length()) < tol.maximumError();
}

// Test if argument vector is co-directional to this vector
template <typename T>
bool Vector2dT<T>::isCodirectionalTo(const Vector2dT& other,
                                     const Tolerance& tol /* = Tolerance::global */) const
{
    Vector2dT norm1 = normalize(*this);
    Vector2dT norm2 = normalize(other);
    return norm1.equals(norm2, tol);
}

// Return rotated vector about z-axis (CCW)
template <typename T>
Vector2dT<T> Vector2dT<T>::rotated(Angle angleCcw) const
{
    auto c = cos(angleCcw);
    auto s = sin(angleCcw);
    return Vector2dT<T>(x * c - y * s, x * s + y * c);
}

// Return absolute vector
template <typename T>
Vector2dT<T> abs(const Vector2dT<T>& v)
{
    return {std::abs(v.x), std::abs(v.y)};
}

// Round to closest integer coordinates
template <typename R, typename T>
R round(const Vector2dT<T>& v,
        const util::Round<typename R::dist>& rounder = util::Round<typename R::dist>())
{
    return R(rounder.nearest(v.x), rounder.nearest(v.y));
}

// Fast cast to different vector type
template <typename R, typename T>
R fastCast(const Vector2dT<T>& v)
{
    return R(util::fastCast<typename R::dist>(v.x), util::fastCast<typename R::dist>(v.y));
}


// Calculate the sign of the dot product using 128-bit integer arithmetic
template <typename T>
constexpr int signOfDotProduct(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    auto p1 = static_cast<coord_128>(lhs.x) * static_cast<coord_128>(rhs.x);
    auto p2 = static_cast<coord_128>(lhs.y) * static_cast<coord_128>(rhs.y);

    if (p1 > -p2)
        return 1;
    else if (p1 == -p2)
        return 0;
    else
        return -1;
}

template <typename T>
constexpr int signOfCrossProduct(const Vector2dT<T>& lhs, const Vector2dT<T>& rhs)
{
    double tolerance = (lhs.length() + rhs.length()) * 1.0e-5;
    auto p1 = static_cast<double>(lhs.x) * static_cast<double>(rhs.y);
    auto p2 = static_cast<double>(lhs.y) * static_cast<double>(rhs.x);
    if (p1 <= p2 - tolerance)
        return -1;
    else if (p1 < p2 + tolerance)
        return 0;
    else
        return 1;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

//! Calculate the cross product of two vectors and return the result with a sign
//!
//! The sign is -1 if the cross product is negative, 0 if it is zero, and 1 if it is positive.
//!
//! \param lhs Left-hand side vector
//! \param rhs Right-hand side vector
//!
//! \return Pair of cross product and sign
template <typename T>
constexpr static std::pair<T, int> crossProductWithSign(const Vector2dT<T>& lhs,
                                                        const Vector2dT<T>& rhs)
{
    double tolerance = (lhs.length() + rhs.length()) * 1.0e-5;
    auto p1 = static_cast<double>(lhs.x) * static_cast<double>(rhs.y);
    auto p2 = static_cast<double>(lhs.y) * static_cast<double>(rhs.x);
    auto cross = p1 - p2;

    // Ensure that the cross product is within the range of T
    if (cross < static_cast<double>(std::numeric_limits<T>::min()) ||
        cross > static_cast<double>(std::numeric_limits<T>::max()))
        throw std::overflow_error("Cross product is out of range");

    if (p1 <= p2 - tolerance)
        return std::make_pair(static_cast<T>(cross), -1);
    else if (p1 < p2 + tolerance)
        return std::make_pair(0, 0);
    else
        return std::make_pair(static_cast<T>(cross), 1);
}

// Stream output operator
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector2dT<T>& v)
{
    return os << "(" << v.x << ", " << v.y << ")";
}

}  // namespace lc::geom
