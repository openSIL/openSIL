/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Ccx.c
 * @brief openSIL CCX IP initialization.
 *
 */

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include <Ccx.h>
#include <CcxCacheInit.h>
#include <CcxApic.h>
#include <MsrReg.h>
#include <xUslCcxRoles.h>
#include <CommonLib/CpuLib.h>
#include <CoreTopologyService.h>
#include <CcxMicrocodePatch.h>
#include <CCX/Common/CcxReg.h>
#include <CcxCmn2Rev.h>
#include <DF/DfIp2Ip.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>
#include <Nbio/NbioIp2Ip.h>
#include <FCH/Common/FchCommon.h>

#define SNP_DISABLE                            0   // SNP is disable no need to allocate memory for RMP table
#define SNP_ENTIRE_MEMORY_ENABLE               1   // SNP is enable need to allocate entire memory size for RMP table
#define SNP_CUSTOM_MEMORY_ENABLE               2   // SNP is enable need to allocate custom memory size for RMP table
#define CONVERT_MB_INTO_BYTES                 20   // covert MB into bytes
#define SIZE_1MB                      0x00100000
#define SIZE_16KB                     0x00004000
#define SIZE_16MB                     0x01000000

static void UpdateCcxOutputData (
  SIL_CONTEXT        *SilContext,
  CCXCLASS_DATA_BLK  *CcxData
  );

/// Default values for Ccx configuration
static const CCXCLASS_DATA_BLK gCcxConfigData = {
  .CcxInputBlock = {
    .AmdApicMode = CONFIG_CCX_APIC_MODE,
    .AmdDownCoreMode = CONFIG_CCX_DOWNCORE_MODE,
    .AmdSmtMode = CONFIG_CCX_SMT_MODE,
    .AmdCcdMode = CONFIG_CCX_CCD_MODE,
    .ApSyncFlag = 0x0,
    .AmdCoreDisCcd[0] = 0x0,// filled by Host
    .AmdCoreDisCcd[1] = 0x0,
    .AmdCoreDisCcd[2] = 0x0,
    .AmdCoreDisCcd[3] = 0x0,
    .AmdCoreDisCcd[4] = 0x0,
    .AmdCoreDisCcd[5] = 0x0,
    .AmdCoreDisCcd[6] = 0x0,
    .AmdCoreDisCcd[7] = 0x0,
    .AmdCoreDisCcd[8] = 0x0,
    .AmdCoreDisCcd[9] = 0x0,
    .AmdCoreDisCcd[10] = 0x0,
    .AmdCoreDisCcd[11] = 0x0,
    .AmdGameMode = false,
    .AmdIbrsEn = false,         // used in reg table matching
    .AmdBranchSampling = false, // used in reg table matching
    .AmdVmplEnable = true,      // used by SNP (mem encrypt)
    .L1BurstPrefetch = false,
    .AmdL1StreamPrefetcher = true, // used in MiscInit:567
    .AmdL1StridePrefetcher = true,
    .AmdL1RegionPrefetcher = true,
    .AmdL2StreamPrefetcher = true,
    .AmdL2UpDownPrefetcher = true,
    //WDT items are set in ApGlobalData & used indirectly in CcxEnableWdt by the cores
    .AmdCpuWdtEn = false,
    .AmdCpuWdtTimeout = 0x100,///< set timeout to 2.682s to make sure it's smaller than DF WDT.
    .AmdCpuWdtSeverity = 0xFF,
    .AmdCStateMode = CONFIG_CCX_CSTATE_ENABLE,
    .AmdCc6Ctrl = CONFIG_CCX_CSTATE_CC6_ENABLE,
    .AmdCStateIoBaseAddress = CONFIG_CCX_CSTATE_IO_ADDR,
    .AmdCpbMode = CONFIG_CCX_CPB_ENABLE,
    .AmdHardwarePrefetchMode = 0x0,
    .AmdSoftwarePrefetchMode = 0x0,
    .AmdSmee = CONFIG_CCX_SMEE_ENABLE,
    .AmdHmkee = CONFIG_CCX_HMKEE_ENABLE,
    .RedirectForReturnDis = 0xFF,
    .OpCacheCtrl = 0xFF,
    .StreamingStoresCtrl = 0xFF,
    .EnSpecStFill = true,
    .EnableRMSS = CONFIG_CCX_RMSS_ENABLE,
    .EnableSvmAVIC = false,
    .IbsHardwareEn = false,
    .EnableAvx512 = 0xFF,
    .EnableSvmX2AVIC = false,
    .MonMwaitDis = 0xFF,
    .AmdFixedMtrr250 = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr258 = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr259 = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr268 = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr269 = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26A = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26B = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26C = 0x1818181818181818,
    .AmdFixedMtrr26D = 0x1818181818181818,
    .AmdFixedMtrr26E = 0x1818181818181818,
    .AmdFixedMtrr26F = 0x1818181818181818,
    .AmdSnpMemCover = 0x0,
    .DisableWcSpecConfig = 0x1,
    .PspBiosBinEntryInfo = {
      .PspBiosBinBase = CONFIG_PSP_BIOS_BIN_BASE,
      .PspBiosBinSize = CONFIG_PSP_BIOS_BIN_SIZE,
    },
    .IommuSupport = CONFIG_IOMMU_SUPPORT,
    .AmdPstatePolicy = 0x0,
    .AmdSplitRmpTable = 0x0,
    .AmdCpuSpeculativeStoreMode = 0xFF,
    .CapsuleUpdateDetected = false,
    .SvmEnable = true,
    .SvmLock = true,
    .P0Setting = CONFIG_CCX_P0_SETTING,
    .P0Freq = CONFIG_CCX_P0_FREQ,
    .PxAutoFreq = CONFIG_CCX_AUTO_FREQ,
    .P0Vid32 = CONFIG_CCX_P0_VID32,
    .PxAutoVid = CONFIG_CCX_AUTO_VID,
    .CcxLoadUcodePatch = true,
  },
  .CcxOutputBlock = {
    .AmdApicMode = 0xFF,
    .AmdDownCoreMode = 0x0,
    .AmdIsSnpSupported = false,
    .AmdRmpTableSize = 0x0,
    .ProcessorId = 0x0000,
  }
};

