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

namespace lc::plugin {

//------------------------------------------------------------------------------
// Interface for building drawing database
//
// This interface provides methods to construct a drawing database by creating
// cells (containers), layers, and various geometric shapes. The typical workflow
// involves:
// 1. Opening a cell
// 2. Selecting a layer
// 3. Creating shapes on that layer
// 4. Closing the cell
//------------------------------------------------------------------------------
struct IDrawingBuilder : IPluginController
{
    //! Layer style flags that control how objects inherit layer properties
    enum LayerFlags
    {
        LayerNormal = 0,  //!< Object uses its own layer properties
        LayerByLayer = 1,  //!< Object inherits properties from its layer
        LayerByBlock = 2  //!< Object inherits properties from containing block/cell
    };

    //! Handle type for saved drawing contexts
    using ContextHandle = size_t;

    virtual ~IDrawingBuilder() = 0;

    //! Get the current arc approximation precision settings
    //! \return Resolution object containing facet count and error tolerance
    virtual const Resolution& resolution() const = 0;

    //! Set the name of the drawing
    //! \param drawingName The name to assign to the drawing (typically the filename without
    //! extension)
    virtual void setDrawingName(std::string_view drawingName) = 0;

    //! Set the modification timestamp of the drawing
    //! \param time Unix timestamp of last modification
    virtual void setDrawingModifTime(time_t time) = 0;

    //! Set the access timestamp of the drawing
    //! \param time Unix timestamp of last access
    virtual void setDrawingAccessTime(time_t time) = 0;

    //! Set how the current entity inherits layer properties
    //! \param layerStyle One of LayerNormal, LayerByLayer, or LayerByBlock
    virtual void setEntityLayerStyle(LayerFlags layerStyle) = 0;

    //! Create a reference to another cell by its number
    //! \param cellNumber The numeric identifier of the cell to reference
    //! \return Pointer to the created reference object, or nullptr on failure
    virtual db::Ref* createRef(int cellNumber) = 0;

    //! Create a reference to another cell by its name
    //! \param cellName The name of the cell to reference
    //! \return Pointer to the created reference object, or nullptr on failure
    virtual db::Ref* createRef(std::string_view cellName) = 0;

    //! Scale the current reference
    //! \param scale Scaling factor (1.0 = no scaling, 2.0 = double size, etc.)
    //! \param isAbsolute If true, sets absolute scale; if false, multiplies existing scale
    virtual void scaleRef(double scale, bool isAbsolute = false) = 0;

    //! Mirror the current reference vertically (flip over horizontal axis)
    //! \param negate If true, performs the mirror; if false, does nothing
    virtual void mirrorRefInY(bool negate = true) = 0;

    //! Mirror the current reference horizontally (flip over vertical axis)
    //! \param negate If true, performs the mirror; if false, does nothing
    virtual void mirrorRefInX(bool negate = true) = 0;

    //! Rotate the current reference
    //! \param angle Rotation angle in counter-clockwise direction
    //! \param isAbsolute If true, sets absolute rotation; if false, adds to existing rotation
    virtual void rotateRef(geom::Angle angle, bool isAbsolute = false) = 0;

    //! Move the current reference to a specific position
    //! \param pt The coordinates where the reference origin should be placed
    virtual void translateRef(const Point& pt) = 0;

    //! Set spacing for reference arrays (when creating multiple instances)
    //! \param dx Horizontal spacing between array elements
    //! \param dy Vertical spacing between array elements
    virtual void setRefArraySpacing(dist dx, dist dy) = 0;

    //! Set dimensions for reference arrays
    //! \param cols Number of columns (horizontal repetitions)
    //! \param rows Number of rows (vertical repetitions)
    virtual void setRefArraySize(int cols, int rows) = 0;

    //! Create a circle shape
    //! \param center Center point of the circle
    //! \param dia Diameter of the circle
    //! \param donut If true, may combine with previous circle to form a donut
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createCircle(const Point& center, dist dia, bool donut = false) = 0;

    //! Create an arc shape
    //! \param center Center point of the arc
    //! \param radius Arc radius
    //! \param width Line width of the arc
    //! \param startAngle Starting angle (0 = east, increases counter-clockwise)
    //! \param endAngle Ending angle
    //! \param endCapStyle How to terminate the arc ends (Round, SquareFlat, SquareExtended)
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createArc(const Point& center,
                                 dist radius,
                                 dist width,
                                 geom::Angle startAngle = geom::Angle::zero,
                                 geom::Angle endAngle = geom::Angle::twoPi,
                                 db::EndCap endCapStyle = db::EndCap::Round) = 0;

    //! Create a donut (annulus) shape
    //! \param center Center point of the donut
    //! \param meanDiameter Average of inner and outer diameters
    //! \param width Radial width (thickness) of the donut ring
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createDonut(const Point& center, dist meanDiameter, dist width) = 0;

