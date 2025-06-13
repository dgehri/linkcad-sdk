//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "shp.h"
#include <string>

namespace lc {
namespace shp {

class Style;
class Tracer;

//------------------------------------------------------------------------------
// Interface for shape fonts
//------------------------------------------------------------------------------
struct SHPAPI IFont
{
    // Destructor
    virtual ~IFont() = default;

    // Get ascent (distance from the baseline to the top of the uppercase letters)
    [[nodiscard]] virtual int ascent() const = 0;

    // Get descent (distance below the baseline that usually corresponds to the
    // bottoms of the descenders [the ""tails" on glyphs such as "p" or "g")
    [[nodiscard]] virtual int descent() const = 0;

    // Render shape (convenience function)
    virtual void renderCharacter(wchar_t wchar, Tracer& tracer, const Style* style) const = 0;

    // Get font name (usually font file name without extension)
    [[nodiscard]] virtual std::string fontName() const = 0;

    // Get the font scaling factor
    [[nodiscard]] virtual double fontScaling() const = 0;
};

}  // namespace shp
}  // namespace lc