/**
 * CcxProgramTablesAtReset
 *
 * @brief   Program Ccx register Tables at different time points
 *
 * @param   SilContext            A context structure through which host firmware defined data
 *                                can be passed to openSIL. The host firmware is responsible
 *                                for initializing the SIL_CONTEXT structure.
 * @param   SleepType             Input of sleep type resuming (S3/S0)
 * @param   InitializedCriteria   entry criteria if already exists
 * @param   CcxConfigData         Ccx input block data
 *
 */
static void
CcxProgramTablesAtReset (
  SIL_CONTEXT                             *SilContext,
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA  *ApLaunchGlobalData,
  CCXCLASS_DATA_BLK                        *CcxConfigData
  )
{
  REGISTER_TABLE_TIME_POINT Timepoint;

  Timepoint = (ApLaunchGlobalData->SleepType == 0x03) ?
    AmdRegisterTableTpAfterApLaunchSecureS3 : AmdRegisterTableTpAfterApLaunch;

  SetRegistersFromTablesAtGivenTimePoint(SilContext,
    (REGISTER_TABLE_AT_GIVEN_TP *) &ApLaunchGlobalData->CcxRegTableListAtGivenTP[0],
    Timepoint,
    (ENTRY_CRITERIA *) &ApLaunchGlobalData->ResetTableCriteria,
    CcxConfigData
    );
}

/**
 * InitializeCcxAndLaunchAps
 *
 * @brief   Initialize Ccx IP
 *
 * @details Based Ccx config data initializes Ccx Cache, downcore, resetTables.
 *
 * @param   SilContext      A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible
 *                          for initializing the SIL_CONTEXT structure.
 * @param   CcxConfigData   Ccx input and output data block
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  SilDeviceError if current CPU is not BSP
 * @retval  SilResetRequestColdImm if ccx requested immediate cold reset
 * @retval  SilResetRequestWarmImm if ccx requested immediate warm reset
 * @retval  SilNotFound if IP transfer table was not found
 * @retval  SilInvalidParameter if DfGetSystemInfo failed
 */
