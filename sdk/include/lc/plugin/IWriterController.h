//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IPluginController.h"
#include <lc/geom/geomdefs.h>
#include <lc/geom/Xform.h>
#include <filesystem>

namespace lc::plugin {

//! Sort order for layer enumeration
enum class SortOrder
{
    Regular,    //!< Normal order (as defined in drawing)
    Reverse     //!< Reversed order
};

//------------------------------------------------------------------------------
// Interface for controlling the writer during export operations
//
// This interface provides methods to control how a drawing is written/exported.
// It manages the export process including progress tracking, layer enumeration,
// cell hierarchy traversal, and coordinate transformations.
//------------------------------------------------------------------------------
struct IWriterController : IPluginController
{
    virtual ~IWriterController() = 0;

    //! Initialize the progress bar counter for export operation
    //! \param forceFlattened If true, counts objects as if hierarchy will be flattened
    //! \note Call this before starting export to enable accurate progress reporting
    virtual void initProgressCounter(bool forceFlattened = false) = 0;

    //! Set whether to flatten the cell hierarchy during export
    //! \param mode If true, all cell references are expanded inline; if false, preserves hierarchy
    //! \note Flattening can significantly increase file size but ensures compatibility
    virtual void flattenCellHierarchy(bool mode = true) = 0;

    //! Specify how entities are converted to polygons when required by the output format
    //! \param holesMode How to handle polygons with holes (Separate, Combined, etc.)
    //! \param polygonType Type of polygons to generate (Filled, Outline, etc.)
    virtual void setPolygonMode(geom::HolesMode holesMode, geom::PolygonType polygonType) = 0;

    //! Get the current count of processed objects
    //! \return Number of objects processed so far
    //! \note Used for progress reporting during export
    [[nodiscard]] virtual size_t objectCount() const = 0;

    //! Set the current object count (for progress tracking)
    //! \param count New object count value
    //! \note Updates progress indicator based on total object count
    virtual void setObjectCount(size_t count) = 0;

    //! Get the total number of objects to be exported
    //! \return Total object count for the entire export operation
    [[nodiscard]] virtual size_t totalObjectCount() const = 0;

    //! Set the total object count (for progress tracking)
    //! \param count Total number of objects that will be exported
    //! \note Should be set before export begins for accurate progress reporting
    virtual void setTotalObjectCount(size_t count) = 0;

    //! Get the output file path
    //! \return Full path to the file being written
    [[nodiscard]] virtual std::filesystem::path fileName() const = 0;

    //! Get the arc approximation precision settings
    //! \return Resolution object specifying how curves should be approximated
    [[nodiscard]] virtual const Resolution& resolution() const = 0;

    //! Get the number of enabled layers in the drawing
    //! \return Count of layers that will be exported (enabled layers only)
    [[nodiscard]] virtual size_t layerCount() const = 0;

    //! Begin enumerating enabled layers
    //! \param sortOrder Order in which layers should be enumerated
    //! \note Call nextLayer() repeatedly to get each layer
    virtual void startEnumLayers(SortOrder sortOrder = SortOrder::Regular) = 0;

    //! Get the next enabled layer in the enumeration
    //! \return Pointer to next layer, or nullptr when enumeration is complete
    //! \note Must call startEnumLayers() before using this method
    virtual const db::Layer* nextLayer() = 0;

    //! Begin enumerating fonts used in the drawing
    //! \note Call enumNextFont() repeatedly to get each font name
    virtual void startEnumFonts() = 0;

    //! Get the next font name in the enumeration
    //! \param font Output parameter to receive the font name
    //! \return true if a font was retrieved, false if enumeration is complete
    //! \note Must call startEnumFonts() before using this method
    virtual bool enumNextFont(std::string& font) = 0;

    //! Begin enumerating sub-cells in child-first order
    //! \note Excludes the main cell, which is accessed via mainCell()
    //! \note Child-first order ensures referenced cells are processed before referencing cells
    virtual void startEnumCells() = 0;

    //! Get the next enabled sub-cell in the enumeration
    //! \param layer If specified, only returns cells that use this layer
    //! \return Pointer to next cell, or nullptr when enumeration is complete
    //! \note Must call startEnumCells() before using this method
    virtual const db::Cell* nextCell(const db::Layer* layer = nullptr) = 0;

    //! Get the main (top-level) cell of the drawing
    //! \return Pointer to the main cell (never null for valid drawings)
    [[nodiscard]] virtual const db::Cell* mainCell() const = 0;

    //! Render all objects in a cell with optional transformation
    //! \param cell The cell whose contents should be rendered
    //! \param layer If specified, only renders objects on this layer
    //! \param xform Transformation to apply to all objects in the cell
    //! \note Handles both shapes and cell references within the cell
    virtual void renderCell(const db::Cell* cell,
                            const db::Layer* layer = nullptr,
                            const Xform& xform = Xform::identity) = 0;

    //! Render a cell's contents in layer order (bottom to top)
    //! \param cell The cell whose contents should be rendered
    //! \param xform Transformation to apply to all objects in the cell
    //! \note Ensures proper layer stacking order in the output
    virtual void renderCellInLayerOrder(const db::Cell* cell,
                                        const Xform& xform = Xform::identity) = 0;

    //! Get the fill rule to use for self-intersecting polygons
    //! \return Fill rule (NonZero, EvenOdd, etc.) for polygon rendering
    virtual db::FillRule fillRule() = 0;

    //! Get the current coordinate transformation matrix
    //! \param persistent If true, returns a transformation that remains valid across calls
    //! \return Current transformation matrix (accumulates all nested transformations)
    //! \note Non-persistent transformations may become invalid after transformation stack changes
    [[nodiscard]] virtual const Xform& transformation(bool persistent = false) const = 0;

    //! Transform a point using the current transformation matrix
    //! \param pt Point to transform
    //! \return Transformed point in output coordinate system
    [[nodiscard]] virtual Point transform(const Point& pt) const = 0;

    //! Transform a double-precision point using the current transformation matrix
    //! \param pt Point to transform
    //! \return Transformed point in output coordinate system
    [[nodiscard]] virtual geom::Point2d transform(const geom::Point2d& pt) const = 0;

    //! Transform a distance/width value (accounts for scaling only)
    //! \param d Distance value to transform
    //! \return Transformed distance (affected by scale but not translation)
    [[nodiscard]] virtual dist transformDistance(dist d) const = 0;

    //! Transform a double-precision distance/width value (accounts for scaling only)
    //! \param d Distance value to transform
    //! \return Transformed distance (affected by scale but not translation)
    [[nodiscard]] virtual double transformDistance(double d) const = 0;
};

inline IWriterController::~IWriterController() = default;

}  // namespace lc::plugin
