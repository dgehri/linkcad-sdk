//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Shape.h"

namespace lc::db {

//! Circular arc
class DBAPI Arc final : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Arc);

public:
    //! Create a new Arc object
    //!
    //! If \p startAngle <= \p endAngle, the arc is drawn counter-clockwise;
    //! otherwise the arc is drawn clockwise.
    //!
    //! \param   cell            Owning cell
    //! \param   layer           Owning layer
    //! \param   center          Arc center
    //! \param   radius          Arc radius
    //! \param   startAngle      (optional) Start angle (measured CCW from the x-axis)
    //! \param   endAngle        (optional) End angle (measured CCW from the x-axis)
    //!
    //! \return  New arc instance
    static Arc* createInstance(Cell* cell,
                               const Layer* layer,
                               const Point& center,
                               dist radius,
                               dist width = 0,
                               geom::Angle startAngle = geom::Angle::zero,
                               geom::Angle endAngle = geom::Angle::twoPi,
                               EndCap style = EndCap::SquareFlat);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    VIRTUAL Arc* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set arc data
    //!
    //! If \p startAngle <= \p endAngle, the arc is drawn counter-clockwise;
    //! otherwise the arc is drawn clockwise.
    //!
    //! \param   center          Arc center
    //! \param   radius          Arc radius
    //! \param   startAngle      Start angle (measured CCW from the x-axis)
    //! \param   endAngle        End angle (measured CCW from the x-axis)
    void set(const Point& center,
             coord radius,
             dist width,
             geom::Angle startAngle,
             geom::Angle endAngle,
             EndCap style = EndCap::SquareFlat);

    //! Set arc center
    //!
    //! \param   center  Arc center
    void setCenter(const Point& center);

    //! Set arc radius
    //!
    //! \param   radius  Arc radius
    void setRadius(dist radius);

    //! Set arc width
    //!
    //! \param   width  Arc width
    void setWidth(dist width);

    //! Set arc start angle
    //!
    //! \param   startAngle      Start angle (measured CCW from the x-axis)

    void setStartAngle(geom::Angle startAngle);

    //! Set arc end angle
    //!
    //! \param   endAngle        End angle (measured CCW from the x-axis)
    void setEndAngle(geom::Angle endAngle);

    //! Get arc center
    //!
    //! \return  Arc center
    Point center() const;

    //! Get arc radius
    //!
    //! \return  Arc radius
    dist radius() const;

    //! Get arc width
    //!
    //! \return  Arc width
    dist width() const;

    //! Get start angle
    //!
    //! \return  Start angle (measured CCW from the x-axis)
    geom::Angle startAngle() const;

    //! Get end angle
    //!
    //! \return  End angle (measured CCW from the x-axis)
    geom::Angle endAngle() const;

    //! Get calculated start point
    Point startPoint() const;

    //! Get calculated end point
    Point endPoint() const;

    //! Check if closed
    static bool closed() { return false; }

    //! Set the end cap type
    void setEndCapStyle(EndCap style);

    //! Get end cap style
    EndCap endCapStyle() const;

    //! Get sample points
    void samplePoints(PointArray& vertices,
                      const Resolution& res,
                      VertexMode mode = VertexMode::RawVertices) const;

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


    //! Calculate arc bounding box
    //!
    //! If \p startAngle <= \p endAngle, the arc is drawn counter-clockwise;
    //! otherwise the arc is drawn clockwise.
    //!
    //! \param   center          Arc center
    //! \param   radius          Arc radius
    //! \param   startAngle      Start angle measured CCW from the x-axis)
    //! \param   endAngle        End angle (measured CCW from the x-axis)
    //! \param [out]  bounds     Calculated bounding box
    static void calculateBounds(const Point& center,
                                dist radius,
                                dist width,
                                geom::Angle startAngle,
                                geom::Angle endAngle,
                                EndCap style,
                                Bounds& bounds);

    //! Arc quadrant
    enum Quadrant
    {
        Quadrant1,  //!< [  0° -  90°)
        Quadrant2,  //!< [ 90° - 180°)
        Quadrant3,  //!< [180° - 270°)
        Quadrant4  //!< [270° - 360°)
    };

    //! Determine quadrant of angle
    //!
    //! \param   angle   Angle (measured CCW from the x-axis)
    //!
    //! \return  Quadrant containing angle
    static Quadrant quadrant(geom::Angle angle);

    //! Determine equivalent bulge
    //!
    //! If \p startAngle <= \p endAngle, the arc is drawn counter-clockwise;
    //! otherwise the arc is drawn clockwise.
    //!
    //! \param   startAngle      Start angle measured CCW from the x-axis)
    //! \param   endAngle        End angle (measured CCW from the x-axis)
    //!
    //! \return Bulge of arc
    static double calculateBulge(geom::Angle startAngle, geom::Angle endAngle);

protected:
    ~Arc();
};

}  // namespace lc::db
