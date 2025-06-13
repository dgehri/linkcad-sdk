//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

// Shared library support
#ifndef SHPAPI
#ifdef SHP_EXPORTS
#ifndef SHP_STATIC
#define SHPAPI __declspec(dllexport)
#else
#define SHPAPI
#endif
#else
#ifndef SHP_STATIC
#define SHPAPI __declspec(dllimport)
#else
#define SHPAPI
#endif

#endif
#endif
