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

//------------------------------------------------------------------------------
// Class for reading TLC (LASI) files
//------------------------------------------------------------------------------
class TlcReader : public plugin::IReaderImpl
{
public:
    // Constructor
    TlcReader() = default;

    // Prevent copying and moving
    TlcReader(const TlcReader&) = delete;
    TlcReader& operator=(const TlcReader&) = delete;
    TlcReader(TlcReader&&) = delete;
    TlcReader& operator=(TlcReader&&) = delete;

protected:
    // Start parsing
    bool parseFile(const std::filesystem::path& filePath,
                   plugin::IDrawingBuilder* ctrl,
                   std::streamsize fileSize,
                   int currentFile,
                   int fileCount) override;

private:
    // Parse a single cell file
    void parseCell(const std::filesystem::path& filePath, const std::filesystem::path& parentPath);

    // Check if cell has already been included
    bool isIncluded(const std::string& name);

    // Scale and round a point
    Point scale(const Point& pt, double scaling) const;

private:
    plugin::IDrawingBuilder* ctrl_ = nullptr;  // drawing builder interface
    std::set<std::string> cellNames_;  // track included cell names to avoid duplicates
};

}  // namespace lc::format::tlcin
