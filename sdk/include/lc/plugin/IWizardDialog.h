//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

namespace lc::plugin {

//------------------------------------------------------------------------------
// Interface for Wizard dialogs
//------------------------------------------------------------------------------
struct IWizardDialog
{
    virtual ~IWizardDialog() = 0;

    // Returns true if the next button is enabled
    virtual bool isNextEnabled() const = 0;

    // Returns true if the back button is enabled
    virtual bool isBackEnabled() const = 0;

    // Refresh the content
    virtual void refresh() = 0;
};

inline IWizardDialog::~IWizardDialog() = default;

}  // namespace lc::plugin
