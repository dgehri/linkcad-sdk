//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/geom/geomdefs.h>
#include <lc/util/enumclassops.h>

#if defined(__DOXYGEN__)
#define VIRTUAL virtual
#elif defined(VIRTUAL)
#undef VIRTUAL
#define VIRTUAL
#else
#define VIRTUAL
#endif

//------------------------------------------------------------------------------
//! Macro for flagging pseudo-virtual functions
//------------------------------------------------------------------------------
#ifdef _MSC_VER
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

//! The \c db namespace contains all types exposed by the drawing database library
namespace lc::db {

//! Fill modes
enum class FillMode : unsigned char
{
    Outline,  //!< only draw polygon, circle etc. outlines
    Filled  //!< fill polygons and circles (may be overridden by application)
};

//! Line style
enum class LineStyle : unsigned char
{
    Solid,  //!< solid line
    Dashed  //!< Dashed line
};

//! End cap styles
enum class EndCap : unsigned char
{
    Round,
    SquareExtended,
    SquareFlat
};

//! Polygon classification
enum class PolygonClass : unsigned char
{
    ConvexSimple,  //!< convex with area to fill
    ConcaveSimple,  //!< concave sections, possibly self-intersecting
    ConvexDegenerate,  //!< polygon is a line (maybe with repeated points)
    ConcaveDegenerate,  //!< the polygon could be drawn with simple lines
                        //!<    but repeated points create a situation where
                        //!<    there are no interior points
    Unclassified
};

//! Fill rules for self-intersecting polygons
using geom::FillRule;

//! Vertex mode
enum class VertexMode : unsigned char
{
    RawVertices = 0,  //!< return raw vertices
    RemoveDuplicates = 1,  //!< remove identical consecutive vertices (except if consisting of only
                           //!< two identical vertices)
    NoDuplicateEnd = 2,  //!< force last vertex != first vertex (if closed)
    ForceDuplicateEnd = 4  //!< force last vertex == first vertex (if closed)
};

IMPL_ENUM_CLASS_OP_BITWISE_OR(VertexMode);
IMPL_ENUM_CLASS_OP_BITWISE_AND(VertexMode);
IMPL_ENUM_CLASS_OP_BITWISE_NOT(VertexMode);
IMPL_ENUM_CLASS_OP_BITWISE_XOR(VertexMode);

//! Cell context
enum class CellContext : unsigned char
{
    DontDescend,  //!< only perform operation on specified cell
    Descend  //!< traverse specified cell and descendents
};

//! Purpose of drawing operation
enum class DrawType : unsigned char
{
    Display,
    Explode
};

//! NURBS Degree
enum class NurbsDegree : unsigned char
{
    Linear = 1,  //!< degree = 1
    Quadratic = 2,  //!< degree = 2
    Cubic = 3,  //!< degree = 3
    Quintic = 5  //!< degree = 5
};

//! Text styles
enum class TextStyle : unsigned char
{
    Default = 0,  //!< default text style

    // Horizontal text alignment
    AlignHLeft = 0,  //!< left of text block
    AlignHCenter = 1,  //!< center of text block
    AlignHRight = 2,  //!< right of text block

    // Vertical alignment of text attachment point
    AlignVBaseline = 0,  //!< baseline of last line
    AlignVBottom = 1 << 2,  //!< bottom of text block
    AlignVMiddle = 2 << 2,  //!< middle of text block (between ascent and descent if present or
                            //!< always when LineSpacingExact)
    AlignVMiddleAscent = 3 << 2,  //!< middle of text block (between ascent and baseline)
    AlignVTop = 4 << 2,  //!< top of text block

    // Text orientation
    OrientH = 0,
    OrientV = 1 << 5,

    // Text line spacing
    LineSpacingExact = 0,  //!< always vertically space text lines by the same amount
    LineSpacingCompact = 1 << 6  //!< spaces text lines closer if no character with descents
};

IMPL_ENUM_CLASS_OP_BITWISE_OR(TextStyle);

enum class TextStyleMask : unsigned char
{
    None = 0,
    AlignH =
        ((int)TextStyle::AlignHLeft | (int)TextStyle::AlignHCenter | (int)TextStyle::AlignHRight),
    AlignV = ((int)TextStyle::AlignVBaseline | (int)TextStyle::AlignVBottom |
              (int)TextStyle::AlignVMiddle | (int)TextStyle::AlignVMiddleAscent |
              (int)TextStyle::AlignVTop),
    Orient = ((int)TextStyle::OrientH | (int)TextStyle::OrientV),
    LineSpacing = ((int)TextStyle::LineSpacingCompact | (int)TextStyle::LineSpacingExact)
};

IMPL_ENUM_CLASS_OP_BITWISE_OR(TextStyleMask);
IMPL_ENUM_CLASS_OP_BITWISE_NOT(TextStyleMask);
IMPL_ENUM_CLASS_OP_BITWISE_AND_2(TextStyle, TextStyleMask);

}  // namespace lc::db
