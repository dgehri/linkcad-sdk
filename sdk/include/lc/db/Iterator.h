//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dllexport.h"
#include "dbtypes.h"
#include <iterator>

namespace lc::db {

//------------------------------------------------------------------------------
// Forward declarations
//------------------------------------------------------------------------------
class Object;
class ObjectTable;
class CollectionBase;

template <class T, class Tag, class Coll, class Base>
class ConstIterator;

template <class T, class Tag>
class Collection;

template <class T, class Tag, class Coll, class Base>
bool operator==(const ConstIterator<T, Tag, Coll, Base>& lhs,
                const ConstIterator<T, Tag, Coll, Base>& rhs);

template <class T, class Tag, class Coll, class Base>
bool operator!=(const ConstIterator<T, Tag, Coll, Base>& lhs,
                const ConstIterator<T, Tag, Coll, Base>& rhs);

//! Default iterator base implementation
//!
//! The iterator class implements a lazy look-ahead algorithm: whenever the
//! the iterator is dereferenced, the next iterator position is pre-calculated.
//! This ensures that incrementing works even if the current element position
//! has been invalidated.
class DBAPI IteratorBase
{
public:  // interface IEnumerator
    //! Move iterator to next position
    bool moveNext();

public:
    //! Get object pointed to by iterator
    Object* dereference() const;

    //! Increment iterator
    void increment();

protected:
    //! Constructor
    IteratorBase();

    //! Compare two iterators
    bool equal(const IteratorBase& other) const;

private:
    friend CollectionBase;

    //! Constructor
    IteratorBase(const CollectionBase& coll, size_t idx);

private:
    ObjectTable* table_;
    size_t idx_;
    mutable size_t nextIdx_;
    uint16_t mask_;
    bool wasReset_;
    mutable bool nextValid_;
};

//! ConstIterator class
template <class T, class Tag, class Coll, class Base = IteratorBase>
class ConstIterator : public Base
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    ConstIterator() = default;
    const T get() const;
    const T operator*() const;
    const T operator->() const;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    friend bool operator== <>(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator!= <>(const ConstIterator& lhs, const ConstIterator& rhs);

protected:
    friend Coll;
    explicit ConstIterator(const Base& base)
        : Base(base)
    {}
};

//! Iterator class
template <class T, class Tag, class Coll, class Base = IteratorBase>
class Iterator : public ConstIterator<T, Tag, Coll, Base>
{
public:
    Iterator() = default;
    T get() const;
    T operator*() const;
    T operator->() const;
    Iterator& operator++();
    Iterator operator++(int);

protected:
    friend Coll;
    explicit Iterator(const Base& base)
        : ConstIterator<T, Tag, Coll, Base>(base)
    {}
};

//------------------------------------------------------------------------------
inline bool IteratorBase::equal(const IteratorBase& other) const
{
    return (table_ == other.table_) && (idx_ == other.idx_);
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
const T ConstIterator<T, Tag, Coll, Base>::get() const
{
    return static_cast<const T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
const T ConstIterator<T, Tag, Coll, Base>::operator*() const
{
    return static_cast<const T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
const T ConstIterator<T, Tag, Coll, Base>::operator->() const
{
    return static_cast<const T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
ConstIterator<T, Tag, Coll, Base>& ConstIterator<T, Tag, Coll, Base>::operator++()
{
    Base::increment();
    return *this;
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
ConstIterator<T, Tag, Coll, Base> ConstIterator<T, Tag, Coll, Base>::operator++(int)
{
    ConstIterator tmp(*this);
    ++*this;
    return tmp;
}
//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
T Iterator<T, Tag, Coll, Base>::get() const
{
    return static_cast<T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
T Iterator<T, Tag, Coll, Base>::operator*() const
{
    return static_cast<T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
T Iterator<T, Tag, Coll, Base>::operator->() const
{
    return static_cast<T>(Base::dereference());
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
Iterator<T, Tag, Coll, Base>& Iterator<T, Tag, Coll, Base>::operator++()
{
    Base::increment();
    return *this;
}

//------------------------------------------------------------------------------
template <class T, class Tag, class Coll, class Base>
Iterator<T, Tag, Coll, Base> Iterator<T, Tag, Coll, Base>::operator++(int)
{
    Iterator tmp(*this);
    ++*this;
    return tmp;
}

//! Compare for inequality
template <class T, class Tag, class Coll, class Base>
bool operator==(const ConstIterator<T, Tag, Coll, Base>& lhs,
                const ConstIterator<T, Tag, Coll, Base>& rhs)
{
    return lhs.equal(rhs);
}

//! Compare for equality
template <class T, class Tag, class Coll, class Base>
bool operator!=(const ConstIterator<T, Tag, Coll, Base>& lhs,
                const ConstIterator<T, Tag, Coll, Base>& rhs)
{
    return !(lhs == rhs);
}

}  // namespace lc::db
