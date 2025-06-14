//------------------------------------------------------------------------------
// Skeleton Reader - Example Import Plugin
//------------------------------------------------------------------------------
#pragma once

#include <lc/plugin/IReaderImpl.h>
#include <filesystem>

namespace lc::format::skeleton {

//------------------------------------------------------------------------------
// Reader for skeleton file format
//------------------------------------------------------------------------------
class SkeletonReader : public plugin::IReaderImpl
{
public:
    SkeletonReader() = default;
    ~SkeletonReader() = default;

    // Prevent copying
    SkeletonReader(const SkeletonReader&) = delete;
    SkeletonReader& operator=(const SkeletonReader&) = delete;
    SkeletonReader(SkeletonReader&&) = delete;
    SkeletonReader& operator=(SkeletonReader&&) = delete;

protected:
    // Main entry point for parsing
    bool parseFile(const std::filesystem::path& filePath,
                   plugin::IDrawingBuilder* ctrl,
                   std::streamsize fileSize,
                   int currentFile,
                   int fileCount) override;

private:
    plugin::IDrawingBuilder* ctrl_ = nullptr;
};

}  // namespace lc::format::skeleton
