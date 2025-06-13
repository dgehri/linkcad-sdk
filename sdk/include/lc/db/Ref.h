//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "CellObject.h"
#include "Layer.h"
#include <lc/geom/Xform.h>

namespace lc::db {

class Cell;

//! Cell reference
class DBAPI Ref final : public CellObject
{
    DECLARE_LCDB_OBJECT(ObjectType::Ref);

public:
    //! Create a new cell reference
    //!
    //! \note    The cell reference doesn't necessarily have to point to a cell. The
    //!          referenced cell can also be specified later using setRefCell().
    static Ref* createInstance(Cell* cell,
                               const Layer* layer = nullptr,
                               const Xform& xform = Xform::identity,
                               const Cell* refCell = nullptr,
                               unsigned int cols = 1,
                               unsigned int rows = 1,
                               dist columnSpacing = 0,
                               dist rowSpacing = 0);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    VIRTUAL Ref* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set referenced cell
    void setRefCell(const Cell* refCell);

    //! Get referenced cell
    Cell* refCell() const;

    //! Set array columns
    void setColumns(unsigned int cols);

    //! Get columns
    unsigned int columns() const;

    //! Set array rows
    void setRows(unsigned int rows);

    //! Get rows
    unsigned int rows() const;

    //! Set column spacing
    void setColumnSpacing(dist columnSpacing);

    //! Get column spacing
    dist columnSpacing() const;

    //! Set row spacing
    void setRowSpacing(dist rowSpacing);

    //! Get row spacing
    dist rowSpacing() const;

    //! Set reference transformation (the transformation is applied to the entire array of
    //! references)
    void setTransformation(const Xform& xform);

    //! Get transformation (the transformation is applied to the entire array of references)
    Xform transformation() const;

    //! Draw shape using specified renderer object
    //!
    //! \param renderer    Renderer to use
    void draw(IRenderer* renderer) const;

    //! Apply transformation
    void applyTransformation(const Xform& xform);

    //! Get grip points for this cell object
    void gripPoints(SnapPoints& grips) const;

protected:
    ~Ref();
};

}  // namespace lc::db
