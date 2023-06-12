/**
  * @file  DfXFabricWdt.c
  * @brief OpenSIL data fabric WDT services for DfX.
  *
  */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <DF/DfX/SilFabricRegistersDfX.h>
#include "DfXFabricRegisterAcc.h"
#include "DfXFabricWdt.h"
#include <DF/Df.h>

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
  bool                    DfWdtEn;
  DF_GLOBAL_CTRL_REGISTER DfGlblCtrl;

  DfGlblCtrl.Value = DfXFabricRegisterAccRead (0, DfGlobalCntlFunc, DfGlobalCntlReg, FABRIC_REG_ACC_BC);
  assert (DfGlblCtrl.Field.WDTBaseSel < NumberOfDfWdtBaseSel);
  assert (DfGlblCtrl.Field.CCMWDTCntSel < NumberOfDfWdtCntSel);
  if (DfGlblCtrl.Field.WDTBaseSel < TIMER_DISABLE) {
    DfWdtEn = true;
    *DfCcmTimeout = (uint64_t) (DfWdtBaseSelDecode[DfGlblCtrl.Field.WDTBaseSel] *
      DfWdtCntSelDecode[DfGlblCtrl.Field.CCMWDTCntSel]);
  } else {
    DfWdtEn = false;
    *DfCcmTimeout = 0;
  }
  return DfWdtEn;
}

