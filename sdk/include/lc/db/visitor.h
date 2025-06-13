//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "dbdefs.h"
#include "dllexport.h"

namespace lc::db {

//------------------------------------------------------------------------------
// Visitor base class
//
// Description:
//      To implement a visitor, derive the visitor class from BaseVisitor.
//      Additionally, for each entity type to visit, derive from
//      IVisitor<EntityType> and implement the corresponding visit(EntityType&)
//      and optionally leave(EntityType&) member function.
//------------------------------------------------------------------------------
class DBAPI NOVTABLE BaseVisitor
{
public:
    virtual ~BaseVisitor() {}
};

//------------------------------------------------------------------------------
// Interface for visitors
//
// See: BaseVisitor
//------------------------------------------------------------------------------
template <class T>
struct NOVTABLE IVisitor
{
    // called by visited function
    virtual void visit(T*) = 0;

    // called after sub-entities have been visited
    virtual void leave(T*) {}
};

//------------------------------------------------------------------------------
// Vtbl interface for visitable classes
//------------------------------------------------------------------------------
struct IVisitableVtbl
{
protected:
    ~IVisitableVtbl() {}

public:
    // Accept visitor
    virtual void accept(Object* obj, BaseVisitor& guest) = 0;

    // Default implementation callable IVisitable::accept()
    template <class T>
    static inline void acceptImpl(Object* obj, BaseVisitor& guest)
    {
        if (IVisitor<T>* p = dynamic_cast<IVisitor<T>*>(&guest))
        {
            p->visit(static_cast<T*>(obj));
            p->leave(static_cast<T*>(obj));
        }
    }
};

// Convenience macro for default visitable implementation
#define IMPLEMENT_VISITABLE(T)                            \
    void accept(Object* obj, BaseVisitor& guest) override \
    {                                                     \
        return acceptImpl<T>(obj, guest);                 \
    }

}  // namespace lc::db
