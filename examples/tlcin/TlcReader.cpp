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
#include <istream>
#include <lc/lcunits.h>
#include <lc/util/lcmath.h>

namespace lc::format::tlcin {

using lc::geom::Angle;
using lc::util::round;
using namespace boost::algorithm;
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

    // add a global cell and parse the file
    ctrl_->openCell(filePath.string(), true);
    parseCell(filePath, fs::path());
    ctrl_->closeCell();

    return true;
}

//------------------------------------------------------------------------------
void TlcReader::parseCell(const fs::path& filePath, const fs::path& parentPath)
{
    try
    {
        double scaling = 1.0;

        std::ifstream file(filePath.string(), std::ios_base::in);

        // parse file
        while (file)
        {
            // read next record
            if (file.get() != '=')
            {
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            char record;
            file >> record;

            switch (record)
            {
            case 'L':  // Layer list
            {
                // Number of layer items<nl>
                int layers;
                file >> layers >> std::ws;

                for (int i = 0; i < layers; ++i)
                {
                    // Layer Name (8 characters max, 4 if CIF format is to be
                    // used)<sp>Layer Number (1 to 256)<nl>
                    std::string layerName;
                    int layer;

                    file >> layerName >> layer >> std::ws;
                    ctrl_->selectLayer(layer);
                    // TODO: TLC has both, layer names & layer numbers
                    //       => store both
                    ctrl_->setLayerComment(layerName);
                }
            }
            break;

            case 'H':  // Header Record
            {
                // Name of Cell (Windows file name)<nl>
                std::string cellName;
                file >> cellName >> std::ws;

                // Version of LASI (string)<nl>
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Version of TLC (string)<nl>
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Basic Units per Physical Unit<nl>
                double units;
                file >> units >> std::ws;

                // Name of Physical Unit (string)<nl>
                std::string unitsName;
                file >> unitsName >> std::ws;
                if (unitsName == "nm")
                {
                    scaling = ONE_NM / units;
                }
                else if (unitsName == "um")
                {
                    scaling = ONE_MICRON / units;
                }
                else if (unitsName == "mm")
                {
                    scaling = ONE_MM / units;
                }
                else if (unitsName == "cm")
                {
                    scaling = ONE_CM / units;
                }
                else
                {
                    scaling = ONE_MICRON / units;
                }

                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            break;

            case 'C':  // Include other cell
            {
                // Name of Lesser Cell<nl>
                std::string cellName;
                file >> cellName >> std::ws;

                ctrl_->createRef(to_upper_copy(cellName));

                if (!isIncluded(cellName))
                {
                    fs::path cellPath(filePath.parent_path() / cellName);
                    cellPath.replace_extension("TLC");
                    ctrl_->openCell(to_upper_copy(cellName));
                    parseCell(cellPath, filePath);  // parse sub-cell
                    ctrl_->closeCell();
                }

                // Orientation (see below)<sp>X Position<sp>Y Position<sp>reserved
                // (presently 0)<nl>
                unsigned long orientFlags, reserved;
                Point pos;
                file >> orientFlags >> pos.x >> pos.y >> reserved >> std::ws;

                if (orientFlags & 0x04)
                {
                    ctrl_->mirrorRefInY();
                }

                switch (orientFlags & 0x03)
                {
                case 1: ctrl_->rotateRef(Angle::piHalf); break;
                case 2: ctrl_->rotateRef(Angle::pi); break;
                case 3: ctrl_->rotateRef(Angle::threePiHalf); break;
                }

                ctrl_->translateRef(scale(pos, scaling));
            }
            break;

            case 'B':  // Box
            {
                // Layer of Box<sp>X of Left Side<sp>Y of Bottom Side<sp>X of Right
                // Side<sp>Y of Top Side<nl>
                int layer;
                Point bottomLeft, topRight;
                file >> layer >> bottomLeft.x >> bottomLeft.y >> topRight.x >> topRight.y >>
                    std::ws;
                ctrl_->selectLayer(layer);
                ctrl_->createRectangle(scale(bottomLeft, scaling), scale(topRight, scaling));
            }
            break;

            case 'P':  // Polygon
            {
                int layer;
                long width, vertexCount;

                // Layer of Path/Poly<sp>Width (in basic units)<sp>No. of Vertices in
                // path/poly<nl>
                file >> layer >> width >> vertexCount >> std::ws;
                ctrl_->selectLayer(layer);
                PointArray vertices;
                for (int i = 0; i < vertexCount; ++i)
                {
                    Point pt;
                    if (!(file >> pt.x >> pt.y))
                        break;

                    vertices.append(scale(pt, scaling));
                }

                if (width > 0)
                {
                    bool closed = (vertices.head() == vertices.tail());
                    ctrl_->createPolyline(round<dist>(width * scaling), vertices, closed,
                                          db::EndCap::SquareFlat);
                }
                else
                {
                    ctrl_->createPolygon(vertices);
                }
                file >> std::ws;
            }
            break;

            case 'T':  // Text
            {
                int layer;
                long height, vertexCount;
                unsigned long orientFlags;

                // Layer of Text<sp>Text Size<sp>No. of Vertices used by text (includes
                // ref point)<sp>Orientation (same as cells)<nl>
                file >> layer >> height >> vertexCount >> orientFlags >> std::ws;
                ctrl_->selectLayer(layer);

                // X of Ref Point<sp>Y of Ref Point<nl>
                Point pt;
                file >> pt.x >> pt.y >> std::ws;

                // Text string (u/l case, up to 64 characters)<nl>
                std::string str;
                getline(file, str);

                ctrl_->createText();
                ctrl_->setTextPosition(scale(pt, scaling));
                ctrl_->setTextHeight(height * scaling);

                if (orientFlags & 0x04)
                {
                    ctrl_->setTextMirroredInY();
                }

                switch (orientFlags & 0x03)
                {
                case 1: ctrl_->setTextRotation(Angle::piHalf); break;
                case 2: ctrl_->setTextRotation(Angle::pi); break;
                case 3: ctrl_->setTextRotation(Angle::threePiHalf); break;
                }

                ctrl_->setUnformattedText(str);
            }
            break;

            default: ASSERT(false); break;
            }
        }
    }
    catch (fs::filesystem_error&)
    {
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
Point TlcReader::scale(const Point& pt, double scaling)
{
    return geom::round<Point>(static_cast<geom::Point2d>(pt) * scaling);
}

//------------------------------------------------------------------------------
bool TlcReader::isIncluded(const std::string& name)
{
    // try inserting, return false if insert failed because name already in set
    return !cellNames_.insert(name).second;
}

}  // namespace lc::format::tlcin
