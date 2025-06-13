//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IReader.h"

namespace lc::plugin {

//! Default implementation of IReader interface
class IReaderImpl : public IReader
{
public:
    // Post-process after parsing
    bool postProcess(Phase, db::Drawing*, env::IEventLog*, const Resolution&) override
    {
        return true;
    }

    // Destroy reader
    void destroy() override { delete this; }
};

}  // namespace lc::plugin
