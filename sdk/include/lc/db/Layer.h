//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "DrawingObject.h"
#include "AutoPtr.h"
#include <lc/util/Color.h>
#include <string>

namespace lc::db {

class Property;
class Drawing;
class Cell;

//! Layer object
class DBAPI Layer final : public DrawingObject
{
    DECLARE_LCDB_OBJECT(ObjectType::Layer);

public:
    //! Lookup or create layer instance
    //!
    //! \param dwg          Drawing database
    //! \param layerName    Layer name to look up, eg. "0" for the null-layer
    static Layer* createInstance(Drawing* dwg, const std::string& layerName);

    //! Destroy object
    void destroy();

    //! Clone layer
    //!
    //! \param dwg          Target database for cloned layer (if nullptr, defaults
    //!                     to this object's drawing database)
    //! \param layerName    Target layer name
    //! \return             Cloned layer, or nullptr if the layer cannot be cloned
    //
    Layer* clone(Drawing* dwg, const std::string& layerName) const;

    //! Lookup layer instance
    //!
    //! \param dwg          Drawing database
    //! \param layerName    Layer name to look up, eg. "0" for the null-layer
    static Layer* lookup(const Drawing* dwg, const std::string& layerName);

    //! Get layer name
    const std::string& name() const;

    //! Change layer name
    //!
    //! \retval true    Success
    //! \retval false   Layer name already used by another layer
    bool setName(const std::string& layerName);

    //! Get layer color
    util::Color color() const;

    //! Set layer color
    void setColor(util::Color color);

    //! Enable / disable layer
    void enable(bool doEnable = true);

    //! Test if layer enabled
    bool enabled() const;

    //! Hide / unhide layer (won't be included in collection Drawing::Layers)
    void hide(bool doHide = true);

    //! Test if layer hidden
    bool hidden() const;

    //! Move layer before other layer
    //!
    //! \param otherLayer   Layer before which to move this layer; if nullptr, move
    //!                     to end of layer list (and *NOT* before null-layer!)
    void moveBefore(const Layer* otherLayer);

    //! Test if layer used by shapes inside any enabled cell
    //!
    //! \note To test if a layer is used by a cell (or its descendants), use
    //!        Cell::usesLayer().
    bool used() const;

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
    ~Layer();
};

//------------------------------------------------------------------------------
template <typename T>
typename T::ValueType Layer::propget(const T* prop) const
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    return prop ? prop->getFor(this) : typename T::ValueType();
}

//------------------------------------------------------------------------------
template <typename T>
void Layer::propset(T* prop, typename T::ArgumentType value)
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    if (!prop)
        throw std::invalid_argument("NULL property specified");

    prop->setFor(this, value);
}

}  // namespace lc::db
