//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "Iterator.h"

namespace lc::db {

class Object;
class ObjectTable;
class Drawing;

//! Collection of database objects
class DBAPI CollectionBase
{
public:
    // Get number of items in collection
    size_t size() const;

    // Return true if collection empty
    bool empty() const;

    // Return true if collection contains object
    bool contains(const Object* obj) const;

    // Get iterator representing first element
    IteratorBase begin() const;

    // Get iterator pointing just beyond the last element
    IteratorBase end() const;

    // Get drawing
    Drawing* drawing() const;

protected:
    friend class IteratorBase;

    CollectionBase();
    explicit CollectionBase(ObjectTable* table, uint16_t flags = 0);

private:
    ObjectTable* table_;
    uint16_t mask_;
};

//! Collection of database objects
//!
//!  \tparam T   Type of Object derived object in collection
//!  \tparam Tag A type used to differentiate collections of identical types (eg.
//!              use class containing the collection objects.
template <class T, class Tag>
class Collection final : protected CollectionBase
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using Iterator = Iterator<T*, Tag, Collection>;
    using ConstIterator = ConstIterator<T*, Tag, Collection>;
    using iterator = Iterator;  // for STL compatibility
    using const_iterator = ConstIterator;  // for STL compatibility

    // Constructors
    Collection();
    explicit Collection(ObjectTable* table, uint16_t flags = 0);

    // Get number of items in collection
    using CollectionBase::size;

    // Return true if collection empty
    using CollectionBase::empty;

    // Get drawing
    using CollectionBase::drawing;

    // Return true if collection contains object
    bool contains(const T* obj) const;

    // Get iterator representing first element
    Iterator begin();
    ConstIterator begin() const;

    // Get iterator pointing just beyond the last element
    Iterator end();
    ConstIterator end() const;
};

//------------------------------------------------------------------------------
template <class T, class Tag>
Collection<T, Tag>::Collection()
    : CollectionBase()
{}

//------------------------------------------------------------------------------
template <class T, class Tag>
Collection<T, Tag>::Collection(ObjectTable* table, uint16_t flags /* = 0 */)
    : CollectionBase(table, flags)
{}

//------------------------------------------------------------------------------
template <class T, class Tag>
bool Collection<T, Tag>::contains(const T* obj) const
{
    return CollectionBase::contains(obj);
}

//------------------------------------------------------------------------------
template <class T, class Tag>
typename Collection<T, Tag>::ConstIterator Collection<T, Tag>::begin() const
{
    return ConstIterator(CollectionBase::begin());
}

//------------------------------------------------------------------------------
template <class T, class Tag>
typename Collection<T, Tag>::ConstIterator Collection<T, Tag>::end() const
{
    return ConstIterator(CollectionBase::end());
}

//------------------------------------------------------------------------------
template <class T, class Tag>
typename Collection<T, Tag>::Iterator Collection<T, Tag>::begin()
{
    return Iterator(CollectionBase::begin());
}

//------------------------------------------------------------------------------
template <class T, class Tag>
typename Collection<T, Tag>::Iterator Collection<T, Tag>::end()
{
    return Iterator(CollectionBase::end());
}

}  // namespace lc::db
