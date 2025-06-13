//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "ObjectType.h"
#include "dbtypes.h"
#include <typeinfo>
#include <type_traits>

namespace lc::db {

class Drawing;
struct IObjectEventListener;

// Declare static type
#define DECLARE_LCDB_OBJECT(TYPE)  \
public:                            \
    static ObjectType staticType() \
    {                              \
        return TYPE;               \
    }                              \
                                   \
private:

//------------------------------------------------------------------------------
// Base class for all database objects
//
// Description:
//   The database hierarchy is as follows:
//   <code>
//
//   Object
//    |
//    +-Property
//    |  |
//    |  +-RealProperty<T>
//    |  +-IntegerProperty<T>
//    |  +-BooleanProperty<T>
//    |
//    +-Tessellation
//    +-Drawing
//    +-DrawingObject
//      |
//      +-Layer
//      +-Cell
//      +-CellObject
//        |
//        +-Ref
//        +-Shape
//          |
//          +-Arc
//          +-Donut
//          +-Ellipse
//          +-Nurbs
//          +-Polygon
//          +-Polyline
//          +-Text
//
//   </code>
//------------------------------------------------------------------------------
class DBAPI Object
{
    DECLARE_LCDB_OBJECT(ObjectType::Object);

public:
    //! Returns dynamic type of object
    ObjectType dynamicType() const;

    //! Returns type_info of derived object instance
    const std::type_info& typeInfo() const;

    //! Returns pointer to the database the object resides in
    Drawing* drawing() const;

    //! Returns true if object is a valid database object
    bool valid() const;

    // Destroy object [virtual]
    VIRTUAL void destroy();

    // Add event listener to object
    void addListener(IObjectEventListener* listener) const;

    // Remove listener
    void removeListener(IObjectEventListener* listener) const;

    //! Gets object ID
    //!
    //! \return unique object ID
    size_t id() const;

    Object(const Object&) = delete;
    Object(const Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(const Object&&) = delete;

    // disallow operator new/delete
    void* operator new(size_t) = delete;
    void* operator new(size_t, void*) = delete;
    void* operator new[](size_t) = delete;
    void* operator new[](size_t, void*) = delete;
    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

protected:
    ~Object() = default;

private:
    // NOTE: if you receive an error that the constructor of Object cannot be
    //       accessed, then you probably tried to instantiate an Object-derived
    //       object on the stack (or heap) instead of using the static
    //       createInstance() member function, which returns a pointer to the
    //       the object.
    Object() = default;
};

//------------------------------------------------------------------------------
template <class TPtr, class U>
TPtr dynamicCast(U* obj)
{
    using T = typename std::remove_pointer<TPtr>::type;
    ObjectType type = obj->dynamicType();
    ObjectType baseType = T::staticType();

    if (type == baseType || isDerivedFrom(type, baseType))
        return static_cast<TPtr>(obj);
    else
        return nullptr;
}

}  // namespace lc::db
