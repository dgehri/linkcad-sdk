//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include "geom.h"
#include "Bounds.h"
#include <list>

namespace lc::geom {

//------------------------------------------------------------------------------
// Class describing a region composed of rectangular bounds
//------------------------------------------------------------------------------
template <typename T>
class Region
{
public:
    using coord = T;
    using Bounds = Bounds<T>;
    using Point = Point2dT<T>;
    using Vector = Vector2dT<T>;

    // Constructor
    Region();
    Region(const Region& reg);
    Region(Region&& reg) noexcept;
    Region(const Bounds& bnd);
    Region(int x, int y, int w, int h);

    // Check if empty
    bool empty() const;

    // Clear region
    void clear();

    // Translate region
    void translate(const Vector& offset);

    // Get bounding box
    Bounds bounds() const;

    // Access rectangles (i: zero-based index to enumerate bounds)
    const Bounds* enumRectangles(int i) const;

    // Swap content
    void swap(Region& other) noexcept;

    // Operators
    Region& operator=(const Region& reg);
    Region& operator+=(const Region& reg);
    Region& operator-=(const Region& reg);
    Region& operator*=(const Region& reg);

private:
    using Rects = std::list<Bounds>;
    struct SortY;

    // Initialize from bounds
    void init(const Bounds& bnd);

    // Add a rectangle
    void addNonMerged(const Bounds& bnd);

    // Remove a rectangle
    void subtractNonMerged(const Bounds& bnd);

    // Intersect with rectangle
    void intersectNonMerged(const Bounds& bnd, Rects& result) const;

    // Merge rectangles
    void merge();

private:
    Rects rects_;
};

//------------------------------------------------------------------------------
template <typename T>
Region<T> operator+(const Region<T>& r1, const Region<T>& r2)
{
    return Region<T>(r1) += r2;
}

//------------------------------------------------------------------------------
template <typename T>
Region<T> operator-(const Region<T>& r1, const Region<T>& r2)
{
    return Region<T>(r1) -= r2;
}

//------------------------------------------------------------------------------
template <typename T>
Region<T> operator*(const Region<T>& r1, const Region<T>& r2)
{
    return Region<T>(r1) *= r2;
}

}  // namespace lc::geom

#include "Region.inl"