static SIL_STATUS
InitializeCcxAndLaunchAps (
  SIL_CONTEXT                       *SilContext,
  CCXCLASS_DATA_BLK *CcxConfigData,
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP
  )
{
  SIL_STATUS         Status = SilPass;
  uint32_t           Socket;
  uint32_t           Die;
  uint32_t           Ccd;
  uint32_t           Ccx;
  uint32_t           Core;
  uint32_t           Thread;
  uint32_t           NumberOfDies = 0;
  uint32_t           NumberOfSockets;
  uint32_t           NumberOfCcds;
  uint32_t           NumberOfComplexes;
  uint32_t           NumberOfCores;
  uint32_t           NumberOfThreads;
  uint8_t            ApicMode;
  uint32_t           ApicId;
  uint32_t           ApNumBfLaunch = 0x0;
  volatile uint16_t  *ApSyncFlag = NULL;
  uint8_t            i = 0;
  DF_IP2IP_API       *DfApi;
  NBIO_IP2IP_API     *NbioApi;
  CCX_XFER_TABLE     *CcxXfer;
  PSP_SMM_HDR_DATA  *PspSmmHdrData = NULL;
  APOB_IP2IP_API     *ApobIp2IpApi;
  CCX_IOMMU_FEATURE_INFO                        CcxIommuFeatureData;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA ApLaunchGlobalData;
  CCX_DATA_BLOCK        *CcxDataBlk = NULL;

  void              *ApStartupBuffer = NULL;
  uint8_t           MemoryContentCopy[AP_TEMP_BUFFER_SIZE];
  uint32_t          MemoryContentCopySize = AP_TEMP_BUFFER_SIZE;

  uint8_t           ApStack[AP_STACK_SIZE];

  AP_MTRR_SETTINGS ApMtrrSyncList[] =
  {
    { MSR_MMIO_CFG_BASE, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX64K_00000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX16K_80000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX16K_A0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_C0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_C8000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_D0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_D8000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_E0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_E8000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_F0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4K_F8000, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE0, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK0, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE1, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK1, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE2, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK2, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE3, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK3, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE4, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK4, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE5, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK5, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE6, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK6, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_BASE7, 0x0000000000000000  },
    { AMD_MTRR_VARIABLE_MASK7, 0x0000000000000000  },
    { 0x000002FF, 0x0000000000000000  },
    { CPU_LIST_TERMINAL, 0x0000000000000000  }
  };

  if (SilGetCommon2RevXferTable(SilContext, SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return SilNotFound;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioApi));
  if (Status != SilPass) {
    return Status;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfApi);
  if (Status != SilPass) {
    return Status;
  }
  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  CcxDataBlk = (CCX_DATA_BLOCK *) xUslFindStructure(SilContext, SilId_CcxClass, 1);
  if (CcxDataBlk == NULL) {
    // Could not find the CCX data block
    return SilNotFound;
  }

  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  ApLaunchGlobalData.SilBaseAddress = SilContext->SilMemBaseAddress;
  CCX_TRACEPOINT(SIL_TRACE_INFO, "Sil Context Info. Sil Mem add 0x%x\n", SilContext->SilMemBaseAddress);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "Sil Context Info. Apob Add 0x%x\n", SilContext->ApobBaseAddress);

  ApLaunchGlobalData.CcxRegTableListAtGivenTP = CcxRegTableListAtGivenTP;

  ApLaunchGlobalData.SleepType = 0;
  if (SilFchReadSleepType () == 0x3 || CcxConfigData->CcxInputBlock.CapsuleUpdateDetected) {
    ApLaunchGlobalData.SleepType = 3;
  }

  if (ApLaunchGlobalData.SleepType != 3) {
    CcxXfer->CcxCacheInit ();
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxCacheInit Done.\n");

    CcxXfer->SetBrandString(SilContext);
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxSetBrandString Done\n");
  } else if (CcxConfigData->CcxInputBlock.CapsuleUpdateDetected) {
    // Before S3 resume capsule mode, FW/OS may set some vMTRRs.
    // It's better to re-init vMTRR to avoid FW codes load to the vMTRR regions.
    Status = InitializeVariableMtrrs ();
    if (Status != SilPass) {
      CCX_TRACEPOINT(SIL_TRACE_INFO, "InitializeVariableMtrrs failed.\n");
      return Status;
    }
    CCX_TRACEPOINT(SIL_TRACE_INFO, "InitializeVariableMtrrs Done\n");
  }

  CcxIommuFeatureData.Iommu_Sup = 1;
  CcxIommuFeatureData.XT_Sup = 1;

  Status = DfApi->DfGetSystemInfo(SilContext, &NumberOfSockets, NULL, NULL, NULL, NULL);
  if (Status == SilPass) {
    Status = DfApi->DfGetProcessorInfo(SilContext, 0, &NumberOfDies, NULL);
  }
  if (Status != SilPass) {
    assert(Status == SilPass);
    return SilInvalidParameter;
  }

  //Get NumberOfComplexes, NumberOfCores, NumberOfThreads of Socket 0
  GetCoreTopologyOnDieMax(SilContext, 0, 0, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);

  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfSockets=%d\n", NumberOfSockets);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfDies=%d\n", NumberOfDies);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfCcds=%d\n", NumberOfCcds);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfComplexes=%d\n", NumberOfComplexes);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfCores=%d\n", NumberOfCores);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "NumberOfThreads=%d\n", NumberOfThreads);

  //When IOMMU is disabled, IRQs can't be remapped, so no X2APIC support
  if (!CcxConfigData->CcxInputBlock.IommuSupport) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "  IOMMU disabled, AmdApicMode is changed to 0x%X\n", xApicMode);
    CcxConfigData->CcxInputBlock.AmdApicMode =
      CcxConfigData->CcxOutputBlock.AmdApicMode = xApicMode;
  }

  switch (CcxConfigData->CcxInputBlock.AmdApicMode) {
  case xApicMode:

    // xApic mode and total thread count is >= 255,
    // downcore to support xApic mode
    if ((NumberOfSockets * NumberOfCcds * NumberOfComplexes *
      NumberOfCores * NumberOfThreads) >= XAPIC_ID_MAX) {
      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "AmdDownCoreMode is changed to CCX_DOWN_CORE_7_0\n"
        );

      CcxConfigData->CcxInputBlock.AmdCcdMode =
        CcxConfigData->CcxOutputBlock.AmdCcdMode = 8;
      CcxConfigData->CcxInputBlock.AmdDownCoreMode =
        CcxConfigData->CcxOutputBlock.AmdDownCoreMode = 10;
    }
    break;
  case x2ApicMode:
    if ((CcxIommuFeatureData.Iommu_Sup == 0) || (CcxIommuFeatureData.XT_Sup == 0)) {
      ApicMode = xApicMode;
      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "IOMMU disabled or has no XT_Sup, Fall back AmdApicMode to 0x%X\n",
        ApicMode
        );
      // Export the ApicMode to host firmware
      CcxConfigData->CcxInputBlock.AmdApicMode = ApicMode;
      CcxConfigData->CcxOutputBlock.AmdApicMode = ApicMode;
    }
    break;
  case ApicAutoMode:
    ApicMode = CcxXfer->GetAmdApicMode(NumberOfSockets,
      NumberOfCcds,
      NumberOfComplexes,
      NumberOfCores,
      NumberOfThreads
      );

    CcxConfigData->CcxInputBlock.AmdApicMode =
      CcxConfigData->CcxOutputBlock.AmdApicMode = ApicMode;
    CCX_TRACEPOINT(SIL_TRACE_INFO,
      " ApicAutoMode AmdApicMode is changed to 0x%X\n",
      ApicMode
      );
    break;
  case ApicCompatibilityMode:
    ApicMode = ApicCompatibilityMode;
    // set xApic mode when total thread count is < 255,
    // this must be done before CcxProgramTablesAtReset ()
    ApicId = CcxXfer->CalcLocalApic(SilContext,
      (NumberOfSockets - 1),
      (NumberOfDies - 1),
      (NumberOfCcds - 1),
      (NumberOfComplexes - 1),
      (NumberOfCores - 1),
      (NumberOfThreads - 1)
      );
    if (ApicId < XAPIC_ID_MAX) {
      ApicMode = xApicMode;
    }

    CCX_TRACEPOINT(SIL_TRACE_INFO,
      "AmdApicMode is changed to 0x%X\n",
      ApicMode
      );

    CcxConfigData->CcxInputBlock.AmdApicMode =
      CcxConfigData->CcxOutputBlock.AmdApicMode = ApicMode;
    break;
  default:
    // ERROR - Undefined Apic mode
    assert(false);
    break;
  }

  CcxXfer->StoreDowncoreCoreCount(SilContext, CcxConfigData);

  if (ApLaunchGlobalData.SleepType == 3) {
    PspSmmHdrData = (PSP_SMM_HDR_DATA *)(uintptr_t)xUslRdMsr(MSR_SMM_ADDR); //SMMADDR_ADDRESS - Start of TSEG
    ApSyncFlag = (volatile uint16_t *)&PspSmmHdrData->ApSyncFlag;
    assert(ApSyncFlag != NULL);
    *ApSyncFlag = 0;
    PspSmmHdrData->ApStackTop = PspSmmHdrData->PspSmmRsmMemInfo.StackPtr + \
      PspSmmHdrData->PspSmmRsmMemInfo.BspStackSize + \
      PspSmmHdrData->PspSmmRsmMemInfo.ApStackSize;
    PspSmmHdrData->CcxEarlyInit = (uint64_t)(uintptr_t)CcxSetMca; // Call for APs when they are launched.
    CcxSetMca ();                                                 // Call for BSP.
  } else {
    ApLaunchGlobalData.ApMtrrSyncList = ApMtrrSyncList;
    ApLaunchGlobalData.SizeOfApMtrr = sizeof (ApMtrrSyncList);
    CCX_TRACEPOINT(SIL_TRACE_INFO,
      "SizeOfApMtrr=%d\n",
      ApLaunchGlobalData.SizeOfApMtrr
      );
    memcpy((void *)(uintptr_t) &ApLaunchGlobalData.ApMsrSyncList[0],
      (void *) CcxXfer->GetApMsrSyncList (),
      sizeof (AP_MSR_SYNC) * MAX_MSR_SYNC
      );

    // Load microcode on the BSP
    if (CcxConfigData->CcxInputBlock.CcxLoadUcodePatch) {
      LoadMicrocodePatch(SilContext, (uint64_t *) &ApLaunchGlobalData.UcodePatchAddr);
    }
    if (xUslRdMsr(MSR_PATCH_LEVEL) == 0) {
      CCX_TRACEPOINT(SIL_TRACE_WARNING, "CPU microcode is not loaded.\n");
    }

    // Reset Table
    GetPlatformFeatures((PLATFORM_FEATS *) &ApLaunchGlobalData.ResetTableCriteria.PlatformFeats,
      CcxConfigData->CcxInputBlock.AmdApicMode,
      CcxConfigData->CcxInputBlock.AmdIbrsEn,
      CcxConfigData->CcxInputBlock.AmdBranchSampling
      );
    GetPerformanceFeatures((PROFILE_FEATS *) &ApLaunchGlobalData.ResetTableCriteria.ProfileFeats);
    GetSocLogicalIdOnCurrentCore((SOC_LOGICAL_ID *) &ApLaunchGlobalData.ResetTableCriteria.SocLogicalId);
    GetCoreLogicalIdOnCurrentCore((CORE_LOGICAL_ID *) &ApLaunchGlobalData.ResetTableCriteria.CoreLogicalId);

    CcxProgramTablesAtReset(SilContext, &ApLaunchGlobalData, CcxConfigData);

    // L1, L2 HW Stream Prefetcher
    CcxXfer->CcxPrefetcher(&(CcxConfigData->CcxInputBlock));

    // MSR misc setting
    CcxSetMiscMsrs(SilContext, &(CcxConfigData->CcxInputBlock));

    // Get input setting for CPU WDT
    ApLaunchGlobalData.CpuWdt.CpuWdtEn = CcxConfigData->CcxInputBlock.AmdCpuWdtEn;
    ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut = CcxConfigData->CcxInputBlock.AmdCpuWdtTimeout;
    if (ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut == 0xFFFF) {
      // If it's 'Auto', set timeout to 2.682s to make sure it's smaller than DF WDT
      ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut = 0x100;
    }

    ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity = CcxConfigData->CcxInputBlock.AmdCpuWdtSeverity;
    if ((ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity != 0xFF) &&
      (ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity > 5)) {
      // 0 ~ 5 is valid
      ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity = 0xFF;
    }

    // Enable watchdog timer
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxEnableWdt\n");
    CcxEnableWdt(SilContext, (CORE_WATCHDOG *) &ApLaunchGlobalData.CpuWdt);

    // Enable SMEE
    CcxEnableSmeeHmkee(CcxConfigData->CcxInputBlock.AmdSmee, CcxConfigData->CcxInputBlock.AmdHmkee);

    // Mca initialization
    CcxSetMca ();

    // CacWeights initialization is split into two routines as APs will not be getting
    // CacWeigts, but will be setting up CacWeights in AP flow
    ApLaunchGlobalData.SetCacWeightsEnable =
      (CcxGetCacWeights(SilContext, (uint64_t *) &ApLaunchGlobalData.CacWeights[0]) == SilPass) ? true : false;
    if (ApLaunchGlobalData.SetCacWeightsEnable) {
      CcxSetCacWeights((uint64_t *) &ApLaunchGlobalData.CacWeights[0]);
    }

    ApLaunchGlobalData.SvmEnable = CcxConfigData->CcxInputBlock.SvmEnable;
    ApLaunchGlobalData.SvmLock = CcxConfigData->CcxInputBlock.SvmLock;
    CcxInitSvm(ApLaunchGlobalData.SvmEnable, ApLaunchGlobalData.SvmLock);

    CcxXfer->CcxInitializeC6(&(CcxConfigData->CcxInputBlock));

    CcxInitializeCpb(CcxConfigData->CcxInputBlock.AmdCpbMode);

    CcxXfer->CcxInitializePrefetchMode(&(CcxConfigData->CcxInputBlock));

    for (i = 0; ApLaunchGlobalData.ApMsrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
      ApLaunchGlobalData.ApMsrSyncList[i].MsrData =
        xUslRdMsr(ApLaunchGlobalData.ApMsrSyncList[i].MsrAddr);
    }

    if (ApLaunchGlobalData.SleepType == 3) {

      ApLaunchGlobalData.AllowToLaunchNextThreadLocation = 0;
      ApSyncFlag = NULL;
      assert(false);
    } else {
      SetupApStartupRegion(&ApLaunchGlobalData,
        &ApStartupBuffer,
        (void *)ApStack,
        (void *)MemoryContentCopy,
        &MemoryContentCopySize,
        CcxConfigData,
        CcxDataBlk
        );
      ApSyncFlag = (volatile uint16_t *)(uintptr_t) ApLaunchGlobalData.AllowToLaunchNextThreadLocation;
    }
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO, "Launching APs\n");

  CCX_TRACEPOINT(SIL_TRACE_INFO, "    System has %d Socket(s)\n", NumberOfSockets);
  for (Socket = 0; Socket < NumberOfSockets; Socket++) {
    Status = DfApi->DfGetProcessorInfo(SilContext, Socket, &NumberOfDies, NULL);
    if (Status != SilPass) {
      NumberOfDies = 0;
    }
    CCX_TRACEPOINT(SIL_TRACE_INFO, "    Socket %d has %d Die(s)\n", Socket, NumberOfDies);
    for (Die = 0; Die < NumberOfDies; Die++) {
      ApobIp2IpApi->ApobGetCcdLogToPhysMap(SilContext, Socket, Die, &ApobCcdLogToPhysMap);
      Status = GetCcdCountOnDie(SilContext, &ApobCcdLogToPhysMap, Socket, Die, &NumberOfCcds);
      if (Status != SilPass) {
        NumberOfCcds = 0;
      }
      CCX_TRACEPOINT(SIL_TRACE_INFO, "    Socket %d Die %d has %d CCD(s)\n", Socket, Die, NumberOfCcds);
      for (Ccd = 0; Ccd < NumberOfCcds; Ccd++) {
        Status = GetComplexCountOnCcd(SilContext, &ApobCcdLogToPhysMap, Socket, Die, Ccd, &NumberOfComplexes);
        if (Status != SilPass) {
          NumberOfComplexes = 0;
        }
        CCX_TRACEPOINT(SIL_TRACE_INFO,
          "    Socket %d Die %d CCD %d has %d CCX(s)\n",
          Socket,
          Die,
          Ccd,
          NumberOfComplexes
          );
        for (Ccx = 0; Ccx < NumberOfComplexes; Ccx++) {
          Status = GetCoreCountOnComplex(SilContext, &ApobCcdLogToPhysMap, Socket, Die, Ccd, Ccx, &NumberOfCores);
          if (Status != SilPass) {
            NumberOfCores = 0;
          }
          CCX_TRACEPOINT(SIL_TRACE_INFO,
            "    Socket %d Die %d CCD %d CCX %d has %d Core(s)\n",
            Socket,
            Die,
            Ccd,
            Ccx,
            NumberOfCores
            );
          for (Core = 0; Core < NumberOfCores; Core++) {
            Status = GetThreadCountOnCore(SilContext,
              &ApobCcdLogToPhysMap,
              Socket,
              Die,
              Ccd,
              Ccx,
              Core,
              &NumberOfThreads
              );
            if (Status != SilPass) {
              NumberOfThreads = 0;
            }
            CCX_TRACEPOINT(SIL_TRACE_INFO,
              "    Socket %d Die %d CCD %d CCX %d Core %d has %d Thread(s)\n",
              Socket,
              Die,
              Ccd,
              Ccx,
              Core,
              NumberOfThreads
              );
            for (Thread = 0; Thread < NumberOfThreads; Thread++) {
              if (!((Socket == 0) && (Die == 0) && (Ccd == 0) && (Ccx == 0) && (Core == 0) && (Thread == 0))) {
                CCX_TRACEPOINT(SIL_TRACE_INFO,
                  "Launch socket %X die %X ccd %X complex %X core %X thread %X\n",
                  Socket,
                  Die,
                  Ccd,
                  Ccx,
                  Core,
                  Thread
                  );
                ApNumBfLaunch++;
                CcxXfer->CcxLaunchThread(SilContext, Socket, Die, Ccd, Ccx, Core, Thread);
                // Wait until the core launch
                if (ApSyncFlag != NULL) {
                  while (*ApSyncFlag != ApNumBfLaunch) {;
                  }
                  CCX_TRACEPOINT(SIL_TRACE_INFO, "Launch success.  Launching next AP.\n");
                }
                if (PspSmmHdrData != NULL) {
                  PspSmmHdrData->ApStackTop += PspSmmHdrData->PspSmmRsmMemInfo.ApStackSize;
                }
              }
            }
          }
        }
      }
    }
  }

  xUslMsrOr(MSRxC0011023, TW_CFG_COMBINED_CR0_CD);

  // Restore the data located at the reset vector
  if (ApLaunchGlobalData.SleepType != 3) {
    if (ApStartupBuffer == NULL)
      return SilOutOfBounds;

    RestoreResetVector(&ApLaunchGlobalData,
      ApNumBfLaunch,
      ApStartupBuffer,
      MemoryContentCopy,
      MemoryContentCopySize
      );
  }

  UpdateCcxOutputData(SilContext, CcxConfigData);
  CCX_TRACEPOINT(SIL_TRACE_EXIT, "Status: 0x%X\n", Status);

  return Status;
}

