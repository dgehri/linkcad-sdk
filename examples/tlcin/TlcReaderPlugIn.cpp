//------------------------------------------------------------------------------
//
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "TlcReader.h"
#include <lc/lic/License.h>
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
bool TlcReaderPlugIn::load(const plugin::IPlugInContext* context, void* /*module*/)
{
    auto registry = context->formatRegistry();
    registry->registerReaderPlugIn(&reader_, "LASI TLC", "*.tlc", lic::License::TlcLicense);
    return true;
}

//------------------------------------------------------------------------------
bool TlcReaderPlugIn::unload()
{
    return true;
}

//------------------------------------------------------------------------------
void TlcReaderPlugIn::Reader::describeFormat(plugin::IFormat* format) const
{
    using namespace plugin;

    // set of valid TLC characters
    static const char* lasiChars = "!#$%&-0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_{}~";

    format->setAttributes(IFormat::LayerNumbers | IFormat::CellFileNames);
    format->setLayerNumberRange(1, 256);
    format->setCellNameLength(32);
    format->setValidCellChars(lasiChars);
}

//------------------------------------------------------------------------------
void TlcReaderPlugIn::Reader::configureFormat() const {}

//------------------------------------------------------------------------------
plugin::IReader* TlcReaderPlugIn::Reader::createInstance() const
{
    return new TlcReader;
}

//------------------------------------------------------------------------------
// Declare plug-in
//------------------------------------------------------------------------------
DECLARE_PLUGIN(TlcReaderPlugIn);

}  // namespace lc::format::tlcin
