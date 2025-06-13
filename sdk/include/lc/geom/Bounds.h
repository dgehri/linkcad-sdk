//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "geom.h"
#include "Point2d.h"
#include <algorithm>
#include <concepts>

namespace lc::geom {

//------------------------------------------------------------------------------
// Rectangular bounding box
//------------------------------------------------------------------------------
template <typename T, bool mayBeEmpty = true>
class Bounds final
{
public:
    using dist = T;
    using coord = T;
    using Point = Point2dT<T>;
    using Vector = Vector2dT<T>;

    // Constructor
    Bounds();
    Bounds(coord x0, coord y0, coord x1, coord y1, bool doSanitize = false);
    Bounds(const Point& bottomLeft, const Point& topRight, bool doSanitize = false);

    // Conversions
    // ReSharper disable once CppNonExplicitConvertingConstructor
    template <bool otherMayBeEmpty>
    Bounds(const Bounds<T, otherMayBeEmpty>& other);

    // Set bounds
    void set(coord x0, coord y0, coord x1, coord y1, bool doSanitize = false);
    void set(const Point& bottomLeft, const Point& topRight, bool doSanitize = false);
    void set(const Bounds& bound);

    // Expand bounds with other bounds
    Bounds& expandWith(const Bounds& bound);

    // Expand bounds
    Bounds& expandWith(coord x0, coord y0, coord x1, coord y1);

    // Expand bounds
    Bounds& expandWith(const Point& bottomLeft, const Point& topRight);

    // Expand bounds
    Bounds& expandWith(coord x, coord y);

    // Expand bounds
    Bounds& expandWith(const Point& v) { return expandWith(v.x, v.y); }

    // Grow bound by n in all directions
    Bounds& grow(coord n);

    // Translate bound
    Bounds& translate(coord dx, coord dy) { return translate(Vector(dx, dy)); }

    // Translate bound
    Bounds& translate(const Vector& offset);

    // Intersect with another bound
    Bounds& intersect(const Bounds& bound);

    // Set center
    Bounds& setCenter(const Point& c);

    // Get center of bounds
    [[nodiscard]] Point center() const;

    // Get center of bounds
    [[nodiscard]] coord centerX() const;

    // Get center of bounds
    [[nodiscard]] coord centerY() const;

    // Get width
    [[nodiscard]] dist width() const;

    // Get height
    [[nodiscard]] dist height() const;

    // Get width as double
    [[nodiscard]] double widthD() const;

    // Get height as double
    [[nodiscard]] double heightD() const;

    // Get width as double
    [[nodiscard]] long double widthLD() const;

    // Get height as double
    [[nodiscard]] long double heightLD() const;

    // Get minimum x coordinate
    [[nodiscard]] coord minX() const;
    T& minX();

    // Get maximum x coordinate
    [[nodiscard]] coord maxX() const;
    T& maxX();

    // Get minimum y coordinate
    [[nodiscard]] coord minY() const;
    T& minY();

    // Get maximum y coordinate
    [[nodiscard]] coord maxY() const;
    T& maxY();

    // Get bottom-left point
    [[nodiscard]] const Point& minXY() const;

    // Get top-right point
    [[nodiscard]] const Point& maxXY() const;

    // Get bottom right point
    [[nodiscard]] Point maxXminY() const;

    // Get top-left point
    [[nodiscard]] Point minXmaxY() const;

    //! Get point through index
    //!
    //! \param index     Index parameter
    //! \return          Point at index (0: minXY, 1: maxXminY, 2: maxXY, 3: minXmaxY)
    Point operator[](int index) const;

    // Get diagonal
    [[nodiscard]] Vector extent() const;

    // Get area
    [[nodiscard]] double area() const;

    // Test if bound overlaps other bound
    [[nodiscard]] bool overlaps(const Bounds& other) const;

    // Test if other bound contained in this bound
    [[nodiscard]] bool contains(const Bounds& other) const;

    // Test if point contained in this bound (optionally using tolerance distance)
    [[nodiscard]] bool contains(const Point& pt, dist tolerance = 0) const;

    // Clip point against bound (returned point will be inside bound)
    [[nodiscard]] Point clip(Point v, dist tolerance = 0) const;

    // Test if empty bound
    [[nodiscard]] bool empty() const;

