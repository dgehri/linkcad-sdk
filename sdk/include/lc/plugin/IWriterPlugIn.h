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
#include "IWriter.h"

namespace lc {
namespace plugin {

//------------------------------------------------------------------------------
// Interface for IWriter factories
//------------------------------------------------------------------------------
struct IWriterPlugIn
{
    virtual ~IWriterPlugIn() = 0;

    // Describe the file format
    virtual void describeFormat(IFormat* format) const = 0;

    // Configure file format
    virtual void configureFormat() const = 0;

    // Create reader instance
    [[nodiscard]] virtual IWriter* createInstance() const = 0;
};

inline IWriterPlugIn::~IWriterPlugIn() = default;

}  // namespace plugin
}  // namespace lc
