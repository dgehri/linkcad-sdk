//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IPlugInContext.h"

namespace lc {
namespace plugin {

struct IPlugIn;

#define PLUGIN_API_VERSION 0x00100000
#define PLUGIN_API_MAJOR(v) (v & 0xfff00000)

//------------------------------------------------------------------------------
// Exported function to obtain IPlugIn interface
//
// Description:
//  The plug-in must export a function named "GetPlugIn" taking no argument
//  and returning a pointer to the plug-in's IPlugIn interface.
//------------------------------------------------------------------------------
typedef IPlugIn* (*GetPlugInFunc)(unsigned apiVersion);

//------------------------------------------------------------------------------
// Interface for plug-ins
//------------------------------------------------------------------------------
struct IPlugIn
{
    // Load plug-in
    virtual bool load(const IPlugInContext* context, void* module) = 0;

    // Unload plug-in
    virtual bool unload() = 0;

    virtual ~IPlugIn() = 0;
};

inline IPlugIn::~IPlugIn() = default;

//------------------------------------------------------------------------------
// Macros for writing plug-ins
//------------------------------------------------------------------------------
#define PLUGIN_API __declspec(dllexport)

// Implements the "GetPlugIn" function, returning a static instance of CLASS
#define DECLARE_PLUGIN(CLASS)                                                     \
    extern "C" PLUGIN_API lc::plugin::IPlugIn* GetPlugIn(unsigned apiVersion)     \
    {                                                                             \
        if (PLUGIN_API_MAJOR(apiVersion) == PLUGIN_API_MAJOR(PLUGIN_API_VERSION)) \
        {                                                                         \
            static CLASS thePlugIn_;                                              \
            return &thePlugIn_;                                                   \
        }                                                                         \
        else                                                                      \
            return 0;                                                             \
    }

}  // namespace plugin
}  // namespace lc
