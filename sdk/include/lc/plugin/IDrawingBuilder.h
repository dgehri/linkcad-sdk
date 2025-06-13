//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IPluginController.h"
#include <lc/geom/Angle.h>
#include <lc/lctypes.h>

namespace lc {
namespace plugin {

//------------------------------------------------------------------------------
// Interface for building drawing database
//------------------------------------------------------------------------------
struct IDrawingBuilder : IPluginController
{
    enum LayerFlags
    {
        LayerNormal = 0,
        LayerByLayer = 1,
        LayerByBlock = 2
    };

    using ContextHandle = size_t;

    virtual ~IDrawingBuilder() = 0;

    // Get arc precision
    virtual const Resolution& resolution() const = 0;

    // Set drawing name
    virtual void setDrawingName(std::string_view drawingName) = 0;

    // Set drawing modification time
    virtual void setDrawingModifTime(time_t time) = 0;

    // Set drawing access time
    virtual void setDrawingAccessTime(time_t time) = 0;

    // Set entity layer style
    virtual void setEntityLayerStyle(LayerFlags layerStyle) = 0;

    // Create a new reference
    virtual db::Ref* createRef(int cellNumber) = 0;

    // Create a new reference
    virtual db::Ref* createRef(std::string_view cellName) = 0;

    // Reference scaling
    virtual void scaleRef(double scale, bool isAbsolute = false) = 0;

    // Flip referenced structure
    virtual void mirrorRefInY(bool negate = true) = 0;

    // Flip referenced structure
    virtual void mirrorRefInX(bool negate = true) = 0;

    // Reference rotation angle (degrees)
    virtual void rotateRef(geom::Angle angle, bool isAbsolute = false) = 0;

    // Set the reference insertion point
    virtual void translateRef(const Point& pt) = 0;

    // Set array spacing
    virtual void setRefArraySpacing(dist dx, dist dy) = 0;

    // Set array dimensions
    virtual void setRefArraySize(int cols, int rows) = 0;

    // create a circle
    virtual db::Shape* createCircle(const Point& center, dist dia, bool donut = false) = 0;

    // create an arc
    virtual db::Shape* createArc(const Point& center,
                                 dist radius,
                                 dist width,
                                 geom::Angle startAngle = geom::Angle::zero,
                                 geom::Angle endAngle = geom::Angle::twoPi,
                                 db::EndCap endCapStyle = db::EndCap::Round) = 0;

    // create a donut
    virtual db::Shape* createDonut(const Point& center, dist meanDiameter, dist width) = 0;

    // create text and return db::Text object
    virtual db::Shape* createText() = 0;

    // Set text position
    virtual void setTextPosition(const Point& pos) = 0;

    // Set initial text height (used as a scaling factor for both height & width)
    virtual void setTextHeight(double height) = 0;

    // set stroke width
    virtual void setTextStrokeWidth(dist strokeWidth) = 0;

    // Set text style
    virtual void setTextStyle(db::TextStyle flags,
                              db::TextStyleMask mask = db::TextStyleMask::None) = 0;

    // sets the text string
    virtual void setFormattedText(std::string_view str) = 0;

    // Set the text string from raw ASCII
    virtual void setUnformattedText(std::string_view str) = 0;

    // Set initial font
    virtual void setTextFont(std::string_view fontName) = 0;

    // Set initial text width factor
    virtual void setTextWidthFactor(double factor) = 0;

    // Set initial oblique angle
    virtual void setTextObliquingAngle(geom::Angle angleCcw) = 0;

    // Mirror text in x-direction
    virtual void setTextMirroredInX(bool mirror = true) = 0;

    // Mirror text in y-direction
    virtual void setTextMirroredInY(bool mirror = true) = 0;

    // Set text rotation
    virtual void setTextRotation(geom::Angle angleCcw, bool absolute = false) = 0;

    // Set the box width used for word wrap calculations as the maximum line
    // width. a value of 0 disables word wrapping.
    virtual void setTextBoxWidth(dist width) = 0;

    // Set line spacing
    virtual void setTextLineSpacing(double spacing) = 0;