    //! Create a text object (must call setText* methods afterwards)
    //! \return Pointer to the created text shape, or nullptr on failure
    virtual db::Shape* createText() = 0;

    //! Set the position of the current text object
    //! \param pos Coordinates for the text insertion point
    virtual void setTextPosition(const Point& pos) = 0;

    //! Set the height of the current text object
    //! \param height Text height in drawing units (also affects width proportionally)
    virtual void setTextHeight(double height) = 0;

    //! Set the stroke width for the current text object
    //! \param strokeWidth Line thickness used to draw text characters
    virtual void setTextStrokeWidth(dist strokeWidth) = 0;

    //! Set style flags for the current text object
    //! \param flags Style bits to set (bold, italic, underline, etc.)
    //! \param mask Which style bits to modify (unmasked bits remain unchanged)
    virtual void setTextStyle(db::TextStyle flags,
                              db::TextStyleMask mask = db::TextStyleMask::None) = 0;

    //! Set the text string with formatting codes
    //! \param str Text string that may contain backslash formatting codes
    virtual void setFormattedText(std::string_view str) = 0;

    //! Set the text string as plain ASCII (escapes any special characters)
    //! \param str Plain text string to display
    virtual void setUnformattedText(std::string_view str) = 0;

    //! Set the font for the current text object
    //! \param fontName Name of the font to use (e.g., "Arial", "simplex.shx")
    virtual void setTextFont(std::string_view fontName) = 0;

    //! Set the width scaling factor for the current text object
    //! \param factor Width multiplier (1.0 = normal, 0.5 = compressed, 2.0 = expanded)
    virtual void setTextWidthFactor(double factor) = 0;

    //! Set the oblique (slant) angle for the current text object
    //! \param angleCcw Slant angle from vertical (positive = lean right)
    virtual void setTextObliquingAngle(geom::Angle angleCcw) = 0;

    //! Mirror the current text object horizontally
    //! \param mirror If true, flips text left-to-right
    virtual void setTextMirroredInX(bool mirror = true) = 0;

    //! Mirror the current text object vertically
    //! \param mirror If true, flips text upside-down
    virtual void setTextMirroredInY(bool mirror = true) = 0;

    //! Set the rotation angle for the current text object
    //! \param angleCcw Rotation angle in counter-clockwise direction
    //! \param absolute If true, sets absolute angle; if false, adds to existing rotation
    virtual void setTextRotation(geom::Angle angleCcw, bool absolute = false) = 0;

    //! Set the bounding box width for text wrapping
    //! \param width Maximum line width before wrapping (0 = no wrapping)
    virtual void setTextBoxWidth(dist width) = 0;

    //! Set the line spacing for multi-line text
    //! \param spacing Line spacing factor (1.0 = normal, 2.0 = double-spaced)
    virtual void setTextLineSpacing(double spacing) = 0;

    //! Create a polyline (connected line segments)
    //! \param width Line width for all segments
    //! \param vertices Array of points defining the polyline path
    //! \param closed If true, connects last vertex to first
    //! \param endCapStyle How to terminate line ends (Round, SquareFlat, SquareExtended)
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createPolyline(dist width,
                                      const PointArray& vertices,
                                      bool closed = false,
                                      db::EndCap endCapStyle = db::EndCap::Round) = 0;

    //! Create a polygon (filled area)
    //! \param vertices Array of points defining the polygon boundary
    //! \param makeSimple If true, resolves self-intersections and ensures proper winding
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createPolygon(const PointArray& vertices, bool makeSimple = false) = 0;

    //! Create a polygon with curved edges
    //! \param vertices Array of points defining the polygon vertices
    //! \param bulges Bulge values for each edge (0 = straight, positive = CCW arc)
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createPolygon(const PointArray& vertices,
                                     const std::vector<double>& bulges) = 0;

    //! Create a rectangular polygon
    //! \param p0 First corner of the rectangle
    //! \param p1 Opposite corner of the rectangle
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createRectangle(const Point& p0, const Point& p1) = 0;

    //! Creates a non-uniform, non-rational B-spline (smooth curve)
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       For periodic NURBS:
    //!       knot count = control point count, and first and last
    //!       control points should be identical
    //!
    //! \param width Line width of the curve
    //! \param degree NURBS degree (1 = polyline, 2 = quadratic, 3 = cubic, etc.)
    //! \param knots Knot vector defining parameter spacing
    //! \param ctrlPoints Control points that influence the curve shape
    //! \param periodic If true, creates a closed curve
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createNurbs(dist width,
                                   int degree,
                                   const std::vector<double>& knots,
                                   const PointArray& ctrlPoints,
                                   bool periodic = false) = 0;

