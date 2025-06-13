//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Collection.h"

namespace lc::db {

class LayerTable;
class Layer;
template <class T, class Tag>
class Collection;

//! Base class for iterators on collections of Layers
class DBAPI LayerIteratorBase
{
public:  // interface IEnumerator
    //! Reset iterator (position *before* first record)
    void reset();

    //! Move iterator to next position
    bool moveNext();

public:
    //! Get object pointed to by iterator
    Layer* dereference() const;

    //! Increment iterator
    void increment();

protected:
    friend class Collection<Layer, Drawing>;

    //! Compare two iterators
    bool equal(const LayerIteratorBase& other) const;

    //! Constructor
    LayerIteratorBase();

    //! Constructor
    LayerIteratorBase(const Collection<Layer, Drawing>& coll, size_t idx);

private:
    LayerTable* table_;
    size_t idx_;
    mutable size_t nextIdx_;
    mutable bool nextValid_;
};

//! Partial specialization of Collection for Layers
//!
//! \see Collection
template <>
class DBAPI Collection<Layer, Drawing>
{
public:
    using value_type = Layer;
    using pointer = Layer*;
    using reference = Layer&;
    using Iterator = Iterator<Layer*, Drawing, Collection, LayerIteratorBase>;
    using ConstIterator = ConstIterator<Layer*, Drawing, Collection, LayerIteratorBase>;
    using iterator = Iterator;  // for STL compatibility
    using const_iterator = ConstIterator;  // for STL compatibility

    //! Constructors
    Collection();
    explicit Collection(LayerTable* table);

    //! Get number of items in collection
    size_t size() const;

    //! Return true if collection empty
    bool empty() const;

    //! Get drawing
    Drawing* drawing() const;

    //! Get iterator representing first layer
    Iterator begin();
    ConstIterator begin() const;

    //! Get iterator pointing just beyond the last layer
    Iterator end();
    ConstIterator end() const;

    //! Return true if collection contains layer
    bool contains(const Layer* layer) const;

private:
    friend class LayerIteratorBase;
    LayerTable* table_;
};

}  // namespace lc::db
