//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Drawing.h"
#include "CellObject.h"
#include "Cell.h"
#include "Layer.h"
#include <string>

namespace lc::db {

//! Base class for specific properties
class DBAPI Property : public Object
{
    DECLARE_LCDB_OBJECT(ObjectType::Property);

public:
    using Properties = std::vector<std::string>;

    //! Destroy object
    void destroy();

    // Get type of object the property is attached to
    ObjectType ownerType() const;

    //! Get list of property names
    //!
    //! \param dwg           Drawing
    //! \param holdingType   One of \c TypeDrawing, \c TypeCell, \c ObjectType::Layer,
    //!                      \c TypeCellObject
    //! \return List of property names
    static Properties properties(const db::Drawing* dwg, ObjectType holdingType);

    //! Get property type from name
    //!
    //! \param dwg           Drawing
    //! \param name          Property name
    //! \param holdingType   One of \c TypeDrawing, \c TypeCell, \c ObjectType::Layer,
    //!                      \c TypeCellObject
    //! \return ObjectType of property
    static ObjectType lookupType(const db::Drawing* dwg,
                                 const std::string& name,
                                 ObjectType holdingType);
};

namespace {
template <class T>
struct PropertyType;
template <>
struct PropertyType<Drawing>
{
    static ObjectType type(ObjectType baseType)
    {
        return ObjectType(static_cast<uint8_t>(baseType) + 0);
    }
};
template <>
struct PropertyType<CellObject>
{
    static ObjectType type(ObjectType baseType)
    {
        return ObjectType(static_cast<uint8_t>(baseType) + 1);
    }
};
template <>
struct PropertyType<Cell>
{
    static ObjectType type(ObjectType baseType)
    {
        return ObjectType(static_cast<uint8_t>(baseType) + 2);
    }
};
template <>
struct PropertyType<Layer>
{
    static ObjectType type(ObjectType baseType)
    {
        return ObjectType(static_cast<uint8_t>(baseType) + 3);
    }
};
}  // namespace

}  // namespace lc::db
