//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "ObjectType.h"
#include "Collection.h"

namespace lc::db {

class CellObjectTable;
class CellObject;
class Cell;
template <class T, class Tag>
class Collection;

//! Base class for iterators on collections of CellObject-derived classes
class DBAPI CellObjectIteratorBase
{
public:  // interface IEnumerator
    //! Move iterator to next position
    bool moveNext();

public:
    //! Get object pointed to by iterator
    CellObject* dereference() const;

    //! Increment iterator
    void increment();

protected:
    friend class CellObjectCollectionBase;

    //! Compare two iterators
    bool equal(const CellObjectIteratorBase& other) const;

    //! Constructor
    CellObjectIteratorBase();

    //! Constructor
    CellObjectIteratorBase(const CellObjectCollectionBase& coll, size_t idx);

private:
    CellObjectTable* table_;
    size_t idx_;  // current CellObject index
    mutable size_t nextIdx_;
    ObjectType firstType_;
    ObjectType lastType_;
    bool selectedOnly_;
    mutable bool nextValid_;
};

//! Base class for collections of CellObject-derived classes
class DBAPI CellObjectCollectionBase
{
public:
    //! Constructors
    CellObjectCollectionBase();

    //! Get number of items in collection
    size_t size() const;

    //! Return true if collection empty
    bool empty() const;

    //! Get drawing
    Drawing* drawing() const;

protected:
    friend class CellObjectIteratorBase;

    //! Constructor
    CellObjectCollectionBase(const Cell* cell,
                             ObjectType first,
                             ObjectType last,
                             bool selectedOnly = false);

    //! Get iterator representing first element
    CellObjectIteratorBase begin() const;

    //! Get iterator pointing just beyond the last element
    CellObjectIteratorBase end() const;

    //! Return true if collection contains shape
    bool contains(const CellObject* obj) const;

protected:
    CellObjectTable* table_;
    size_t cellIdx_;
    ObjectType firstType_;
    ObjectType lastType_;
    bool selectedOnly_;
};

//------------------------------------------------------------------------------
__forceinline CellObjectIteratorBase::CellObjectIteratorBase(const CellObjectCollectionBase& coll,
                                                             size_t idx)
    : table_(coll.table_)
    , idx_(idx)
    , nextIdx_(static_cast<size_t>(-2))
    , firstType_(coll.firstType_)
    , lastType_(coll.lastType_)
    , selectedOnly_(coll.selectedOnly_)
    , nextValid_(false)
{}

//------------------------------------------------------------------------------
__forceinline bool CellObjectIteratorBase::equal(const CellObjectIteratorBase& other) const
{
    return (table_ == other.table_) && (idx_ == other.idx_);
}

//------------------------------------------------------------------------------
__forceinline CellObjectIteratorBase CellObjectCollectionBase::end() const
{
    return {*this, static_cast<size_t>(-1)};
}

//! Partial specialization of Collection for CellObject in a Cell
//!
//! \see Collection
template <class T>
class Collection<T, Cell> : public CellObjectCollectionBase
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using Iterator = Iterator<T*, Cell, Collection, CellObjectIteratorBase>;
    using ConstIterator = ConstIterator<T*, Cell, Collection, CellObjectIteratorBase>;
    using iterator = Iterator;  // for STL compatibility
    using const_iterator = ConstIterator;  // for STL compatibility

    //! Constructors
    Collection<T, Cell>() = default;
    Collection(const Cell* cell, ObjectType first, ObjectType last, bool selectedOnly = false);

    //! Get iterator representing first element
    Iterator begin();
    ConstIterator begin() const;

    //! Get iterator pointing just beyond the last element
    Iterator end();
    ConstIterator end() const;

    //! Return true if collection contains entity
    bool contains(const T* obj) const;

    //! Only return selected entity
    void setSelectedOnly(bool selected = true);
};

//------------------------------------------------------------------------------
template <class T>
Collection<T, Cell>::Collection(const Cell* cell,
                                ObjectType first,
                                ObjectType last,
                                bool selectedOnly /* = false */)
    : CellObjectCollectionBase(cell, first, last, selectedOnly)
{}

//------------------------------------------------------------------------------
template <class T>
typename Collection<T, Cell>::ConstIterator Collection<T, Cell>::begin() const
{
    return ConstIterator(CellObjectCollectionBase::begin());
}

//------------------------------------------------------------------------------
template <class T>
typename Collection<T, Cell>::ConstIterator Collection<T, Cell>::end() const
{
    return ConstIterator(CellObjectCollectionBase::end());
}

//------------------------------------------------------------------------------
template <class T>
typename Collection<T, Cell>::Iterator Collection<T, Cell>::begin()
{
    return Iterator(CellObjectCollectionBase::begin());
}

//------------------------------------------------------------------------------
template <class T>
typename Collection<T, Cell>::Iterator Collection<T, Cell>::end()
{
    return Iterator(CellObjectCollectionBase::end());
}

//------------------------------------------------------------------------------
template <class T>
bool Collection<T, Cell>::contains(const T* obj) const
{
    return CellObjectCollectionBase::contains(obj);
}

//------------------------------------------------------------------------------
template <class T>
void Collection<T, Cell>::setSelectedOnly(bool selected /* = true */)
{
    selectedOnly_ = selected;
}

}  // namespace lc::db
