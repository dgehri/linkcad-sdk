//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "TlcWriterPlugIn.h"
#include "TlcWriter.h"
#include <lc/lic/License.h>

namespace lc::format::tlcout {

//------------------------------------------------------------------------------
bool TlcWriterPlugIn::load(const plugin::IPlugInContext* context, void* /*module*/)
{
    auto registry = context->formatRegistry();
    registry->registerWriterPlugIn(&writer_, "LASI TLC", "*.tlc", lic::License::TlcLicense);
    return true;
}

//------------------------------------------------------------------------------
bool TlcWriterPlugIn::unload()
{
    return true;
}

//------------------------------------------------------------------------------
void TlcWriterPlugIn::Writer::describeFormat(plugin::IFormat* format) const
{
    using namespace plugin;

    format->setAttributes(IFormat::NoAttribute);

    // set of valid TLC characters
    static const char* lasiChars = "!#$%&-0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_{}~";

    format->setAttributes(IFormat::LayerNumbers | IFormat::CellFileNames);
    format->setLayerNumberRange(1, 256);
    format->setCellNameLength(32);
    format->setValidCellChars(lasiChars);
    format->setFileNameExtension("tlc");
}

//------------------------------------------------------------------------------
void TlcWriterPlugIn::Writer::configureFormat() const {}

//------------------------------------------------------------------------------
plugin::IWriter* TlcWriterPlugIn::Writer::createInstance() const
{
    return new TlcWriter;
}

}  // namespace lc::format::tlcout
