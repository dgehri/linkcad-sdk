//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#ifndef __clang__
#pragma managed(push, off)
#pragma push_macro("_MANAGED")
#undef _MANAGED
#endif

#include "dbdefs.h"
#include <lc/lcunits.h>
#include <lc/lctypes.h>
#include "dllexport.h"
#include <memory>
#include <vector>

namespace lc::db {

template <class T>
struct DBAPI Deleter
{
    void operator()(T* p) const;
};

template <class T>
DBAPI ::std::unique_ptr<T, Deleter<T>> allocate();

using PointArrayPtr = ::std::unique_ptr<PointArray, Deleter<PointArray>>;
using DoubleArrayPtr = ::std::unique_ptr<::std::vector<double>, Deleter<::std::vector<double>>>;

}  // namespace lc::db

#ifndef __clang__
#pragma pop_macro("_MANAGED")
#pragma managed(pop)
#endif