/**
 * InitializeCcx
 *
 * @brief   Initialize CCX IP data
 *
 * @details Fetches pointer to Ccx block data and based on that initializes
 *          Ccx Cache, downcore, resetTables based on timepoints.
 *
 * @param   SilContext                A context structure through which host firmware defined data
 *                                    can be passed to openSIL. The host firmware is responsible
 *                                    for initializing the SIL_CONTEXT structure.
 * @param   CcxRegTableListAtGivenTP  Input Ccx Register config table
 *
 * @return  SIL_STATUS
 *
 * @retval  SilDeviceError if InitializeCcxData returns error
 * @retval  SilResetRequestColdImm is ccx requested immediate cold reset
 * @retval  SilResetRequestWarmImm is ccx requested immediate warm reset
 *
 */
SIL_STATUS
InitializeCcx (
  SIL_CONTEXT                       *SilContext,
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP
  )
{
  SIL_STATUS        Status;
  CCXCLASS_DATA_BLK *LclCcxDataBlk = NULL;

  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclCcxDataBlk = (CCXCLASS_DATA_BLK *) xUslFindStructure(SilContext, SilId_CcxClass, 0);
  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL Ccx found blk at: 0x%x \n",
    LclCcxDataBlk
    );

  if (LclCcxDataBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  Status = InitializeCcxAndLaunchAps(SilContext, LclCcxDataBlk, CcxRegTableListAtGivenTP);
  return Status;
}

