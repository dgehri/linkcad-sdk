//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "shp.h"
#include <lc/geom/Angle.h>
#include <string>

namespace lc {
namespace shp {

struct IFont;

//------------------------------------------------------------------------------
// Font style
//------------------------------------------------------------------------------
class SHPAPI Style
{
public:
    // Constructor
    Style(double scaling = 1.0);

    // Reset style
    void clear();

    // Set a new font
    void setFont(const std::string& fontName);

    // Set font scaling
    void setScaling(double scaling);

    // Scale font relative
    void scale(double scaling);

    // Set width scaling
    void setWidthScaling(double scaling);

    // Set horizontal spacing (TODO: not implemented)
    void setHorizontalSpacing(double spacing);

    // Set obliquing angle
    void setObliquingAngle(geom::Angle angleCcw);

    // Enable / disable underlining
    void underline(bool enable = true);

    // Enable / disable overlining
    void overline(bool enable = true);

    // Get shape file
    std::string font() const;

    // Get scaling factor
    double scaling() const;

    // Get width scaling
    double widthScaling() const;

    // Get horizontal spacing
    double horizontalSpacing() const;

    // Get obliquing angle
    geom::Angle obliquingAngle() const;

    // Return true if underline enabled
    bool isUnderlined() const;

    // Return true if overline enabled
    bool isOverlined() const;

    // Comparison
    bool operator==(const Style& rhs) const;
    bool operator!=(const Style& rhs) const;

private:
#pragma warning(push)
#pragma warning(disable : 4251)  // fontName_ needs DLL interface to be used by clients
    std::string fontName_;
    double scaling_;
    double widthScaling_;
    double horizontalSpacing_;
    geom::Angle obliquingAngle_;
    bool underline_;
    bool overline_;
#pragma warning(pop)
};

}  // namespace shp
}  // namespace lc
