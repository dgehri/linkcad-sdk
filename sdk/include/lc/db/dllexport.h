//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

//------------------------------------------------------------------------------
// Shared library support
//------------------------------------------------------------------------------
#ifndef DBAPI
#ifdef DB_EXPORTS
#ifndef DB_STATIC
#define DBAPI __declspec(dllexport)
#else
#define DBAPI
#endif
#else
#ifndef DB_STATIC
#define DBAPI __declspec(dllimport)
#else
#define DBAPI
#endif
#endif
#endif
