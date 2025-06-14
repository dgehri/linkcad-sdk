//------------------------------------------------------------------------------
// Skeleton Reader - Example Import Plugin
//------------------------------------------------------------------------------
#include "SkeletonReader.h"
#include <fstream>
#include <format>

namespace lc::format::skeleton {

bool SkeletonReader::parseFile(const std::filesystem::path& filePath,
                               plugin::IDrawingBuilder* ctrl,
                               std::streamsize fileSize,
                               [[maybe_unused]] int currentFile,
                               [[maybe_unused]] int fileCount)
{
    ctrl_ = ctrl;

    try
    {
        auto file = std::ifstream(filePath, std::ios::binary);
        if (!file)
        {
            ctrl_->log()->log(env::Severity::Error,
                              std::format("Cannot open file '{}'", filePath.string()));
            return false;
        }

        // TODO: Add your file parsing logic here
        // Example structure:

        // 1. Read file header
        // 2. Parse entities
        // 3. Build geometry using ctrl_ interface

        // Example: Create a simple rectangle on layer 1
        ctrl_->selectLayer(1);
        ctrl_->createRectangle(Point{0, 0}, Point{1000, 1000});

        // Report progress (optional)
        if (fileSize > 0)
        {
            auto position = file.tellg();
            ctrl_->setProgress(position * 100 / fileSize);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        ctrl_->log()->log(env::Severity::Error, std::format("Error reading file: {}", e.what()));
        return false;
    }
}

}  // namespace lc::format::skeleton
