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
#include <lc/geom/PointArray.h>
#include <vector>

namespace lc::db {

//! Non-uniform B-spline, rational (NURBS) and non-rational shape
class DBAPI Nurbs final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Nurbs);

public:
    using CtrlPoints = PointArray;  //!<  Control point vector
    using Weights = std::vector<double>;  //!<  Weight vector
    using Knots = std::vector<double>;  //!<  Knot vector

    //! Creates a non-uniform, non-rational B-spline
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   cell            Containing cell
    //! \param   layer           Layer
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 = cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    //!
    //! \return  New non-uniform, non-rational B-spline instance
    static Nurbs* createInstance(Cell* cell,
                                 const Layer* layer,
                                 dist width,
                                 NurbsDegree degree,
                                 const Knots& knots,
                                 const CtrlPoints& ctrlPoints,
                                 bool periodic = false);

    //! Creates a non-uniform, non-rational B-spline (NURBS)
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   cell            Containing cell
    //! \param   layer           Layer
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 = cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   weights         Weights vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    //!
    //! \return  New NURBS instance
    static Nurbs* createInstance(Cell* cell,
                                 const Layer* layer,
                                 dist width,
                                 NurbsDegree degree,
                                 const Knots& knots,
                                 const CtrlPoints& ctrlPoints,
                                 const Weights& weights,
                                 bool periodic = false);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    Nurbs* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set parameters for non-uniform, non-rational B-spline
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 = cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    void set(dist width,  // line width
             NurbsDegree degree,  // NURBS degree
             const Knots& knots,  // knot vector
             const CtrlPoints& ctrlPoints,  // control point vector
             bool periodic = false  // true if periodic NURBS
    );

    //! Set parameters for non-uniform, non-rational B-spline (NURBS)
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 = cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   weights         Weights vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    void set(dist width,  // line width
             NurbsDegree degree,  // NURBS degree
             const Knots& knots,  // knot vector
             const CtrlPoints& ctrlPoints,  // control point vector
             const Weights& weights,  // weight vector
             bool periodic = false  // true if periodic NURBS
    );

    //! check if periodic (and therefore closed)
    bool closed() const;

    //! Close Nurbs
    void close(bool doClose = true);

    //! return true if rational B-Spline
    bool rational() const;

    //! Get width
    dist width() const;

    //! Set width
    void setWidth(dist with);

    //! Get degree
    NurbsDegree degree() const;

    //! Get knot vector
    void knots(Knots& k) const;

    //! Get know count
    size_t knotCount() const;

    //! Get control points vector
    void controlPoints(CtrlPoints& ctrlPoints) const;

    //! Get number of control points
    size_t controlPointCount() const;

    //! Get control points vector
    void weights(Weights& w) const;

    //! Get sample points
    void samplePoints(PointArray& vertices,
                      const Resolution& res,
                      VertexMode mode = VertexMode::RawVertices) const;

    //! Calculate enclosed area
    //!
    //! \return  Enclosed area
    static double area();

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

    //! Calculate bounding box (convex hull)
    static void calculateBounds(const CtrlPoints& ctrlPoints, dist width, Bounds& bounds);

protected:
    ~Nurbs();
};

}  // namespace lc::db
