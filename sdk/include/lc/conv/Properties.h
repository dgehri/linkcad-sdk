//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <lc/db/db.h>

namespace lc::conv {

//------------------------------------------------------------------------------
// Static struct for managing commonly used database properties
//------------------------------------------------------------------------------
struct Properties
{
    // Property flag for validated layers or cells
    template <class T>
    static db::BooleanProperty<T>* validatedName(db::Drawing* dwg, bool create = false);

    // Property flag for validated layers or cells
    template <class T>
    static db::BooleanProperty<T>* validatedNumber(db::Drawing* dwg, bool create = false);

    // Property for export cell name
    using ExportCellName = db::StringProperty<db::Cell>;
    static ExportCellName* exportCellName(db::Drawing* dwg, bool create = false);

    // Property for export cell number
    using ExportCellNumber = db::TypedIntegerProperty<db::Cell, int>;
    static ExportCellNumber* exportCellNumber(db::Drawing* dwg, bool create = false);

    // Property for alternative import cell name
    using AltImportCellName = db::StringProperty<db::Cell>;
    static AltImportCellName* altImportCellName(db::Drawing* dwg, bool create = false);

    // Property for flagging anonymous cells
    using IsAnonymousCell = db::BooleanProperty<db::Cell>;
    static IsAnonymousCell* isAnonymousCell(db::Drawing* dwg, bool create = false);

    // Property for export layer (major) number
    using ImportLayerNumber = db::TypedIntegerProperty<db::Layer, int>;
    static ImportLayerNumber* importLayerNumber(db::Drawing* dwg, bool create = false);

    // Property for export layer name
    using ExportLayerName = db::StringProperty<db::Layer>;
    static ExportLayerName* exportLayerName(db::Drawing* dwg, bool create = false);

    // Property for export layer (major) number
    using ExportLayerNumber = db::TypedIntegerProperty<db::Layer, int>;
    static ExportLayerNumber* exportLayerNumber(db::Drawing* dwg, bool create = false);

    // Property for export layer data type
    using ExportLayerDataType = db::TypedIntegerProperty<db::Layer, int>;
    static ExportLayerDataType* exportLayerDataType(db::Drawing* dwg, bool create = false);

    // Property for layer comments
    using LayerComment = db::StringProperty<db::Layer>;
    static LayerComment* layerComment(db::Drawing* dwg, bool create = false);

    // Property for storing layer elevation
    using LayerZ = db::TypedIntegerProperty<db::Layer, coord>;
    static LayerZ* layerZ(db::Drawing* dwg, bool create = false);

    // Property for storing layer thickness
    using LayerThickness = db::TypedIntegerProperty<db::Layer, dist>;
    static LayerThickness* layerThickness(db::Drawing* dwg, bool create = false);

    // Property for storing layer material
    using LayerMaterial = db::TypedIntegerProperty<db::Layer, int>;
    static LayerMaterial* layerMaterial(db::Drawing* dwg, bool create = false);

    // Property for storing layer polarity
    using LayerPolarityPos = db::BooleanProperty<db::Layer>;
    static LayerPolarityPos* layerPolarityPos(db::Drawing* dwg, bool create = false);

    // Property for forcing layer exporting
    using ForceLayerExport = db::BooleanProperty<db::Layer>;
    static ForceLayerExport* forceLayerExport(db::Drawing* dwg, bool create = false);

    // Property for stacking layer above layer below
    using LayerStack = db::BooleanProperty<db::Layer>;
    static LayerStack* layerStack(db::Drawing* dwg, bool create = false);

    // Property for extruding layer
    using ExtrudeLayer = db::BooleanProperty<db::Layer>;
    static ExtrudeLayer* extrudeLayer(db::Drawing* dwg, bool create = false);

    // Property for flashing polygons using apertures
    using FlashRectangles = db::BooleanProperty<db::Layer>;
    static FlashRectangles* flashRectangles(db::Drawing* dwg, bool create = false);

    // Property for flashing circles using apertures
    using FlashCircles = db::BooleanProperty<db::Layer>;
    static FlashCircles* flashCircles(db::Drawing* dwg, bool create = false);

