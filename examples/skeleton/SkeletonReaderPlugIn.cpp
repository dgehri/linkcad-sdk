//------------------------------------------------------------------------------
// Skeleton Reader Plugin - Example Import Plugin
//------------------------------------------------------------------------------
#include "SkeletonReader.h"
#include <lc/plugin/IReaderPlugIn.h>
#include <lc/plugin/IFormat.h>
#include <lc/plugin/IFormatRegistry.h>
#include <lc/plugin/IPlugInContext.h>

namespace lc::format::skeleton {

//------------------------------------------------------------------------------
// Plugin implementation
//------------------------------------------------------------------------------
class SkeletonReaderPlugIn : public plugin::IPlugIn
{
public:
    bool load(const plugin::IPlugInContext* context, void* /* module */) override
    {
        auto registry = context->formatRegistry();
        // Register with a dummy license - replace with actual license
        registry->registerReaderPlugIn(&reader_, "Skeleton Format", "*.skel",
                                       lc::lic::License::BasicLicense);
        return true;
    }

    bool unload() override { return true; }

private:
    struct Reader : plugin::IReaderPlugIn
    {
        void describeFormat(plugin::IFormat* format) const override
        {
            // Describe your format capabilities
            format->setAttributes(plugin::IFormat::NoAttribute);

            // Example: Support layer numbers 0-255
            format->setLayerNumberRange(0, 255);

            // Example: Cell names up to 64 characters
            format->setCellNameLength(64);

            // Example: Valid characters for cell names
            format->setValidCellChars("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
        }

        void configureFormat() const override
        {
            // Add configuration dialog if needed
        }

        plugin::IReader* createInstance() const override { return new SkeletonReader; }
    };

    Reader reader_;
};

//------------------------------------------------------------------------------
// Plugin registration
//------------------------------------------------------------------------------
DECLARE_PLUGIN(SkeletonReaderPlugIn);

}  // namespace lc::format::skeleton
