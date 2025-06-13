//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbtypes.h"
#include "Property.h"

namespace lc::db {

//! Property for attaching integer values to database objects
//!
//! \tparam T    Class to which property is attached, must be either a Drawing,
//!              CellObject, Cell or Layer.
template <class T>
class DBAPI IntegerProperty : public Property
{
    DECLARE_LCDB_OBJECT(PropertyType<T>::type(ObjectType::IntegerProperty));

public:
    using ValueType = dbint;
    using ArgumentType = dbint;

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
    static IntegerProperty* createInstance(const Drawing* dwg,
                                           dbint defaultValue = 0,
                                           const std::string& name = std::string());

    //! Lookup existing property, returns nullptr if it doesn't exist
    static IntegerProperty* lookup(const Drawing* dwg, const std::string& name);

    //! Set value
    void setFor(T* obj, dbint value);
    static void setFor(T* obj, const std::string& name, dbint value);

    //! Get value
    dbint getFor(const T* obj) const;
    static dbint getFor(const T* obj, const std::string& name);
};

//! Property for attaching integer values to database objects
//!
//! \tparam T    Class to which property is attached, must be either a Drawing,
//!              CellObject, Cell or Layer.
//! \tparam U    Integer type
template <class T, typename U>
class TypedIntegerProperty final : public IntegerProperty<T>
{
    static_assert(std::is_convertible<U, dbint>::value || std::is_enum<U>::value,
                  "U should be convertible to dbint");

public:
    using ValueType = U;
    using ArgumentType = U;

    //! Create or return existing property
    //!
    //! \param name  Property name; leave empty for anonymous properties, which
    //!                 won't be undo-recorded, and that don't fire modification
    //!                 events.
    static TypedIntegerProperty* createInstance(const Drawing* dwg,
                                                U defaultValue = U(),
                                                const std::string& name = std::string())
    {
        return static_cast<TypedIntegerProperty*>(
            IntegerProperty<T>::createInstance(dwg, static_cast<dbint>(defaultValue), name));
    }

    //! Lookup existing property, returns nullptr if it doesn't exist
    static TypedIntegerProperty* lookup(const Drawing* dwg, const std::string& name)
    {
        return static_cast<TypedIntegerProperty*>(IntegerProperty<T>::lookup(dwg, name));
    }

    //! Set value
    void setFor(T* obj, U value) { IntegerProperty<T>::setFor(obj, static_cast<dbint>(value)); }

    static void setFor(T* obj, const std::string& name, U value)
    {
        IntegerProperty<T>::setFor(obj, name, static_cast<dbint>(value));
    }

    //! Get value
    U getFor(const T* obj) const { return static_cast<U>(IntegerProperty<T>::getFor(obj)); }

    static U getFor(const T* obj, const std::string& name)
    {
        return static_cast<U>(IntegerProperty<T>::getFor(obj, name));
    }
};

}  // namespace lc::db
