//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "TlcWriter.h"
#include <lc/db/db.h>
#include <lc/env/IEventLog.h>
#include <lc/util/string_conv.h>
#include <fstream>
#include <string>
#include <filesystem>

namespace lc::format::tlcout {

namespace fs = std::filesystem;

//------------------------------------------------------------------------------
bool TlcWriter::writeFile(const std::filesystem::path& /*filePath*/,
                          plugin::IWriterController* controller)
{
    try
    {
        // init
        ctrl_ = controller;
        scaling_ = ONE_MICRON;
        ctrl_->initProgressCounter();

        cellName_ = conv::Properties::exportCellName(ctrl_->drawing());
        layerNumber_ = conv::Properties::exportLayerNumber(ctrl_->drawing());

        // Create directory
        auto outputDir = ctrl_->fileName();
        if (!fs::exists(outputDir) || !fs::is_directory(outputDir))
        {
            if (fs::exists(outputDir))
            {
                fs::remove(outputDir);
            }

            fs::create_directory(outputDir);
        }

        // write sub-cells
        ctrl_->startEnumCells();
        while (auto cell = ctrl_->nextCell())
        {
            if (!writeCell(cell))
                return false;
        }

        // write main cell
        if (!writeCell(ctrl_->mainCell()))
            return false;

        return true;
    }
    catch (const fs::filesystem_error& e)
    {
        ctrl_->log()->log(lc::env::Severity::Error,
                          std::format("Failed to write TLC file: {}", e.what()));
        return false;
    }
}

//------------------------------------------------------------------------------
bool TlcWriter::writeCell(const db::Cell* cell)
{
    auto cellFileName = fs::path(ctrl_->fileName()) / cell->propget(cellName_);
    cellFileName.replace_extension("tlc");

    os_.open(cellFileName, std::ios::binary);

    if (!os_.is_open())  // Check if file was successfully opened
    {
        ctrl_->log()->log(lc::env::Severity::Error,
                          std::format("Failed to open file '{}'", cellFileName.string()));
        std::remove(cellFileName.string().c_str());
        return false;
    }

    // File is open. All subsequent paths must ensure os_.close() is eventually called.
    // If any operation fails before a successful close, the file should be removed.
    try
    {
        auto bounds = cell->bounds();

        os_ << "=H\n" << cellFileName.stem().string() << "\n";
        os_ << "6.0\n6.0\n";
        os_ << 1 << "\num\n";
        os_ << "01/01/99\n00:00:00\n";
        os_ << cell->childLevels() + 1 << " " << bounds.minX() / scaling_ << " "
            << bounds.minY() / scaling_ << " " << bounds.maxX() / scaling_ << " "
            << bounds.maxY() / scaling_ << "\n";
        os_ << "0 0 0 0\n";  // TODO: number of boxes, etc...

        if (os_.fail())  // Check for stream errors after writing header
        {
            ctrl_->log()->log(
                lc::env::Severity::Error,
                std::format("Failed to write header for cell '{}'", cellFileName.string()));
            os_.close();  // Attempt to close the stream
            std::remove(cellFileName.string().c_str());  // Remove the problematic file
            return false;
        }

        ctrl_->renderCell(cell);  // This can throw or set os_.fail()

        if (os_.fail())  // Check for stream errors after rendering cell
        {
            ctrl_->log()->log(
                lc::env::Severity::Error,
                std::format("Stream error after rendering cell '{}'", cellFileName.string()));
            os_.close();  // Attempt to close the stream
            std::remove(cellFileName.string().c_str());  // Remove the problematic file
            return false;
        }

        // All writes and operations seem successful. Now, close the file.
        os_.close();

        if (os_.fail())  // Check if the close operation itself failed (e.g., error during flush)
        {
            ctrl_->log()->log(lc::env::Severity::Error,
                              std::format("Failed to close file '{}'", cellFileName.string()));
            std::remove(cellFileName.string().c_str());  // Remove the potentially corrupt file
            return false;
        }

        // If we reach here, all operations including close were successful.
        // This is equivalent to the original guard.commit() path.
        return true;
    }
    catch (...)
    {
        // An exception occurred during operations in the try block.
        // Ensure the stream is closed (if it was open and not already closed) and the file is
        // removed.
        if (os_.is_open())
        {
            os_.close();
        }
        std::remove(cellFileName.string().c_str());
        throw;  // Re-throw the exception to propagate it
    }
}

//------------------------------------------------------------------------------
bool TlcWriter::writeEntity(const db::Polygon* poly, geom::FillRule /*fillRule*/)
{
    if (!poly->isBox())
    {
        PointArray vertices;
        poly->vertices(vertices,
                       db::VertexMode::RemoveDuplicates | db::VertexMode::ForceDuplicateEnd);

        os_ << "=P\n"
            << poly->layer()->propget(layerNumber_) << " " << 0 << " " << vertices.size() << "\n";

        for (size_t i = 0; i < vertices.size(); ++i)
        {
            auto pt = scale(vertices[i]);
            os_ << pt.x << " " << pt.y << " ";

            // split in groups of five vertices per line
            if (i != 0 && i < vertices.size() - 1 && (i + 1) % 5 == 0)
            {
                os_ << "\n";
            }
        }
        os_ << "\n";
    }
    else
    {
        auto bounds = poly->bounds();
        auto pt0 = scale(bounds.minXY());
        auto pt1 = scale(bounds.maxXY());

        os_ << "=B\n"
            << poly->layer()->propget(layerNumber_) << " " << pt0.x << " " << pt0.y << " " << pt1.x
            << " " << pt1.y << "\n";
    }

    return true;
}

//------------------------------------------------------------------------------
bool TlcWriter::writeEntity(const db::Polyline* pline)
{
    PointArray vertices;
    pline->vertices(vertices, db::VertexMode::RemoveDuplicates | db::VertexMode::ForceDuplicateEnd);

    os_ << "=P\n"
        << pline->layer()->propget(layerNumber_) << " " << scale(pline->width()) << " "
        << vertices.size() << "\n";

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        auto pt = scale(vertices[i]);
        os_ << pt.x << " " << pt.y << " ";

        // split in groups of five vertices per line
        if (i != 0 && i < vertices.size() - 1 && (i + 1) % 5 == 0)
        {
            os_ << "\n";
        }
    }

