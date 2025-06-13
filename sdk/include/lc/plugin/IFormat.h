//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

namespace lc::plugin {

#define OpenEnded (-1)
#define AsciiCharSet (0)
#define SameAsCharSet (0)

//------------------------------------------------------------------------------
// Interface for describing a file format
//------------------------------------------------------------------------------
struct IFormat {
  // Attribute flags
  enum FormatAttributes {
    NoAttribute = 0x00000000,           // no attribute
    LayerNumbers = 0x00000001,          // layers have numbers
    LayerNames = 0x00000002,            // layers have names
    LayerFileNames = 0x00000200,        // each layer has own file
    LayerComments = 0x00000004,         // layers have comments
    LayerColors = 0x00000008,           // layers have colors
    LayerElevation = 0x00000010,        // layers have elevation (z)
    LayerFlashedRects = 0x00000020,     // flash squares on layer
    LayerFlashedCircles = 0x00000040,   // flash circles on layer
    LayerFilledPolygons = 0x00000080,   // fill polygons on layer
    LayerOutlinedPolygons = 0x00000100, // outline polygons on layer
    CellNumbers = 0x00000400,           // cells have numbers
    CellNames = 0x00000800,             // cells have names
    CellFileNames = 0x00001000,         // each cell has own file
    CellNamesIgnoreCase = 0x00002000,   // cells names are case insensitive
    LayerComponents = 0x00004000,       // layers have names
    LayerMaterials = 0x00008000,        // material attributes
    LayerThicknesses = 0x00010000,      // layer has thickness
    LayerDataTypes =
        0x00020000, // layer number includes data type in bits 8 - 15
    LayerExtruded = 0x00040000, // extrude layer to next higher layer
    LayerStack = 0x00080000, // layers may be stacked to set elevation (implies
                             // thickness and elevation)
    MultipleFiles =
        0x00100000, // allow multiple files to be merged during import
    WriteUnresolvedRefs =
        0x00200000, // format capable of writing unresolved refs
    SupportBulgePolygons =
        0x00400000,             // format capable of writing bulge polygons
    LayerPolarity = 0x00800000, // format uses layer polarity
    SinglePass = 0x01100000, // allow multiple files to be merged during import
                             // in a single pass
  };

  // Set format attributes
  virtual void setAttributes(unsigned int attributes) = 0;

  // Set layer number range
  virtual void setLayerNumberRange(int min, int max = OpenEnded) = 0;

  // Set layer name length
  virtual void setLayerNameLength(int length) = 0;

  // Set valid layer characters
  virtual void setValidLayerChars(const char *charSet,
                                  const char *initialChars = SameAsCharSet,
                                  const char *preferredPrefix = nullptr) = 0;

  // Set cell number range
  virtual void setCellNumberRange(int min, int max = OpenEnded) = 0;

  // Set cell name length
  virtual void setCellNameLength(int length) = 0;

  // Set valid cell characters
  virtual void setValidCellChars(const char *charSet,
                                 const char *initialChars = SameAsCharSet,
                                 const char *preferredPrefix = nullptr) = 0;

  // Set extension for cell name based file names
  virtual void setFileNameExtension(const char *ext) = 0;

  virtual ~IFormat() = 0;
};

inline IFormat::~IFormat() = default;

} // namespace lc::plugin
