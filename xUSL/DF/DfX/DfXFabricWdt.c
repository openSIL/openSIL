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

/// Data Fabric CCM Watchdog Timer Count select.
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

/// Data Fabric Watchdog Timer timebase select in ns
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
 * @param  SilContext              A context structure through which host firmware defined data
 *                                 can be passed to openSIL. The host firmware is responsible
 *                                 for initializing the SIL_CONTEXT structure.
 * @param  DfCcmTimeout            DF CCM WDT time in ns
 * @param  DfGlobalCntlFunc        DF Global Control Function
 * @param  DfGlobalCntlReg         DF Global Control Register
 *
 * @retval true  Watchdog Timer is enabled
 * @retval false Watchdog Timer is disabled
 *
 */
bool
DfXGetWdtInfo (
  SIL_CONTEXT  *SilContext,
  uint64_t     *DfCcmTimeout,
  uint16_t     DfGlobalCntlFunc,
  uint16_t     DfGlobalCntlReg
  )
{
  bool                           DfWdtEn;
  SIL_STATUS                     Status;
  uint32_t                       WdtBaseSel;
  uint32_t                       CcmWdtCntSel;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    DfWdtEn = false;
    assert(Status == SilPass);
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get DF CMN 2 REV table\n");
  } else {
    DfXfer->DfAbstractRegAcc(SilContext,
      (uint8_t)DF_GLOBAL_CTRL_WDTBaseSel,
      0,
      0,
      FABRIC_REG_ACC_BC,
      0,
      DF_ABSTRACT_REG_READ,
      0,
      &WdtBaseSel
      );

    DfXfer->DfAbstractRegAcc(SilContext,
      (uint8_t)DF_GLOBAL_CTRL_CCMWDTCntSel,
      0,
      0,
      FABRIC_REG_ACC_BC,
      0,
      DF_ABSTRACT_REG_READ,
      0,
      &CcmWdtCntSel
      );

    assert(WdtBaseSel < NumberOfDfWdtBaseSel);
    assert(CcmWdtCntSel < NumberOfDfWdtCntSel);

    if (WdtBaseSel < TIMER_DISABLE) {
      DfWdtEn = true;
      *DfCcmTimeout = (uint64_t) (DfWdtBaseSelDecode[WdtBaseSel] * DfWdtCntSelDecode[CcmWdtCntSel]);
    } else {
      DfWdtEn = false;
      WdtBaseSel = 0;
      CcmWdtCntSel = 0;
      *DfCcmTimeout = 0;
    }
  }

  return DfWdtEn;
}
