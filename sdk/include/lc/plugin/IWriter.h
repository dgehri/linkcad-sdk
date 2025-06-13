//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IWriterController.h"
#include <lc/db/db_fwd.h>
#include <filesystem>

namespace lc {
namespace plugin {

//------------------------------------------------------------------------------
// Interface for format writers
//------------------------------------------------------------------------------
struct IWriter
{
    // Convert file
    virtual bool writeFile(const std::filesystem::path& filePath,
                           IWriterController* controller) = 0;

    //! Render polygon
    //!
    //! \param poly         Polygon to render
    //! \param fillRule     Fill rule to use for rendering
    //! \retval true        Polygon was rendered successfully
    //! \retval false       Polygon was not rendered - the controlling class
    //!                     will simplify the polygon and again call this member
    //!                     with `fillRule = geom::FillRule::NonZero`.
    virtual bool writeEntity(const db::Polygon* poly, geom::FillRule fillRule) = 0;

    // Render polyline
    virtual bool writeEntity(const db::Polyline* pline) = 0;

    // Render arc
    virtual bool writeEntity(const db::Arc* arc) = 0;

    // Render NURBS
    virtual bool writeEntity(const db::Nurbs* nurbs) = 0;

    // Render circle
    virtual bool writeEntity(const db::Ellipse* circle) = 0;

    // Render donut
    virtual bool writeEntity(const db::Donut* donut) = 0;

    // Render text
    virtual bool writeEntity(const db::Text* text) = 0;

    // Render reference
    virtual bool writeEntity(const db::Ref* ref, const db::Layer* layer) = 0;

    // Destroy writer
    virtual void destroy() = 0;

    virtual ~IWriter() = 0;
};

inline IWriter::~IWriter() = default;

}  // namespace plugin
}  // namespace lc
