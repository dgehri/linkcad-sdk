//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "IDrawingBuilder.h"
#include <filesystem>
#include <lc/db/db_fwd.h>

namespace lc {
namespace plugin {

enum class Phase
{
    ParsedFile,
    ParsedAll,
    ClosedOpenCells,
    ResolvedRefs,
    SelectedMainCell,
    ResolvedLayersByBlock,
    AutoNumberedZ
};

//! Interface for format readers
struct IReader
{
    // Parse the file
    virtual bool parseFile(const std::filesystem::path& filePath,
                           IDrawingBuilder* ctrl,
                           std::streamsize fileSize,
                           int currentFile,
                           int fileCount) = 0;

    // Post-process after parsing
    virtual bool postProcess(Phase phase,
                             db::Drawing* dwg,
                             env::IEventLog* log,
                             const Resolution& res) = 0;

    // Destroy reader
    virtual void destroy() = 0;

    virtual ~IReader() = default;
};

}  // namespace plugin
}  // namespace lc
