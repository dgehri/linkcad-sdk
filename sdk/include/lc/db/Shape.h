//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "CellObject.h"
#include <memory>

namespace lc::db {

class Layer;

//! Shape entity
//!
//! The Shape class is the base class for all graphical entities
class DBAPI Shape : public CellObject
{
    DECLARE_LCDB_OBJECT(ObjectType::Shape);

public:
    //! Compare to other shape
    //!
    //! \param   otherShape  Shape to compare to
    //! \param   ignoreSense (optional) If true, and applicable, compares two
    //!                      shapes as equivalent even if they differ in orientation
    //!                      (sense).
    //!
    //! \return  true if other shape is equivalent to this shape; false otherwise
    VIRTUAL bool equivalentTo(const Shape* otherShape, bool ignoreSense = true) const;

    //! Calculate enclosed area
    //!
    //! \return  Enclosed area
    VIRTUAL double area() const;

    //! Test if shape closed
    VIRTUAL bool closed() const;

    //! Get shape trace width
    VIRTUAL dist width() const;

    //! Get sample points
    VIRTUAL void samplePoints(PointArray& vertices,
                              const Resolution& res,
                              VertexMode mode = VertexMode::RawVertices) const;

protected:
    ~Shape();
};

}  // namespace lc::db
