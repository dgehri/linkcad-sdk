//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "dbtypes.h"
#include <lc/geom/Xform.h>

namespace lc::db {

class Shape;
class Layer;
class Ref;

//------------------------------------------------------------------------------
//! Base interface clients of region queries
//------------------------------------------------------------------------------
struct NOVTABLE IRegionQuery
{
    virtual ~IRegionQuery() = default;

    //! Callback for array references larger than 1 x 1
    virtual bool onArrayReference(Ref* ref) = 0;

    //! Callback for begin of references
    virtual bool onReferenceBegin(Ref* ref,
                                  const Xform& xform,
                                  const Xform& combinedXform,
                                  unsigned int col,
                                  unsigned int row) = 0;

    //! Callback for end of references
    virtual void onReferenceEnd(Ref* ref,
                                const Xform& combinedXform,
                                unsigned int col,
                                unsigned int row) = 0;
};

//------------------------------------------------------------------------------
//! Interface for clients of window queries
//------------------------------------------------------------------------------
struct NOVTABLE IWindowQuery : IRegionQuery
{
    //! Callback for shapes
    //!
    //! \retval true    Continue searching
    //! \retval false   Abort search
    virtual bool onShapeFound(Shape* shape,
                              const Layer* layer,
                              const Bounds& localBounds,
                              const Bounds& worldBounds) = 0;
};

//------------------------------------------------------------------------------
//! Interface for clients of window queries
//------------------------------------------------------------------------------
struct NOVTABLE IPointQuery : IRegionQuery
{
    //! Callback for shapes
    //!
    //! \param[in] shape    Shape within current tolerance distance
    //! \param[in] layer    Layer of \c shape
    //! \param[in] rect     Boundary box of \c shape
    //! \param[in] squareDist   The minimum squared distance of the shape to the
    //!                     query point
    //! \param[in,out] maxSquareDistance    The current squared tolerance
    //!                     distance. May be updated (eg. to \c squareDist) to
    //!                     further restrict the tolerance distance.
    //!
    //! \retval true    Continue searching for shapes within \c maxSquareDistance
    //! \retval false   Abort search
    virtual bool onShapeFound(Shape* shape,
                              const Layer* layer,
                              const Bounds& rect,
                              double squareDist,
                              double& maxSquareDistance) = 0;
};

}  // namespace lc::db
