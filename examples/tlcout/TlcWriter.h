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
    // Constructor
    TlcWriter() = default;
    ~TlcWriter() = default;

    //! Prevent copying
    TlcWriter(const TlcWriter&) = delete;
    TlcWriter& operator=(const TlcWriter&) = delete;
    TlcWriter(TlcWriter&&) = delete;
    TlcWriter& operator=(TlcWriter&&) = delete;

protected:
    // Convert file
    bool writeFile(const std::filesystem::path& filePath,
                   plugin::IWriterController* controller) override;

    // Render polygon
    bool writeEntity(const db::Polygon* poly, geom::FillRule fillRule) override;

    // Render polyline
    bool writeEntity(const db::Polyline* pline) override;

    // Render reference
    bool writeEntity(const db::Ref* ref, const db::Layer* layer) override;

private:
    // Write cell
    bool writeCell(const db::Cell* cell);

    // Scale coordinate
    [[nodiscard]] Point scale(const Point& pt) const;

    // Scale width
    [[nodiscard]] int scale(dist d) const;

private:
    plugin::IWriterController* ctrl_{};
    std::ofstream os_;
    int scaling_{1};
    conv::Properties::ExportCellName* cellName_{};
    conv::Properties::ExportLayerNumber* layerNumber_{};
};

}  // namespace lc::format::tlcout
