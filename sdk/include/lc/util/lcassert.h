//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#undef ASSERT
#undef TRACE
#undef TRACE1
#undef TRACE2
#undef TRACE3
#undef TRACE4
#undef UNREACHABLE

// define ASSERT and UNREACHABLE
#ifdef __GNUC__

#include <assert.h>
#define ASSERT(x) assert(x)
#define ASSUME(x) assert(x)

#else

#include <crtdbg.h>  // use windows' _ASSERTE instead of FOX'
#define ASSERT _ASSERTE
#define TRACE _RPTF0
#define TRACE1 _RPTF1
#define TRACE2 _RPTF2
#define TRACE3 _RPTF3
#define TRACE4 _RPTF4

#define TRACE_WARN _CRT_WARN
#define TRACE_ERROR _CRT_ERROR

#define INIT_TRACE                                          \
    {                                                       \
        _CrtSetReportMode(TRACE_WARN, _CRTDBG_MODE_DEBUG);  \
        _CrtSetReportMode(TRACE_ERROR, _CRTDBG_MODE_DEBUG); \
    }

#if defined(_DEBUG)

#define UNREACHABLE_RETURN(ret) \
    ASSERT(false);              \
    return ret;

#ifdef __clang__
#define UNREACHABLE \
    ASSERT(false);  \
    __builtin_unreachable()
#else
#define UNREACHABLE ASSERT(false);
#endif

#define ASSUME(x) ASSERT(x)

#elif defined(PROFILE)

#define UNREACHABLE_RETURN(ret) \
    ASSERT(false);              \
    return ret;

#ifdef __clang__
#define UNREACHABLE \
    ASSERT(false);  \
    __builtin_unreachable()
#else
#define UNREACHABLE ASSERT(false);
#endif

#define ASSUME(x) __assume(x)

#else

#define UNREACHABLE_RETURN(ret) __assume(0);
#define UNREACHABLE __assume(0)
#define ASSUME(x) __assume(x)

#endif

#endif

#define FAIL_RETURN(ret) \
    {                    \
        ASSERT(false);   \
        return (ret);    \
    }
#define FAIL(expr)     \
    {                  \
        ASSERT(false); \
        expr;          \
    }

#define ASSERT_SILENT()            \
    _CrtSetReportMode(_CRT_ASSERT, \
                      _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_REPORT_MODE) & ~_CRTDBG_MODE_WNDW);
#define ASSERT_WINDOW()            \
    _CrtSetReportMode(_CRT_ASSERT, \
                      _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_REPORT_MODE) | _CRTDBG_MODE_WNDW);

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define WARNING_MSG(msg) message(__FILE__ "(" STRING(__LINE__) ") : warning: "##msg)
#define ERROR_MSG(msg) message(__FILE__ "(" STRING(__LINE__) ") : error: "##msg)
