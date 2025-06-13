//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "DrawingObject.h"
#include "CellObjectCollection.h"
#include "CellRefCollection.h"
#include "AutoPtr.h"
#include "visitor.h"

namespace lc::db {

class Property;
class Drawing;
class CellObject;
class Shape;
class Arc;
class Donut;
class Ellipse;
class Nurbs;
class Polygon;
class Polyline;
class Text;
class Ref;
class Layer;

//! Cell class
class DBAPI Cell final : public DrawingObject
{
    DECLARE_LCDB_OBJECT(ObjectType::Cell);

public:
    // Collections
    using CellObjects = Collection<CellObject, Cell>;
    using Shapes = Collection<Shape, Cell>;
    using Arcs = Collection<Arc, Cell>;
    using Donuts = Collection<Donut, Cell>;
    using Ellipses = Collection<Ellipse, Cell>;
    using NurbsCollection = Collection<Nurbs, Cell>;
    using Polygons = Collection<Polygon, Cell>;
    using Polylines = Collection<Polyline, Cell>;
    using Texts = Collection<Text, Cell>;
    using Refs = Collection<Ref, Cell>;
    using CellRefs = Collection<Ref, CellRefTag>;

    //! Lookup or create cell instance
    static Cell* createInstance(Drawing* dwg, const std::string& cellName);

    //! Lookup cell instance
    static Cell* lookup(const Drawing* dwg, const std::string& cellName);

    //! Clone cell, including all contained cell objects
    Cell* clone(const std::string& cellName, const Xform& xform = Xform::identity) const;

    //! Get cell name
    const std::string& name() const;

    //! Change cell name
    //!
    //! \return false if cell name already in use by another cell.
    bool setName(const std::string& cellName);

    //! Set modification time
    void setModifTime(time_t mtime);

    //! Get modification time
    time_t modifTime() const;

    //! Set last access time
    void setAccessTime(time_t atime);

    //! Get last access time
    time_t accessTime() const;

    //! Get number of selected CellObjects
    size_t selectedCount() const;

    //! Get first selected CellObject
    CellObject* firstSelected() const;

    //! Get all contained objects
    CellObjects cellObjects(bool selectedOnly = false);
    const CellObjects cellObjects(bool selectedOnly = false) const;

    //! Get cell objects of specified type
    CellObjects cellObjects(db::ObjectType type, bool selectedOnly = false);
    const CellObjects cellObjects(db::ObjectType type, bool selectedOnly = false) const;

    //! Get cell objects of specified type
    //!
    //! To iterate over a specific object type, use this function like this:
    //!
    //!     Cell::Donuts donuts = cell->cellObjects<Donut>();
    //!     for (Cell::Donuts::Iterator donut = donuts.begin(); donut != donuts.end(); ++donut)
    //!     {
    //!         donut->doSomething(...);
    //!     }
    template <class T>
    Collection<T, Cell> cellObjects(bool selectedOnly = false);

    //! Get cell objects of specified type
    //!
    //! To iterate over a specific object type, use this function like this:
    //!
    //!     Cell::Donuts donuts = cell->cellObjects<Donut>();
    //!     for (Cell::Donuts::Iterator donut = donuts.begin(); donut != donuts.end(); ++donut)
    //!     {
    //!         donut->doSomething(...);
    //!     }
    template <class T>
    const Collection<T, Cell> cellObjects(bool selectedOnly = false) const;

    //! Get cell shape collection
    Shapes shapes(bool selectedOnly = false);
    const Shapes shapes(bool selectedOnly = false) const;

    //! Get collection of references to this cell (not *inside* this cell!)
    CellRefs cellRefs();
    const CellRefs cellRefs() const;

    //! Enable / disable this cell
    void enable(bool enableCell = true,
                const Cell* contextCell = nullptr,
                CellContext context = CellContext::Descend);

    //! Check if cell enabled when used within cell 'contextCell';
    bool enabled(const Cell* contextCell) const;

