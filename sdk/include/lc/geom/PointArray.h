//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include "Point2d.h"
#include "Bounds.h"
#include <vector>

namespace lc::geom {

//------------------------------------------------------------------------------
// Class for storing an array of Points
//------------------------------------------------------------------------------
template <typename T, class Alloc = std::allocator<Point2dT<T>>>
class PointArray : public std::vector<Point2dT<T>, Alloc>
{
public:
    using Base = std::vector<Point2dT<T>, Alloc>;
    using coord = T;
    using Point = Point2dT<T>;
    using Vector = Vector2dT<T>;

    // Construct empty PointArray
    PointArray() = default;

    // Destructor
    ~PointArray() = default;

    // Construct empty PointArray
    explicit PointArray(const Alloc& alloc)
        : Base(alloc)
    {}

    template <class InputIterator>
    PointArray(InputIterator first, InputIterator last);

    // Construct from list of Points
    PointArray(const Point vertices[], size_t cnt, const Alloc& alloc);

    template <class InputIterator>
    PointArray(InputIterator first, InputIterator last, const Alloc& alloc);

    // Construct from list of Points
    PointArray(const Point vertices[], size_t cnt);

    // Copy constructor semantics
    PointArray(const PointArray& other);

    // Constructor with move semantics
    PointArray(PointArray&& other) noexcept;

    // Copy assignment
    PointArray& operator=(PointArray other);

    template <typename U>
    PointArray<U> cast() const
    {
        PointArray<U> result;
        result.reserve(this->size());
        for (auto&& pt : *this)
        {
            result.emplace_back(geom::cast<typename PointArray<U>::Point>(pt));
        }
        return result;
    }

    // Assign
    void assign(const Point vertices[], size_t cnt) { Base::assign(vertices, vertices + cnt); }

    void assign(const PointArray& vertices) { Base::assign(vertices.begin(), vertices.end()); }

    using Base::assign;

    // Get first vertex
    Point& head() { return Base::front(); }
    const Point& head() const { return Base::front(); }

    // Get last vertex
    Point& tail() { return Base::back(); }
    const Point& tail() const { return Base::back(); }

    // get head vector (from 1st to 2nd vertex)
    Vector headVector() const;

    // get head vector (from last to 2nd-last vertex)
    Vector tailVector() const;

    // Get start of vertex buffer
    Point* buffer() { return Base::empty() ? nullptr : &(*this)[0]; }
    const Point* buffer() const { return Base::empty() ? nullptr : &(*this)[0]; }

    // Append data
    void append(const Base& other, bool uniqueOnly = false, size_t offset = 0);

    void append(const Point& value) { Base::push_back(value); }

    // Get bounds
    Bounds<T> bounds() const;

    // Get polygon area
    [[nodiscard]] double signedArea() const;

    // Test if polygon encloses (contains) point
    bool enclosesPoint(const Point& pt) const;

    // Swap points with other PointArray
    void swap(PointArray<T, Alloc>& other) noexcept { Base::swap(other); }

    // Reverse point array
    void reverse() { std::reverse(Base::begin(), Base::end()); }

    bool equals(const Base& rhs, bool ignoreSense = false) const;

