//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "db_fwd.h"

namespace lc::db {

class DBAPI AutoPtrBase
{
protected:
    static void destroy(Object* p) noexcept;
};

//------------------------------------------------------------------------------
// Helper class for automatic destruction of database objects
//------------------------------------------------------------------------------
template <class T>
class AutoPtr final : public AutoPtrBase
{
public:
    using PointerType = T*;

    template <class Q>
    struct AutoPtrRef
    {
        explicit AutoPtrRef(Q* p)
            : ref(p)
        {}
        Q* ref;
    };

    // Constructor
    explicit AutoPtr(T* p = nullptr) noexcept;

    // Move constructor
    AutoPtr(AutoPtr&& rhs) noexcept;

    // Move constructor from compatible type
    template <class U>
    explicit AutoPtr(AutoPtr<U>&& rhs) noexcept;

    // Construct from AutoPtrRef
    explicit AutoPtr(AutoPtrRef<T>&& rhs) noexcept;

    // Move assignment
    AutoPtr& operator=(AutoPtr&& rhs) noexcept;

    // Move assignment from compatible AutoPtr
    template <class U>
    AutoPtr& operator=(AutoPtr<U>&& rhs) noexcept;

    // Move assignment from AutoPtrRef
    AutoPtr& operator=(AutoPtrRef<T>&& rhs) noexcept;

    // No copy constructor
    AutoPtr(const AutoPtr& rhs) noexcept = delete;

    // No copy assignment
    AutoPtr& operator=(AutoPtr& rhs) noexcept = delete;

    // Destructor
    ~AutoPtr() noexcept;

    // Value access
    T* get() const noexcept;

    // Release pointer
    T* release() noexcept;

    // Reset pointer
    void reset(T* p = nullptr) noexcept;

    // Return reference
    T& operator*() const noexcept { return *p_; }

    // Return stored pointer
    T* operator->() const noexcept { return (&**this); }

    // Return compatible AutoPtr
    template <class U>
    operator AutoPtr<U>() noexcept;

    // Return AutoPtrRef
    template <class U>
    operator AutoPtrRef<U>() noexcept;

private:
    T* p_;
};

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>::AutoPtr(T* p /* = nullptr */) noexcept
    : p_(p)
{}

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>::AutoPtr(AutoPtr&& rhs) noexcept
    : p_(rhs.release())
{}

//------------------------------------------------------------------------------
template <class T>
template <class U>
AutoPtr<T>::AutoPtr(AutoPtr<U>&& rhs) noexcept
    : p_(rhs.release())
{}

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>::AutoPtr(AutoPtrRef<T>&& rhs) noexcept
    : p_(rhs.ref_)
{}

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>::~AutoPtr() noexcept
{
    // if you receive error C2664 make sure to include the
    // lc::db headers.
    destroy(p_);
}

//------------------------------------------------------------------------------
template <class T>
T* AutoPtr<T>::release() noexcept
{
    T* temp = p_;
    p_ = nullptr;
    return temp;
}

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>& AutoPtr<T>::operator=(AutoPtr&& rhs) noexcept
{
    reset(rhs.release());
    return *this;
}

//------------------------------------------------------------------------------
template <class T>
template <class U>
AutoPtr<T>& AutoPtr<T>::operator=(AutoPtr<U>&& rhs) noexcept
{
    reset(rhs.release());
    return *this;
}

//------------------------------------------------------------------------------
template <class T>
AutoPtr<T>& AutoPtr<T>::operator=(AutoPtrRef<T>&& rhs) noexcept
{
    reset(rhs.ref_);
    return *this;
}

//------------------------------------------------------------------------------
template <class T>
T* AutoPtr<T>::get() const noexcept
{
    return p_;
}

//------------------------------------------------------------------------------
template <class T>
void AutoPtr<T>::reset(T* p /* = nullptr */) noexcept
{
    if (p != p_)
    {
        destroy(p_);
        p_ = p;
    }
}

//------------------------------------------------------------------------------
template <class T>
template <class U>
AutoPtr<T>::operator AutoPtr<U>() noexcept
{
    return AutoPtr<U>(release());
}

//------------------------------------------------------------------------------
template <class T>
template <class U>
AutoPtr<T>::operator typename AutoPtr<T>::AutoPtrRef<U>() noexcept
{
    return AutoPtrRef<U>(release());
}

}  // namespace lc::db
