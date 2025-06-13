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
#include <lc/shp/IFont.h>
#include <string>
#include <filesystem>

namespace lc::db {

//! Global font repository
class DBAPI FontManager final
{
public:
    //! Return the one-and-only FontManager object
    static FontManager& instance();

    //! Re-read settings and clear cache
    void reset();

    //! Append font search path
    void appendFontPath(const std::filesystem::path& fontFolder);

    //! Set default font name (default is "TXT")
    void setDefaultFontName(const std::string& name);

    //! Set font map path (default is "lcad.fmp" in program directory)
    void setFontMapPath(const std::filesystem::path& mapPath);

    //! Get default font name
    std::string defaultFontName() const;

    //! Return font
    const shp::IFont* lookupFont(const std::string& fontName,
                                 bool acceptDefault = true,
                                 double scaling = 1.0);

    //! Return the default font
    const shp::IFont* defaultFont(double scaling = 1.0, const char* storeAs = "TXT");

    enum Error
    {
        FontNotFound,
        FontLoadingError
    };

    //! Get next font error
    //!
    //! \todo evaluate font errors
    bool nextFontError(std::string& fontName, Error& error) const;

private:
    FontManager();
    FontManager(const FontManager&);
    ~FontManager();
    FontManager& operator=(const FontManager&);

    struct Impl;
    Impl* pimpl_;
};

}  // namespace lc::db
