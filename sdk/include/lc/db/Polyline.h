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

//! Polyline shape class
class DBAPI Polyline final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Polyline);

public:
    // Create a new Polyline object
    static Polyline* createInstance(Cell* cell,
                                    const Layer* layer,
                                    const PointArray& vertices,
                                    dist width = 0,
                                    EndCap style = EndCap::Round,
                                    bool closed = false,
                                    VertexMode mode = VertexMode::RawVertices);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    Polyline* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    // Set polyline data
    void set(const PointArray& vertices,
             dist width,
             EndCap style,
             bool closed,
             VertexMode mode = VertexMode::RawVertices);

    // Set vertices
    void setVertices(const PointArray& vertices, VertexMode mode = VertexMode::RawVertices);


    // Append vertices to Polyline
    //
    // Parameters:
    //      vertices :      array of polyline vertices to append
    //      uniqueOnly :    set to true to filter out duplicate vertices
    //
    void append(const PointArray& vertices, VertexMode mode = VertexMode::RawVertices);
    void append(const Point& vertex, VertexMode mode = VertexMode::RawVertices);

    // Prepend vertices to Polyline
    //
    // Parameters:
    //      vertices :      array of polyline vertices to prepend
    //      uniqueOnly :    set to true to filter out duplicate vertices
    //
    void prepend(const PointArray& vertices, VertexMode mode = VertexMode::RawVertices);
    void prepend(const Point& vertex, VertexMode mode = VertexMode::RawVertices);

    //! Reverses the vertex order of this Polyline
    void reverse();

    //! Gets the Polyline vertices
    //!
    //! \param [in,out]  vertices    Vertex array to receive vertices. The array
    //!                              will be resized to the appropriate size and
    //!                              any preexisting data will be overwritten.
    //! \param   mode                (optional) Specifies how vertices are
    //!                              returned. Note that vertexCount() returns
    //!                              the number of vertices when mode = VertexMode::RawVertices.
    void vertices(PointArray& vertices, VertexMode mode = VertexMode::RawVertices) const;

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

    //! Set width
    void setWidth(dist width);

    //! Get width
    dist width() const;

    //! Close polyline
    void close(bool doClose = true);

    //! Check if closed
    bool closed() const;

    //! Set the end cap type
    void setEndCapStyle(EndCap style);

    //! Get end cap style
    EndCap endCapStyle() const;

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
    ~Polyline();

private:
    // closed flag
    static const uint16_t polylineClosedShift_ = 4;
    static const uint16_t polylineClosedMask_ = 0x01;

    // polyline end cap style
    static const uint16_t polylineEndCapShift_ = 5;
    static const uint16_t polylineEndCapMask_ = 0x03;
};

}  // namespace lc::db
