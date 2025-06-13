//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IFormatRegistry.h"

namespace lc {
namespace plugin {

struct IPlugInContext
{
    // Get IFormatRegistry interface
    virtual IFormatRegistry* formatRegistry() const = 0;

    virtual ~IPlugInContext() = 0;
};

inline IPlugInContext::~IPlugInContext() = default;

}  // namespace plugin
}  // namespace lc
