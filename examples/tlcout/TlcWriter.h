//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/plugin/IWriterImpl.h>
#include <lc/conv/Properties.h>
#include <fstream>

namespace lc::format::tlcout {

//------------------------------------------------------------------------------
// Class for writing TLC files
//------------------------------------------------------------------------------
class TlcWriter final : public plugin::IWriterImpl
{
public:
    // Constructor/Destructor
    TlcWriter() = default;
    ~TlcWriter() = default;

    // Prevent copying and moving
    TlcWriter(const TlcWriter&) = delete;
    TlcWriter& operator=(const TlcWriter&) = delete;
    TlcWriter(TlcWriter&&) = delete;
    TlcWriter& operator=(TlcWriter&&) = delete;

protected:
    // Main entry point - write the drawing to a file
    bool writeFile(const std::filesystem::path& filePath,
                   plugin::IWriterController* controller) override;

    // Write a polygon entity
    bool writeEntity(const db::Polygon* poly, geom::FillRule fillRule) override;

    // Write a polyline entity
    bool writeEntity(const db::Polyline* pline) override;

    // Write a reference entity
    bool writeEntity(const db::Ref* ref, const db::Layer* layer) override;

private:
    // Write a complete cell
    bool writeCell(const db::Cell* cell);

    // Scale a point from internal units to TLC units
    [[nodiscard]] Point scale(const Point& pt) const;

    // Scale a distance from internal units to TLC units
    [[nodiscard]] int scale(dist d) const;

private:
    plugin::IWriterController* ctrl_ = nullptr;  // writer controller interface
    std::ofstream os_;  // output file stream
    int scaling_ = 1;  // scaling factor
    conv::Properties::ExportCellName* cellName_ = nullptr;  // cell naming property
    conv::Properties::ExportLayerNumber* layerNumber_ = nullptr;  // layer numbering property
};

}  // namespace lc::format::tlcout