    // Property for filling polygons
    using FillPolygons = db::BooleanProperty<db::Layer>;
    static FillPolygons* fillPolygons(db::Drawing* dwg, bool create = false);

    // Property for recording entity trace color (of type util::Color)
    using TraceColor = db::TypedIntegerProperty<db::CellObject, uint32_t>;
    static TraceColor* traceColor(db::Drawing* dwg, bool create = false);

    // Property for tagging entities as representing HATCHes
    using IsHatch = db::BooleanProperty<db::CellObject>;
    static IsHatch* isHatch(db::Drawing* dwg, bool create = false);

    // Property for layer (major) number when using GDS-TXT to communicate with ORTech code
    using OrTechGdsTxtLayerNumber = db::TypedIntegerProperty<db::Layer, int>;
    static OrTechGdsTxtLayerNumber* orTechGdsTxtLayerNumber(db::Drawing* dwg, bool create = false);

    // Property for layer (major) number when using GDS-TXT to communicate with ORTech code
    using OrTechGdsTxtCellName = db::StringProperty<db::Cell>;
    static OrTechGdsTxtCellName* orTechGdsTxtCellName(db::Drawing* dwg, bool create = false);
};

//------------------------------------------------------------------------------
template <>
inline db::BooleanProperty<db::Layer>* Properties::validatedName(db::Drawing* dwg,
                                                                 bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.layerValidatedName";

    if (create)
        return db::BooleanProperty<db::Layer>::createInstance(dwg, false, propertyName);
    else
        return db::BooleanProperty<db::Layer>::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
template <>
inline db::BooleanProperty<db::Cell>* Properties::validatedName(db::Drawing* dwg,
                                                                bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.cellValidatedName";

    if (create)
        return db::BooleanProperty<db::Cell>::createInstance(dwg, false, propertyName);
    else
        return db::BooleanProperty<db::Cell>::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
template <>
inline db::BooleanProperty<db::Layer>* Properties::validatedNumber(db::Drawing* dwg,
                                                                   bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.layerValidatedNumber";

    if (create)
        return db::BooleanProperty<db::Layer>::createInstance(dwg, false, propertyName);
    else
        return db::BooleanProperty<db::Layer>::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
template <>
inline db::BooleanProperty<db::Cell>* Properties::validatedNumber(db::Drawing* dwg,
                                                                  bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.cellValidatedNumber";

    if (create)
        return db::BooleanProperty<db::Cell>::createInstance(dwg, false, propertyName);
    else
        return db::BooleanProperty<db::Cell>::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportCellName* Properties::exportCellName(db::Drawing* dwg,
                                                              bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.exportCellName";

    if (create)
        return ExportCellName::createInstance(dwg, "", propertyName);
    else
        return ExportCellName::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::AltImportCellName* Properties::altImportCellName(db::Drawing* dwg,
                                                                    bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.altImportCellName";

    if (create)
        return ExportCellName::createInstance(dwg, "", propertyName);
    else
        return ExportCellName::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::IsAnonymousCell* Properties::isAnonymousCell(db::Drawing* dwg,
                                                                bool create /*= false*/)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.isAnonymousCell";

    if (create)
        return IsAnonymousCell::createInstance(dwg, false, propertyName);
    else
        return IsAnonymousCell::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportCellNumber* Properties::exportCellNumber(db::Drawing* dwg,
                                                                  bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.exportCellNumber";

    if (create)
        return ExportCellNumber::createInstance(dwg, 0, propertyName);
    else
        return ExportCellNumber::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportLayerNumber* Properties::importLayerNumber(db::Drawing* dwg,
                                                                    bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.importLayerNumber";

    if (create)
        return ExportLayerNumber::createInstance(dwg, 0, propertyName);
    else
        return ExportLayerNumber::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportLayerName* Properties::exportLayerName(db::Drawing* dwg,
                                                                bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.exportLayerName";

    if (create)
        return ExportLayerName::createInstance(dwg, "", propertyName);
    else
        return ExportLayerName::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportLayerNumber* Properties::exportLayerNumber(db::Drawing* dwg,
                                                                    bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.exportLayerNumber";

    if (create)
        return ExportLayerNumber::createInstance(dwg, 0, propertyName);
    else
        return ExportLayerNumber::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExportLayerDataType* Properties::exportLayerDataType(db::Drawing* dwg,
                                                                        bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.exportLayerDataType";

    if (create)
        return ExportLayerDataType::createInstance(dwg, 0, propertyName);
    else
        return ExportLayerDataType::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerComment* Properties::layerComment(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerComment";

    if (create)
        return LayerComment::createInstance(dwg, "", propertyName);
    else
        return LayerComment::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerZ* Properties::layerZ(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerZ";

    if (create)
        return LayerZ::createInstance(dwg, 0, propertyName);
    else
        return LayerZ::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerThickness* Properties::layerThickness(db::Drawing* dwg,
                                                              bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerThickness";

    if (create)
        return LayerZ::createInstance(dwg, 0, propertyName);
    else
        return LayerZ::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerMaterial* Properties::layerMaterial(db::Drawing* dwg,
                                                            bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerMaterial";

    if (create)
        return LayerMaterial::createInstance(dwg, 0, propertyName);
    else
        return LayerMaterial::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerPolarityPos* Properties::layerPolarityPos(db::Drawing* dwg,
                                                                  bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerPolarityPos";

    if (create)
        return LayerPolarityPos::createInstance(dwg, true, propertyName);
    else
        return LayerPolarityPos::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ForceLayerExport* Properties::forceLayerExport(db::Drawing* dwg,
                                                                  bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.forceLayerExport";

    if (create)
        return ForceLayerExport::createInstance(dwg, false, propertyName);
    else
        return ForceLayerExport::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::LayerStack* Properties::layerStack(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.layerStack";

    if (create)
        return LayerStack::createInstance(dwg, true, propertyName);
    else
        return LayerStack::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::ExtrudeLayer* Properties::extrudeLayer(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.extrudeLayer";

    if (create)
        return ExtrudeLayer::createInstance(dwg, false, propertyName);
    else
        return ExtrudeLayer::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::FlashRectangles* Properties::flashRectangles(db::Drawing* dwg,
                                                                bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.flashRectangles";

    if (create)
        return FlashRectangles::createInstance(dwg, false, propertyName);
    else
        return FlashRectangles::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::FlashCircles* Properties::flashCircles(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.flashCircles";

    if (create)
        return FlashCircles::createInstance(dwg, false, propertyName);
    else
        return FlashCircles::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::FillPolygons* Properties::fillPolygons(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.fillPolygons";

    if (create)
        return FillPolygons::createInstance(dwg, false, propertyName);
    else
        return FillPolygons::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::TraceColor* Properties::traceColor(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.traceColor";

    if (create)
        return TraceColor::createInstance(dwg, util::Color::Black.asRGBA(), propertyName);
    else
        return TraceColor::lookup(dwg, propertyName);
}

//------------------------------------------------------------------------------
inline Properties::IsHatch* Properties::isHatch(db::Drawing* dwg, bool create /*= false*/)
{
    static const char* propertyName = "lc.conv.isHatch";

    if (create)
        return IsHatch::createInstance(dwg, false, propertyName);
    else
        return IsHatch::lookup(dwg, propertyName);
}

inline Properties::OrTechGdsTxtLayerNumber* Properties::orTechGdsTxtLayerNumber(db::Drawing* dwg,
                                                                                bool create)
{
    static const char* propertyName = "lc.conv.orTechGdsTxtLayerNumber";

    if (create)
        return ExportLayerNumber::createInstance(dwg, 0, propertyName);
    else
        return ExportLayerNumber::lookup(dwg, propertyName);
}

inline Properties::OrTechGdsTxtCellName* Properties::orTechGdsTxtCellName(db::Drawing* dwg,
                                                                          bool create)
{
    // Name of property for export layer name
    static const char* propertyName = "lc.conv.orTechGdsTxtCellName";

    if (create)
        return ExportCellName::createInstance(dwg, "", propertyName);
    else
        return ExportCellName::lookup(dwg, propertyName);
}

}  // namespace lc::conv
