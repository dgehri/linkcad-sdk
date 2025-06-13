//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <exception>

namespace lc::db {

//------------------------------------------------------------------------------
class Exception : public std::runtime_error
{
public:
    // Constructor
    explicit Exception(const std::string& message)
        : std::runtime_error(message)
    {}
};

//------------------------------------------------------------------------------
class DatabaseException : public Exception
{
public:
    // Constructor
    explicit DatabaseException(const std::string& message)
        : Exception(message)
    {}
};

//------------------------------------------------------------------------------
class DatabaseFatalError : public DatabaseException
{
public:
    // Constructor
    explicit DatabaseFatalError(const std::string& message)
        : DatabaseException(message)
    {}
};

//------------------------------------------------------------------------------
class DatabaseSoftError : public DatabaseException
{
public:
    // Constructor
    explicit DatabaseSoftError(const std::string& message)
        : DatabaseException(message)
    {}
};

}  // namespace lc::db
