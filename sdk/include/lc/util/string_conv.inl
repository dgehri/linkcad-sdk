//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "string_conv.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace lc::util {

inline std::string to_string(std::wstring_view wstr)
{
    auto len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.length()), nullptr,
                                   0, nullptr, nullptr);

    std::string strTo(len, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.length()), strTo.data(), len,
                        nullptr, nullptr);
    return strTo;
}

inline std::string to_string(std::u8string_view utf8)
{
    return to_string(to_wstring(utf8));
}

inline std::string as_string(std::u8string_view utf8)
{
    return std::string(utf8.cbegin(), utf8.cend());
}

inline std::wstring to_wstring(const std::u8string_view utf8)
{
    if (utf8.empty())
        return {};

    auto len = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&utf8[0]),
                                   static_cast<int>(utf8.size()), nullptr, 0);

    std::wstring wstrTo(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&utf8[0]),
                        static_cast<int>(utf8.size()), wstrTo.data(), len);
    return wstrTo;
}

inline std::wstring to_wstring(std::string_view str)
{
    if (str.empty())
        return {};

    auto len = MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<const char*>(&str[0]),
                                   static_cast<int>(str.size()), nullptr, 0);

    std::wstring wstrTo(len, 0);
    MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<const char*>(&str[0]),
                        static_cast<int>(str.size()), wstrTo.data(), len);
    return wstrTo;
}

inline void utf8_to_utf16(wchar_t* utf16, const std::u8string_view utf8)
{
    auto str = to_wstring(utf8);
    std::wcscpy(utf16, str.c_str());
}

inline std::filesystem::path to_path(std::wstring_view utf16)
{
    return to_u8string(utf16);
}

inline std::u8string to_u8string(std::wstring_view utf16)
{
    if (utf16.empty())
        return {};

    auto len = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()),
                                   nullptr, 0, nullptr, nullptr);

    std::u8string strTo(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()),
                        reinterpret_cast<char*>(strTo.data()), len, nullptr, nullptr);
    return strTo;
}

}  // namespace lc::util
