//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/plugin/IWriterPlugIn.h>

namespace lc::format::tlcout {

//------------------------------------------------------------------------------
// IPlugIn implementation
//------------------------------------------------------------------------------
class TlcWriterPlugIn : public lc::plugin::IPlugIn
{
public:
    // Load plug-in
    virtual bool load(const plugin::IPlugInContext* context, void* /* module */);

    // Unload plug-in
    virtual bool unload();

private:
    struct Writer : lc::plugin::IWriterPlugIn
    {
        // Describe the file format
        virtual void describeFormat(plugin::IFormat* format) const;

        // Configure file format
        virtual void configureFormat() const;

        // Create writer instance
        virtual lc::plugin::IWriter* createInstance() const;
    };

    Writer writer_;
};

//------------------------------------------------------------------------------
// Declare plug-in
//------------------------------------------------------------------------------
DECLARE_PLUGIN(TlcWriterPlugIn);


}  // namespace lc::format::tlcout
