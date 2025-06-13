//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Object.h"
#include "Collection.h"
#include "LayerCollection.h"
#include "Property.h"
#include "AutoPtr.h"
#include "visitor.h"
#include <string>

namespace lc::db {

class Cell;
class Layer;
struct IDrawingEventListener;

enum class LockType
{
    ReadAccess,
    WriteAccess,
    VolatileWriteAccess
};

//! Drawing class
class DBAPI Drawing final : public Object
{
    DECLARE_LCDB_OBJECT(ObjectType::Drawing);

public:
    //! Collections
    using Cells = Collection<Cell, Drawing>;
    using Layers = Collection<Layer, Drawing>;

    //! Use this function to create a new drawing
    static Drawing* createInstance(const std::string& name);

    //! Destroy object
    void destroy();

    //! Change drawing name
    void setName(const std::string& name);

    //! Get drawing name
    const std::string& name() const;

    //! Set the size of a database unit in meters (an exception is thrown is database is not empty)
    void setUnits(double unitsPerMeter);

    //! Get size of a database unit in meters.
    double units() const;

    //! Get cells collection
    Cells cells();
    const Cells cells() const;

    //! Get layer collection
    Layers layers();
    const Layers layers() const;

    //! Set the new main cell
    //!
    //! \note The drawing automatically clears the main cell if the cell
    //!       is destroyed.
    void setMainCell(const Cell* cell);

    //! Get current main cell
    Cell* mainCell() const;

    //! Set modification time
    void setModifTime(time_t mtime);

    //! Get modification time
    time_t modifTime() const;

    //! Set last access time
    void setAccessTime(time_t atime);

    //! Get last access time
    time_t accessTime() const;

    //! Enable / disable undo
    void enableUndo(bool enable = true);

    //! Check if undo enabled
    bool undoEnabled() const;

    //! Start undo marker
    void beginUndoMarker(unsigned int tag = 0);

    //! End undo marker
    void endUndoMarker();

    //! Test if action can be undone and return tag
    bool canUndo(unsigned int& tag) const;

    //! Undo up to last marker
    void undo();

    //! Test if action can be redone and return tag
    bool canRedo(unsigned int& tag) const;

    //! Redo up to previous marker
    void redo();

    //! Add new IDrawingEventListener
    void addListener(IDrawingEventListener* listener) const;

    //! Remove IDrawingEventListener
    void removeListener(IDrawingEventListener* listener) const;

    //! Add new IObjectEventListener
    using Object::addListener;

    //! Remove IObjectEventListener
    using Object::removeListener;

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

    //! Return memory consumption
    static size_t memoryUsage();

    //! Dump memory usage to std::clog
    static void dumpMemoryUsage();

    // Test if drawing database locked
    static bool locked(bool byAnyThread = true);

    // Lock database for read access
    template <LockType lockType>
    class DBAPI Lock final
    {
    public:
        // Acquire lock
        explicit Lock(bool waitForLock = true);

        // Releases lock, if not yet released
        ~Lock();

        // Move constructor
        Lock(Lock&& other) noexcept;

        // Test if lock was acquired
        bool acquired() const;

        // Release lock manually
        void release();

        // Re-acquire after releasing
        void reacquire(bool waitForLock = true);

        // Temporarily release lock
        class Unlock final
        {
        public:
            // Release lock
            explicit Unlock(Lock& lock)
                : lock_(lock)
            {
                lock_.release();
            }

            // Destructor
            ~Unlock() { lock_.reacquire(); }

            Unlock(const Unlock&) = delete;
            Unlock(const Unlock&&) = delete;
            Unlock& operator=(const Unlock&) = delete;
            Unlock& operator=(const Unlock&&) = delete;

        private:
            Lock& lock_;
        };

    protected:
        int token_;
    };

    using WriteLock = Lock<LockType::WriteAccess>;
    using VolatileWriteLock = Lock<LockType::VolatileWriteAccess>;
    using ReadLock = Lock<LockType::ReadAccess>;

    // Automatic transaction object
    class DBAPI Transaction
    {
    public:
        explicit Transaction(Drawing* dwg, unsigned int tag = 0);
        ~Transaction();

        // Commit transaction
        void commit();

    private:
        Drawing* dwg_;
        WriteLock lock_;
    };

protected:
    ~Drawing();
};

//------------------------------------------------------------------------------
template <typename T>
typename T::ValueType Drawing::propget(const T* prop) const
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    return prop ? prop->getFor(this) : typename T::ValueType();
}

//------------------------------------------------------------------------------
template <typename T>
void Drawing::propset(T* prop, typename T::ArgumentType value)
{
    static_assert(std::is_base_of<db::Property, T>::value, "T must be derived from db::Property");
    if (!prop)
        throw std::invalid_argument("NULL property specified");

    prop->setFor(this, value);
}

}  // namespace lc::db
