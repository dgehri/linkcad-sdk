//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Iterator.h"
#include "Ref.h"

namespace lc::db {

class CellTable;
class CellRefTag;  // tag to distinguish this collection
template <class T, class Tag>
class Collection;

//------------------------------------------------------------------------------
// Iterator implementation for collection of references to a cell
//------------------------------------------------------------------------------
class DBAPI CellRefIteratorBase
{
public:  // interface IEnumerator
    // Reset iterator (position *before* first record)
    void reset();

    // Move iterator to next position
    bool moveNext();

public:
    // Constructor
    CellRefIteratorBase();

    // Get object pointed to by iterator
    Ref* dereference() const;

    // Increment iterator
    void increment();

    // Compare two iterators
    bool equal(const CellRefIteratorBase& other) const;

    // Get drawing
    Drawing* drawing() const;

private:
    friend class Collection<Ref, CellRefTag>;

    // Constructor
    CellRefIteratorBase(CellTable* table, size_t posIdx, size_t refIdx);

private:
    CellTable* table_;
    mutable size_t posIdx_;
    size_t refIdx_;
    mutable size_t nextRefIdx_;
};

//------------------------------------------------------------------------------
// Collection of all references to a cell
//------------------------------------------------------------------------------
template <>
class DBAPI Collection<Ref, CellRefTag>
{
public:
    using value_type = Ref;
    using pointer = Ref*;
    using reference = Ref&;
    using Iterator = Iterator<Ref*, CellRefTag, Collection, CellRefIteratorBase>;
    using ConstIterator = ConstIterator<Ref*, CellRefTag, Collection, CellRefIteratorBase>;
    using iterator = Iterator;  // for STL compatibility
    using const_iterator = ConstIterator;  // for STL compatibility

    // Constructors
    Collection();
    explicit Collection(const Cell* cell);

    // Get iterator representing first element
    Iterator begin();
    ConstIterator begin() const;

    // Get iterator pointing just beyond the last element
    Iterator end();
    ConstIterator end() const;

    // Get number of items in collection
    size_t size() const;

    // Return true if collection empty
    bool empty() const;

    // Return true if collection contains reference
    bool contains(const Ref* ref) const;

private:
    friend class CellRefIteratorBase;

    const Cell* cell_;
};

}  // namespace lc::db
