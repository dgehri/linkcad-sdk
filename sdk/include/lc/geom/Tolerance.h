//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Angle.h"
#include <cmath>
#include <iostream>

namespace lc::geom {

//! Class describing acceptable tolerance
class Tolerance final
{
public:
    //! Constructor
    Tolerance(double eps, double cumulativeEps, Angle angularEps = Angle::epsilon)
        : eps_(eps)
        , cumulativeEps_(cumulativeEps)
        , angularEps_(angularEps)
    {}

    //! Global tolerance value
    static Tolerance global;

    //! Maximum error between approximation and original curve
    double maximumError() const { return eps_; }

    //! Maximum cumulative error between approximation and original curve
    double maximumCumulativeError() const { return cumulativeEps_; }

    //! Maximum angular error
    Angle maximumAngularError() const { return angularEps_; }

    //! Set maximum error
    void setMaximumError(double error) { eps_ = error; }

    //! Set maximum error
    void setMaximumCumulativeError(double error) { cumulativeEps_ = error; }

    //! Set maximum angular error
    void setMaximumAngularError(Angle error) { angularEps_ = error; }

    //! Test if two lengths are identical within allowable error
    bool isEqual(double d1, double d2) const;

    //! Test if two lengths are identical within allowable error
    bool isCumulativeEqual(double d1, double d2) const;

    //! Return angular error
    operator Angle() const { return maximumAngularError(); }

private:
    double eps_;
    double cumulativeEps_;
    Angle angularEps_;
};

//------------------------------------------------------------------------------
__declspec(selectany) Tolerance Tolerance::global(1.0e-12, 1.0e-10);

//------------------------------------------------------------------------------
inline bool Tolerance::isEqual(double d1, double d2) const
{
    return std::abs(d1 - d2) < eps_;
}

//------------------------------------------------------------------------------
inline bool Tolerance::isCumulativeEqual(double d1, double d2) const
{
    return std::abs(d1 - d2) < cumulativeEps_;
}

//! Stream output operator
inline std::ostream& operator<<(std::ostream& os, const Tolerance& tol)
{
    return os << "Tolerance(max=" << tol.maximumError() 
              << ", cumulative=" << tol.maximumCumulativeError() 
              << ", angular=" << tol.maximumAngularError() << ")";
}

}  // namespace lc::geom