    os_ << "\n";
    return true;
}

//------------------------------------------------------------------------------
bool TlcWriter::writeEntity(const db::Ref* ref, const db::Layer* /*layer*/)
{
    auto cellName = ref->refCell()->propget(cellName_);
    const auto xform = ref->transformation();

    if (fmod(xform.rotation().degrees(), 90.0) != 0.0)
    {
        ctrl_->log()->log(
            env::Severity::Warning,
            std::format("Ignored non-90 degree rotation in reference '{}'.", cellName));
    }

    if (xform.isRotationAbsolute())
    {
        ctrl_->log()->log(env::Severity::Warning,
                          std::format("Ignored absolute rotation in reference '{}'.", cellName));
    }

    if (xform.isScalingAbsolute())
    {
        ctrl_->log()->log(
            env::Severity::Warning,
            std::format("Ignored absolute magnification in reference '{}'.", cellName));
    }

    for (auto col = 0u; col < ref->columns(); ++col)
    {
        for (auto row = 0u; row < ref->rows(); ++row)
        {
            Vector offset(col * ref->columnSpacing(), row * ref->rowSpacing());
            auto transform(xform.applyTo(Xform(offset)));
            transform.canonicalize();

            os_ << "=C\n";
            os_ << std::filesystem::path(cellName).stem() << "\n";

            auto orientation = (static_cast<int>(xform.rotation().degrees()) + 45) / 90;

            if (transform.isMirroredInY())
            {
                orientation |= 4;
            }

            os_ << orientation << " " << transform.translation().x / scaling_ << " "
                << transform.translation().y / scaling_ << " 0\n";
        }
    }

    return true;
}

//------------------------------------------------------------------------------
Point TlcWriter::scale(const Point& pt) const
{
    return geom::round<Point>(ctrl_->transform(static_cast<geom::Point2d>(pt)) / scaling_);
}

//------------------------------------------------------------------------------
int TlcWriter::scale(dist d) const
{
    return util::round<int>(ctrl_->transformDistance(static_cast<double>(d)) / scaling_);
}


}  // namespace lc::format::tlcout