/**
 * CcxClassSetInputBlk
 * @brief Establish CCX config data
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS CcxClassSetInputBlk (
  SIL_CONTEXT  *SilContext
  )
{
  CCXCLASS_DATA_BLK *CcxConfigData;
  CCX_DATA_BLOCK    *CcxInitData;

  CcxConfigData = (CCXCLASS_DATA_BLK *)SilCreateInfoBlock(SilContext,
    SilId_CcxClass,
    sizeof (CCXCLASS_DATA_BLK),
    CCXCLASS_INSTANCE,
    CCXCLASS_MAJOR_REV,
    CCXCLASS_MINOR_REV
    );
  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL CcxClassSetInputBlk at: 0x%x \n",
    CcxConfigData
    );

  if (CcxConfigData == NULL) {
    return SilAborted;
  }

  CcxInitData = (CCX_DATA_BLOCK *)SilCreateInfoBlock(SilContext,
    SilId_CcxClass,
    sizeof (CCX_DATA_BLOCK),
    CCX_DATA_BLOCK_INSTANCE,
    CCXCLASS_MAJOR_REV,
    CCXCLASS_MINOR_REV
    );
  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL CcxClassSetCcxInitDataBlk at: 0x%x \n",
    CcxInitData
    );

  if (CcxInitData == NULL) {
    return SilAborted;
  }

  memset(CcxInitData, 0, sizeof (CCX_DATA_BLOCK));

  // fill CCX structure with defaults
  memcpy((void *)CcxConfigData, &gCcxConfigData, sizeof (CCXCLASS_DATA_BLK));
  CcxConfigData->CcxOutputBlock.ProcessorId = (uint16_t)xUslGetProcessorId ();

  return SilPass;
}

/**
 * RegSettingBeforeLaunchingNextThread
 *
 * @brief Necessary register setting before launching next thread
 *
 */
