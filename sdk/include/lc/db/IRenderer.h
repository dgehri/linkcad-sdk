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
#include <lc/util/Color.h>

namespace lc::db {

class DrawingObject;
class Layer;
class Polygon;

//------------------------------------------------------------------------------
// Interface for drawing objects
//------------------------------------------------------------------------------
struct IRenderer
{
    virtual ~IRenderer() = 0;

    // Get purpose of drawing operation
    virtual DrawType type() const = 0;

    /**
     * \brief  Get rendering resolution
     * \return Resolution to be used for rendering.
     *
     * \note The resolution unites are always expressed in the renderer's current
     *       coordinate system. That is, in the same units as used by subsequent
     *       drawPolygon() and other calls.
     */
    virtual Resolution resolution() const = 0;

    // Fill rule to use for polygons
    virtual FillRule fillRule() const = 0;

    // Set fill mode
    virtual void setDrawMode(FillMode fillMode, db::LineStyle lineStyle) = 0;

    // Set layer
    virtual void setLayer(const db::Layer* layer) = 0;

    // Set end cap style for polylines
    virtual void setEndCapStyle(EndCap endCapStyle) = 0;

    // Set line width for polylines
    virtual void setLineWidth(dist width) = 0;

    // Set marker for subsequent calls to draw*()
    virtual void setMarker(unsigned int marker) = 0;

    // Draw a circle
    virtual void drawCircle(const Point& center, dist radius) = 0;

    // Draw a closed polygon
    virtual bool drawPolygon(const db::Polygon* polygon, PolygonClass polygonClass) = 0;

    // Draw a closed polygon
    virtual void drawPolygon(const PointArray& vertices,
                             PolygonClass polygonClass,
                             geom::FillRule fillRule) = 0;

    //! Draw a circular arc using current line style and width
    //!
    //! If \p startAngle <= \p endAngle, the arc is drawn counter-clockwise;
    //! otherwise the arc is drawn clockwise.
    //!
    //! \param   startAngle      Start angle measured CCW from the x-axis)
    //! \param   endAngle        End angle (measured CCW from the x-axis)
    //!
    virtual void drawCircularArc(const Point& center,
                                 dist radius,
                                 geom::Angle startAngle,
                                 geom::Angle endAngle,
                                 bool closed) = 0;

    // Draw a polyline using current line style and width
    //
    // Parameters:
    //  vertices -      vertex array containing at least two vertices
    //  closed -        if true, draw a closed polyline (closing a polyline does
    //                      *not* fill the enclosed area!)
    //  baseMarker -    if non zero, apply as marker to edges of polyline,
    //                      incrementing by one for each subsequent edge.
    //
    virtual void drawPolyline(const PointArray& vertices,
                              bool closed,
                              unsigned int baseMarker = 0) = 0;
};

inline IRenderer::~IRenderer() = default;

}  // namespace lc::db