    [[nodiscard]] bool isClockwise() const;
};

using PointArray2d = PointArray<double>;

template <typename T, class Alloc>
void swap(PointArray<T, Alloc>& lhs, PointArray<T, Alloc>& rhs) noexcept
{
    lhs.swap(rhs);
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
PointArray<T, Alloc>::PointArray(const Point vertices[], size_t cnt)
    : Base(vertices, vertices + cnt)
{}


//------------------------------------------------------------------------------
template <typename T, class Alloc>
template <class InputIterator>
PointArray<T, Alloc>::PointArray(InputIterator first, InputIterator last)
    : Base(first, last)
{}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
PointArray<T, Alloc>::PointArray(const Point vertices[], size_t cnt, const Alloc& alloc)
    : Base(vertices, vertices + cnt, alloc)
{}


//------------------------------------------------------------------------------
template <typename T, class Alloc>
template <class InputIterator>
PointArray<T, Alloc>::PointArray(InputIterator first, InputIterator last, const Alloc& alloc)
    : Base(first, last, alloc)
{}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
PointArray<T, Alloc>::PointArray(const PointArray& other)
    : Base(other)
{}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
PointArray<T, Alloc>::PointArray(PointArray&& other) noexcept
    : PointArray()
{
    swap(other);
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
PointArray<T, Alloc>& PointArray<T, Alloc>::operator=(PointArray other)
{
    swap(other);
    return *this;
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
void PointArray<T, Alloc>::append(const Base& other,
                                  bool uniqueOnly /*= false*/,
                                  size_t offset /*= 0*/)
{
    if (offset >= other.size())
        return;

    if (!uniqueOnly)
    {
        Base::insert(Base::end(), other.begin() + offset, other.end());
    }
    else
    {
        // don't add duplicate start points
        auto otherBegin = other.begin() + offset;
        size_t oldSize = Base::size();

        if (oldSize > 0)
        {
            while (otherBegin != other.end() && *otherBegin == Base::back())
            {
                ++otherBegin;
            }
        }

        // reserve space for new unique points
        Base::resize(oldSize + (other.end() - otherBegin));

        // copy unique points
        auto newEnd = std::unique_copy(otherBegin, other.end(), Base::begin() + oldSize);

        // set correct size
        Base::resize(newEnd - Base::begin());
    }
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
typename PointArray<T, Alloc>::Vector PointArray<T, Alloc>::tailVector() const
{
    size_t cnt = Base::size();

    return cnt < 2 ? Vector::zeroVector() : (*this)[cnt - 2] - (*this)[cnt - 1];
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
typename PointArray<T, Alloc>::Vector PointArray<T, Alloc>::headVector() const
{
    return Base::size() < 2 ? Vector::zeroVector() : (*this)[1] - (*this)[0];
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
Bounds<T> PointArray<T, Alloc>::bounds() const
{
    Bounds<T> bounds;
    for (auto&& pt : *this)
    {
        bounds.expandWith(pt);
    }

    return bounds;
}

//------------------------------------------------------------------------------
// Calculates polygon area
//
// References:
//      [O' Rourke (C)] Thm. 1.3.3, p. 21; [Gems II] pp. 5-6:
//      "The Area of a Simple Polygon", Jon Rokne.
//------------------------------------------------------------------------------
template <typename T, class Alloc>
double PointArray<T, Alloc>::signedArea() const
{
    auto area2 = 0.0;

    if (Base::size() > 1)
    {
        auto prev = Base::end() - 1;

        for (auto cur = Base::begin(); cur != Base::end(); ++cur)
        {
            if (*cur != *prev)
            {
                area2 += (static_cast<double>(cur->x) + static_cast<double>(prev->x)) *
                         (static_cast<double>(cur->y) - static_cast<double>(prev->y));
                prev = cur;
            }
        }
    }

    return area2 / 2;
}

//------------------------------------------------------------------------------
// Test if polygon encloses point
//------------------------------------------------------------------------------
template <typename T, class Alloc>
bool PointArray<T, Alloc>::enclosesPoint(const Point& pt) const
{
    auto result = false;

    if (Base::size() > 1)
    {
        auto prev = Base::end() - 1;

        for (auto cur = Base::begin(); cur != Base::end(); ++cur)
        {
            if (*cur == *prev)
                continue;

            if (((cur->y <= pt.y && pt.y < prev->y) || (prev->y <= pt.y && pt.y < cur->y)) &&
                (static_cast<double>(pt.x) < static_cast<double>(prev->x - cur->x) *
                                                     static_cast<double>(pt.y - cur->y) /
                                                     static_cast<double>(prev->y - cur->y) +
                                                 static_cast<double>(cur->x)))
            {
                result = !result;
            }

            prev = cur;
        }
    }

    return result;
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
bool PointArray<T, Alloc>::equals(const Base& rhs, bool ignoreSense /*= false*/) const
{
    return Base::size() == rhs.size() &&
           (std::equal(Base::begin(), Base::end(), rhs.begin()) ||
            (ignoreSense && std::equal(Base::begin(), Base::end(), rhs.rbegin())));
}

//------------------------------------------------------------------------------
template <typename T, class Alloc>
bool PointArray<T, Alloc>::isClockwise() const
{
    return signedArea() <= 0.0;
}

}  // namespace lc::geom