    // Create a polyline
    virtual db::Shape* createPolyline(dist width,
                                      const PointArray& vertices,
                                      bool closed = false,
                                      db::EndCap endCapStyle = db::EndCap::Round) = 0;

    // Create a polygon
    virtual db::Shape* createPolygon(const PointArray& vertices, bool makeSimple = false) = 0;

    // Create a polygon
    virtual db::Shape* createPolygon(const PointArray& vertices,
                                     const std::vector<double>& bulges) = 0;

    // Create a rectangular polygon
    virtual db::Shape* createRectangle(const Point& p0, const Point& p1) = 0;

    //! Creates a non-uniform, non-rational B-spline
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 =
    //! cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    //!
    //! \return  New non-uniform, non-rational B-spline instance
    virtual db::Shape* createNurbs(dist width,
                                   int degree,
                                   const std::vector<double>& knots,
                                   const PointArray& ctrlPoints,
                                   bool periodic = false) = 0;

    //! Creates a non-uniform, non-rational B-spline (NURBS)
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       otherwise knot count = control point count, and first and last
    //!       control point identical
    //!
    //! \param   width           Line width
    //! \param   degree          NURBS degree (1 = polyline, 2 = quadratric, 3 =
    //! cubic, ...)
    //! \param   knots           Knot vector
    //! \param   ctrlPoints      Control points vector
    //! \param   weights         Weights vector
    //! \param   periodic        (optional) If true, NURBS is periodic (closed)
    //!
    //! \return  New NURBS instance
    virtual db::Shape* createNurbs(dist width,
                                   int degree,
                                   const std::vector<double>& knots,
                                   const PointArray& ctrlPoints,
                                   const std::vector<double>& weights,
                                   bool periodic = false) = 0;

    // Add and open a new cell
    virtual void openCell(int cellNumber, bool isMainCell = false, bool reopen = false) = 0;

    // Add and open a new cell
    virtual void openCell(std::string_view name, bool isMainCell = false, bool reopen = false) = 0;

    // Set current cell's name
    virtual void setCellName(std::string_view name) = 0;

    // Set current cell's modification time
    virtual void setCellModifTime(time_t time) = 0;

    // Set current cell's access time
    virtual void setCellAccessTime(time_t time) = 0;

    // Close current cell
    virtual void closeCell() = 0;

    // Delete current cell
    virtual void deleteCell() = 0;

    // Test if cell exists
    virtual bool findCell(std::string_view name) = 0;

    // Select current drawing layer
    virtual db::Layer* selectLayer(db::Layer* layer) = 0;

    // Select current drawing layer
    virtual db::Layer* selectLayer(std::string_view layerName) = 0;

    // Select current drawing layer
    virtual db::Layer* selectLayer(int layerNumber) = 0;

    // Select current drawing layer
    virtual db::Layer* selectLayer(int major, int minor) = 0;

    // Set current layer's comment
    virtual void setLayerComment(std::string_view comment) = 0;

    // Set current layer's color
    virtual void setLayerColor(util::Color color) = 0;

    // Enable / disable current layer
    virtual void setLayerEnabled(bool enabled) = 0;

    // Set current layer's z value
    virtual void setLayerZ(coord z) = 0;

    // Set current layer's polarity
    virtual void setLayerPolarityPositive(bool positive = true) = 0;

    //! Gets current Cell
    //!
    //! \return  Current Cell; or nullptr if none
    virtual db::Cell* cell() = 0;

    //! Gets current Layer
    //!
    //! \return  Current Layer; or nullptr if none
    virtual db::Layer* layer() = 0;

    //! Gets current CellObject
    //!
    //! \return  Current CellObject; or nullptr if none
    virtual db::CellObject* cellObject() = 0;

    //! Save context and obtain handle
    virtual ContextHandle saveContext() = 0;

    //! Restore saved context from handle
    virtual void enterContext(ContextHandle handle) = 0;

    //! Leave restored context
    virtual void leaveContext() = 0;
};

inline IDrawingBuilder::~IDrawingBuilder() = default;

}  // namespace plugin
}  // namespace lc
