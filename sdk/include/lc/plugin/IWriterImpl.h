//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IWriter.h"

namespace lc {
namespace plugin {

//! Default implementation of IWriter interface
class IWriterImpl : public IWriter
{
public:
    // Render polygon
    bool writeEntity(const db::Polygon*, geom::FillRule) override { return false; }

    // Render polyline
    bool writeEntity(const db::Polyline*) override { return false; }

    // Render arc
    bool writeEntity(const db::Arc*) override { return false; }

    // Render NURBS
    bool writeEntity(const db::Nurbs*) override { return false; }

    // Render circle
    bool writeEntity(const db::Ellipse*) override { return false; }

    // Render donut
    bool writeEntity(const db::Donut*) override { return false; }

    // Render text
    bool writeEntity(const db::Text*) override { return false; }

    // Render reference
    bool writeEntity(const db::Ref*, const db::Layer*) override { return false; }

    // Destroy writer
    void destroy() override { delete this; }
};

}  // namespace plugin
}  // namespace lc
