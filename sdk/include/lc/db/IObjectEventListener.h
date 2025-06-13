//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"

namespace lc::db {

class Object;

//------------------------------------------------------------------------------
// Interface for objects wanting to receive Object events
//------------------------------------------------------------------------------
struct NOVTABLE IObjectEventListener
{
    virtual ~IObjectEventListener() = 0;

    // Source object is about to be modified
    virtual void onModify(const Object* obj) = 0;

    // Source object has been modified (only sent if not destroyed)
    virtual void onModified(Object* obj) = 0;

    // Source object is about to be deleted from memory
    virtual void onDestroy(const Object* obj) = 0;
};

inline IObjectEventListener::~IObjectEventListener() = default;

//------------------------------------------------------------------------------
// Stub implementations for IDrawingEventListener interface
//------------------------------------------------------------------------------
class NOVTABLE DBAPI ObjectEventListener : public IObjectEventListener
{
public:
    // Source object is about to be modified
    void onModify(const Object* /*obj*/) override {}

    // Source object has been modified
    void onModified(Object* /*obj*/) override {}

    // Source object is about to be deleted from memory
    void onDestroy(const Object* /*obj*/) override {}

protected:
    ObjectEventListener() = default;
};

}  // namespace lc::db
