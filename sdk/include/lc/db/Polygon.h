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
#include "Tessellation.h"

namespace lc::db {

//! Polygon shape class
class DBAPI Polygon final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Polygon);

public:
    //! Creates a new polygon instance
    //!
    //! \param   cell            Cell containing polygon
    //! \param   layer           Layer containing polygon
    //! \param   vertices        Polygon vertices
    //! \param   mode            (optional) Vertex mode
    //!
    //! \return  New polygon instance
    static Polygon* createInstance(Cell* cell,
                                   const Layer* layer,
                                   const PointArray& vertices,
                                   VertexMode mode = VertexMode::RawVertices);

    //! Creates a new square polygon instance
    //!
    //! \param   cell            Cell containing polygon
    //! \param   layer           Layer containing polygon
    //! \param   p0              First corner
    //! \param   p1              Second corner opposite first corner
    //!
    //! \return  New polygon instance
    static Polygon* createInstance(Cell* cell,
                                   const Layer* layer,
                                   const Point& p0,
                                   const Point& p1);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    VIRTUAL Polygon* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    // Set polygon data

    //! Set polygon vertices
    //!
    //! \param   vertices        Polygon vertices
    //! \param   mode            (optional) Vertex mode
    void set(const PointArray& vertices, VertexMode mode = VertexMode::RawVertices);

    //! Set polygon vertices with bulges
    //!
    //! \param   vertices        Polygon vertices
    //! \param   bulges          Bulge vector
    void set(const PointArray& vertices, const std::vector<double>& bulges);

    //! Gets the Polygon vertices
    //!
    //! \param [in,out]  vertices    Vertex array to receive vertices. The array
    //!                              will be resized to the appropriate size and
    //!                              any preexisting data will be overwritten.
    //! \param   mode                (optional) Specifies how vertices are
    //!                              returned. Note that vertexCount() returns
    //!                              the number of vertices when mode = VertexMode::RawVertices.
    void vertices(PointArray& vertices, VertexMode mode = VertexMode::RawVertices) const;

    //! Gets the Polygon vertices
    //!
    //! \param [in,out]  vertices    Vertex array to receive vertices. The array
    //!                              will be resized to the appropriate size and
    //!                              any preexisting data will be overwritten.
    //! \param [in,out] bulges       Bulges
    void vertices(PointArray& vertices, std::vector<double>& bulges) const;

    //! Set bulge at specified index
    void setBulgeAt(size_t index, double bulge);

    //! Get bulge at specified index
    double bulgeAt(size_t index) const;

    //! Check if polygon has non-zero bulges
    bool hasBulges() const;

    //! Get first vertex
    Point head() const;

    //! Get last vertex
    Point tail() const;

    //! Get number of vertices
    //!
    //! \note This method returns the number of raw vertices, which are returned
    //!       when specifying \c mode = VertexMode::RawVertices in vertices().
    //!
    //! \return  Vertex count
    size_t vertexCount() const;

    //! Returns true if polygon is an axis aligned square
    bool isBox() const;

    //! Gets the polygon classification
    //!
    //! \return  Polygon classification
    PolygonClass classification() const;

    //! Determines the class of a polygon
    //!
    //! \param vertices  Polygon to classify
    //! \return          Polygon classification
    static PolygonClass classify(const PointArray& vertices);

    //! Tests if polygon is self-intersecting
    bool isSelfIntersecting() const;

    //! Tests if vertices are self-intersecting
    static bool isSelfIntersecting(const PointArray& vertices);

    //! Creates the tessellation data of this Polygon using the specified
    //! fill rule
    //!
    //! \param   Fill rule to use for tessellation
    //! \return  Tessellation data for this Polygon
    Tessellation* createTessellation(FillRule fillRule);

    //! Get width
    dist width() const { return 0; }

    //! Check if closed
    bool closed() const { return true; }

    //! Get sample points
    void samplePoints(PointArray& vertices, const Resolution& res, VertexMode mode) const;

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
    ~Polygon();
};

}  // namespace lc::db
