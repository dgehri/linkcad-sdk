//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <concepts>

namespace lc::geom {

template <typename T>
class Point2dT;
template <typename T>
class Point3dT;
template <typename T>
class Vector2dT;
template <typename T>
class Vector3dT;
template <typename T>
class Xform;
class Tolerance;

template <typename T>
struct is_custom_arithmetic : std::false_type
{};

template <typename T>
constexpr bool is_custom_arithmetic_v = is_custom_arithmetic<T>::value;

template <typename T>
concept Scalar = std::is_arithmetic_v<T> || is_custom_arithmetic_v<T>;

template <typename T>
struct is_custom_floating_point : std::false_type
{};

template <typename T>
constexpr bool is_custom_floating_point_v = is_custom_floating_point<T>::value;

template <typename T>
concept FloatingPoint = std::floating_point<T> || is_custom_floating_point_v<T>;

}  // namespace lc::geom
