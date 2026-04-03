/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuInitPhx.c
 * @brief PHX SMU core silicon initialization code.
 *
 */

#include <SilCommon.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/SmuClass-api.h>
#include <Utils.h>
#include <CommonLib/SmnAccess.h>
#include <CpuLib.h>
#include <string.h>
#include <ProjSocConst.h>
#include "SmuPhxReg.h"
#include "SmuPhxIp2Ip.h"
#include "SmuInitPhx.h"
#include "SmuPhx.h"

// automatic fan policy table
FAN_POLICY_TABLE  DefaultPhx1AM5FanTable[] = {
  // 35W TDP
  {
    0x230000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      35,  // FanTable_TempMed
      94,  // FanTable_TempHigh
      100, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      5, // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 45W TDP
  {
    0x2D0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      35,  // FanTable_TempMed
      95,  // FanTable_TempHigh
      100, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      6,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 65W TDP
  {
    0x410000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      35,  // FanTable_TempMed
      95,  // FanTable_TempHigh
      100, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      38,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // null - end of table
  {
    0x0,
    {0}
  },
};

// automatic fan policy table
FAN_POLICY_TABLE  DefaultPhx2AM5FanTable[] = {
  // 35W TDP
  {
    0x230000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      35,  // FanTable_TempMed
      94,  // FanTable_TempHigh
      100, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      5, // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 45W TDP
  {
    0x2D0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      94,  // FanTable_TempMed
      95,  // FanTable_TempHigh
      100, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      10,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 65W TDP
  {
    0x410000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      5,   // FanTable_Hysteresis
      0,  // FanTable_TempLow
      35,  // FanTable_TempMed
      113,  // FanTable_TempHigh
      114, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      0,  // FanTable_PwmMed
      41,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // null - end of table
  {
    0x0,
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
  },
};

FAN_POLICY_TABLE  DefaultFP7FP8FanTable[] = {
  // 15W TDP
  {
    0xF0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      66,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      8,   // FanTable_PwmMed
      10,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 28W TDP
  {
    0x1C0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      76,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      8,   // FanTable_PwmMed
      13,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 30W TDP
  {
    0x1E0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      77,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      8,   // FanTable_PwmMed
      14,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 35W TDP
  {
    0x230000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      81,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      10,  // FanTable_PwmMed
      18,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 45W TDP
  {
    0x2D0000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      72,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      10,  // FanTable_PwmMed
      38,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 54W TDP
  {
    0x360000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      79,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      10,  // FanTable_PwmMed
      45,  // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // 20W TDP
  {
    0x140000,
    { 0,   // ForceFanPwmEn [1 means use the ForceFanPwm value below]
      0,   // ForceFanPwm
      1,   // FanTable_Override [1 means use the full set of data specified below; 0 means use default fan table]
      4,   // FanTable_Hysteresis
      50,  // FanTable_TempLow
      51,  // FanTable_TempMed
      71,  // FanTable_TempHigh
      105, // FanTable_TempCritical
      0,   // FanTable_PwmLow
      8,   // FanTable_PwmMed
      8,   // FanTable_PwmHigh
      0,   // FanTable_PwmFreq [0  = 25kHz; 1 = 100Hz]
      1    // FanTable_Polarity [0 = negative; 1 = positive]
    }
  },
  // null - end of table
  {
    0x0,
    {0}
  },
};

static void
Phx1Am5SmuControl (
  SMUCLASS_INPUT_BLK *SmuInputBlock,
  SMU_PP_TABLE       *PPTable
)
{
  SmuInputBlock->SmuFeatureControl = PHX1AM5_SMU_FEATURES_ENABLE_DEFAULT;
  SmuInputBlock->SmuFeatureControlExt = PHX1AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT;
  SmuInputBlock->SmuFeatureControl64 = PHX1AM5_SMU_FEATURES_ENABLE_64_DEFAULT;
  SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX AM5 Smu Feature Default:\n  SmuFeatureControl = 0x%08x\n"
                                 "SmuFeatureControlExt = 0x%08x\n  SmuFeatureControl64 = 0x%08x\n",
    (uint32_t)PHX1AM5_SMU_FEATURES_ENABLE_DEFAULT,
    (uint32_t)PHX1AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT,
    (uint32_t)PHX1AM5_SMU_FEATURES_ENABLE_64_DEFAULT);
  SmuInputBlock->FllBtcEnable = true;
  SmuInputBlock->DcBtcVid = 0x97;

  PPTable->VddmCldoFastStartupEn = 0;
  PPTable->GfxClkSource = 1;
  PPTable->GfxclkInitFreq = 800;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
static void
SmuFixupPlatformConfigAM5 (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
  )
{
  FAN_POLICY_TABLE     *FanPolicyTable;
  uint32_t             SmuArg[6];
  
  SMU_TRACEPOINT(SIL_TRACE_INFO, "AM5 Fan Table\n");

  // read the TDP and TJMAX
  SmuServiceInitArgumentsCommon(SmuArg);
  switch (SmuInputBlock->CfgSystemConfiguration) {
    case 1:
      SmuArg[0] = 0x230000;
      break;
    case 2:
      SmuArg[0] = 0x2D0000;
      break;
    case 3:
      SmuArg[0] = 0x410000;
      break;
    case 0:
    default:
      // read the TDP and TJMAX
      SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_GetSustainedPowerAndThmLimit, SmuArg, 0);
      break;
  }
  SMU_TRACEPOINT(SIL_TRACE_INFO, "SystemConfig=%d, TDP/TJMAX = 0x%x\n", SmuInputBlock->CfgSystemConfiguration, SmuArg[0]);

  SmuInputBlock->PublicFanControlEnable = true;

  if (ISSOCPHX1AM5){
    // Fan Table policy default update.
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX1 AM5 Fan Table\n", SmuInputBlock->CfgSystemConfiguration, SmuArg[0]);
    FanPolicyTable = (FAN_POLICY_TABLE *) &DefaultPhx1AM5FanTable[0];
  }

  if (ISSOCPHX2AM5){
    // Fan Table policy default update.
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX2 AM5 Fan Table\n", SmuInputBlock->CfgSystemConfiguration, SmuArg[0]);
    FanPolicyTable = (FAN_POLICY_TABLE *) &DefaultPhx2AM5FanTable[0];
  }

  if (FanPolicyTable == NULL){
    SMU_TRACEPOINT(SIL_TRACE_INFO, "NBIO no fan table found\n");
    return;
  }

  if (SmuArg[0] != 0) {
    while (FanPolicyTable->TdpTjMax != 0) {
      if (FanPolicyTable->TdpTjMax == SmuArg[0]) {

        SMU_TRACEPOINT(SIL_TRACE_INFO, "FanPolicyTable->TdpTjMax  %x, SmuArg[0] %x\n", FanPolicyTable->TdpTjMax , SmuArg[0]);
        PPTable->FanTable_Override = FanPolicyTable->FanPolicy.FanTable_Override;
        PPTable->FanTable_Hysteresis = FanPolicyTable->FanPolicy.FanTable_Hysteresis;
        PPTable->FanTable_TempLow = FanPolicyTable->FanPolicy.FanTable_TempLow;
        PPTable->FanTable_TempMed = FanPolicyTable->FanPolicy.FanTable_TempMed;
        PPTable->FanTable_TempHigh = FanPolicyTable->FanPolicy.FanTable_TempHigh;
        PPTable->FanTable_TempCritical = FanPolicyTable->FanPolicy.FanTable_TempCritical;
        PPTable->FanTable_PwmLow = FanPolicyTable->FanPolicy.FanTable_PwmLow;
        PPTable->FanTable_PwmMed = FanPolicyTable->FanPolicy.FanTable_PwmMed;
        PPTable->FanTable_PwmHigh = FanPolicyTable->FanPolicy.FanTable_PwmHigh;
        PPTable->FanTable_PwmFreq = FanPolicyTable->FanPolicy.FanTable_PwmFreq;
        PPTable->FanTable_Polarity = FanPolicyTable->FanPolicy.FanTable_Polarity;
        break;
      }
      FanPolicyTable ++;
    }
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
static void
SmuFixupPlatformConfigFP8 (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
  )
{
  FAN_POLICY_TABLE                *FanPolicyTable;
  uint32_t                          SmuArg[6];

  SmuInputBlock->SmuFeatureControl = (uint32_t)FP8_SMU_FEATURES_ENABLE_DEFAULT;
  SmuInputBlock->SmuFeatureControlExt = (uint32_t)FP8_SMU_FEATURES_ENABLE_EXT_DEFAULT;
  SmuInputBlock->SmuFeatureControl64 = (uint32_t)FP8_SMU_FEATURES_ENABLE_64_DEFAULT;

  SMU_TRACEPOINT(SIL_TRACE_INFO, "Smu Feature Default:\n  SmuFeatureControl = 0x%08x\n"
                                 "SmuFeatureControlExt = 0x%08x\n  SmuFeatureControl64 = 0x%08x\n",
                    (uint32_t)FP8_SMU_FEATURES_ENABLE_DEFAULT,
                    (uint32_t)FP8_SMU_FEATURES_ENABLE_EXT_DEFAULT,
                    (uint32_t)FP8_SMU_FEATURES_ENABLE_64_DEFAULT);

  SmuInputBlock->DcBtcVid = 0x97;
  SmuInputBlock->FllBtcEnable = true;

  SmuServiceInitArgumentsCommon(SmuArg);
  switch (SmuInputBlock->CfgSystemConfiguration) {
    case 1:
      SmuArg[0] = 0xF0000;
      break;
    case 2:
      SmuArg[0] = 0x1C0000;
      break;
    case 3:
      SmuArg[0] = 0x1E0000;
      break;
    case 4:
      SmuArg[0] = 0x230000;
      break;
    case 5:
      SmuArg[0] = 0x2D0000;
      break;
    case 6:
      SmuArg[0] = 0x360000;
      break;
    case 7:
      SmuArg[0] = 0x140000;
      break;
    case 0:
    default:
      // read the TDP and TJMAX
      SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_GetSustainedPowerAndThmLimit, SmuArg, 0);
      break;
  }
  SMU_TRACEPOINT(SIL_TRACE_INFO, "SystemConfig=%d, TDP/TJMAX = 0x%x\n", SmuInputBlock->CfgSystemConfiguration, SmuArg[0]);
  // Fan Table policy default update.
  FanPolicyTable = (FAN_POLICY_TABLE *) &DefaultFP7FP8FanTable[0];
  if (SmuArg[0] != 0) {
    while (FanPolicyTable->TdpTjMax != 0) {
      if (FanPolicyTable->TdpTjMax == SmuArg[0]) {
        PPTable->FanTable_Override = FanPolicyTable->FanPolicy.FanTable_Override;
        PPTable->FanTable_Hysteresis = FanPolicyTable->FanPolicy.FanTable_Hysteresis;
        PPTable->FanTable_TempLow = FanPolicyTable->FanPolicy.FanTable_TempLow;
        PPTable->FanTable_TempMed = FanPolicyTable->FanPolicy.FanTable_TempMed;
        PPTable->FanTable_TempHigh = FanPolicyTable->FanPolicy.FanTable_TempHigh;
        PPTable->FanTable_TempCritical = FanPolicyTable->FanPolicy.FanTable_TempCritical;
        PPTable->FanTable_PwmLow = FanPolicyTable->FanPolicy.FanTable_PwmLow;
        PPTable->FanTable_PwmMed = FanPolicyTable->FanPolicy.FanTable_PwmMed;
        PPTable->FanTable_PwmHigh = FanPolicyTable->FanPolicy.FanTable_PwmHigh;
        PPTable->FanTable_PwmFreq = FanPolicyTable->FanPolicy.FanTable_PwmFreq;
        PPTable->FanTable_Polarity = FanPolicyTable->FanPolicy.FanTable_Polarity;
        break;
      }
      FanPolicyTable++;
    }
  }

  if (!SmuInputBlock->PublicFanControlEnable) {
      SmuInputBlock->SmuFeatureControl &= ~((uint32_t)(1 << (FEATURE_FAN_CONTROLLER_BIT % 32)));
    } else {
      SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_FAN_CONTROLLER_BIT % 32));
    }

  return;
}

/**
 * Phx2 AM5 Smu Control
 *
 */
static SIL_STATUS
Phx2Am5SmuControl (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
)
{
  SmuInputBlock->SmuFeatureControl = PHX2AM5_SMU_FEATURES_ENABLE_DEFAULT;
  SmuInputBlock->SmuFeatureControlExt = PHX2AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT;
  SmuInputBlock->SmuFeatureControl64 = PHX2AM5_SMU_FEATURES_ENABLE_64_DEFAULT;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "PHX2 AM5 Smu Feature Default:\nSmuFeatureControlDefines = 0x%08x\n"
                                  "SmuFeatureControlDefinesExt = 0x%08x\nSmuFeatureControlDefines64 = 0x%08x\n",
                              (uint32_t) PHX2AM5_SMU_FEATURES_ENABLE_DEFAULT,
                              (uint32_t) PHX2AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT,
                              (uint32_t) PHX2AM5_SMU_FEATURES_ENABLE_64_DEFAULT);

  SmuInputBlock->FllBtcEnable = true;
  SmuInputBlock->DcBtcVid = 0x97;

  return SilPass;
}

/**
 * Phx2 FP7 Smu Control
 *
 */
static SIL_STATUS
Phx2SmuControl (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
  )
{

  SmuInputBlock->SmuFeatureControl = PHX2_SMU_FEATURES_ENABLE_DEFAULT;
  SmuInputBlock->SmuFeatureControlExt = PHX2_SMU_FEATURES_ENABLE_EXT_DEFAULT;
  SmuInputBlock->SmuFeatureControl64 = PHX2_SMU_FEATURES_ENABLE_64_DEFAULT;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "PHX2 Smu Feature Default:\nSmuFeatureControlDefines = 0x%08x\n"
                                  "SmuFeatureControlDefinesExt = 0x%08x\nSmuFeatureControlDefines64 = 0x%08x\n",
                              (uint32_t) PHX2_SMU_FEATURES_ENABLE_DEFAULT,
                              (uint32_t) PHX2_SMU_FEATURES_ENABLE_EXT_DEFAULT,
                              (uint32_t) PHX2_SMU_FEATURES_ENABLE_64_DEFAULT);

  return SilPass;
}

/**

 * PHX2 Before Smu Platform Config
 *
 *  @param[in]  PPTable            Context for PHX2 Smu Platform
 *
 *  @retval SilInvalidParameter  If parameters are invalid, return this value.
 *  @retval SilPass              If free operation is successful, return this value.
 *
 */
static SIL_STATUS
Phx2FixupSmuPlatformConfig (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
)
{
  SIL_STATUS                      Status;
  ZSC_WAKE_EVENT_MASK_0_STRUCT    ZscWakeMask0;
  uint32_t                        Value32;

  ZscWakeMask0.Value = 0;
  Status = SilPass;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Check if it is PHX2 AM5
  if (ISSOCPHX2AM5) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX2 AM5 SMU\n");
    Status = Phx2Am5SmuControl(GnbHandle, SmuInputBlock, PPTable);
    PPTable->VddmCldoFastStartupEn = 0;   //CBS SMU Feature: VDDM CLDO fast startup Disable
    PPTable->GfxClkSource = 1;            //Gfx CLK use DFLL
    PPTable->GfxclkInitFreq = 800;        //GfxClkFinit -> 800

  // Check if it is PHX2 FP7 or PHX2 FP7r2 or HPT2 FP7r2
  } else if (ISSOCPHX2FP7 || ISSOCPHX2FP7R2 || ISSOCHPT2FP7R2 || ISSOCHPT2FP7){
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX2 Fp7 SMU\n");
    PPTable->GfxDldoBypass = 1;   //CBS SMU Feature default GfxDldoBypass =1
    PPTable->VddmCldoFastStartupEn = 0;   //CBS SMU Feature: VDDM CLDO fast startup Disable
    if (SmuInputBlock->AmdZstatesMaskEnable) {
      PPTable->ZstatesMaskEnable = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Timer) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_TIMER = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0S2nsTimer) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_S2NS_TIMER = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Fch) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_FCH = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Acp) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_ACP = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Mp2) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_MP2 = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Dbreq) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_DBREQ = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0DcnS) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_DCN_S = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0DcnNs) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_DCN_NS = 1;
    }
    if (SmuInputBlock->AmdZstatesWake0Df) {
      ZscWakeMask0.Field.WAKE_EVENT_MASK_DF = 1;
    }
    SMU_TRACEPOINT(SIL_TRACE_INFO, "ZscWakeMask0.Value %x \n",ZscWakeMask0.Value);
    PPTable->ZscWakeEventMask0 = ZscWakeMask0.Value;
    Status = Phx2SmuControl(GnbHandle, SmuInputBlock, PPTable);
    Value32 = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, 0x5DA78);
    if (((Value32 >> 18) & 0x7F) == 0x33) {
      SmuInputBlock->SmuFeatureControlExt &= ~((uint32_t)(1 << (FEATURE_DVO_BIT % 32)));
    }
  }

  if(ISSOCPHX2FP7 || ISSOCHPT2FP7) {
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_SMU_LOW_POWER_BIT % 32));
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
void
SmuFixupPlatformConfig (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable,
  uint32_t             PackageType
  )
{
  uint32_t              Value32Lower;
  uint32_t              FMax = 0;
  uint32_t              GfxMax = 0;
  SIL_STATUS            Status;

  // Enable default SMU Features based on package type
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "PackageType = %d\n", PackageType);

  // Check if it is PHX FP8
  if (ISSOCPHXFP8) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX FP8\n");
    SmuFixupPlatformConfigFP8 (GnbHandle, SmuInputBlock, PPTable);
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_SMU_LOW_POWER_BIT % 32));
    SMU_TRACEPOINT(SIL_TRACE_INFO, "Disable DVO\n");
    SmuInputBlock->SmuFeatureControlExt &= ~(uint32_t)(0 << (FEATURE_DVO_BIT % 32));
    if (SmuInputBlock->AmdSmuZStateControlEnable) {
      SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_WHISPER_MODE_BIT % 32));
    }
  }

  // Check if it is PHX FP7
  if (ISSOCPHXFP7) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX FP7\n");
    SmuFixupPlatformConfigFP8 (GnbHandle, SmuInputBlock, PPTable);
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_SMU_LOW_POWER_BIT % 32));
    if (SmuInputBlock->AmdSmuZStateControlEnable) {
      SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_WHISPER_MODE_BIT % 32));
    }
  }

  // Check if it is PHX FP7r2
  if (ISSOCPHXFP7R2) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX FP7r2\n");
    SmuFixupPlatformConfigFP8 (GnbHandle, SmuInputBlock, PPTable);
    if (SmuInputBlock->AmdSmuZStateControlEnable) {
      SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_WHISPER_MODE_BIT % 32));
    }
  }

  // Check if it is PHX2
  if (ISSOCPHX2 || ISSOCHPT2) {
    Status = Phx2FixupSmuPlatformConfig(GnbHandle, SmuInputBlock, PPTable);
    if (Status != SilPass) {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "Phx2FixupSmuPlatformConfig %x\n", Status);
    }
  }

  if (ISSOCPHXAM5) {
    SmuFixupPlatformConfigAM5(GnbHandle, SmuInputBlock, PPTable);
  }

  // Check if it is PHX1 AM5
  if (ISSOCPHX1AM5) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX AM5\n");
    Phx1Am5SmuControl(SmuInputBlock, PPTable);
  }


  if(0 /* capsule */) {
    SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_CCLK_DPM_BIT));
  } else {
    SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_CCLK_DPM_BIT));
  }


  if (!SmuInputBlock->AmdSmuZStateControlEnable) {
    SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_ZSTATES_BIT));
  } else {
    SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_ZSTATES_BIT));
  }

  if (SmuInputBlock->S0i3Enable) {
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_S0I3_BIT % 32));
  } else {
    SmuInputBlock->SmuFeatureControlExt &= ~(uint32_t)(1 << (FEATURE_S0I3_BIT % 32));
  }

  if (SmuInputBlock->AmdSmuHspClkDsEn) {
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_DS_HSPCLK_BIT % 32));
  } else {
    SmuInputBlock->SmuFeatureControlExt &= ~(uint32_t)(1 << (FEATURE_DS_HSPCLK_BIT % 32));
  }

  if (!SmuInputBlock->AmdSmuFclkDpmEn /* || capsule */) {
    SmuInputBlock->SmuFeatureControl &= ~(uint32_t)(1 << (FEATURE_FCLK_DPM_BIT));
  } else {
    SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_FCLK_DPM_BIT));
  }

  if (!SmuInputBlock->EnableStapm) {
    SmuInputBlock->SmuFeatureControlExt &= ~(uint32_t)(1 << (FEATURE_STAPM_BIT % 32));
  } else {
    SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_STAPM_BIT % 32));
  }

  if (SmuInputBlock->CpuBoostClockOverride == 0x01) {
    if (SmuInputBlock->FMaxFrequency != 0) {
      Value32Lower = xUSLSmnRead (0, GnbHandle->Address.Address.Bus, 0x5D8F4);
      if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
        FMax = (Value32Lower >> 1) & 0xFF;
        if ((SmuInputBlock->FMaxFrequency <= 200) ||
            ((uint32_t)(0 - SmuInputBlock->FMaxFrequency) <= 1000)) {
          FMax = FMax * 25;
          FMax += SmuInputBlock->FMaxFrequency;
          PPTable->CclkFmaxOverride = (uint16_t)FMax;
          SMU_TRACEPOINT(SIL_TRACE_INFO, "CPU Fmax= %d\n", FMax);
        }
      }
    }
  }

  if (SmuInputBlock->GpuBoostClockOverride == 0x01) {
    if (SmuInputBlock->GFXFMaxFrequency != 0) {
      Value32Lower = xUSLSmnRead (0, GnbHandle->Address.Address.Bus, 0x5D8F8);
      if ((Value32Lower != 0x0) && (Value32Lower != 0xFFFFFFFF)) {
        GfxMax = (Value32Lower >> 2) & 0xFF;
        if (SmuInputBlock->GFXFMaxFrequency <= 200) {
          GfxMax = GfxMax * 25;
          if (ISSOCPHXR5UA1 && (GfxMax > 2500) && ((1 << ZEN4_PKG_AM5) != PackageType)) {
            SMU_TRACEPOINT(SIL_TRACE_INFO, "ISSOCPHXR5UA1 0x00A70F41 \n", GfxMax);
            GfxMax -= 200;
          }
          GfxMax += SmuInputBlock->GFXFMaxFrequency;
          PPTable->GfxclkFmaxOverride = (uint16_t)GfxMax;
          SMU_TRACEPOINT(SIL_TRACE_INFO, "GPU Fmax= %d\n", GfxMax);
        }
      }
    }
  }

  if (ISSOCPHX1 && !ISSOCPHXAM5) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PHX1 FP8 & FP7\n");
    Value32Lower = xUSLSmnRead (0, GnbHandle->Address.Address.Bus, 0x5DA78);
    if (((Value32Lower >> 18) & 0x7F) == 0x33) {
      if (SmuInputBlock->AmdSmuGfxDpmEn) {
        SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_GFX_DPM_BIT));
      }
      PPTable->GfxClkSource = 1;
      PPTable->GfxclkInitFreq = 0x320;
      PPTable->CoreDldoPsmMargin = (uint16_t)(0 - (uint16_t)0x14);
      PPTable->GfxclkFminOverride = 0x258;
      PPTable->VddmCldoFastStartupEn = 0;
    } else {
      if (SmuInputBlock->AmdSmuGfxDpmEn) {
        SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_GFX_DPM_BIT));
      }
      SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_FIT_BIT));
      PPTable->GfxClkSource = 1;
      PPTable->GfxclkInitFreq = 0x320;
      PPTable->VddmCldoFastStartupEn = 0;
    }
  }

  if(ISSOCPHXAM5){
    SMU_TRACEPOINT(SIL_TRACE_INFO, "ISSOCPHXAM5 Package\n");
    if (SmuInputBlock->AmdSmuGfxDpmEn) {
      SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_GFX_DPM_BIT));
    }
    SmuInputBlock->SmuFeatureControl |= (uint32_t)(1 << (FEATURE_FIT_BIT));
    PPTable->GfxClkSource = 1;
    PPTable->GfxclkInitFreq = 0x320;
    PPTable->VddmCldoFastStartupEn = 0;

    PPTable->DfCstateConfigOverride = 1;
    PPTable->DfCstateMmStutterOptEn = 1;
    PPTable->DfCstateUclkPwrDnEn = 1;
    PPTable->DfCstatePwrGateEn = 0;
    PPTable->DfCstateFclkPwrDnEn = 1;
    PPTable->DfCstateSelfRefrEn = 1;
  }
  if (ISSOCHPT1||ISSOCPHXAM5) {
    PPTable->ApccEnable = 0;
  }

  SMU_TRACEPOINT(SIL_TRACE_INFO, "%a End\n", __FUNCTION__);
}

