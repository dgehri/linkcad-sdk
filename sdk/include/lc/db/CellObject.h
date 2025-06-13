//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "DrawingObject.h"
#include "Layer.h"
#include "IRenderer.h"
#include "SnapPoint.h"
#include "visitor.h"
#include <lc/geom/Xform.h>

namespace lc::db {

class Property;
class Cell;

//! Base class for all objects inside a cell
class DBAPI CellObject : public DrawingObject
{
    DECLARE_LCDB_OBJECT(ObjectType::CellObject);

public:
    using SnapPoints = std::vector<SnapPoint>;

    //! Destroy object
    void destroy();

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    VIRTUAL CellObject* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Applies a transformation to the cell object
    //!
    //! \param   xform   The transformation
    VIRTUAL void applyTransformation(const Xform& xform);

    //! Accept visitor
    VIRTUAL void accept(BaseVisitor& guest);

    //! Get grip points for this cell object
    VIRTUAL void gripPoints(SnapPoints& grips) const;

    //! Draw shape using specified renderer object
    //!
    //! \param renderer    Renderer to use
    VIRTUAL void draw(IRenderer* renderer) const;

    //! Get cell containing this CellObject
    Cell* owningCell() const;

    //! Get CellObject bounds
    const Bounds bounds() const;

    //! Set layer
    void setLayer(const Layer* layer);

    //! Set layer by name; creates layer if it doesn't exist yet
    void setLayerByName(const std::string& layerName);

    //! Get layer
    Layer* layer() const;

    //! Get layer name
    const std::string& layerName() const;

    //! Select entity
    void select(bool doSelect = true);

    //! Test if selected
    bool selected() const;

    //! Toggle selection
    void toggleSelection();

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
    //! Destructor
    ~CellObject();

    // Set up drawing traits
    void setCommonTraits(IRenderer* renderer) const;
};

//------------------------------------------------------------------------------
template <typename T>
typename T::ValueType CellObject::propget(const T* prop) const
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    return prop ? prop->getFor(this) : typename T::ValueType();
}

//------------------------------------------------------------------------------
template <typename T>
void CellObject::propset(T* prop, typename T::ArgumentType value)
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    if (!prop)
        throw std::invalid_argument("NULL property specified");

    prop->setFor(this, value);
}

}  // namespace lc::db
