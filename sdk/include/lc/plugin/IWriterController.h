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

enum class SortOrder
{
    Regular,
    Reverse
};

//------------------------------------------------------------------------------
// Interface for controlling the writer
//------------------------------------------------------------------------------
struct IWriterController : IPluginController
{
    virtual ~IWriterController() = 0;

    // Initialize the progress bar counter
    virtual void initProgressCounter(bool forceFlattened = false) = 0;

    // Set flatten mode
    virtual void flattenCellHierarchy(bool mode = true) = 0;

    // Specify how entities are exploded into polygons
    virtual void setPolygonMode(geom::HolesMode holesMode, geom::PolygonType polygonType) = 0;

    // Get size_t count
    [[nodiscard]] virtual size_t objectCount() const = 0;

    // Change object count
    virtual void setObjectCount(size_t count) = 0;

    // Get total object count
    [[nodiscard]] virtual size_t totalObjectCount() const = 0;

    // Change object count
    virtual void setTotalObjectCount(size_t count) = 0;

    // Get the output file name
    [[nodiscard]] virtual std::filesystem::path fileName() const = 0;

    // Get arc precision
    [[nodiscard]] virtual const Resolution& resolution() const = 0;

    // Get the number of enabled layers
    [[nodiscard]] virtual size_t layerCount() const = 0;

    // Start enumerating enabled layers
    virtual void startEnumLayers(SortOrder sortOrder = SortOrder::Regular) = 0;

    // Enumerate next enabled layer
    virtual const db::Layer* nextLayer() = 0;

    // Start enumerating fonts in database
    virtual void startEnumFonts() = 0;

    // Enumerate next font
    virtual bool enumNextFont(std::string& font) = 0;

    /**
     * \brief Enumerate sub-cells (excluding main cell), in child first order
     */
    virtual void startEnumCells() = 0;

    // Enumerate next enabled sub-cell
    virtual const db::Cell* nextCell(const db::Layer* layer = nullptr) = 0;

    // Get main (top) cell
    [[nodiscard]] virtual const db::Cell* mainCell() const = 0;

    // Render a cell
    virtual void renderCell(const db::Cell* cell,
                            const db::Layer* layer = nullptr,
                            const Xform& xform = Xform::identity) = 0;

    // Render a cell (in layer order)
    virtual void renderCellInLayerOrder(const db::Cell* cell,
                                        const Xform& xform = Xform::identity) = 0;

    //! Get fill rule to use for rendering self-intersecting polygons
    virtual db::FillRule fillRule() = 0;

    // Get current transformation
    [[nodiscard]] virtual const Xform& transformation(bool persistent = false) const = 0;

    // Point transformation
    [[nodiscard]] virtual Point transform(const Point& pt) const = 0;
    [[nodiscard]] virtual geom::Point2d transform(const geom::Point2d& pt) const = 0;

    // Transform width (relative distance)
    [[nodiscard]] virtual dist transformDistance(dist d) const = 0;
    [[nodiscard]] virtual double transformDistance(double d) const = 0;
};

inline IWriterController::~IWriterController() = default;

}  // namespace lc::plugin