void
RegSettingBeforeLaunchingNextThread (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  bool IsSmtDisabled;

  IsSmtDisabled = xUslIsSmtDisabled ();

  // Thread 0s are launched before thread 1s.  We want the last thread of
  // a core to set the bit. When SMT is disabled, thread 0 should set the bit.
  if (IsSmtDisabled || !xUslIsComputeUnitPrimary ()) {
    xUslMsrOr(MSRxC0011023, BIT_64(49));
  }
}

/**
 * ApEntryPointInC
 * @brief This routine is the C entry point for APs and is called from ApAsmCode
 * @param ApLaunchGlobalData AP launch global data
 *
 */
NASM_ABI
void
ApEntryPointInC (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  SIL_CONTEXT *SilContext = NULL;
  SIL_CONTEXT Temp[1];
  SilContext = (SIL_CONTEXT *)&Temp;
  if (ApLaunchGlobalData != NULL) {
    if (SilContext != NULL) {
      SilContext->SilMemBaseAddress = ApLaunchGlobalData->SilBaseAddress;
      // Enable watchdog timer
      if (xUslIsComputeUnitPrimary ()) {
        CcxEnableWdt(SilContext, (CORE_WATCHDOG *) &ApLaunchGlobalData->CpuWdt);
      }

      // Skip loading microcode patch on AP if BSP's patch level is 0.
      if (ApLaunchGlobalData->BspPatchLevel != 0) {
        // Using the address saved by BSP previously
        if (ApLaunchGlobalData->UcodePatchAddr != 0) {
          xUslWrMsr(MSR_PATCH_LOADER, ApLaunchGlobalData->UcodePatchAddr);
        }
      }

      CcxProgramTablesAtReset(SilContext, ApLaunchGlobalData, NULL);

      // Mca initialization
      CcxSetMca ();

      // Cac Weights initialization
      if (ApLaunchGlobalData->SetCacWeightsEnable) {
        CcxSetCacWeights((uint64_t *) &ApLaunchGlobalData->CacWeights[0]);
      }

      // Configure SVM
      CcxInitSvm(ApLaunchGlobalData->SvmEnable, ApLaunchGlobalData->SvmLock);

      // Last step: Sync up MSRs with BSP
      CcxSyncMiscMsrs(ApLaunchGlobalData);
    }
  }
}

