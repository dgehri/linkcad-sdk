//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include "Shape.h"
#include "Cell.h"
#include <lc/shp/Style.h>
#include <lc/shp/IShapeRenderer.h>
#include <lc/geom/Xform.h>

namespace lc::db {

//! Text shape
//!
//! The following table shows all supported formatting codes.
//!
//! \note    Format codes are preceded with a single backslash, but Doxygen and
//!          C++ both require a double-backslash for escaping
//!
//! Format code     | Purpose
//! --------------- | ---------------------------------------------------
//! %%%             | Inserts a percent sign (%)
//! %%d             | Inserts the degree sign
//! %%p             | Inserts the plus-minus sign
//! %%c             | Inserts the diameter sign
//! %%o             | Start overline
//! %%u             | Start underline
//! \\\\            | Inserts a backslash
//! \\~             | Inserts a non-breaking space
//! \\P             | Line end
//! \\U+value;      | Unicode value (eg. "U+00B0")
//! \\O...\\o       | Turns overline on and off
//! \\L...\\l       | Turns underline on and off
//! {...}           | Nest formatting
//! \\{...\\}       | Inserts an opening and closing brace
//! \\Ffont;        | Changes to the specified font file (eg. "Fisocteur.shx;")
//! \\ffont|param;  | Changes to the specified font file
//! \\Hvalue;       | Changes to the text height specified in drawing units
//! \\Hvaluex;      | Changes the text height to a multiple of the current text height
//! \\Wvalue;       | Changes width factor to produce wide text
//! \\Qangle;       | Changes obliquing angle
//! \\Tvalue;       | Adjusts the space between characters, from .75 to 4 times
//! \\S...^...;     | Stacks the subsequent text at the , #, or ^ symbol (ignored)
//! \\Avalue;       | Sets the alignment value; valid values: 0, 1, 2 (bottom, center, top)
//! (ignored)
//! \\Cvalue;       | Changes to the specified color (ignored)
//!
class DBAPI Text : public Shape
{
    DECLARE_LCDB_OBJECT(ObjectType::Text);

public:
    //! Create a new Text shape
    //!
    //! \param   cell            Owning cell
    //! \param   layer           Owning layer
    //! \param   text            Text string with formatting codes (use Text::escape() to escape
    //! codes) \param   xform           Text transformation:
    //!                          - attachment point of entire text block (as
    //!                            defined by AlignmentH and AlignmentV in the
    //!                            'style' parameter);
    //!                          - mirroring in x and y;
    //!                          - rotation
    //! \param   fontStyle       Font style: specify the character height for this
    //!                          parameter to create a corresponding Style
    //!
    //! \param   textStyle       (optional) Combination of AlignmentH, AlignmentV,
    //!                              Orientation and LineStyle.
    //! \param   endCapStyle     (optional) EndCap::Round, EndCap::SquareExtended or
    //! EndCap::SquareFlat \param   lineSpacing     (optional) Spacing factor \param   strokeWidth
    //! (optional) Stroke width \param   strokeWidthRel  (optional) If stroke width is zero,
    //! calculate it
    //!                          as strokeWidthRel * character height
    //! \param   boxWidth        (optional) Width of text box (0 = unconstrained)
    //!
    //! \return  New Text shape instance
    static Text* createInstance(Cell* cell,
                                const Layer* layer,
                                const std::string& text,
                                const Xform& xform,
                                const shp::Style& fontStyle,
                                TextStyle textStyle = TextStyle::Default,
                                EndCap endCapStyle = EndCap::Round,
                                double lineSpacing = 1.0,
                                dist strokeWidth = 0,
                                double strokeWidthRel = 0.05,
                                dist boxWidth = 0);

    //! Clone this object
    //!
    //! \param   cell    (optional) [in,out] Owning cell for cloned object (if nullptr, use original
    //! cell) \param   xform   (optional) Transformation to apply to clone
    //!
    //! \return  Copy of this object; else nullptr
    Text* clone(Cell* cell = nullptr, const Xform& xform = Xform::identity) const;

    //! Set the text string, interpreting formatting codes (use Text::escape to store unformatted
    //! text)
    void setText(const std::string& str);

    //! Set initial font
    void setFont(const std::string& fontName);

