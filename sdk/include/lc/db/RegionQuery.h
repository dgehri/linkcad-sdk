//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dllexport.h"
#include "Cell.h"
#include "IRegionQuery.h"

namespace lc::db {

//! Enum specifying query window type
enum class QueryWindow
{
    Overlap,  //!< Returns objects overlapping query window
    Inside,  //!< Return objects enteriely inside query window
    All  //!< Return all objects, disregarding query window
};

//! Query all objects on enabled cells overlapping the query window
//!
//! \param client    Interface pointer for reporting found objects
//! \param type      `QueryType`
//! \param startCell Cell at which the region query starts
//! \param rect      Query window
//! \param minSize   Minimum bounding box width/height of object to be
//!                  returned by query
//! \param layer     Restrict query to objects on this layer only;
//!                  query all layers if `layer = nullptr`.
//! \param startLevel Start query at this level (`startCell` having level 0)
//! \param endLevel  End query at this level (inclusive)
//!
//! \retval true     Successfully completed
//! \retval false    User cancelled query
DBAPI bool windowQuery(IWindowQuery* client,
                       QueryWindow type,
                       const Cell* startCell,
                       const Bounds& rect,
                       dist minSize = 0,
                       const Layer* layer = nullptr,
                       unsigned int startLevel = 0,
                       unsigned int endLevel = std::numeric_limits<unsigned int>::max());

//! Return objects on enabled cells closest to specified point
//!
//! \param client    Interface pointer for reporting found objects
//! \param startCell Cell at which the region query starts
//! \param pt        Query point
//! \param maxDist   Max query distance
//! \param minSize   Minimum bounding box width/height of object to be
//!                  returned by query
//! \param layer     Restrict query to objects on this layer only;
//!                  query all layers if `layer = nullptr`.
//! \param startLevel Start query at this level (`startCell` having level 0)
//! \param endLevel  End query at this level (inclusive)
//!
//! \retval true     Successfully completed
//! \retval false    User cancelled query
DBAPI bool pointQuery(IPointQuery* client,
                      const Cell* startCell,
                      const Point& pt,
                      dist maxDist = std::numeric_limits<dist>::max(),
                      dist minSize = 0,
                      const Layer* layer = nullptr,
                      unsigned int startLevel = 0,
                      unsigned int endLevel = std::numeric_limits<unsigned int>::max());

}  // namespace lc::db
