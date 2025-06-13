//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#pragma once

#include "lctypes.h"
#include <stdexcept>

namespace lc {

//! Defined units
enum class Unit
{
    None = -3,
    DotsPerInch = -2,
    Facets = -1,

    UnitMin = 0,
    Nanometer = 0,
    Micron = 1,
    Mil = 2,
    Point = 3,
    Millimeter = 4,
    Centimeter = 5,
    Inch = 6,
    Meter = 7,
    Feet = 8,
    Picometer = 9,
    UnitMax = 9,

    Database = Picometer
};

template <typename T>
T checked_enum_cast(int value) = delete;

//! Safely cast integer to `Unit` enum
//!
//! \param value     Index to cast to `Unit` enum
//! \return          `value` cast to `Unit` enum
//! \throw std::out_of_range     If `value` is not a valid `Unit`
template <>
inline Unit checked_enum_cast<Unit>(int value)
{
    if (value < -2 || value > 9)
        throw std::out_of_range("Invalid Unit");

    return static_cast<Unit>(value);
}

//! Structure containing unit display name and Unit enum
struct UnitInfo
{
    const char* name;
    Unit unit;
};

//! Array of `UnitInfo` for display in user interface
extern const __declspec(selectany) UnitInfo visibleUnits[] = {
    {"nm", Unit::Nanometer}, {"um", Unit::Micron},     {"mil", Unit::Mil},
    {"point", Unit::Point},  {"mm", Unit::Millimeter}, {"cm", Unit::Centimeter},
    {"in", Unit::Inch},      {"m", Unit::Meter},       {"ft", Unit::Feet}};

//! Returns short name of units
inline std::string to_string(Unit units)
{
    switch (units)
    {
    case Unit::None: return "";
    case Unit::DotsPerInch: return "dpi";
    case Unit::Nanometer: return "nm";
    case Unit::Micron: return "um";
    case Unit::Mil: return "mil";
    case Unit::Point: return "point";
    case Unit::Millimeter: return "mm";
    case Unit::Centimeter: return "cm";
    case Unit::Inch: return "in";
    case Unit::Meter: return "m";
    case Unit::Feet: return "ft";
    case Unit::Picometer: return "pm";
    default: return "";
    }
}

//! Scaling factors to convert to Unit::Database
#define ONE_PM (1)
#define ONE_NM (1000)
#define ONE_MICRON (1000000)
#define ONE_MIL (25400000)
#define ONE_POINT (352777778)
#define ONE_MM (1000000000)
#define ONE_CM (10000000000)
#define ONE_INCH (25400000000)
#define ONE_FOOT (304800000000)
#define ONE_METER (1000000000000)

//------------------------------------------------------------------------------
// Various conversion functions
//------------------------------------------------------------------------------
#define CM2PT(x) ((int)((double)(x) * 72.0 / 2.54))
#define PT2DB(x) (lc::util::round<coord>(static_cast<double>(x) * ONE_POINT))
#define DB2PT(x) ((int)(static_cast<double>(x) / ONE_POINT))

//! Get scaling factor
//!
//! \param units     Unit for which to return scaling factor
//! \tparam T        Type of returned scaling factor
//! \return          `units` expressed in database units (currently picometers)
template <typename T>
constexpr T unitsToScalingFactor(Unit units)
{
    auto result = T(ONE_PM);
    switch (units)
    {
    case Unit::Nanometer: result = T(ONE_NM); break;  // nm
    case Unit::Micron: result = T(ONE_MICRON); break;  // microns
    case Unit::Mil: result = T(ONE_MIL); break;  // mil
    case Unit::Point: result = T(ONE_POINT); break;  // point
    case Unit::Millimeter: result = T(ONE_MM); break;  // mm
    case Unit::Centimeter: result = T(ONE_CM); break;  // cm
    case Unit::Inch: result = T(ONE_INCH); break;  // inch
    case Unit::Meter: result = T(ONE_METER); break;  // meter
    case Unit::Feet: result = T(ONE_FOOT); break;  // foot
    case Unit::Picometer: result = T(ONE_PM); break;  // pm
    default: result = T(ONE_PM); break;  // nm
    }

    return result;
}

//! Convert specified value between two units
//!
//! \param value         Source value
//! \param sourceUnits   Units of source value
//! \param targetUnits   Units for return value
//! \return              Source value converted into target units
template <typename T, typename U = T>
constexpr T convertUnits(T value, Unit sourceUnits, Unit targetUnits)
{
    static_assert(std::is_floating_point_v<U>, "Use floating point type for U");
    auto scaling = unitsToScalingFactor<U>(sourceUnits) / unitsToScalingFactor<U>(targetUnits);
    return value * scaling;
}

//! Convert specified value between two units
//!
//! \param value         Source value
//! \param sourceUnits   Units of source value
//! \tparam targetUnits  Units for return value
//! \return              Source value converted into target units
template <Unit targetUnits, typename T, typename U = T>
constexpr T convertUnits(T value, Unit sourceUnits)
{
    static_assert(std::is_floating_point_v<U> || targetUnits == Unit::Picometer,
                  "Use floating point type for U");
    auto scaling = unitsToScalingFactor<U>(sourceUnits) / unitsToScalingFactor<U>(targetUnits);
    return value * scaling;
}

//! Convert specified value between two units
//!
//! \param value         String containing source value
//! \param sourceUnits   Units of source value
//! \tparam T            Type of return value
//! \tparam targetUnits  Units for return value
//! \return              Source value converted into target units
//!
//! \throw std::invalid_argument if `value` cannot be parsed as a floating point
//!        value
template <Unit targetUnits, typename T>
T convertUnits(const std::string& value, Unit sourceUnits)
{
    auto scaling = static_cast<double>(unitsToScalingFactor<T>(sourceUnits));
    scaling /= unitsToScalingFactor<T>(targetUnits);
    return static_cast<T>(std::stod(value) * scaling);
}

template <Unit targetUnits, std::enable_if_t<targetUnits == Unit::Database>>
coord convertUnits(const std::string& value, Unit sourceUnits)
{
    auto scaling = static_cast<double>(unitsToScalingFactor<coord>(sourceUnits));
    return static_cast<coord>(std::stod(value) * scaling);
}

//! Convert specified value between two units
//!
//! \param value         String containing source value
//! \param sourceUnits   Units of source value
//! \tparam T            Type of return value
//! \param targetUnits   Units for return value
//! \return              Source value converted into target units
//!
//! \throw std::invalid_argument if `value` cannot be parsed as a floating point
//!        value
template <typename T>
T convertUnits(const std::string& value, Unit sourceUnits, Unit targetUnits)
{
    auto scaling = unitsToScalingFactor<double>(sourceUnits);
    scaling /= unitsToScalingFactor<double>(targetUnits);
    return static_cast<T>(std::stod(value) * scaling);
}

}  // namespace lc
