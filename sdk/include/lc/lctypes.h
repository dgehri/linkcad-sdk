//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <lc/geom/Point2d.h>
#include <lc/geom/Vector2d.h>
#include <lc/geom/Bounds.h>
#include <lc/geom/Xform.h>
#include <lc/geom/Resolution.h>
#include <lc/geom/PointArray.h>
#include <lc/geom/Region.h>

//! The \c lc namespace contains all types exposed by the LinkCAD application
namespace lc {

using dbint = int64_t;
using dbuint = uint64_t;
using coord = dbint;
using dist = dbint;
using udist = dbuint;
using Point = geom::Point2dT<coord>;
using Vector = geom::Vector2dT<dist>;
using Bounds = geom::Bounds<coord>;
using Xform = geom::Xform<coord>;
using FastBounds = geom::Bounds<coord, false>;
using PointArray = geom::PointArray<coord>;
using Resolution = geom::Resolution<dist>;
using Region = geom::Region<coord>;

}  // namespace lc
