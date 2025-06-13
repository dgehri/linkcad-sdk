//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Shape.h"
#include "Cell.h"

namespace lc::db {

//! Donut shape class
class DBAPI Donut final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Donut);

public:
    //! Create a new Donut object
    //!
    //! \param   cell            Owning cell
    //! \param   layer           Owning layer
    //! \param   center          Donut center
    //! \param   meanDiameter    Mean diameter ([outer diameter + inner diameter] / 2)
    //! \param   width           Ring width
    //!
    //! \return  New donut instance

    static Donut* createInstance(
        Cell* cell, const Layer* layer, const Point& center, dist meanDiameter, dist width);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    Donut* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set donut data
    void set(const Point& center, dist meanDiameter, dist width);

    //! Set donut center
    void setCenter(const Point& center);

    //! Set mean donut diameter
    void setMeanDiameter(dist meanDiameter);

    //! Set donut ring width
    void setWidth(dist width);

    //! Get donut center
    const Point& center() const;

    //! Get mean donut diameter
    dist meanDiameter() const;

    //! Get donut ring width
    dist width() const;

    //! Get outer diameter (= meanDiameter + width)
    dist outerDiameter() const;

    //! Get inner diameter (= meanDiameter - width)
    dist innerDiameter() const;

    //! Get mean radius (= meanDiameter / 2)
    dist meanRadius() const;

    //! Get outer radius (= outerDiameter / 2)
    dist outerRadius() const;

    //! Get inner radius (= innerDiameter / 2)
    dist innerRadius() const;

    //! Check if closed
    bool closed() const { return true; }

    //! Calculate bounding box
    static void calculateBounds(const Point& center, dist meanDiameter, dist width, Bounds& bounds);

    //! Get sample points
    static void samplePoints(PointArray& vertices,
                             const Resolution& res,
                             VertexMode mode = VertexMode::RawVertices);

    //! Calculate enclosed area
    //!
    //! \return  Enclosed area
    double area() const;

    //! Compare to other shape
    //!
    //! \param   otherShape  Shape to compare to
    //! \param   ignoreSense (optional) If true, and applicable, compares two
    //!                      shapes as equivalent even if they differ in orientation
    //!                      (sense).
    //!
    //! \return  true if other shape is equivalent to this shape; false otherwise
    bool equivalentTo(const Shape* otherShape, bool ignoreSense = true) const;

    //! Draw shape using specified renderer object
    //!
    //! \param renderer    Renderer to use
    void draw(IRenderer* renderer) const;

    //! Apply transformation
    void applyTransformation(const Xform& xform);

    //! Get grip points for this cell object
    void gripPoints(SnapPoints& grips) const;

protected:
    ~Donut();
};

}  // namespace lc::db