    //! Set text style
    void setTextStyle(TextStyle style, TextStyleMask mask = TextStyleMask::None);

    //! Set the box width used for word wrap calculations as the maximum line width.
    //! a value of 0 disables word wrapping.
    void setBoxWidth(dist width);

    //! Set line spacing
    void setLineSpacing(double spacing);

    //! Set text attachment point
    void setPosition(const Point& pos);

    //! mirror text in x-direction
    void setMirroredInX(bool mirror = true);

    //! mirror text in y-direction
    void setMirroredInY(bool mirror = true);

    //! Set text rotation
    void setRotation(geom::Angle angleCcw, bool absolute = false);

    //! Set initial text height (used as a scaling factor for both height & width)
    void setHeight(double height);

    //! Set initial text width factor
    void setWidthFactor(double factor);

    //! Set initial oblique angle
    void setObliquingAngle(geom::Angle angleCcw);

    //! Set stroke width
    void setStrokeWidth(dist width);

    //! Set relative stroke width
    void setStrokeWidthRel(double width);

    //! Set end-cap style
    void setEndCapStyle(EndCap endCapStyle);

    //! Get text style
    TextStyle textStyle() const;

    //! Get box width
    coord boxWidth() const;

    //! Get line spacing
    double lineSpacing() const;

    //! Get text attachment point
    Point position() const;

    //! Test if mirrored in x-direction
    bool mirroredInX() const;

    //! Test if mirrored in y-direction
    bool mirroredInY() const;

    //! Get text rotation angle
    geom::Angle rotation() const;

    //! Get initial text height
    double height() const;

    //! Get initial text width factor
    double widthFactor() const;

    //! Get initial obliquing angle
    geom::Angle obliquingAngle() const;

    //!  Get width
    dist strokeWidth() const;

    //! Get relative stroke width
    double strokeWidthRel() const;

    //! Get end-cap style
    EndCap endCapStyle() const;

    //! Get last line's descender
    dist descentOfLastLine() const;

    //! Get full transformation
    Xform transformation() const;

    //! Get raw text with formatting codes
    std::string text() const;

    //! Get text as unformatted ASCII
    //!
    //! \param lineNumber   Requested text line (zero-based index)
    //! \param text         Pointer to std::string to receive text line; may be nullptr.
    //! \param anchorFlags  Specify requested alignment of anchorPos parameter;
    //!                     combination of AlignH* and AlignV*.
    //! \param anchorPos    Anchor location of text line; specify reference point using
    //!                     anchorFlags; may be nullptr.
    //! \param ignoreNonPrintable Remove all non-printable characters (ASCII not
    //!                     in range 0x20 - 0x7E)
    //! \return true if successful; false if invalid lineNumber specififed.
    bool unformattedText(uint32_t lineNumber,
                         std::string* text,
                         TextStyle anchorFlags,
                         Point* anchorPos,
                         bool ignoreNonPrintable = true) const;

    //! Get text font
    std::string font() const;

    //! Escape special characters in unformatted text string
    static std::string escape(const std::string& text);

    //! Render shape
    void render(shp::IShapeRenderer* renderer) const;

    //! Get width
    dist width() const { return strokeWidth(); }

    //! Check if closed
    bool closed() const { return true; }

    //! Get sample points
    void samplePoints(PointArray& vertices, const Resolution& res, VertexMode mode) const;

    //! Calculate enclosed area
    //!
    //! \return  Enclosed area
    double area() const;

    //! Compare to other shape
    //!
    //! \param   otherShape  Shape to compare to
    //! \param   ignoreSense (optional) If true, and applicable, compares two
    //!                      shapes as equivalent even if they differ in orientation
    //!                      (sense).
    //!
    //! \return  true if other shape is equivalent to this shape; false otherwise
    bool equivalentTo(const Shape* otherShape, bool ignoreSense = true) const;

    //! Draw shape using specified renderer object
    //!
    //! \param renderer    Renderer to use
    void draw(IRenderer* renderer) const;

    //! Apply transformation
    void applyTransformation(const Xform& xform);

    //! Get grip points for this cell object
    void gripPoints(SnapPoints& grips) const;

protected:
    ~Text();
};

}  // namespace lc::db
