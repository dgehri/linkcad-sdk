//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "TlcWriter.h"
#include <lc/lic/License.h>
#include <lc/plugin/IWriterPlugIn.h>
#include <lc/plugin/IFormat.h>

namespace lc::format::tlcout {

//------------------------------------------------------------------------------
// TLC Writer Plugin
//------------------------------------------------------------------------------
class TlcWriterPlugIn : public lc::plugin::IPlugIn
{
public:
    // Load the plugin
    bool load(const plugin::IPlugInContext* context, void* /* module */) override
    {
        auto registry = context->formatRegistry();
        registry->registerWriterPlugIn(&writer_, "LASI TLC", "*.tlc", lic::License::TlcLicense);
        return true;
    }

    // Unload the plugin
    bool unload() override { return true; }

private:
    // Writer factory implementation
    struct Writer : lc::plugin::IWriterPlugIn
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
            format->setFileNameExtension("tlc");
        }

        // Configure format settings (nothing to configure for TLC)
        void configureFormat() const override {}

        // Create a new writer instance
        plugin::IWriter* createInstance() const override { return new TlcWriter; }
    };

    Writer writer_;
};

//------------------------------------------------------------------------------
// Plugin registration
//------------------------------------------------------------------------------
DECLARE_PLUGIN(TlcWriterPlugIn);

}  // namespace lc::format::tlcout
