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

//------------------------------------------------------------------------------
// Ellipse shape class
//------------------------------------------------------------------------------
class DBAPI Ellipse final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Ellipse);

public:
    // create a new Ellipse object
    static Ellipse* createInstance(Cell* cell,
                                   const Layer* layer,
                                   const Point& center,
                                   coord diameter);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    VIRTUAL Ellipse* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set circle data
    void set(const Point& center, dist diameter);

    //! Set circle center
    void setCenter(const Point& center);

    //! Set circle diameter
    void setDiameter(dist diameter);

    //! Get circle center
    Point center() const;

    //! Get circle diameter
    dist diameter() const;

    //! Get circle radius (= diameter / 2)
    dist radius() const;

    //! Get width
    dist width() const { return 0; }

    //! Check if closed
    bool closed() const { return true; }

    //! Calculate bounding box
    static void calculateBounds(const Point& center, dist diameter, Bounds& bounds);

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
    ~Ellipse();
};

}  // namespace lc::db
