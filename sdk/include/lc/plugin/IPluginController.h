//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/db/db_fwd.h>
#include <lc/env/IEventLog.h>

namespace lc {
namespace plugin {

struct IPluginController
{
    virtual ~IPluginController() = 0;

    // Get log
    virtual env::IEventLog* log() const = 0;

    // Set current progress
    virtual void setProgress(int precentCompleted) = 0;

    //! Gets the drawing database
    //!
    //! \return  Current Cell; or nullptr if none
    virtual db::Drawing* drawing() = 0;

    //! Windows handle of main window
    virtual void* windowHandle() const = 0;
};

inline IPluginController::~IPluginController() = default;

}  // namespace plugin
}  // namespace lc
