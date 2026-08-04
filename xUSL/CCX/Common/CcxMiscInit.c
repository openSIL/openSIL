/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxMiscInit.c
 * @brief CCX Miscellaneous feature initialization.
 *
 */

#include <SilCommon.h>
#include <Ccx.h>
#include <CcxCmn2Rev.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <DF/DfIp2Ip.h>

#define NUMBER_OF_CPU_WDT_CNT_SEL \
        (sizeof (CpuWdtCountSelDecode) / sizeof (CpuWdtCountSelDecode[0]))
#define MIN_CPU_WDT_COUNT_SEL 7
#define MAX_CPU_WDT_COUNT_SEL 9

#define NUM_OF_CPU_WDT_TIME_BASE \
        (sizeof (CpuWdtTimeBaseDecode) / sizeof (CpuWdtTimeBaseDecode[0]))
#define MIN_CPU_WDT_TIME_BASE 1

/// CPU Watchdog Timer Count Select
static const uint32_t CpuWdtCountSelDecode [] =
{
  4095,
  2047,
  1023,
  511,
  255,
  127,
  63,
  31,
  8191,
  16383
};

/// CPU Watchdog Timer Count Select bump up number
static const uint8_t CpuWdtCountSelBumpUp [] =
{
  7,
  6,
  5,
  4,
  3,
  2,
  1,
  0,
  8,
  9
};

/// CPU watchdog timer time base
static const uint64_t CpuWdtTimeBaseDecode [] =
{
  1310000,
  1280
};

/**
 * CcxSyncMiscMsrs
 *
 * @brief This routine is only executed by the APs to sync MSR data to match BSP
 * This synchronizes the MSRs in ApLaunchGlobalData->ApMsrSyncList across all APs
 *
 * @param ApLaunchGlobalData AP launch global data
 *
 */
void
CcxSyncMiscMsrs (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  uint8_t i = 0;

  for (i = 0; ApLaunchGlobalData->ApMsrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    xUslMsrAndThenOr(ApLaunchGlobalData->ApMsrSyncList[i].MsrAddr,
      ~(ApLaunchGlobalData->ApMsrSyncList[i].MsrMask),
      (ApLaunchGlobalData->ApMsrSyncList[i].MsrData &
      ApLaunchGlobalData->ApMsrSyncList[i].MsrMask)
      );
  }
}

/**
 * CcxSetMiscMsrs
 *
 * @brief This routine sets miscellaneous MSRs and is only executed on the BSP.
 * APs will sync the applicable MSRs settings through ApMsrSyncList
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param CcxInputBlock         CCX input data block
 *
 */
