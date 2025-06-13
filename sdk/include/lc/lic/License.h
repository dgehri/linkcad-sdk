//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <string>
#include <lc/util/enumclassops.h>

namespace lc::lic {


enum class License : uint32_t
{
    NoLicense = 0x00000000,

    LcBasicLicense = 0x00000001,
    LcGerberLicense = 0x00000002,
    LcViewLicense = 0x00000004,
    LcGdsTxtLicense = 0x00000008,
    LcToolsLicense = 0x00000010,
    LcAnsysLicense = 0x00000020,
    LcLegacyAcisLicense = 0x00000040,
    LcEditLicense = 0x00000080,
    LcSystemInterfaceLicense = 0x00000100,
    LcLegacyIe3dLicense = 0x00000200,
    LcModelerLicense = 0x00000400,

    CadxioBasicLicense = 0x00010000,
    CadxioDxfLicense = 0x00020000,
    CadxioOtImageLicense = 0x00040000,
    CadxioOtOdbppLicense = 0x00080000,
    CadxioOtEtchLicense = 0x00100000,
    CadxioStepLicense = 0x00200000,
    CadxioOtGbrLicense = 0x00400000,
    CadxioXtLicense = 0x00800000,

    // license mapping to features
    BasicLicense = LcBasicLicense | CadxioBasicLicense,
    GdsiiLicense = LcBasicLicense | CadxioBasicLicense,
    DxfLicense = LcBasicLicense | CadxioDxfLicense,
    RasterLicense = LcBasicLicense | CadxioOtImageLicense,
    PsLicense = LcBasicLicense,
    PdfLicense = LcBasicLicense,
    DbfLicense = LcSystemInterfaceLicense,
    SvgLicense = LcBasicLicense,
    TlcLicense = LcBasicLicense,
    LyLicense = LcBasicLicense,
    DwgLicense = LcBasicLicense,
    AsciiLicense = LcBasicLicense,
    CifLicense = LcBasicLicense,
    GerberLicense = LcGerberLicense | LcAnsysLicense,
    ViewLicense = LcViewLicense | CadxioBasicLicense,
    GdsTxtLicense = LcGdsTxtLicense | CadxioBasicLicense | LcAnsysLicense,
    ToolsLicense = LcToolsLicense | CadxioBasicLicense,
    EditLicense = LcEditLicense | CadxioBasicLicense,
    StepLicense = LcModelerLicense | CadxioStepLicense | LcAnsysLicense,
    BdfLicense = LcSystemInterfaceLicense,
    Ie3dLicense = LcSystemInterfaceLicense | LcLegacyIe3dLicense,
    SonLicense = LcSystemInterfaceLicense,
    ModlaLicense = LcSystemInterfaceLicense,
    AcisLicense = LcModelerLicense | LcAnsysLicense,
    AnfLicense = LcModelerLicense | LcAnsysLicense,
    XtLicense = LcModelerLicense | LcAnsysLicense | CadxioXtLicense,
    TetinLicense = LcModelerLicense | LcAnsysLicense,
    AnfV4License = LcModelerLicense | LcAnsysLicense,

#ifdef CADXIO
    FullLicense = CadxioBasicLicense | CadxioDxfLicense | CadxioOtImageLicense |
                  CadxioOtOdbppLicense | CadxioOtEtchLicense | CadxioStepLicense |
                  CadxioOtGbrLicense | CadxioXtLicense,
#else
    FullLicense = LcBasicLicense | LcGerberLicense | LcViewLicense | LcGdsTxtLicense |
                  LcToolsLicense | LcAnsysLicense | LcLegacyAcisLicense | LcEditLicense |
                  LcModelerLicense | LcSystemInterfaceLicense,
#endif
};

IMPL_ENUM_CLASS_OP_BITWISE_OR(License)
IMPL_ENUM_CLASS_OP_BITWISE_AND(License)

}  // namespace lc::lic