    //! Check if cell enabled in any context
    bool enabled() const;

    //! Check if cell (or, if `context = CellContextDescend`, any of its child
    //! cells) contains cell objects on specified layer
    //!
    //! \param layer    Layer to check
    //! \param context  Specifies if only the current cell or also child cells
    //!                 are checked
    //! \param enabledOnly  [optional] If true, only enabled cells (including
    //!                 `this`cell) are checked
    //! \return         True if cell (or, if `context = CellContextDescend`, any
    //!                 of its child cells) contains cell objects on specified
    //!                 layer
    bool usesLayer(const db::Layer* layer, CellContext context, bool enabledOnly = false) const;

    //! Get first layer used by a contained shape
    Layer* firstShapeLayer() const;

    //! Get this cell's highest nesting level (aka "rank"), starting at 0 for a top cell
    unsigned int nestingLevel() const;

    //! Return the number of child levels of this cell (0 means no children)
    unsigned int childLevels() const;

    //! Return bounding box of shapes in cell and sub-cells
    Bounds bounds(const db::Layer* layer = nullptr) const;

    //! Accept visitor
    void accept(BaseVisitor& guest);

    //! Return property value
    template <typename T>
    typename T::ValueType propget(const T* prop) const;

    //! Return property value
    template <typename T>
    typename T::ValueType propget(const AutoPtr<T>& prop) const
    {
        return propget(prop.get());
    }

    //! Set property value
    template <typename T>
    void propset(T* prop, typename T::ArgumentType value);

    //! Set property value
    template <typename T>
    void propset(const AutoPtr<T>& prop, typename T::ArgumentType value)
    {
        propset(prop.get(), value);
    }

protected:
    ~Cell();
};

//------------------------------------------------------------------------------
template <class T>
Collection<T, Cell> Cell::cellObjects(bool selectedOnly /* = false */)
{
    return Collection<T, Cell>(this, T::staticType(), T::staticType(), selectedOnly);
}

//------------------------------------------------------------------------------
template <class T>
const Collection<T, Cell> Cell::cellObjects(bool selectedOnly /* = false */) const
{
    return Collection<T, Cell>(this, T::staticType(), T::staticType(), selectedOnly);
}

//------------------------------------------------------------------------------
template <>
inline Collection<CellObject, Cell> Cell::cellObjects<CellObject>(bool selectedOnly /* = false */)
{
    return Collection<CellObject, Cell>(this, ObjectType::_CellObjectFirst,
                                        ObjectType::_CellObjectLast, selectedOnly);
}

//------------------------------------------------------------------------------
template <>
inline const Collection<CellObject, Cell> Cell::cellObjects<CellObject>(
    bool selectedOnly /* = false */) const
{
    return Collection<CellObject, Cell>(this, ObjectType::_CellObjectFirst,
                                        ObjectType::_CellObjectLast, selectedOnly);
}

//------------------------------------------------------------------------------
template <>
inline Collection<Shape, Cell> Cell::cellObjects<Shape>(bool selectedOnly /* = false */)
{
    return Collection<Shape, Cell>(this, ObjectType::_ShapeFirst, ObjectType::_ShapeLast,
                                   selectedOnly);
}

//------------------------------------------------------------------------------
template <>
inline const Collection<Shape, Cell> Cell::cellObjects<Shape>(bool selectedOnly /* = false */) const
{
    return Collection<Shape, Cell>(this, ObjectType::_ShapeFirst, ObjectType::_ShapeLast,
                                   selectedOnly);
}

//------------------------------------------------------------------------------
template <typename T>
typename T::ValueType Cell::propget(const T* prop) const
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    return prop ? prop->getFor(this) : typename T::ValueType();
}

//------------------------------------------------------------------------------
template <typename T>
void Cell::propset(T* prop, typename T::ArgumentType value)
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    if (!prop)
        throw std::invalid_argument("NULL property specified");

    prop->setFor(this, value);
}

}  // namespace lc::db
