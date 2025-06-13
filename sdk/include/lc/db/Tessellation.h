//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Object.h"
#include "dbtypes.h"

namespace lc::db {

//! Tessellation object
class DBAPI Tessellation : public Object
{
    DECLARE_LCDB_OBJECT(ObjectType::Tessellation);

public:
    //! Type of primitives in parts
    enum PrimitiveType : uint8_t
    {
        Triangles,  //!< Series of separate triangles
        TriangleStrip,  //!< Strip of triangles sharing an edge
        TriangleFan  //!< Fan of triangle sharing an edge and a vertex
    };

    //! Storage requirements for indices in parts
    enum IndexType : uint8_t
    {
        Index8Bit = 1,
        Index16Bit = 2,
        Index32Bit = 4
    };

    //! Part information
    struct Part
    {
        PrimitiveType type;  //!< Primitive type used in part
        size_t count;  //!< Number of vertex indices in part
        IndexType indexType;  //!< Index type used to store vertex indices
    };

    //! Get the vertex array
    void vertices(PointArray& vertices) const;

    //! Get storage requirements for all indices
    size_t indexBufferSize() const;

    //! Get position of first partData
    bool firstPart(size_t& idx) const;

    //! Get position of next partData
    bool nextPart(size_t& idx) const;

    //! Get partData at position
    void partData(size_t idx, Part& primitive) const;

    //! Get indices of part
    //!
    //! \param idx       Part index
    //! \param indices   Memory buffer for storing the part vertex indices
    //! \param size      Size, in bytes, of `indices`
    void partIndices(size_t idx, void* indices, size_t size) const;

protected:
    //! Destructor
    ~Tessellation();
};

}  // namespace lc::db
