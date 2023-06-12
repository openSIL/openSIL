/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXFabricWdt.c
 * @brief Data fabric WDT services for DfX
 *
 */

#include <DF/Df.h>
#include <DF/Common/DfCmn2Rev.h>
#include "SilFabricRegistersDfX.h"
#include "DfXFabricRegisterAcc.h"
#include "DfXFabricWdt.h"

/// Data Fabric CCM Watchdog Timer Count select. defined in PPR DF::DfGlobalCtrl::CCMWDTCntSel
static uint32_t DfWdtCntSelDecode [] =
{
  31,
  127,
  255,
  511,
  1023,
  2047,
  4095,
  16383
};

#define NumberOfDfWdtCntSel (sizeof (DfWdtCntSelDecode) / sizeof (DfWdtCntSelDecode[0]))

/// Data Fabric Watchdog Timer timebase select in ns defined in PPR DF::DfGlobalCtrl::WDTBaseSel
static uint64_t DfWdtBaseSelDecode [] =
{
  1310000,
  1280,
  10,
  0
};

#define NumberOfDfWdtBaseSel (sizeof (DfWdtBaseSelDecode) / sizeof (DfWdtBaseSelDecode[0]))

/**
 * DfXGetWdtInfo
 * @brief returns if DF WDT is enabled and fetches DF watchdog timer's timeout value.
 *
 * @param[in, out] DfCcmTimeout            DF CCM WDT time in ns
 * @param[in]      DfGlobalCntlFunc        DF Global Control Function
 * @param[in]      DfGlobalCntlReg         DF Global Control Register
 *
 * @retval true  Watchdog Timer is enabled
 * @retval false Watchdog Timer is disabled
 *
 */
bool
DfXGetWdtInfo (
  uint64_t *DfCcmTimeout,
  uint16_t  DfGlobalCntlFunc,
  uint16_t  DfGlobalCntlReg
  )
{
  bool                           DfWdtEn;
  SIL_STATUS                     Status;
  uint32_t                       WDTBaseSel;
  uint32_t                       CCMWDTCntSel;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    DfWdtEn = false;
    assert(Status == SilPass);
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get DF CMN 2 REV table\n");
  } else {
    DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_WDTBaseSel,
      0,
      0,
      FABRIC_REG_ACC_BC,
      0,
      DF_ABSTRACT_REG_READ,
      0,
      &WDTBaseSel
      );

    DfXfer->DfAbstractRegAcc((uint8_t)DF_GLOBAL_CTRL_CCMWDTCntSel,
      0,
      0,
      FABRIC_REG_ACC_BC,
      0,
      DF_ABSTRACT_REG_READ,
      0,
      &CCMWDTCntSel
      );

    assert(WDTBaseSel < NumberOfDfWdtBaseSel);
    assert(CCMWDTCntSel < NumberOfDfWdtCntSel);

    if (WDTBaseSel < TIMER_DISABLE) {
      DfWdtEn = true;
      *DfCcmTimeout = (uint64_t) (DfWdtBaseSelDecode[WDTBaseSel] * DfWdtCntSelDecode[CCMWDTCntSel]);
    } else {
      DfWdtEn = false;
      WDTBaseSel = 0;
      CCMWDTCntSel = 0;
      *DfCcmTimeout = 0;
    }
  }

  return DfWdtEn;
}
