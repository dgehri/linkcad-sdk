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
class Drawing;
class Cell;

//------------------------------------------------------------------------------
// Interface for objects wanting to receive Drawing object events
//------------------------------------------------------------------------------
struct NOVTABLE IDrawingEventListener
{
    virtual ~IDrawingEventListener() = 0;

    // The top cell has been changed
    virtual void onMainCellChanged(Drawing* drawing, Cell* oldCell) = 0;

    // A new Object has been added to the Drawing
    virtual void onObjectAdded(Drawing* drawing, Object* object) = 0;

    // An Object has been modified
    virtual void onObjectModified(Drawing* drawing, Object* object) = 0;

    // An Object is about to be deleted from memory
    virtual void onObjectDestroy(Drawing* drawing, const Object* object) = 0;
};

inline IDrawingEventListener::~IDrawingEventListener() = default;

//------------------------------------------------------------------------------
// Stub implementations for IDrawingEventListener interface
//------------------------------------------------------------------------------
class NOVTABLE DBAPI DrawingEventListener : public IDrawingEventListener
{
public:
    // The top cell has been changed
    void onMainCellChanged(Drawing* /*drawing*/, Cell* /*oldCell*/) override {}

    // A new Object has been added to the Drawing
    void onObjectAdded(Drawing* /*drawing*/, Object* /*object*/) override {}

    // An Object has been modified
    void onObjectModified(Drawing* /*drawing*/, Object* /*object*/) override {}

    // An Object is about to be deleted from memory
    void onObjectDestroy(Drawing* /*drawing*/, const Object* /*object*/) override {}

protected:
    DrawingEventListener() = default;
};

}  // namespace lc::db
