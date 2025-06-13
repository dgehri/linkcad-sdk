//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <format>
#include <string_view>

namespace lc::env {

enum class Severity
{
    Success,
    Informational,  // informative messages, no impact on the conversion
    Warning,  // warning messages
    Error  // error messages
};

struct IEventLog
{
    // Add a log message
    virtual void log(Severity level, std::string_view msg) = 0;

    virtual ~IEventLog() = default;
};

}  // namespace lc::env
