//-------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// May not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//-------------------------------------------------------------------------------
#pragma once

#include "geom.h"
#include "Tolerance.h"
#include <lc/util/Round.h>
#include <string>
#include <stdexcept>
#include <iostream>

namespace lc::geom {

//! Point template class
//!
//! \paramT  Coordinate type
template <typename T>
class Point2dT
{
public:
    using coord = T;
    using dist = T;
    using VectorT = Vector2dT<T>;

    //! Origin point
    static constexpr Point2dT origin() { return Point2dT(T(0), T(0)); }

    //! Default constructor
    constexpr Point2dT()
        : x()
        , y()
    {}

    //! Parameter constructor
    constexpr Point2dT(T x, T y)
        : x(x)
        , y(y)
    {}

    //! Explicit conversion
    template <typename U>
    explicit Point2dT(const Point2dT<U>& other);

    //! Implicit conversion
    template <typename U>
    explicit operator Point2dT<U>()
    {
        return Point2dT<U>(this);
    }

    //! Construct from Point
    explicit Point2dT(const VectorT& v);

    //! Assignment operator
    template <typename U>
    Point2dT& operator=(const Point2dT<U>& other);

    //! Set components
    void set(T x, T y);

    //! Offset by vector
    Point2dT& operator+=(const VectorT& v);

    //! Sum of points
    Point2dT& operator+=(const Point2dT& pt);

    //! Offset by vector
    Point2dT& operator-=(const VectorT& v);

    //! Division by scalar
    Point2dT& operator/=(T d);

    //! Multiplication by scalar
    Point2dT& operator*=(T m);

    //! Unary negation
    Point2dT operator-() const;

    //! Unary +
    Point2dT operator+() const;

    //! Equality operator
    bool operator==(const Point2dT& pt) const;

    //! Inequality operator
    bool operator!=(const Point2dT& pt) const;

    //! less than
    bool operator<(const Point2dT& pt) const;

    //! less than or equal
    bool operator<=(const Point2dT& pt) const;

    //! greater than
    bool operator>(const Point2dT& pt) const;

    //! greater than or equal
    bool operator>=(const Point2dT& pt) const;

    //! Indexed-based vector component extraction
    T operator[](int index) const;

    //! Extract coordinates into 2D array
    const T* coords2d(T c[2]) const;

    //! Extract coordinates into 3D array
    const T* coords3d(T c[3]) const;

    //! Swap x- and y-coordinates
    void swap();