/**
 * GetSnpSupportStatus
 *
 * @brief   Get the SNP Support Status
 *
 * @return  SNP Support status
 *
 * @retval  false - Not SNP Support
 * @retval  true  - SNP Support
 *
 */
static
bool
GetSnpSupportStatus (void)
{
  bool SnpSupported;
  SECURE_ENCRYPTION_EAX SecureEncryptionEax;

  SnpSupported = false;

  SecureEncryptionEax.Value = xUslGetSecureEncryption ();
  if (SecureEncryptionEax.Field.SNP == 1) {
    SnpSupported = true;
  } else {
    SnpSupported = false;
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO, "SNP Support Status=%x\n", SnpSupported);

  return (SnpSupported);
}

/**
 * GetRmpTableSize
 *
 * @brief   Get the Total RAM Table Memory Size
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   CcxDataConfigData   Ccx input and output data block
 *
 */
static
void
GetRmpTableSize (
  SIL_CONTEXT       *SilContext,
  CCXCLASS_DATA_BLK *CcxDataConfigData
  )
{
  uint64_t                RmpTableSize;
  uint64_t                SnpMemSizeToCover;
  uint64_t                SnpMemSizeToCoverMax;
  uint8_t                 BytesPer4kPage;
  uint64_t                RmpTableBase;
  SECURE_ENCRYPTION_EAX   SecureEncryptionEax;
  CCX_XFER_TABLE          *CcxXfer;

  if (SilGetCommon2RevXferTable(SilContext, SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return;
  }

  RmpTableSize = 0;
  SnpMemSizeToCover = 0;
  SnpMemSizeToCoverMax = 0;
  BytesPer4kPage = 8;
  RmpTableBase = 0;

  if (CcxDataConfigData->CcxInputBlock.AmdSnpMemCover != SNP_DISABLE) {
    if (CcxDataConfigData->CcxInputBlock.AmdVmplEnable) {
      SecureEncryptionEax.Value = xUslGetSecureEncryption ();
      if (SecureEncryptionEax.Field.VMPL != 0) {
        CCX_TRACEPOINT(SIL_TRACE_INFO, "VMPL Enabled\n");
        xUslWrMsr(MSR_SYS_CFG, (xUslRdMsr(MSR_SYS_CFG) | BIT_64(25)));
        BytesPer4kPage = 16;
      }
    }
    SnpMemSizeToCover = xUslRdMsr(MSR_TOM2);

    // if custom memory size, use specified size
    if (CcxDataConfigData->CcxInputBlock.AmdSnpMemCover == SNP_CUSTOM_MEMORY_ENABLE) {
      SnpMemSizeToCoverMax = SnpMemSizeToCover;

      // AmdSnpMemSize is in MB, so convert to bytes
      SnpMemSizeToCover = (uint64_t) CcxDataConfigData->CcxInputBlock.AmdSnpMemSize;
      SnpMemSizeToCover = (SnpMemSizeToCover << CONVERT_MB_INTO_BYTES);
      if ((SnpMemSizeToCover < SIZE_16MB)) {
        SnpMemSizeToCover = SIZE_16MB;
      } else if (SnpMemSizeToCover > SnpMemSizeToCoverMax) {
        SnpMemSizeToCover = SnpMemSizeToCoverMax;
      }
    }

    // 16KB + ((Bytes/4K-page) * (# of pages [SnpMemSizeToCover/4K]))
    RmpTableSize = SIZE_16KB + ((uint64_t) BytesPer4kPage * (SnpMemSizeToCover >> 12));

    // round RMP table size up to nearest 1MB
    RmpTableSize = (RmpTableSize + SIZE_1MB - 1) & ~(SIZE_1MB - 1);

    CcxXfer->CalcRMPTableBase(SilContext,
      &RmpTableSize,
      &RmpTableBase,
      &SnpMemSizeToCover,
      CcxDataConfigData->CcxInputBlock.AmdSplitRmpTable
      );

    CCX_TRACEPOINT(SIL_TRACE_INFO, "RMP Table Memory Size =%x\n", RmpTableSize);
  } else {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "SNP DISABLE \n");
  }

  CcxDataConfigData->CcxOutputBlock.AmdRmpTableSize = RmpTableSize;
  CcxDataConfigData->CcxOutputBlock.AmdRmpTableBase = RmpTableBase;
}

/**
 * UpdateCcxOutputData
 *
 * @brief   Update the Ccx Output Data
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 * @param   CcxData     Ccx input and output data block
 *
 */
static
void
UpdateCcxOutputData (
  SIL_CONTEXT        *SilContext,
  CCXCLASS_DATA_BLK  *CcxData
  )
{
  CcxData->CcxOutputBlock.AmdIsSnpSupported = GetSnpSupportStatus ();
  GetRmpTableSize(SilContext, CcxData);
}
