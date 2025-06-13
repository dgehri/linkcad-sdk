//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Object.h"

namespace lc::db {

//------------------------------------------------------------------------------
// Base class for all objects inside a Drawing
//------------------------------------------------------------------------------
class DBAPI DrawingObject : public Object
{
    DECLARE_LCDB_OBJECT(ObjectType::DrawingObject);

protected:
    ~DrawingObject();
};

}  // namespace lc::db
