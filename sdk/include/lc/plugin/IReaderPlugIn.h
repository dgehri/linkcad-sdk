//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IPlugIn.h"
#include "IFormat.h"
#include "IReader.h"

namespace lc {
namespace plugin {

//------------------------------------------------------------------------------
// Interface for IReader factories
//------------------------------------------------------------------------------
struct IReaderPlugIn
{
    virtual ~IReaderPlugIn() = 0;

    // Describe the file format
    virtual void describeFormat(IFormat* format) const = 0;

    // Configure file format
    virtual void configureFormat() const = 0;

    // Create reader instance
    virtual IReader* createInstance() const = 0;
};

inline IReaderPlugIn::~IReaderPlugIn() = default;

}  // namespace plugin
}  // namespace lc
