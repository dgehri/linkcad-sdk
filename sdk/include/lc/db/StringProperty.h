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

//! Property for attaching character strings to database objects
//!
//! \tparam T    Class to which property is attached, must be either a Drawing,
//!              CellObject, Cell or Layer.
template <class T>
class DBAPI StringProperty final : public Property
{
    DECLARE_LCDB_OBJECT(PropertyType<T>::type(ObjectType::StringProperty));

public:
    using ValueType = std::string;
    using ArgumentType = const std::string&;

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
    static StringProperty<T>* createInstance(const Drawing* dwg,
                                             const std::string& defaultValue = std::string(),
                                             const std::string& name = std::string());

    //! Lookup existing property, returns nullptr if it doesn't exist
    static StringProperty<T>* lookup(const Drawing* dwg, const std::string& name);

    //! Set value
    void setFor(T* obj, const std::string& value);
    static void setFor(T* obj, const std::string& name, const std::string& value);

    //! Get value
    std::string getFor(const T* obj) const;
    static std::string getFor(const T* obj, const std::string& name);
};

}  // namespace lc::db
