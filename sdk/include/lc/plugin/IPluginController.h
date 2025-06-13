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

namespace lc::plugin {

//------------------------------------------------------------------------------
// Base interface for plugin controllers
//
// This interface provides common functionality needed by all plugin controllers,
// including access to the drawing database, logging facilities, and progress
// reporting during long operations.
//------------------------------------------------------------------------------
struct IPluginController
{
    virtual ~IPluginController() = 0;

    //! Get the event log for recording messages, warnings, and errors
    //! \return Pointer to the event log interface (never null during normal operation)
    //! \note Use this to report errors, warnings, or informational messages to the user
    virtual env::IEventLog* log() const = 0;

    //! Update the progress indicator for long-running operations
    //! \param percentCompleted Progress value from 0 to 100
    //! \note Call this periodically during import/export to provide user feedback
    //! \note May trigger cancellation check - operations should handle exceptions
    virtual void setProgress(int percentCompleted) = 0;

    //! Get the drawing database being constructed or processed
    //! \return Pointer to the drawing database, or nullptr if not yet initialized
    //! \note For readers: initially null, created during import
    //! \note For writers: contains the drawing to be exported
    virtual db::Drawing* drawing() = 0;

    //! Get the native window handle of the main application window
    //! \return Platform-specific window handle (HWND on Windows, etc.)
    //! \note Can be used for displaying modal dialogs or other UI elements
    //! \note May be null in console/batch mode operations
    virtual void* windowHandle() const = 0;
};

inline IPluginController::~IPluginController() = default;

}  // namespace lc::plugin