void
CcxSetMiscMsrs (
  SIL_CONTEXT        *SilContext,
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint64_t        LocalMsrRegister;
  uint8_t         LocalRedirectForReturnDis;
  uint8_t         LocalOpCacheCtrl;
  uint8_t         LocalStreamingStoresCtrl;
  bool            LocalEnableRMSS;
  bool            LocalEnableSvmAVIC;
  bool            LocalIbsHardwareEn;
  uint8_t         LocalEnableAvx512;
  bool            LocalEnableSvmX2AVIC;
  uint8_t         LocalMonMwaitDis;
  uint8_t         LocalDisableWcSpecConfig;
  bool            LocalEnableFSRM;
  bool            LocalEnableERMS;
  bool            LocalEnableSCP;
  uint8_t         LocalCpuPauseDelay;
  CCX_XFER_TABLE  *CcxXfer;


  if (SilGetCommon2RevXferTable(SilContext, SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return;
  }

  LocalRedirectForReturnDis = CcxInputBlock->RedirectForReturnDis;
  LocalOpCacheCtrl = CcxInputBlock->OpCacheCtrl;
  LocalStreamingStoresCtrl = CcxInputBlock->StreamingStoresCtrl;
  LocalEnableRMSS = CcxInputBlock->EnableRMSS;
  LocalEnableSvmAVIC = CcxInputBlock->EnableSvmAVIC;
  LocalEnableSvmX2AVIC = CcxInputBlock->EnableSvmX2AVIC;
  LocalIbsHardwareEn = CcxInputBlock->IbsHardwareEn;
  LocalEnableAvx512 = CcxInputBlock->EnableAvx512;
  LocalMonMwaitDis = CcxInputBlock->MonMwaitDis;
  LocalDisableWcSpecConfig = CcxInputBlock->DisableWcSpecConfig;
  LocalEnableFSRM = CcxInputBlock->AmdEnableFSRM;
  LocalEnableERMS = CcxInputBlock->AmdEnableERMS;
  LocalEnableSCP = CcxInputBlock->AmdStatisticalCorrectPredictor;
  LocalCpuPauseDelay = CcxInputBlock->AmdCpuPauseDelay;

  // Force recalc of TSC on all threads after loading patch
  LocalMsrRegister = xUslRdMsr(MSR_PSTATE_DEF_ADDRESS);
  xUslWrMsr(MSR_PSTATE_DEF_ADDRESS, LocalMsrRegister);

  if (LocalRedirectForReturnDis != 0xFF) {
    xUslMsrAndThenOr(MSR_DE_CFG, ~(uint64_t) BIT_64(14), ((LocalRedirectForReturnDis) ? (uint64_t) BIT_64(14) : 0));
  }

  if (LocalOpCacheCtrl != 0xFF) {
    xUslMsrAndThenOr(MSR_IC_CFG, ~(uint64_t) BIT_64(5), ((LocalOpCacheCtrl) ? (uint64_t) BIT_64(5) : 0));
  }

  if (LocalStreamingStoresCtrl != 0xFF) {
    xUslMsrAndThenOr(MSR_LS_CFG, ~(uint64_t) BIT_64(28), ((LocalStreamingStoresCtrl) ? (uint64_t) BIT_64(28) : 0));
  }

  if (LocalEnableAvx512 != 0xFF) {
    xUslMsrAndThenOr(MSRxC0011002,
      ~(uint64_t) 0xD0230000,
      LocalEnableAvx512 ? 0xD0230000 : 0
      );
  }

  if (LocalEnableFSRM) {
    xUslMsrOr (MSRxC00110DF, BIT_64(36));
  } else {
    xUslMsrAnd (MSRxC00110DF, ~BIT_64(36));
  }

  if (LocalEnableERMS) {
    xUslMsrOr (MSRxC0011002, BIT_64(9));
  } else {
    xUslMsrAnd (MSRxC0011002, ~BIT_64(9));
  }


  if (LocalEnableRMSS) {
    xUslMsrAnd(MSRxC0011000, ~((uint64_t) BIT_64(15)));
  } else {
    xUslMsrOr(MSRxC0011000, (uint64_t) BIT_64(15));
  }

  if (LocalIbsHardwareEn) {
    xUslMsrOr(MSR_LS_CFG, (uint64_t) BIT_64(54));
  }

  if (LocalEnableSvmAVIC) {
    xUslMsrOr(MSRxC00110DD, (uint64_t) BIT_64(13));
  } else {
    xUslMsrAnd(MSRxC00110DD, ~((uint64_t) BIT_64(13)));
  }

  if (LocalEnableSvmX2AVIC) {
    xUslMsrOr(MSRxC00110DD, (uint64_t) BIT_64(18));
  } else {
    xUslMsrAnd(MSRxC00110DD, ~((uint64_t) BIT_64(18)));
  }

  if (LocalMonMwaitDis != 0xFF) {
    xUslMsrAndThenOr(MSR_HWCR,
      ~(uint64_t) BIT_64(9),
      ((LocalMonMwaitDis) ? BIT_64(9) : 0)
      );
  }

  if (LocalDisableWcSpecConfig != 0xFF) {
    xUslMsrAnd(MSR_LS_CFG, ~BIT_64(53));
    xUslMsrAndThenOr(MSRxC00110E5, ~(uint64_t) 0x240000000, LocalDisableWcSpecConfig ? 0x240000000 : 0);
  }

  if (LocalEnableSCP) {
    xUslMsrAnd (MSRxC001102E, ~BIT_64(35));
  } else {
    xUslMsrOr (MSRxC001102E, BIT_64(35));
  }

  switch (LocalCpuPauseDelay) {
  case 0xff:
    break;
  case 0:
    xUslMsrOr (MSR_DE_CFG, BIT_64(31));
    break;
  case 1:
    xUslMsrAnd (MSR_DE_CFG, ~BIT_64(31));
    xUslMsrAnd (MSRxC00110E3, ~(BIT_64(16) | BIT_64(17)));
    break;
  case 2:
    xUslMsrAnd (MSR_DE_CFG, ~BIT_64(31));
    xUslMsrAndThenOr (MSRxC00110E3, ~(BIT_64(16) | BIT_64(17)), BIT_64(16));
    break;
  case 3:
    xUslMsrAnd (MSR_DE_CFG, ~BIT_64(31));
    xUslMsrAndThenOr (MSRxC00110E3, ~(BIT_64(16) | BIT_64(17)), BIT_64(17));
    break;
  case 4:
    xUslMsrAnd (MSR_DE_CFG, ~BIT_64(31));
    xUslMsrAndThenOr (MSRxC00110E3, ~(BIT_64(16) | BIT_64(17)), BIT_64(16) | BIT_64(17));
    break;
  default:
    break;
  }

  CcxXfer->SetMiscMsrs(CcxInputBlock);
}

/**
 * UpdateApMtrrSettings
 * @brief Update AP MTRR settings list according to input blk data
 *
 * @param[in, out] ApMtrrSettingsList List of MTRR settings for AP
 * @param[in] CcxInputBlock CCX Input data block
 *
 */
void
UpdateApMtrrSettings (
  volatile AP_MTRR_SETTINGS *ApMtrrSettingsList,
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  while (ApMtrrSettingsList->MsrAddr != CPU_LIST_TERMINAL) {
    switch (ApMtrrSettingsList->MsrAddr) {
    case AMD_AP_MTRR_FIX64K_00000:
      if (CcxInputBlock->AmdFixedMtrr250 != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr250;
      }
      break;
    case AMD_AP_MTRR_FIX16K_80000:
      if (CcxInputBlock->AmdFixedMtrr258 != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr258;
      }
      break;
    case AMD_AP_MTRR_FIX16K_A0000:
      if (CcxInputBlock->AmdFixedMtrr259 != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr259;
      }
      break;
    case AMD_AP_MTRR_FIX4K_C0000:
      if (CcxInputBlock->AmdFixedMtrr268 != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr268;
      }
      break;
    case AMD_AP_MTRR_FIX4K_C8000:
      if (CcxInputBlock->AmdFixedMtrr269 != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr269;
      }
      break;
    case AMD_AP_MTRR_FIX4K_D0000:
      if (CcxInputBlock->AmdFixedMtrr26A != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26A;
      }
      break;
    case AMD_AP_MTRR_FIX4K_D8000:
      if (CcxInputBlock->AmdFixedMtrr26B != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26B;
      }
      break;
    case AMD_AP_MTRR_FIX4K_E0000:
      if (CcxInputBlock->AmdFixedMtrr26C != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26C;
      }
      break;
    case AMD_AP_MTRR_FIX4K_E8000:
      if (CcxInputBlock->AmdFixedMtrr26D != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26D;
      }
      break;
    case AMD_AP_MTRR_FIX4K_F0000:
      if (CcxInputBlock->AmdFixedMtrr26E != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26E;
      }
      break;
    case AMD_AP_MTRR_FIX4K_F8000:
      if (CcxInputBlock->AmdFixedMtrr26F != 0xFFFFFFFFFFFFFFFF) {
        ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26F;
      }
      break;
    default:
      break;
    }

    ApMtrrSettingsList++;
  }
}

/**
 *  CcxZen4EnableSmeHmkee
 *
 * @brief This routine enables either secure memory encryption
 *    or host multi-Key encryption.
 *    Attempting to enable both simultaneously will result in #GP
 * @param SmeeEnable Secure Memory Encryption Enable flag
 */
void CcxEnableSmeeHmkee (bool SmeeEnable, bool HmkeeEnable)
{
  if (SmeeEnable && HmkeeEnable) {
    assert(false);
    return;
  }

  if (SmeeEnable) {
    xUslMsrOr(MSR_SYS_CFG, BIT_64(23));
  } else if (HmkeeEnable) {
    xUslMsrOr(MSR_SYS_CFG, BIT_64(26));
  }
}
/**
 * CcxEnableSvm
 * @brief This routine initializes SVM
 *
 * @param SvmEnable  Input parameter to enable SVM
 * @param SvmLock  Input parameter to configure SVM lock
 */
void
CcxInitSvm (bool SvmEnable, bool SvmLock)
{
  if (SvmLock) {
    xUslWrMsr(MSR_SVM_LOCK_KEY, 0);
  }

  if (SvmEnable) {
    xUslMsrAnd(MSR_VM_CR, ~(BIT_64(4)));
  } else {
    xUslMsrOr(MSR_VM_CR, BIT_64(4));
  }

  if (SvmLock) {
    xUslMsrOr(MSR_VM_CR, BIT_64(3));
  } else {
    xUslMsrAnd(MSR_VM_CR, ~(BIT_64(3)));
  }
}

/**
 * CcxEnableWdt
 * @brief This routine enables cpu watchdog timer.
 *
 * @param SilContext A context structure through which host firmware defined data
 *                   can be passed to openSIL. The host firmware is responsible
 *                   for initializing the SIL_CONTEXT structure.
 * @param CpuWdt     Input pointer to struct containing WDT configuration information.
 */
void
CcxEnableWdt (
  SIL_CONTEXT    *SilContext,
  CORE_WATCHDOG  *CpuWdt
  )
{
  uint8_t           BumpUpIndex;
  uint16_t          CpuWdtTimeBase;
  uint16_t          CpuWdtCountSel;
  uint64_t          CpuWdtTime;
  uint64_t          DfCcmWdtTime;
  SIL_RESERVED_UNION_0005   CpuWdtCfg;
  bool              WdtEnable;
  DF_IP2IP_API *DfIp2IpAPi;
  SIL_STATUS        Status;

  if (xUslIsBsp ()) {
    /*
     * SilGetIp2IpApi moved inside if (xUslIsBsp ()) since this function is used by APs.
     * APs hang the system upon calling tracepoint functions, and SilGetIp2IpApi contains
     * tracepoints.
     */
    Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpAPi);
    assert(Status == SilPass);
    WdtEnable = DfIp2IpAPi->DfGetWdtInfo(SilContext,
      &DfCcmWdtTime,
      0x3,
      0x44
      );
    if (WdtEnable && (CpuWdt->CpuWdtEn)) {

      CpuWdtTimeBase = CpuWdt->CpuWdtTimeOut & 0xFF;
      CpuWdtCountSel = (CpuWdt->CpuWdtTimeOut & 0xFF00) >> 8;
      assert(CpuWdtTimeBase < NUM_OF_CPU_WDT_TIME_BASE);
      assert(CpuWdtCountSel < NUMBER_OF_CPU_WDT_CNT_SEL);

      // RESTRICTION: When both CPU WDT & DF WDT are enable,
      // the DF CCM WDT timeout must be greater than the CPU WDT timeout limit
      CpuWdtTime = (uint64_t) (CpuWdtTimeBaseDecode[CpuWdtTimeBase] *
        CpuWdtCountSelDecode[CpuWdtCountSel]);
      if (DfCcmWdtTime <= CpuWdtTime) {
        // slow down the CPU WDT to at least the value of the DF CCM WDT
        CpuWdt->CpuWdtEn = false; // Disable CPU WDT
        while ((CpuWdtCountSel != MIN_CPU_WDT_COUNT_SEL) ||
          (CpuWdtTimeBase != MIN_CPU_WDT_TIME_BASE)) {
          if ((CpuWdtCountSel == MIN_CPU_WDT_COUNT_SEL) &&
            (CpuWdtTimeBase != MIN_CPU_WDT_TIME_BASE)) {
            // Try to change TimeBase
            CpuWdtCountSel = MAX_CPU_WDT_COUNT_SEL;
            CpuWdtTimeBase = MIN_CPU_WDT_TIME_BASE;
          } else {
            // Try to change CountSel
            for (BumpUpIndex = (NUMBER_OF_CPU_WDT_CNT_SEL - 1); BumpUpIndex > 0;
              BumpUpIndex--) {
              if (CpuWdtCountSelBumpUp[BumpUpIndex] == CpuWdtCountSel) {
                CpuWdtCountSel = CpuWdtCountSelBumpUp[BumpUpIndex - 1];
                break;
              }
            }
          }

          CpuWdtTime = (uint64_t) (CpuWdtTimeBaseDecode[CpuWdtTimeBase] *
            CpuWdtCountSelDecode[CpuWdtCountSel]);

          if (DfCcmWdtTime > CpuWdtTime) {
            // Slow down succeed, enable CPU WDT
            CpuWdt->CpuWdtEn = true;
            CpuWdt->CpuWdtTimeOut = CpuWdtTimeBase | (CpuWdtCountSel << 8);
            break;
          }
        }
      }
    }
  }

  CpuWdtCfg.Value = xUslRdMsr(MSR_CPU_WDT_CFG);
  if (CpuWdt->CpuWdtEn) {
    CpuWdtCfg.Field.field_bit_0 = 1;
    CpuWdtCfg.Field.field_bits_1_to_2 = CpuWdt->CpuWdtTimeOut & 0xFF;
    CpuWdtCfg.Field.field_bits_3_to_6 = (CpuWdt->CpuWdtTimeOut & 0xFF00) >> 8;
    if (CpuWdt->CpuWdTmrCfgSeverity != 0xFF) {
      CpuWdtCfg.Field.field_bits_7_to_9 = CpuWdt->CpuWdTmrCfgSeverity;
    }
  } else {
    CpuWdtCfg.Field.field_bit_0 = 0;
    CpuWdtCfg.Field.field_bits_1_to_2 = 0;
    CpuWdtCfg.Field.field_bits_3_to_6 = 0;
  }

  xUslWrMsr(MSR_CPU_WDT_CFG, CpuWdtCfg.Value);
}

