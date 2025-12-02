/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  SmuPkgTypeFixups.c
 * @brief Package specific fixups for SMU
 *
 */

#include <SilCommon.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/Common/NbioPcieTopologyHelper.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include <SMU/SmuClass-api.h>
#include "SmuBrh.h"
#include "SmuInitBrh.h"

 /*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
static void
SmuFixupPlatformConfigSP5 (
  PPTable_t                  *PPTable,
  SMUCLASS_INPUT_BLK         *SmuInputBlock
  )
{
  uint8_t                    Value8;
  uint32_t                   Value32;
  uint32_t                   SilReserved;
  GNB_HANDLE                 *GnbHandle;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // TODO: SMU feature defaults for BRHD
  GnbHandle = GetGnbHandle ();
  if (GnbHandle == NULL) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "Failed to find GNB handle\n");
    return;
  }

  SilReserved = xUSLSmnRead(
                  GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  SIL_RESERVED_ADDR_0x3810A84);

  // Defaults for Emulation and SIMNOW
  if (((SilReserved & 0xF) == 1) || (SilReserved & BIT_32(17))) {
    SmuInputBlock->SmuFeatureControl = 0x18030000;
    SmuInputBlock->FllBtcEnable = false;
  }

  // DF Cstate
  Value8 = SmuInputBlock->DfCstateEnable;
  if (Value8 != 0xF) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "DfCstateEnable = %d\n", Value8);
    Value32 = SmuInputBlock->SmuFeatureControl;
    Value32 &= ~BIT_32(19);
    Value32 |= Value8 ? BIT_32(19) : 0;
    SmuInputBlock->SmuFeatureControl = Value32;
  }

  // CPPC
  Value8 = SmuInputBlock->CfgCPPCMode;
  if (Value8 != 0xF) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "CfgCPPCMode = %d\n", Value8);
    Value32 = SmuInputBlock->SmuFeatureControl;
    Value32 &= ~BIT_32(22);
    Value32 |= Value8 ? BIT_32(22) : 0;
    SmuInputBlock->SmuFeatureControl = Value32;
  }

  // GMI Folding
  Value8 = SmuInputBlock->AmdGmiFolding;
  if (Value8 != 0xF) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "AmdGmiFolding = %d\n", Value8);
    Value32 = SmuInputBlock->SmuFeatureControl;
    Value32 &= ~BIT_32(23);
    Value32 |= Value8 ? BIT_32(23) : 0;
    SmuInputBlock->SmuFeatureControl = Value32;
  }

  // PC6
  Value8 = SmuInputBlock->AmdPowerSupplyIdleControl;
  if (Value8 != 0xF) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "AmdPowerSupplyIdleControl = %d\n", Value8);
    Value32 = SmuInputBlock->SmuFeatureControl;
    Value32 &= ~BIT_32(18);
    Value32 |= Value8 ? BIT_32(18) : 0;
    SmuInputBlock->SmuFeatureControl = Value32;
  }

  // Diagnostic Mode
  Value8 = SmuInputBlock->CfgDiagnosticMode;
  if (Value8 != 0xFF) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "CfgDiagnosticMode = %d\n", Value8);
    Value32 = SmuInputBlock->SmuFeatureControlExt;
    Value32 &= ~BIT_32(1);
    Value32 |= Value8 ? BIT_32(1) : 0;
    SmuInputBlock->SmuFeatureControlExt = Value32;
  }

  SmuInputBlock->SocDcBtcEnable = false;

  SMU_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return;
}

/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 */
void
SmuFixupPlatformConfig (
  uint32_t                   PackageType,
  PPTable_t                  *PPTable,
  SMUCLASS_INPUT_BLK         *SmuInputBlock
  )
{
  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  // Enable default SMU Features based on package type
  SMU_TRACEPOINT (SIL_TRACE_INFO, "PackageType = %d\n", PackageType);
  SmuFixupPlatformConfigSP5(PPTable, SmuInputBlock);
  SMU_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

