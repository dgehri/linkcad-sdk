//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dllexport.h"
#include "dbtypes.h"
#include <lc/util/enumclassops.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace lc::db {

enum class SnapMode
{
    None = 0x0000,
    EndPoint = 0x0001,
    MidPoint = 0x0002,
    CenterPoint = 0x0004,
    NodePoint = 0x0008,
    QuadrantPoint = 0x0010,
    InsertionPoint = 0x0020,
    PerpPoint = 0x0040,
    TanPoint = 0x0080,
    NearPoint = 0x0100
};

IMPL_ENUM_CLASS_OP_BITWISE_OR(SnapMode);

//------------------------------------------------------------------------------
// Snap point class
//------------------------------------------------------------------------------
class DBAPI SnapPoint
{
public:
    // Constructor
    SnapPoint();
    SnapPoint(const Point& pt, SnapMode modeMask, size_t userData = 0);

    // Get point
    const Point& point() const;

    // Get mode mask
    SnapMode modeMask() const;

    // Get user data
    size_t userData() const;

private:
    Point pt_;
    SnapMode modeMask_;
    size_t userData_;
};

}  // namespace lc::db

#ifdef _MSC_VER
#pragma warning(pop)
#endif
