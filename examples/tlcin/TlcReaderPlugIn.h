//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/plugin/IReaderPlugIn.h>

namespace lc::format::tlcin {

//------------------------------------------------------------------------------
// IPlugIn implementation
//------------------------------------------------------------------------------
class TlcReaderPlugIn : public lc::plugin::IPlugIn
{
public:
    // Load plug-in
    virtual bool load(const plugin::IPlugInContext* context, void* /* module */);

    // Unload plug-in
    virtual bool unload();

private:
    struct Reader : lc::plugin::IReaderPlugIn
    {
        // Describe the file format
        virtual void describeFormat(plugin::IFormat* format) const;

        // Configure file format
        virtual void configureFormat() const;

        // Create reader instance
        virtual lc::plugin::IReader* createInstance() const;
    };

    Reader reader_;
};

//------------------------------------------------------------------------------
// Declare plug-in
//------------------------------------------------------------------------------
DECLARE_PLUGIN(TlcReaderPlugIn);


}  // namespace lc::format::tlcin
