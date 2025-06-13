//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/geom/PointArray.h>
#include <lc/lctypes.h>

namespace lc {
namespace shp {

//------------------------------------------------------------------------------
// IShapeRenderer interface
//------------------------------------------------------------------------------
struct IShapeRenderer
{
    virtual ~IShapeRenderer() = default;

    // consume next trace
    virtual void render(const PointArray& vertices, double charSize) = 0;
};

}  // namespace shp
}  // namespace lc
