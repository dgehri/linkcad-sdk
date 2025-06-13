//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Property.h"

namespace lc::db {

//! Property for attaching floating point values to database objects
//!
//! \tparam T    Class to which property is attached, must be either a Drawing,
//!              CellObject, Cell or Layer.
template <class T>
class DBAPI RealProperty final : public Property
{
    DECLARE_LCDB_OBJECT(PropertyType<T>::type(ObjectType::RealProperty));

public:
    using ValueType = double;
    using ArgumentType = double;

    //! Create or return existing property
    //!
    //! \param   dwg             Drawing database.
    //! \param   defaultValue    (optional) Default value of property
    //! \param   name            (optional) Property name; leave empty for
    //!                          anonymous properties, which won't be undo-recorded,
    //!                          and that don't fire modification events.
    //!
    //! \return  If a property of the same name exists, the parameters `defaultValue`
    //!          and `name` are ignored, and the existing instance is returned;
    //!          otherwise, a new instance is returned
    static RealProperty<T>* createInstance(const Drawing* dwg,
                                           double defaultValue = 0.0,
                                           const std::string& name = std::string());

    //! Lookup existing property, returns nullptr if it doesn't exist
    static RealProperty<T>* lookup(const Drawing* dwg, const std::string& name);

    //! Set value
    void setFor(T* obj, double value);
    static void setFor(T* obj, const std::string& name, double value);

    //! Get value
    double getFor(const T* obj) const;
    static double getFor(const T* obj, const std::string& name);
};

}  // namespace lc::db
