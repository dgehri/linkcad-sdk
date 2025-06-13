//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include <limits>
#include <cstdint>

namespace lc::geom {

//! Class describing required resolution
template <typename T>
class Resolution final
{
public:
    //! Constructor
    //!
    //! \param minFacets     Minimum number of facets per full circle
    //! \param errorMax      Maximum error between approximation and original curve
    explicit Resolution(uint32_t minFacets = 0, T errorMax = std::numeric_limits<T>::max())
        : maxError_(errorMax)
        , minFacets_(minFacets)
    {}

    //! Conversion construction
    template <typename U>
    Resolution(const Resolution<U>& rhs)
        : maxError_(static_cast<T>(rhs.maximumError()))
        , minFacets_(rhs.minimumFacets())
    {}

    //! Maximum error between approximation and original curve
    T maximumError() const { return maxError_; }

    //! Minimum number of facets per full circle
    uint32_t minimumFacets() const { return minFacets_; }

    //! Set maximum error
    void setMaximumError(T error) { maxError_ = error; }

    //! Set minimum number of facets per full circle
    void setMinimumFacets(uint32_t facets) { minFacets_ = facets; }

    //! Get tolerance corresponding to resolution
    Tolerance asTolerance() const
    {
        return {static_cast<double>(maxError_), static_cast<double>(maxError_)};
    }

    //! Get segment count required to render circle of specified diameter
    uint32_t segmentCount(T dia) const;

private:
    T maxError_;
    uint32_t minFacets_;
};

//------------------------------------------------------------------------------
template <typename T>
uint32_t Resolution<T>::segmentCount(T dia) const
{
    if (dia == T())
        return minFacets_;

    double r = static_cast<double>(dia) / 2.0;
    double q = 1.0 - maxError_ / r;
    if (q > 0.0)
    {
        auto minFacetsFromError = static_cast<uint32_t>(4.0 * ceil(PI / (4.0 * acos(q))));
        return std::max(minFacets_, minFacetsFromError);
    }
    else
        return minFacets_;
}


}  // namespace lc::geom