/**
 * CcxGetCacWeights
 *
 * @brief This routine is executed by BSP, gets CAC weights from SMU and data will be
 *        shared with APs through ApLaunchGlobalData
 *
 * @param  SilContext    A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @param  CacWeights    holds CAC weights read from SMU
 *
 */
SIL_STATUS
CcxGetCacWeights (
  SIL_CONTEXT  *SilContext,
  uint64_t     *CacWeights
  )
{
  SMU_IP2IP_API *SmuApi;
  SIL_STATUS    Status;

  Status = SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return SilAborted;
  }

  Status = SmuApi->SmuReadCacWeights(SilContext, MAX_CAC_WEIGHT_NUM, CacWeights);
  return Status;
}

/**
 * CcxSetCacWeights
 * @brief This routine sets all CAC weights
 *
 * @param CacWeights
 *
 */
void CcxSetCacWeights (uint64_t *CacWeights)
{
  uint8_t  WeightIndex;
  uint64_t LocalMsr;

  if (xUslIsComputeUnitPrimary ()) {
    LocalMsr = xUslRdMsr(MSRxC0011074);
    xUslWrMsr(MSRxC0011074, 0);

    for (WeightIndex = 0; WeightIndex < MAX_CAC_WEIGHT_NUM; WeightIndex++) {
      xUslWrMsr(MSRxC0011076, WeightIndex);
      xUslWrMsr(MSRxC0011077, CacWeights[WeightIndex]);
    }

    xUslWrMsr(MSRxC0011074, (LocalMsr | BIT_64(63)));
  }
}

/**
 * CcxInitializeCpb
 * @brief Initializes Core Performance Boost.
 *
 * @details Write to Core::X86::Msr::HWCR[CpbDis].
 * This routine is only executed on the BSP. APs will sync applicable
 * settings through ApMsrSyncList.
 *
 * @param[in]  CpbEnable - boolean for desired state of the feature
 *
 * @retval     None
 *
 */
void
CcxInitializeCpb (
  uint8_t CpbEnable
  )
{
  if (CpbEnable == 0) {
    xUslMsrOr(MSR_HWCR, BIT_64(25));
  }
}