    // Test if bound empty or area zero
    [[nodiscard]] bool areaEmpty() const;

    // Clear bound
    Bounds& clear();

    template <typename U>
    Bounds<typename U::coord> cast() const;

    // Compare bound with other
    bool operator==(const Bounds& rhs) const;
    bool operator!=(const Bounds& rhs) const;

    // Translate
    Bounds& operator+=(const Vector& offset);
    Bounds& operator-=(const Vector& offset);

    Bounds& operator*=(dist factor);
    friend Bounds operator*(dist factor, Bounds other) { return other *= factor; }
    friend Bounds operator*(Bounds other, dist factor) { return other *= factor; }

private:
    template <typename U, bool otherMayBeEmpty>
    friend class Bounds;

    void sanitize();

private:
    Point p0_;
    Point p1_;
};

// -----------------------------------------------------------------------------
using Bounds2d = Bounds<double>;
using Bounds2f = Bounds<float>;

//------------------------------------------------------------------------------
template <typename T, bool e>
void Bounds<T, e>::sanitize()
{
    if (p0_.x > p1_.x)
    {
        std::swap(p0_.x, p1_.x);
    }

    if (p0_.y > p1_.y)
    {
        std::swap(p0_.y, p1_.y);
    }
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>::Bounds()
    : p0_(1, 0)
    , p1_(-1, 0)
{}

//------------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>::Bounds(const Point& bottomLeft, const Point& topRight, bool doSanitize /* = false */)
    : p0_(bottomLeft)
    , p1_(topRight)
{
    if (doSanitize)
    {
        sanitize();
    }
    else
    {
        ASSERT(p0_.x <= p1_.x);
        ASSERT(p0_.y <= p1_.y);
    }
}

// -----------------------------------------------------------------------------
template <typename T, bool mayBeEmpty>
template <bool otherMayBeEmpty>
Bounds<T, mayBeEmpty>::Bounds(const Bounds<T, otherMayBeEmpty>& other)
    : p0_(other.p0_)
    , p1_(other.p1_)
{}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>::Bounds(coord x0, coord y0, coord x1, coord y1, bool doSanitize /* = false */)
    : p0_(x0, y0)
    , p1_(x1, y1)
{
    if (doSanitize)
    {
        sanitize();
    }
    else
    {
        ASSERT(p0_.x <= p1_.x);
        ASSERT(p0_.y <= p1_.y);
    }
}

//------------------------------------------------------------------------------
template <typename T, bool e>
void Bounds<T, e>::set(const Point& bottomLeft,
                       const Point& topRight,
                       bool doSanitize /* = false */)
{
    p0_ = Point(bottomLeft);
    p1_ = Point(topRight);

    if (doSanitize)
    {
        sanitize();
    }
    else
    {
        ASSERT(p0_.x <= p1_.x);
        ASSERT(p0_.y <= p1_.y);
    }
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
void Bounds<T, e>::set(coord x0, coord y0, coord x1, coord y1, bool doSanitize /* = false */)
{
    p0_ = Point(x0, y0);
    p1_ = Point(x1, y1);

    if (doSanitize)
    {
        sanitize();
    }
    else
    {
        ASSERT(p0_.x <= p1_.x);
        ASSERT(p0_.y <= p1_.y);
    }
}

//------------------------------------------------------------------------------
template <typename T, bool e>
void Bounds<T, e>::set(const Bounds& bound)
{
    p0_ = bound.p0_;
    p1_ = bound.p1_;
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::expandWith(T x0, T y0, T x1, T y1)
{
    Point p0Tmp(std::min(x0, x1), std::min(y0, y1));
    Point p1Tmp(std::max(x0, x1), std::max(y0, y1));

    if (!empty())
    {
        if (p0Tmp.x < p0_.x)
            p0_.x = p0Tmp.x;
        if (p1Tmp.x > p1_.x)
            p1_.x = p1Tmp.x;
        if (p0Tmp.y < p0_.y)
            p0_.y = p0Tmp.y;
        if (p1Tmp.y > p1_.y)
            p1_.y = p1Tmp.y;
    }
    else
    {
        // Don't check the first bounding box against the existing values
        p0_ = p0Tmp;
        p1_ = p1Tmp;
    }

    return (*this);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::expandWith(const Point& p0, const Point& p1)
{
    Point p0Tmp(std::min(p0.x, p1.x), std::min(p0.y, p1.y));
    Point p1Tmp(std::max(p0.x, p1.x), std::max(p0.y, p1.y));

    if (!empty())
    {
        if (p0Tmp.x < p0_.x)
            p0_.x = p0Tmp.x;
        if (p1Tmp.x > p1_.x)
            p1_.x = p1Tmp.x;
        if (p0Tmp.y < p0_.y)
            p0_.y = p0Tmp.y;
        if (p1Tmp.y > p1_.y)
            p1_.y = p1Tmp.y;
    }
    else
    {
        // Don't check the first bounding box against the existing values
        p0_ = p0Tmp;
        p1_ = p1Tmp;
    }

    return (*this);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::expandWith(const Bounds<T, e>& bound)
{
    if (!bound.empty())
    {
        if (!empty())
        {
            if (bound.p0_.x < p0_.x)
                p0_.x = bound.p0_.x;
            if (bound.p1_.x > p1_.x)
                p1_.x = bound.p1_.x;
            if (bound.p0_.y < p0_.y)
                p0_.y = bound.p0_.y;
            if (bound.p1_.y > p1_.y)
                p1_.y = bound.p1_.y;
        }
        else
        {
            p0_ = bound.p0_;
            p1_ = bound.p1_;
        }
    }

    return (*this);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::expandWith(T x, T y)
{
    if (!empty())
    {
        if (x < p0_.x)
            p0_.x = x;
        if (x > p1_.x)
            p1_.x = x;
        if (y < p0_.y)
            p0_.y = y;
        if (y > p1_.y)
            p1_.y = y;
    }
    else
    {
        p0_.x = p1_.x = x;
        p0_.y = p1_.y = y;
    }

    return (*this);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::grow(T n)
{
    if (!empty())
    {
        p0_ -= Vector(n, n);
        p1_ += Vector(n, n);
    }

    return (*this);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::overlaps(const Bounds& other) const
{
    return (!empty() && !other.empty() && p0_.x <= other.p1_.x && p1_.x >= other.p0_.x &&
            p0_.y <= other.p1_.y && p1_.y >= other.p0_.y);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::contains(const Bounds& other) const
{
    return (other.empty() || (!empty() && p0_.x <= other.p0_.x && p1_.x >= other.p1_.x &&
                              p0_.y <= other.p0_.y && p1_.y >= other.p1_.y));
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::contains(const Point& pt, dist tolerance) const
{
    return (!empty() && p0_.x <= pt.x + tolerance && p1_.x >= pt.x - tolerance &&
            p0_.y <= pt.y + tolerance && p1_.y >= pt.y - tolerance);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::translate(const Vector& offset)
{
    ASSERT(!empty());
    if (!empty())
    {
        p0_ += offset;
        p1_ += offset;
    }

    return (*this);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::operator-=(const Vector& offset)
{
    return translate(-offset);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::operator+=(const Vector& offset)
{
    return translate(offset);
}

// -----------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::intersect(const Bounds<T, e>& bound)
{
    if (empty() || bound.empty())
    {
        clear();
    }
    else
    {
        p0_.x = std::max(p0_.x, bound.p0_.x);
        p0_.y = std::max(p0_.y, bound.p0_.y);
        p1_.x = std::min(p1_.x, bound.p1_.x);
        p1_.y = std::min(p1_.y, bound.p1_.y);
        if ((p1_.x < p0_.x) || (p1_.y < p0_.y))
        {
            clear();
        }
    }

    return (*this);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::setCenter(const Point& c)
{
    return translate(c - center());
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Point Bounds<T, e>::center() const
{
    return Point(centerX(), centerY());
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::centerX() const
{
    if constexpr (std::integral<T>)
    {
        return (p1_.x >> 1) + (p0_.x >> 1) + ((p1_.x & p0_.x) & 1);
    }
    else
    {
        return (p1_.x + p0_.x) / 2.0;
    }
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::centerY() const
{
    if constexpr (std::integral<T>)
    {
        return (p1_.y >> 1) + (p0_.y >> 1) + ((p1_.y & p0_.y) & 1);
    }
    else
    {
        return (p1_.y + p0_.y) / 2.0;
    }
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::dist Bounds<T, e>::width() const
{
    return empty() ? 0 : p1_.x - p0_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::dist Bounds<T, e>::height() const
{
    return empty() ? 0 : p1_.y - p0_.y;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
double Bounds<T, e>::widthD() const
{
    return empty() ? 0.0 : static_cast<double>(p1_.x) - static_cast<double>(p0_.x);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
double Bounds<T, e>::heightD() const
{
    return empty() ? 0.0 : static_cast<double>(p1_.y) - static_cast<double>(p0_.y);
}


//------------------------------------------------------------------------------
template <typename T, bool e>
long double Bounds<T, e>::widthLD() const
{
    return empty() ? 0.0 : static_cast<long double>(p1_.x) - static_cast<long double>(p0_.x);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
long double Bounds<T, e>::heightLD() const
{
    return empty() ? 0.0 : static_cast<long double>(p1_.y) - static_cast<long double>(p0_.y);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::minX() const
{
    return p0_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
T& Bounds<T, e>::minX()
{
    return p0_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::maxX() const
{
    return p1_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
T& Bounds<T, e>::maxX()
{
    return p1_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::maxY() const
{
    return p1_.y;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
T& Bounds<T, e>::maxY()
{
    return p1_.y;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::coord Bounds<T, e>::minY() const
{
    return p0_.y;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
T& Bounds<T, e>::minY()
{
    return p0_.y;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
const typename Bounds<T, e>::Point& Bounds<T, e>::minXY() const
{
    return p0_;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
const typename Bounds<T, e>::Point& Bounds<T, e>::maxXY() const
{
    return p1_;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Point Bounds<T, e>::maxXminY() const
{
    return Point(p1_.x, p0_.y);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Point Bounds<T, e>::minXmaxY() const
{
    return Point(p0_.x, p1_.y);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Point Bounds<T, e>::operator[](int index) const
{
    switch (index % 4)
    {
    case 0: return minXY();
    case 1: return maxXminY();
    case 2: return maxXY();
    case 3: return minXmaxY();
    default: throw std::invalid_argument("Index out of bound");
    }
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Vector Bounds<T, e>::extent() const
{
    return empty() ? Vector() : p1_ - p0_;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
double Bounds<T, e>::area() const
{
    if (empty())
        return 0.0;
    else
        return (static_cast<double>(p1_.x) - static_cast<double>(p0_.x)) *
               (static_cast<double>(p1_.y) - static_cast<double>(p0_.y));
}

//------------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::operator==(const Bounds<T, e>& rhs) const
{
    return (empty() && rhs.empty()) ||
           (!empty() && !rhs.empty() && p0_.x == rhs.p0_.x && p0_.y == rhs.p0_.y &&
            p1_.x == rhs.p1_.x && p1_.y == rhs.p1_.y);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::operator!=(const Bounds<T, e>& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
template <typename T, bool e>
typename Bounds<T, e>::Point Bounds<T, e>::clip(Point pt, dist tolerance /* = 0 */) const
{
    if (!empty())
    {
        pt.x = std::max(pt.x, p0_.x - tolerance);
        pt.x = std::min(pt.x, p1_.x + tolerance);
        pt.y = std::max(pt.y, p0_.y - tolerance);
        pt.y = std::min(pt.y, p1_.y + tolerance);
    }
    else
    {
        ASSERT(false);
    }

    return pt;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::empty() const
{
    ASSERT(e || p0_.x <= p1_.x);
    return e ? (p0_.x > p1_.x) : false;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
bool Bounds<T, e>::areaEmpty() const
{
    return empty() || p1_.y <= p0_.y || p1_.x <= p0_.x;
}

//------------------------------------------------------------------------------
template <typename T, bool e>
Bounds<T, e>& Bounds<T, e>::clear()
{
    p0_ = Point(1, 0);
    p1_ = Point(-1, 0);
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, bool mayBeEmpty>
template <typename U>
Bounds<typename U::coord> Bounds<T, mayBeEmpty>::cast() const
{
    return Bounds<typename U::coord>(Point2dT<typename U::coord>(p0_),
                                     Point2dT<typename U::coord>(p1_));
}

//------------------------------------------------------------------------------
template <typename T, bool mayBeEmpty>
Bounds<T, mayBeEmpty>& Bounds<T, mayBeEmpty>::operator*=(dist factor)
{
    p0_ *= factor;
    p1_ *= factor;
    return *this;
}

}  // namespace lc::geom
