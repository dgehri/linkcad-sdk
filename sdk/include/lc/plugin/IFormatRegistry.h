//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IWizardDialog.h"
#include <lc/lic/License.h>
#include <string>

namespace FX {
class FXComposite;
}

namespace lc::plugin {

struct IReaderPlugIn;
struct IWriterPlugIn;

//------------------------------------------------------------------------------
// Interface for registering readers and writers
//------------------------------------------------------------------------------
struct IFormatRegistry
{
    using PFN_CREATE_DIALOG = IWizardDialog* (*)(FX::FXComposite*, const std::string& formatName);

    // register an import format
    virtual void registerReaderPlugIn(IReaderPlugIn* plugIn,
                                      const char* displayName,
                                      const char* extensions,
                                      lic::License licenseLevel,
                                      const char* pluginName = nullptr) = 0;

    // register an export format
    virtual void registerWriterPlugIn(IWriterPlugIn* plugIn,
                                      const char* displayName,
                                      const char* extensions,
                                      lic::License licenseLevel,
                                      const char* pluginName = nullptr) = 0;

    // register an import format
    virtual void registerFormatReaderDialog(const char* name, PFN_CREATE_DIALOG createDialog) = 0;

    // register an export format
    virtual void registerFormatWriterDialog(const char* name, PFN_CREATE_DIALOG createDialog) = 0;

    virtual ~IFormatRegistry() = 0;
};

inline IFormatRegistry::~IFormatRegistry() = default;

}  // namespace lc::plugin
