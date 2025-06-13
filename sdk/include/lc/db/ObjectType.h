//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "dllexport.h"

namespace lc::db {

//! List of class types in database
enum class ObjectType : unsigned char
{
    Invalid = 0,

    BooleanProperty,
    DrawingBooleanProperty = BooleanProperty,
    CellObjectBooleanProperty,
    CellBooleanProperty,
    LayerBooleanProperty,
    IntegerProperty,
    DrawingIntegerProperty = IntegerProperty,
    CellObjectIntegerProperty,
    CellIntegerProperty,
    LayerIntegerProperty,
    RealProperty,
    DrawingRealProperty = RealProperty,
    CellObjectRealProperty,
    CellRealProperty,
    LayerRealProperty,
    StringProperty,
    DrawingStringProperty = StringProperty,
    CellObjectStringProperty,
    CellStringProperty,
    LayerStringProperty,

    Drawing,
    Layer,
    Cell,

    _CellObjectFirst,
    Ref = _CellObjectFirst,
    _ShapeFirst,
    Arc = _ShapeFirst,
    Polygon,
    Polyline,
    Donut,
    Text,
    Ellipse,
    Nurbs,
    _ShapeLast = Nurbs,
    _CellObjectLast = _ShapeLast,
    Tessellation,

    // abstract types
    Object,
    Property,
    DrawingObject,
    CellObject,
    Shape,

    _Last
};

// Get type name
DBAPI const char* nameOf(ObjectType type);

// Check if type is derived from other
DBAPI bool isDerivedFrom(ObjectType type, ObjectType baseType);

// Get offset
inline int operator-(ObjectType type, ObjectType base)
{
    return static_cast<int>(type) - static_cast<int>(base);
}

}  // namespace lc::db
