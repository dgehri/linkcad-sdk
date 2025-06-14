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
#include <lc/plugin/IFormat.h>

namespace lc::format::tlcin {

//------------------------------------------------------------------------------
// TLC Reader Plugin
//------------------------------------------------------------------------------
class TlcReaderPlugIn : public lc::plugin::IPlugIn
{
public:
    // Load the plugin
    bool load(const plugin::IPlugInContext* context, void* /* module */) override
    {
        auto registry = context->formatRegistry();
        registry->registerReaderPlugIn(&reader_, "LASI TLC", "*.tlc", lic::License::TlcLicense);
        return true;
    }

    // Unload the plugin
    bool unload() override { return true; }

private:
    // Reader factory implementation
    struct Reader : lc::plugin::IReaderPlugIn
    {
        // Describe the TLC file format capabilities
        void describeFormat(plugin::IFormat* format) const override
        {
            // Valid characters for TLC cell names
            static constexpr const char* VALID_CHARS =
                "!#$%&-0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_{}~";

            format->setAttributes(plugin::IFormat::LayerNumbers | plugin::IFormat::CellFileNames);
            format->setLayerNumberRange(1, 256);
            format->setCellNameLength(32);
            format->setValidCellChars(VALID_CHARS);
        }

        // Configure format settings (nothing to configure for TLC)
        void configureFormat() const override {}

        // Create a new reader instance
        plugin::IReader* createInstance() const override { return new TlcReader; }
    };

    Reader reader_;
};

//------------------------------------------------------------------------------
// Plugin registration
//------------------------------------------------------------------------------
DECLARE_PLUGIN(TlcReaderPlugIn);

}  // namespace lc::format::tlcin