    //! Test if this point is equal to other point within specified tolerance
    [[nodiscard]] bool equals(const Point2dT& other,
                              const Tolerance& tol = Tolerance::global) const;


public:
    T x;  //!< x component
    T y;  //!< y component

private:
    template <typename U>
    friend class Point2dT;
    template <typename U>
    Point2dT(const Point2dT<U>*);
};

//------------------------------------------------------------------------------
using Point2d = Point2dT<double>;

template <typename T>
template <typename U>
Point2dT<T>::Point2dT(const Point2dT<U>& other)
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
{}

template <typename T>
template <typename U>
Point2dT<T>::Point2dT(const Point2dT<U>* other)
    : x(static_cast<T>(other->x))
    , y(static_cast<T>(other->y))
{}

template <typename T>
Point2dT<T>::Point2dT(const VectorT& v)
    : x(v.x)
    , y(v.y)
{}

// Assignment
template <typename T>
template <typename U>
Point2dT<T>& Point2dT<T>::operator=(const Point2dT<U>& other)
{
    x = other.x;
    y = other.y;
    return *this;
}

// Set components
template <typename T>
void Point2dT<T>::set(T x, T y)
{
    this->x = x;
    this->y = y;
}

// Offset by vector
template <typename T>
Point2dT<T>& Point2dT<T>::operator+=(const Vector2dT<T>& v)
{
    x += v.x;
    y += v.y;
    return (*this);
}

// Normal addition:        P = Q + v
template <typename T>
Point2dT<T> operator+(Point2dT<T> lhs, const Vector2dT<T>& rhs)
{
    return (lhs += rhs);
}

// Sum of points
template <typename T>
Point2dT<T>& Point2dT<T>::operator+=(const Point2dT<T>& pt)
{
    x += pt.x;
    y += pt.y;
    return (*this);
}

// Convenience function:   Q = P + R
template <typename T>
Point2dT<T> operator+(Point2dT<T> lhs, const Point2dT<T>& rhs)
{
    return (lhs += rhs);
}

// Offset by vector
template <typename T>
Point2dT<T>& Point2dT<T>::operator-=(const Vector2dT<T>& v)
{
    x -= v.x;
    y -= v.y;
    return (*this);
}

// Normal subtraction:     Q = P - v
template <typename T>
Point2dT<T> operator-(Point2dT<T> lhs, const Vector2dT<T>& rhs)
{
    return lhs -= rhs;
}

// Normal subtraction:     v = P - Q
template <typename T>
Vector2dT<T> operator-(Point2dT<T> lhs, const Point2dT<T>& rhs)
{
    return Vector2dT<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Division by scalar
template <typename T>
Point2dT<T>& Point2dT<T>::operator/=(T d)
{
    x /= d;
    y /= d;
    return (*this);
}

// Division by scalar
template <typename T, Scalar U>
Point2dT<T> operator/(Point2dT<T> pt, U d)
{
    return pt /= d;
}

// Multiplication by scalar
template <typename T>
Point2dT<T>& Point2dT<T>::operator*=(T m)
{
    x *= m;
    y *= m;
    return (*this);
}

// Multiplication with scalar
template <typename T, Scalar U>
Point2dT<T> operator*(Point2dT<T> pt, U m)
{
    return pt *= m;
}

// Multiplication with scalar
template <typename T, Scalar U>
Point2dT<T> operator*(U m, Point2dT<T> pt)
{
    return pt *= m;
}

// Unary negation
template <typename T>
Point2dT<T> Point2dT<T>::operator-() const
{
    return Point2dT(-x, -y);
}

// Unary +
template <typename T>
Point2dT<T> Point2dT<T>::operator+() const
{
    return *this;
}

// Equality operator
template <typename T>
bool Point2dT<T>::operator==(const Point2dT<T>& pt) const
{
    return (x == pt.x) && (y == pt.y);
}

// Inequality operator
template <typename T>
bool Point2dT<T>::operator!=(const Point2dT<T>& pt) const
{
    return !(*this == pt);
}

// less than
template <typename T>
bool Point2dT<T>::operator<(const Point2dT<T>& pt) const
{
    return (x != pt.x) ? (x < pt.x) : (y < pt.y);
}

// less than or equal
template <typename T>
bool Point2dT<T>::operator<=(const Point2dT<T>& pt) const
{
    return (x != pt.x) ? (x < pt.x) : (y <= pt.y);
}

// greater than
template <typename T>
bool Point2dT<T>::operator>(const Point2dT<T>& pt) const
{
    return !(*this <= pt);
}

// greater than or equal
template <typename T>
bool Point2dT<T>::operator>=(const Point2dT<T>& pt) const
{
    return !(*this < pt);
}

// Indexed-based vector component extraction
template <typename T>
T Point2dT<T>::operator[](int index) const
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    default: throw std::invalid_argument("index out of range");
    }
}

// Extract coordinates into 2D array
template <typename T>
const T* Point2dT<T>::coords2d(T c[2]) const
{
    c[0] = x;
    c[1] = y;
    return c;
}

// Extract coordinates into 3D array
template <typename T>
const T* Point2dT<T>::coords3d(T c[3]) const
{
    c[0] = x;
    c[1] = y;
    c[3] = T();
    return c;
}

// Swap x- and y-coordinates
template <typename T>
void Point2dT<T>::swap()
{
    std::swap(x, y);
}

// Test if this vector is equal to other vector within specified tolerance
template <typename T>
bool Point2dT<T>::equals(const Point2dT& other,
                         const Tolerance& tol /* = Tolerance::global */) const
{
    return (*this - other).length() < tol.maximumCumulativeError();
}

// Round point coordinates to nearest integer
template <typename R, typename T>
R round(const Point2dT<T>& pt,
        const util::Round<typename R::coord>& rounder = util::Round<typename R::coord>())
{
    return R(rounder.nearest(pt.x), rounder.nearest(pt.y));
}

// Return absolute point
template <typename T>
Point2dT<T> abs(const Point2dT<T>& pt)
{
    return Point2dT<T>(std::abs(pt.x), std::abs(pt.y));
}

// Fast cast to different point type
template <typename R, typename T>
R cast(const Point2dT<T>& pt)
{
    return R(static_cast<typename R::coord>(pt.x), static_cast<typename R::coord>(pt.y));
}


// Fast cast to different point type
template <typename R, typename T>
R fastCast(const Point2dT<T>& pt)
{
    return R(util::fastCast<typename R::coord>(pt.x), util::fastCast<typename R::coord>(pt.y));
}

// Return absolute point
template <typename T>
std::string to_string(const Point2dT<T>& pt)
{
    using std::to_string;

    return std::string("(") + to_string(pt.x) + "," + to_string(pt.y) + ")";
}

// Stream output operator
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point2dT<T>& pt)
{
    return os << "(" << pt.x << ", " << pt.y << ")";
}

}  // namespace lc::geom