    //! Creates a non-uniform, rational B-spline (NURBS) with weights
    //!
    //! \note For non-periodic NURBS:
    //!       knot count = control point count + degree + 1;
    //!       For periodic NURBS:
    //!       knot count = control point count, and first and last
    //!       control points should be identical
    //!
    //! \param width Line width of the curve
    //! \param degree NURBS degree (1 = polyline, 2 = quadratic, 3 = cubic, etc.)
    //! \param knots Knot vector defining parameter spacing
    //! \param ctrlPoints Control points that influence the curve shape
    //! \param weights Weight values for each control point (higher = stronger influence)
    //! \param periodic If true, creates a closed curve
    //! \return Pointer to the created shape, or nullptr on failure
    virtual db::Shape* createNurbs(dist width,
                                   int degree,
                                   const std::vector<double>& knots,
                                   const PointArray& ctrlPoints,
                                   const std::vector<double>& weights,
                                   bool periodic = false) = 0;

    //! Open a new cell for adding shapes (identified by number)
    //! \param cellNumber Numeric identifier for the cell
    //! \param isMainCell If true, this becomes the top-level cell for export
    //! \param reopen If true, allows reopening an existing cell
    virtual void openCell(int cellNumber, bool isMainCell = false, bool reopen = false) = 0;

    //! Open a new cell for adding shapes (identified by name)
    //! \param name String identifier for the cell
    //! \param isMainCell If true, this becomes the top-level cell for export
    //! \param reopen If true, allows reopening an existing cell
    virtual void openCell(std::string_view name, bool isMainCell = false, bool reopen = false) = 0;

    //! Change the name of the current cell
    //! \param name New name to assign to the cell
    virtual void setCellName(std::string_view name) = 0;

    //! Set the modification timestamp of the current cell
    //! \param time Unix timestamp of last modification
    virtual void setCellModifTime(time_t time) = 0;

    //! Set the access timestamp of the current cell
    //! \param time Unix timestamp of last access
    virtual void setCellAccessTime(time_t time) = 0;

    //! Close the current cell (must be called after openCell)
    virtual void closeCell() = 0;

    //! Delete the current cell and all its contents
    virtual void deleteCell() = 0;

    //! Check if a cell with the given name exists
    //! \param name Cell name to search for
    //! \return true if the cell exists, false otherwise
    virtual bool findCell(std::string_view name) = 0;

    //! Select an existing layer as the current layer
    //! \param layer Pointer to the layer to select
    //! \return Pointer to the selected layer
    virtual db::Layer* selectLayer(db::Layer* layer) = 0;

    //! Select a layer by name (creates if doesn't exist)
    //! \param layerName Name of the layer to select or create
    //! \return Pointer to the selected layer
    virtual db::Layer* selectLayer(std::string_view layerName) = 0;

    //! Select a layer by number (creates if doesn't exist)
    //! \param layerNumber Numeric identifier for the layer
    //! \return Pointer to the selected layer
    virtual db::Layer* selectLayer(int layerNumber) = 0;

    //! Select a layer by major/minor numbers (creates if doesn't exist)
    //! \param major Major layer number (e.g., for layer "2:1", major = 2)
    //! \param minor Minor layer number (e.g., for layer "2:1", minor = 1)
    //! \return Pointer to the selected layer
    virtual db::Layer* selectLayer(int major, int minor) = 0;

    //! Set a descriptive comment for the current layer
    //! \param comment Text description of the layer's purpose
    virtual void setLayerComment(std::string_view comment) = 0;

    //! Set the display color for the current layer
    //! \param color RGB color value for layer visualization
    virtual void setLayerColor(util::Color color) = 0;

    //! Enable or disable the current layer
    //! \param enabled If false, layer contents won't be exported/displayed
    virtual void setLayerEnabled(bool enabled) = 0;

    //! Set the Z-coordinate (elevation) for the current layer
    //! \param z Vertical position in 3D space
    virtual void setLayerZ(coord z) = 0;

    //! Set the polarity of the current layer
    //! \param positive If true, layer adds material; if false, layer removes material
    virtual void setLayerPolarityPositive(bool positive = true) = 0;

    //! Get the currently active cell
    //! \return Current cell pointer, or nullptr if no cell is open
    virtual db::Cell* cell() = 0;

    //! Get the currently selected layer
    //! \return Current layer pointer, or nullptr if no layer is selected
    virtual db::Layer* layer() = 0;

    //! Get the most recently created cell object (shape, text, etc.)
    //! \return Current cell object pointer, or nullptr if none exists
    virtual db::CellObject* cellObject() = 0;

    //! Save the current drawing context for later restoration
    //! \return Handle that can be used to restore this context
    virtual ContextHandle saveContext() = 0;

    //! Switch to a previously saved drawing context
    //! \param handle Context handle obtained from saveContext()
    virtual void enterContext(ContextHandle handle) = 0;

    //! Return from a restored context to the previous context
    virtual void leaveContext() = 0;
};

inline IDrawingBuilder::~IDrawingBuilder() = default;

}  // namespace lc::plugin
