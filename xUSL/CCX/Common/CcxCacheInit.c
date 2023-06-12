/**
 * @file CcxCacheInit.c
 * @brief OpenSIL Ccx cache initialization.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <xSIM.h>
#include <xUslCcxRoles.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

#include "CcxCacheInit.h"

/**
 * InitializeVariableMtrrs
 *
 * @details Initialize variable MTRRs and Program 0~TOM as WB.  Use MTRR
 * pairs starting with Core::X86::Msr::MtrrVarBase.
 *
 * "Limit" in the context of this procedure is not the numerically correct
 * limit, but rather the Last address+1, for purposes of coding efficiency
 * and readability.  Size of a region is then Limit-Base.
 *
 * 1. Size of each range must be a power of two
 * 2. Each range must be naturally aligned (Base is same as size)
 *
 * There are two code paths: the ascending path and descending path (analogous
 * to bsf and bsr), where the next limit is a function of the next set bit in
 * a forward or backward sequence of bits (as a function of the Limit).  We
 * start with the ascending path, to ensure that regions are naturally aligned,
 * then we switch to the descending path to maximize MTRR usage efficiency.
 * Base=0 is a special case where we start with the descending path.
 * Correct Mask for region is 2comp(Size-1)-1,
 * which is 2comp(Limit-Base-1)-1 *
 *
 * @return     SIL_STATUS
 */
SIL_STATUS
InitializeVariableMtrrs (void
  )
{
  uint32_t Cache32bTOP;
  uint64_t LocalMsrRegister;
  uint32_t CurBase;
  uint32_t CurLimit;
  uint32_t CurSize;
  uint32_t CurAddr;
  uint32_t Value32;

  LocalMsrRegister = xUslRdMsr (MSR_TOM);
  Cache32bTOP = (uint32_t) ((LocalMsrRegister >> 16) & 0xFFFFFFFF);

  CurBase = 0;
  CurLimit = Cache32bTOP;
  CurAddr = AMD_AP_MTRR_VARIABLE_BASE0;

  while ((CurAddr >= 0x200) && (CurAddr < 0x20E) && (CurBase < Cache32bTOP)) {
    CurSize = CurLimit = (uint32_t) 1 << (xUslLowBitSet32 (CurBase) == -1 ? 31 : xUslLowBitSet32 (CurBase)) ;
    CurLimit += CurBase;
    if ((CurBase == 0) || (Cache32bTOP < CurLimit)) {
      CurLimit = Cache32bTOP - CurBase;
      CurSize = CurLimit = (uint32_t) 1 << (xUslHighBitSet32 (CurLimit) == -1 ? 31 : xUslHighBitSet32 (CurLimit));
      CurLimit += CurBase;
    }

    // prog. MTRR with current region Base
    LocalMsrRegister = (CurBase << 16) | (uint32_t) CacheWriteBack;
    xUslWrMsr (CurAddr, LocalMsrRegister);

    // prog. MTRR with current region Mask
    CurAddr++;                              // other half of MSR pair
    Value32 = CurSize - (uint32_t) 1;
    Value32 = ~Value32;
    LocalMsrRegister = 0xFFFF00000000ull | (Value32 << 16) | ((uint32_t) 1 << 11);
    xUslWrMsr (CurAddr, LocalMsrRegister);

    CurBase = CurLimit;
    CurAddr++;                              // next MSR pair
  }

  if (CurLimit < Cache32bTOP) {
    // Announce failure
    assert(CurLimit >= Cache32bTOP);
    return SilOutOfResources;
  }

  while ((CurAddr >= 0x200) && (CurAddr <= 0x20F)) {
    if ((CurAddr % 2) == 0) {
      xUslWrMsr (CurAddr + 1, 0);
      xUslWrMsr (CurAddr, 0);
      CurAddr += 2;
      continue;
    }
    CurAddr++;
  }

  return SilPass;
}

/**
 * CcxCacheInit
 * @brief cache initialization
 *
 */
void CcxCacheInit (void) {
  uint64_t   LocalMsrRegister;

  //
  //======================================================================
  // Set default values for CPU registers
  //======================================================================
  //
  xUslWrMsr (MSR_SYS_CFG, xUslRdMsr (MSR_SYS_CFG) | SYS_CFG_MTRR_FIX_DRAM_MOD_EN);

  LocalMsrRegister = 0x1E1E1E1E1E1E1E1Eull;
  xUslWrMsr (0x250, LocalMsrRegister);      // 0 - 512K = WB Mem
  LocalMsrRegister = 0x1E1E1E1E1E1E1E1Eull;
  xUslWrMsr (0x258, LocalMsrRegister);      // 512K - 640K = WB Mem

  xUslWrMsr (MSR_SYS_CFG, xUslRdMsr (MSR_SYS_CFG) | (SYS_CFG_MTRR_FIX_DRAM_EN | SYS_CFG_MTRR_VAR_DRAM_EN));

  LocalMsrRegister = xUslRdMsr (MSR_SYS_CFG);
  if ((LocalMsrRegister & SYS_CFG_MTRR_TOM2_EN) != 0) {
    LocalMsrRegister |= SYS_CFG_TOM2_FORCE_MEM_TYPE_WB;
  }

  LocalMsrRegister &= ~SYS_CFG_MTRR_FIX_DRAM_MOD_EN;
  xUslWrMsr (MSR_SYS_CFG, LocalMsrRegister);
}
