//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include <string>

namespace lc::geom {

//! Fill rules for self-intersecting polygons
enum class FillRule : unsigned char
{
    NonZero,  //!< Fill using non-zero winding rule
    EvenOdd  //!< Fill using even-odd rule
};

enum class HolesMode : unsigned char
{
    Link,  //!< Link holes to outer contour
    Split,  //!< Split polygons at holes
    Extract,  //!< Neither link nor split at holes
    Keep  //!< Keep holes as outer polygons
};

enum class PolygonType : unsigned char
{
    AllowComplex,  //!< Allow self-intersecting polygons
    ForceSimple  //!< Produce simple, non self-intersecting polygons
};

enum class VertexType : unsigned char
{
    First,
    Intermediate,
    Last
};

inline std::string to_string(FillRule rule)
{
    switch (rule)
    {
    case FillRule::NonZero: return "NonZero";
    case FillRule::EvenOdd: return "EvenOdd";
    default: return "";
    }
}

inline std::string to_string(HolesMode mode)
{
    switch (mode)
    {
    case HolesMode::Link: return "Link";
    case HolesMode::Split: return "Split";
    case HolesMode::Keep: return "Keep";
    case HolesMode::Extract: return "Extract";
    default: return "";
    }
}

inline std::string to_string(PolygonType type)
{
    switch (type)
    {
    case PolygonType::AllowComplex: return "AllowComplex";
    case PolygonType::ForceSimple: return "ForceSimple";
    default: return "";
    }
}

inline std::string to_string(VertexType type)
{
    switch (type)
    {
    case VertexType::First: return "First";
    case VertexType::Intermediate: return "Intermediate";
    case VertexType::Last: return "Last";
    default: return "";
    }
}

}  // namespace lc::geom
