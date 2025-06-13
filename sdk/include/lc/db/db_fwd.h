//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbtypes.h"
#include "dbdefs.h"
#include "IDrawingEventListener.h"
#include "IObjectEventListener.h"
#include "IRegionQuery.h"
#include "IRenderer.h"
#include "AutoPtr.h"

namespace lc::db {

//------------------------------------------------------------------------------
// Forward-declare database object types
//------------------------------------------------------------------------------
class Object;
class Drawing;
class Property;
template <class T>
class IntegerProperty;
template <class T>
class RealProperty;
template <class T>
class BooleanProperty;
template <class T>
class StringProperty;
template <class T, typename U>
class TypedIntegerProperty;
class DrawingObject;
class Layer;
class Cell;
class CellObject;
class Ref;
class Shape;
class Arc;
class Donut;
class Ellipse;
class Nurbs;
class Polygon;
class Polyline;
class Text;
class Tessellation;
class SnapPoint;
class Exception;
class DatabaseException;
class DatabaseFatalError;
class DatabaseSoftError;
class FontManager;
class BaseVisitor;

}  // namespace lc::db
