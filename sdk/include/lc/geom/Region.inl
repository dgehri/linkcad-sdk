//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------

#pragma once
namespace lc::geom {

//------------------------------------------------------------------------------
template <typename T>
struct Region<T>::SortY
{
    bool operator()(const Bounds& lhs, const Bounds& rhs) const
    {
        return (lhs.minY() != rhs.minY()) ? (lhs.minY() < rhs.minY()) : (lhs.minX() < rhs.minX());
    }
};

//------------------------------------------------------------------------------
template <typename T>
Region<T>::Region()
{
    ASSERT(sizeof(Region) == sizeof(Rects));
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>::Region(const Region& reg)
    : rects_(reg.rects_)
{
    ASSERT(sizeof(Region) == sizeof(Rects));
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>::Region(Region&& reg) noexcept
    : rects_(std::move(reg.rects_))
{
    ASSERT(sizeof(Region) == sizeof(Rects));
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>::Region(const Bounds& bnd)
{
    init(bnd);
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>::Region(int x, int y, int w, int h)
{
    if (w >= 0 && h >= 0)
    {
        init(Bounds(x, y, x + w, y + h));
    }
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::init(const Bounds& bnd)
{
    ASSERT(sizeof(Region) == sizeof(Rects));
    if (!bnd.areaEmpty())
    {
        rects_.push_back(bnd);
    }
}

//------------------------------------------------------------------------------
template <typename T>
bool Region<T>::empty() const
{
    return rects_.empty();
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::clear()
{
    rects_.clear();
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::translate(const Vector& offset)
{
    for (auto it = rects_.begin(); it != rects_.end(); ++it)
    {
        it->translate(offset);
    }
}

//------------------------------------------------------------------------------
template <typename T>
typename Region<T>::Bounds Region<T>::bounds() const
{
    Bounds bnd;
    for (auto it = rects_.begin(); it != rects_.end(); ++it)
    {
        bnd.expandWith(*it);
    }
    return bnd;
}

//------------------------------------------------------------------------------
template <typename T>
const typename Region<T>::Bounds* Region<T>::enumRectangles(int i) const
{
    if (i >= static_cast<int>(rects_.size()))
        return nullptr;

    auto it = rects_.begin();
    std::advance(it, i);
    return &(*it);
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::swap(Region& other) noexcept
{
    rects_.swap(other.rects_);
}

//------------------------------------------------------------------------------
//
// Merge rectangles
//
// Description:
//
// The merge algorithm first merges horizontally adjacent objects.
// Since the rectangles in the region are sorted according to their
// bottom-left corner, horizontally adjacent objects also appear
// as neighbors in the list of rectangles.
//
// If two horizontally adjacent rectangles differ in height, a
// the oversized fraction is inserted as a new rectangle into the
// list, and the remainder is merged:
//
// +-----+             +-----+
// |     |             |     |
// |     +-----+  ==>  +-----+-----+
// |     |     |       |           |
// +-----+-----+       +-----------+
//
// The merge algorithm proceeds by merging vertically adjacent
// rectangles. The horizontal merge pass guarantees that vertically
// adjacent rectangles appear as neighbors in the list of rectangles.
// Only rectangles of identical width are merged.
//
// +-----+             +-----+
// |     |             |     |
// +-----+             |     |
// |     |             |     |
// +-----+-----+  ==>  +-----+-----+
// |           |       |           |
// +-----------+       +-----------+
//
//------------------------------------------------------------------------------
template <typename T>
void Region<T>::merge()
{
    if (rects_.size() < 2)
        return;

    // merge horizontally
    auto it = rects_.begin();
    while (it != rects_.end())
    {
        auto nextIt = it;
        if (++nextIt == rects_.end())
            break;

        if (it->minY() == nextIt->minY() && it->maxX() == nextIt->minX())
        {
            if (it->height() > nextIt->height())
            {
                // fracture left rectangle
                Bounds fract(it->minX(), nextIt->maxY(), it->maxX(), it->maxY());
                rects_.insert(std::lower_bound(nextIt, rects_.end(), fract, SortY()), fract);

                Bounds merged(it->minX(), it->minY(), nextIt->maxX(), nextIt->maxY());
                (*it) = merged;

                rects_.erase(nextIt);
                continue;  // continue with merged rectangle
            }
            else if (it->height() < nextIt->height())
            {
                // fracture right rectangle
                Bounds fract(nextIt->minX(), it->maxY(), nextIt->maxX(), nextIt->maxY());
                rects_.insert(std::lower_bound(nextIt, rects_.end(), fract, SortY()), fract);

                Bounds merged(it->minX(), it->minY(), nextIt->maxX(), it->maxY());
                (*it) = merged;

                rects_.erase(nextIt);
                continue;  // continue with merged rectangle
            }
            else
            {
                Bounds merged(it->minX(), it->minY(), nextIt->maxX(), it->maxY());
                (*it) = merged;

                rects_.erase(nextIt);
                continue;  // continue with merged rectangle
            }
        }
        ++it;
    }

    // merge vertically
    it = rects_.begin();
    while (it != rects_.end())
    {
        auto nextIt = it;
        if (++nextIt == rects_.end())
            break;

        if (it->minX() == nextIt->minX() && it->maxY() == nextIt->minY() &&
            it->width() == nextIt->width())
        {
            Bounds merged(it->minX(), it->minY(), nextIt->maxX(), nextIt->maxY());
            (*it) = merged;

            rects_.erase(nextIt);
            continue;  // continue with merged rectangle
        }
        ++it;
    }
}

//------------------------------------------------------------------------------
//
// Add a rectangle
//
// Description:
//
// The rectangle is compared to all rectangles that possibly overlap
// with it. If it overlaps, it is fractured and the search continues
// with the non-overlapping fractions. After all overlapping fractions
// have been removed the remaining fractions are inserted in the
// sorted list of rectangles.
//
//------------------------------------------------------------------------------
template <typename T>
void Region<T>::addNonMerged(const Bounds& bnd)
{
    Rects bndsAdd;  // list of rectangles to insert
    bndsAdd.push_back(bnd);

    Bounds bndCmp(bnd.minX(), bnd.maxY(), bnd.maxX(), bnd.maxY());
    auto itLast = std::lower_bound(rects_.begin(), rects_.end(), bndCmp, SortY());
    for (auto it = rects_.begin(); it != itLast; ++it)
    {
        auto itAdd = bndsAdd.begin();
        while (itAdd != bndsAdd.end())
        {
            Bounds i(*it);
            i.intersect(*itAdd);
            if (!i.areaEmpty())
            {
                // Overlapping -> split new rectangle
                auto itAddNext = itAdd;
                ++itAddNext;

                // bottom left
                Bounds bottomLeft(itAdd->minX(), itAdd->minY(), i.minX(), i.minY());
                if (!bottomLeft.areaEmpty())
                    bndsAdd.insert(itAddNext, bottomLeft);

                // bottom
                Bounds bottom(i.minX(), itAdd->minY(), i.maxX(), i.minY());
                if (!bottom.areaEmpty())
                    bndsAdd.insert(itAddNext, bottom);

                // bottom right
                Bounds bottomRight(i.maxX(), itAdd->minY(), itAdd->maxX(), i.minY());
                if (!bottomRight.areaEmpty())
                    bndsAdd.insert(itAddNext, bottomRight);

                // left
                Bounds left(itAdd->minX(), i.minY(), i.minX(), i.maxY());
                if (!left.areaEmpty())
                    bndsAdd.insert(itAddNext, left);

                // right
                Bounds right(i.maxX(), i.minY(), itAdd->maxX(), i.maxY());
                if (!right.areaEmpty())
                    bndsAdd.insert(itAddNext, right);

                // top left
                Bounds topLeft(itAdd->minX(), i.maxY(), i.minX(), itAdd->maxY());
                if (!topLeft.areaEmpty())
                    bndsAdd.insert(itAddNext, topLeft);

                // top
                Bounds top(i.minX(), i.maxY(), i.maxX(), itAdd->maxY());
                if (!top.areaEmpty())
                    bndsAdd.insert(itAddNext, top);

                // top right
                Bounds topRight(i.maxX(), i.maxY(), itAdd->maxX(), itAdd->maxY());
                if (!topRight.areaEmpty())
                    bndsAdd.insert(itAddNext, topRight);

                itAdd = bndsAdd.erase(itAdd);
                continue;
            }
            ++itAdd;
        }
    }

    // merge new rectangles into list
    bndsAdd.sort(SortY());
    rects_.merge(bndsAdd, SortY());
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::subtractNonMerged(const Bounds& bnd)
{
    Rects bndsAdd;  // list of fractured rectangles

    Bounds bndCmp(bnd.minX(), bnd.maxY(), bnd.maxX(), bnd.maxY());
    auto itLast = std::lower_bound(rects_.begin(), rects_.end(), bndCmp, SortY());

    auto it = rects_.begin();
    while (it != itLast)
    {
        Bounds i(bnd);
        i.intersect(*it);

        if (!i.areaEmpty())
        {
            // Overlapping -> split original rectangle

            // bottom left
            Bounds bottomLeft(it->minX(), it->minY(), i.minX(), i.minY());
            if (!bottomLeft.areaEmpty())
                rects_.insert(it, bottomLeft);

            // bottom
            Bounds bottom(i.minX(), it->minY(), i.maxX(), i.minY());
            if (!bottom.areaEmpty())
                rects_.insert(it, bottom);

            // bottom right
            Bounds bottomRight(i.maxX(), it->minY(), it->maxX(), i.minY());
            if (!bottomRight.areaEmpty())
                rects_.insert(it, bottomRight);

            //--------
            // Inserting the following rectangles would break the sort order;
            // they will be merged into the list of rectangles later.

            // left
            Bounds left(it->minX(), i.minY(), i.minX(), i.maxY());
            if (!left.areaEmpty())
                bndsAdd.push_back(left);

            // right
            Bounds right(i.maxX(), i.minY(), it->maxX(), i.maxY());
            if (!right.areaEmpty())
                bndsAdd.push_back(right);

            // top left
            Bounds topLeft(it->minX(), i.maxY(), i.minX(), it->maxY());
            if (!topLeft.areaEmpty())
                bndsAdd.push_back(topLeft);

            // top
            Bounds top(i.minX(), i.maxY(), i.maxX(), it->maxY());
            if (!top.areaEmpty())
                bndsAdd.push_back(top);

            // top right
            Bounds topRight(i.maxX(), i.maxY(), it->maxX(), it->maxY());
            if (!topRight.areaEmpty())
                bndsAdd.push_back(topRight);

            it = rects_.erase(it);
            continue;
        }

        ++it;
    }

    // merge new rectangles into list
    bndsAdd.sort(SortY());
    rects_.merge(bndsAdd, SortY());
}

//------------------------------------------------------------------------------
template <typename T>
void Region<T>::intersectNonMerged(const Bounds& bnd, Rects& result) const
{
    for (auto it = rects_.begin(); it != rects_.end(); ++it)
    {
        if (it->minY() >= bnd.maxY())
            break;

        Bounds i(bnd);
        i.intersect(*it);

        if (!i.areaEmpty())
        {
            result.push_back(i);
        }
    }
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>& Region<T>::operator=(const Region<T>& reg)
{
    rects_.assign(reg.rects_.begin(), reg.rects_.end());
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>& Region<T>::operator+=(const Region<T>& reg)
{
    for (auto it = reg.rects_.begin(); it != reg.rects_.end(); ++it)
    {
        addNonMerged(*it);
    }

    merge();
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>& Region<T>::operator-=(const Region<T>& reg)
{
    for (auto it = reg.rects_.begin(); it != reg.rects_.end(); ++it)
    {
        subtractNonMerged(*it);
    }

    merge();
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Region<T>& Region<T>::operator*=(const Region<T>& reg)
{
    if (reg.empty())
    {
        clear();
    }
    else
    {
        Rects result;
        for (auto it = reg.rects_.begin(); it != reg.rects_.end(); ++it)
        {
            Rects tmp;
            intersectNonMerged(*it, tmp);
            result.merge(tmp, SortY());
        }

        rects_.swap(result);
        merge();
    }

    return (*this);
}

}  // namespace lc::geom
