/**
 * @file  CcxMiscInit.c
 * @brief CCX Miscellaneous feature initialization.
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Ccx.h>
#include <CcxCmn2Rev.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <DF/DfIp2Ip.h>
#include <RAS/Common/Ras.h>

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
    xUslMsrAndThenOr (
        ApLaunchGlobalData->ApMsrSyncList[i].MsrAddr,
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
 * @param CcxInputBlock CCX input data block
 *
 */
void
CcxSetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint64_t        LocalMsrRegister;
  uint8_t         LocalAmdReserved;
  uint8_t         LocalAmdReserved1;
  uint8_t         LocalAmdReserved2;
  bool            LocalEnableSvmAVIC;
  bool            LocalIbsHardwareEn;
  CCX_XFER_TABLE  *CcxXfer;

  if (SilGetCommon2RevXferTable (SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return;
  }

  LocalAmdReserved          = CcxInputBlock->AmdReserved;
  LocalAmdReserved1         = CcxInputBlock->AmdReserved1;
  LocalAmdReserved2         = CcxInputBlock->AmdReserved2;
  LocalEnableSvmAVIC        = CcxInputBlock->EnableSvmAVIC;
  LocalIbsHardwareEn        = CcxInputBlock->IbsHardwareEn;

  // Workaround to enable GCC build
  unsigned long tempWa = 0x0;
  tempWa += (unsigned long) LocalAmdReserved;
  tempWa += (unsigned long) LocalAmdReserved1;
  tempWa += (unsigned long) LocalAmdReserved2;
  tempWa += (unsigned long) LocalEnableSvmAVIC;
  tempWa += (unsigned long) LocalIbsHardwareEn;
  // end workaround
   // Force recalc of TSC on all threads after loading patch
  LocalMsrRegister = xUslRdMsr (MSR_PSTATE_DEF_ADDRESS);
  xUslWrMsr (MSR_PSTATE_DEF_ADDRESS, LocalMsrRegister);

  // MSR_C001_10DD[13]
  if (LocalEnableSvmAVIC) {
    xUslMsrOr (0xC00110DD, (uint64_t) BIT_64(13));
  } else {
    xUslMsrAnd (0xC00110DD, ~((uint64_t) BIT_64(13)));
  }

  // MSR_C001_10DF[36]
  xUslMsrAnd (0xC00110DF, ~((uint64_t) BIT_64(36)));

  xUslMsrAnd (SIL_RESERVED2_919, ~((uint64_t) BIT_64(15)));

  CcxXfer->SetMiscMsrs (CcxInputBlock);
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
      case AMD_AP_MTRR_FIX64k_00000:
        if (CcxInputBlock->AmdFixedMtrr250 != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr250;
        }
        break;
      case AMD_AP_MTRR_FIX16k_80000:
        if (CcxInputBlock->AmdFixedMtrr258 != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr258;
        }
        break;
      case AMD_AP_MTRR_FIX16k_A0000:
        if (CcxInputBlock->AmdFixedMtrr259 != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr259;
        }
        break;
      case AMD_AP_MTRR_FIX4k_C0000:
        if (CcxInputBlock->AmdFixedMtrr268 != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr268;
        }
        break;
      case AMD_AP_MTRR_FIX4k_C8000:
        if (CcxInputBlock->AmdFixedMtrr269 != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr269;
        }
        break;
      case AMD_AP_MTRR_FIX4k_D0000:
        if (CcxInputBlock->AmdFixedMtrr26A != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26A;
        }
        break;
      case AMD_AP_MTRR_FIX4k_D8000:
        if (CcxInputBlock->AmdFixedMtrr26B != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26B;
        }
        break;
      case AMD_AP_MTRR_FIX4k_E0000:
        if (CcxInputBlock->AmdFixedMtrr26C != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26C;
        }
        break;
      case AMD_AP_MTRR_FIX4k_E8000:
        if (CcxInputBlock->AmdFixedMtrr26D != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26D;
        }
        break;
      case AMD_AP_MTRR_FIX4k_F0000:
        if (CcxInputBlock->AmdFixedMtrr26E != 0xFFFFFFFFFFFFFFFF) {
          ApMtrrSettingsList->MsrData = CcxInputBlock->AmdFixedMtrr26E;
        }
        break;
      case AMD_AP_MTRR_FIX4k_F8000:
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
 * CcxEnableSmee
 *
 * @brief This routine enables secure memory encryption
 * @param SmeeEnable Secure Memory Encryption Enable flag
 */
void CcxEnableSmee (bool SmeeEnable)
{
  if (SmeeEnable) {
    xUslMsrOr (MSR_SYS_CFG, BIT_64(23)); ///< BIT23 enables SME (secure memory encryption enable)
  }
}

/**
 * CcxGetCacWeights
 *
 * @brief This routine is executed by BSP, gets CAC weights from SMU and data will be
 *        shared with APs through ApLaunchGlobalData
 *
 * @param[in] CacWeights holds CAC weights read from SMU
 *
 */
SIL_STATUS
CcxGetCacWeights (uint64_t *CacWeights) {
  SMU_IP2IP_API *SmuApi;

  SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if(SmuApi == NULL) {
    assert (SmuApi != NULL);
    return SilAborted;
  }

  SmuApi->SmuReadCacWeights (MAX_CAC_WEIGHT_NUM, CacWeights);
  return SilPass;
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
    LocalMsr = xUslRdMsr (SIL_RESERVED2_909);
    xUslWrMsr (SIL_RESERVED2_909, 0);

    for (WeightIndex = 0; WeightIndex < MAX_CAC_WEIGHT_NUM; WeightIndex++) {
      xUslWrMsr (SIL_RESERVED2_915, WeightIndex);
      xUslWrMsr (SIL_RESERVED2_901, CacWeights[WeightIndex]);
    }

    xUslWrMsr (SIL_RESERVED2_909, (LocalMsr | BIT_64(63)));  }
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
    /* Bit 25 ('CpdDis') is a 'set=1 to disable' type of control.*/
    xUslMsrOr (MSR_HWCR, BIT_64(25));
  }
}
