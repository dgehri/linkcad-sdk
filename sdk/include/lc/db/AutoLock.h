//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Drawing.h"

namespace lc::db {

//! Class for locking the database until end of sequence point
//!
//! Use with `with_readLock()` and `with_writeLock()`
//!
//! Use like this:
//!     auto name = with_readLock(dwg_)->mainCell()->name();
template <class T, LockType lockType>
class AutoLock
{
public:
    //! Constructor
    explicit AutoLock(T* obj)
        : lock_()
        , obj_(obj)
    {}

    //! Dereferencing
    T* operator->() const { return obj_; }

private:
    Drawing::Lock<lockType> lock_;
    T* obj_;
};

//! Read-lock database until next sequence point
//!
//! Use like this:
//!     auto name = with_readLock(dwg_)->mainCell()->name();
template <class T>
AutoLock<T, LockType::ReadAccess> with_readLock(T* obj)
{
    return AutoLock<T, LockType::ReadAccess>(obj);
}

//! Write-lock database until next sequence point
//!
//! Use like this:
//!     with_writeLock(dwg_)->mainCell()->setName("NewName");
template <class T>
AutoLock<T, LockType::WriteAccess> with_writeLock(T* obj)
{
    return AutoLock<T, LockType::WriteAccess>(obj);
}

//! Write-lock database until next sequence point
//!
//! Use like this:
//!     with_writeLock(dwg_)->mainCell()->setName("NewName");
template <class T>
AutoLock<T, LockType::VolatileWriteAccess> with_volatileWriteLock(T* obj)
{
    return AutoLock<T, LockType::VolatileWriteAccess>(obj);
}


}  // namespace lc::db
