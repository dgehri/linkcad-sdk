//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/plugin/IReaderImpl.h>
#include <set>
#include <string>
#include <filesystem>

namespace lc::format::tlcin {

namespace fs = std::filesystem;

//------------------------------------------------------------------------------
// Class for reading TLC (LASI) files
//------------------------------------------------------------------------------
class TlcReader : public plugin::IReaderImpl
{
public:
    // Constructor
    TlcReader() = default;

    //! Prevent copying
    TlcReader(const TlcReader&) = delete;
    TlcReader& operator=(const TlcReader&) = delete;

protected:
    // Start parsing
    virtual bool parseFile(const std::filesystem::path& filePath,
                           plugin::IDrawingBuilder* ctrl,
                           std::streamsize fileSize,
                           int currentFile,
                           int fileCount);

private:
    // Parse a cell
    void parseCell(const fs::path& filePath, const fs::path& parentPath);

    // Check if cell already included
    bool isIncluded(const std::string& name);

    // Scale and round a vertex
    static Point scale(const Point&, double scaling);

private:
    // list of cell names
    using CellNames = std::set<std::string>;

    plugin::IDrawingBuilder* ctrl_;  // drawing builder
    CellNames cellNames_;
};

}  // namespace lc::format::tlcin
