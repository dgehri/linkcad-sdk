//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <filesystem>
#include <string_view>
#include <string>

namespace lc::util {

/**
 * \brief Convert a UTF-8 encoded string to the current system code page
 * \param wstr UTF-16 encoded string
 * \return String encoded using current code page
 */
std::string to_string(std::wstring_view wstr);

/**
 * \brief Convert a UTF-8 encoded string to the current system code page
 * \param utf8 UTF-8 encoded string
 * \return String encoded using current code page
 */
std::string to_string(std::u8string_view utf8);

/**
 * \brief Convert a UTF-8 encoded string to UTF-16
 * \param utf8 UTF-8 encoded string
 * \return UTF-16 encoded string
 */
std::wstring to_wstring(std::u8string_view utf8);

/**
 * \brief Convert string in current system code page to UTF-16
 * \param str String encoded using current code page
 * \return UTF-16 encoded string
 */
std::wstring to_wstring(std::string_view str);

/**
 * \brief Convert a UTF-16 encoded string to UTF-8
 * \param utf16 UTF-16 encoded string
 * \return UTF-8 encoded string
 */
std::u8string to_u8string(std::wstring_view utf16);

/**
 * \brief Convert a UTF-8 encoded string to UTF-16
 * \param utf8 UTF-8 encoded string
 * \param utf16 UTF-16 encoded string
 */
void utf8_to_utf16(wchar_t* utf16, std::u8string_view utf8);

std::filesystem::path to_path(std::wstring_view utf16);

/**
 * Cast std::string to UTF-8
 */
inline std::u8string as_u8string(std::string_view s)
{
    return std::u8string(s.begin(), s.end());
}

/**
 * \brief Cast UTF-8 to std::string w/o conversion
 */
std::string as_string(std::u8string_view utf8);

}  // namespace lc::util

#include "string_conv.inl"
