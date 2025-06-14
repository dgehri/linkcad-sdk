//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#include "TlcReader.h"
#include <boost/algorithm/string.hpp>
#include <filesystem>
#include <format>
#include <fstream>
#include <lc/lcunits.h>
#include <lc/util/lcmath.h>
#include <string_view>

namespace lc::format::tlcin {

using lc::geom::Angle;
using lc::util::round;
namespace fs = std::filesystem;

//------------------------------------------------------------------------------
bool TlcReader::parseFile(const std::filesystem::path& filePath,
                          plugin::IDrawingBuilder* ctrl,
                          std::streamsize /*fileSize*/,
                          int /*currentFile*/,
                          int /*fileCount*/)
{
    ctrl_ = ctrl;
    cellNames_.clear();

    // Create a top-level cell and parse the file
    ctrl_->openCell(filePath.string(), true);
    parseCell(filePath, fs::path{});
    ctrl_->closeCell();

    return true;
}

//------------------------------------------------------------------------------
void TlcReader::parseCell(const fs::path& filePath, const fs::path& parentPath)
{
    try
    {
        std::ifstream file(filePath);
        if (!file)
        {
            throw fs::filesystem_error("Cannot open file", filePath, std::error_code{});
        }

        double scaling = 1.0;

        // Parse TLC records
        while (file)
        {
            // TLC records start with '='
            if (file.get() != '=')
            {
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            char recordType;
            file >> recordType;

            switch (recordType)
            {
            case 'L':  // Layer list
            {
                int layerCount;
                file >> layerCount >> std::ws;

                for (int i = 0; i < layerCount; ++i)
                {
                    std::string layerName;
                    int layerNumber;
                    file >> layerName >> layerNumber >> std::ws;

                    ctrl_->selectLayer(layerNumber);
                    ctrl_->setLayerComment(layerName);
                }
                break;
            }

            case 'H':  // Header Record
            {
                std::string cellName;
                file >> cellName >> std::ws;

                // Skip LASI version
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Skip TLC version
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Read units
                double unitsPerPhysical;
                file >> unitsPerPhysical >> std::ws;

                // Read unit name and calculate scaling factor
                std::string unitName;
                file >> unitName >> std::ws;

                // Convert to internal units (nanometers)
                if (unitName == "nm")
                    scaling = ONE_NM / unitsPerPhysical;
                else if (unitName == "um")
                    scaling = ONE_MICRON / unitsPerPhysical;
                else if (unitName == "mm")
                    scaling = ONE_MM / unitsPerPhysical;
                else if (unitName == "cm")
                    scaling = ONE_CM / unitsPerPhysical;
                else
                    scaling = ONE_MICRON / unitsPerPhysical;  // default to microns

                // Skip remaining header lines
                for (int i = 0; i < 4; ++i)
                {
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                break;
            }

            case 'C':  // Cell reference
            {
                std::string cellName;
                file >> cellName >> std::ws;

                // Convert to uppercase for consistency
                auto upperCellName = boost::algorithm::to_upper_copy(cellName);
                ctrl_->createRef(upperCellName);

                // Parse sub-cell if not already included
                if (!isIncluded(cellName))
                {
                    auto cellPath = filePath.parent_path() / cellName;
                    cellPath.replace_extension("TLC");

                    ctrl_->openCell(upperCellName);
                    parseCell(cellPath, filePath);
                    ctrl_->closeCell();
                }

                // Read transformation
                unsigned long orientFlags, reserved;
                Point position;
                file >> orientFlags >> position.x >> position.y >> reserved >> std::ws;

                // Apply mirroring
                if (orientFlags & 0x04)
                {
                    ctrl_->mirrorRefInY();
                }

                // Apply rotation
                switch (orientFlags & 0x03)
                {
                case 1: ctrl_->rotateRef(Angle::piHalf); break;
                case 2: ctrl_->rotateRef(Angle::pi); break;
                case 3: ctrl_->rotateRef(Angle::threePiHalf); break;
                }

                // Apply translation
                ctrl_->translateRef(scale(position, scaling));
                break;
            }

            case 'B':  // Box (Rectangle)
            {
                int layer;
                Point bottomLeft, topRight;
                file >> layer >> bottomLeft.x >> bottomLeft.y >> topRight.x >> topRight.y >>
                    std::ws;

                ctrl_->selectLayer(layer);
                ctrl_->createRectangle(scale(bottomLeft, scaling), scale(topRight, scaling));
                break;
            }

            case 'P':  // Path or Polygon
            {
                int layer;
                long width, vertexCount;
                file >> layer >> width >> vertexCount >> std::ws;

                ctrl_->selectLayer(layer);

                // Read vertices
                PointArray vertices;
                for (long i = 0; i < vertexCount; ++i)
                {
                    Point pt;
                    if (!(file >> pt.x >> pt.y))
                        break;
                    vertices.append(scale(pt, scaling));
                }

                if (width > 0)
                {
                    // Create path with width
                    bool closed = (vertices.head() == vertices.tail());
                    ctrl_->createPolyline(round<dist>(width * scaling), vertices, closed,
                                          db::EndCap::SquareFlat);
                }
                else
                {
                    // Create polygon (width = 0)
                    ctrl_->createPolygon(vertices);
                }
                file >> std::ws;
                break;
            }

            case 'T':  // Text
            {
                int layer;
                long height, vertexCount;
                unsigned long orientFlags;
                file >> layer >> height >> vertexCount >> orientFlags >> std::ws;

                ctrl_->selectLayer(layer);

                // Read reference point
                Point position;
                file >> position.x >> position.y >> std::ws;

                // Read text string
                std::string text;
                std::getline(file, text);

                // Create and configure text
                ctrl_->createText();
                ctrl_->setTextPosition(scale(position, scaling));
                ctrl_->setTextHeight(height * scaling);

                // Apply mirroring
                if (orientFlags & 0x04)
                {
                    ctrl_->setTextMirroredInY();
                }

                // Apply rotation
                switch (orientFlags & 0x03)
                {
                case 1: ctrl_->setTextRotation(Angle::piHalf); break;
                case 2: ctrl_->setTextRotation(Angle::pi); break;
                case 3: ctrl_->setTextRotation(Angle::threePiHalf); break;
                }

                ctrl_->setUnformattedText(text);
                break;
            }

            default:
                ASSERT(false);  // Unknown record type
                break;
            }
        }
    }
    catch (const fs::filesystem_error&)
    {
        // Log error with context
        if (parentPath.empty())
        {
            ctrl_->log()->log(lc::env::Severity::Error,
                              std::format("Cannot open cell file '{}'.", filePath.string()));
        }
        else
        {
            ctrl_->log()->log(lc::env::Severity::Error,
                              std::format("Cannot open cell file '{}' included by '{}'",
                                          filePath.string(), parentPath.string()));
        }
    }
}

//------------------------------------------------------------------------------
Point TlcReader::scale(const Point& pt, double scaling) const
{
    return geom::round<Point>(static_cast<geom::Point2d>(pt) * scaling);
}

//------------------------------------------------------------------------------
bool TlcReader::isIncluded(const std::string& name)
{
    // Try to insert the name. If it already exists, insert returns false
    // for the second element of the pair, indicating it was already included
    return !cellNames_.insert(name).second;
}

}  // namespace lc::format::tlcin
