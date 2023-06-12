/**
 * @file  Ccx.c
 * @brief openSIL CCX IP initialization.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include <Ccx.h>
#include <CcxCacheInit.h>
#include <CcxApic.h>
#include <CcxBrandString.h>
#include <CcxDownCoreInit.h>
#include <MsrReg.h>
#include <xUslCcxRoles.h>
#include <SMU/SmuIp2Ip.h>
#include <CommonLib/CpuLib.h>
#include <CoreTopologyService.h>
#include <CcxMicrocodePatch.h>
#include <CCX/Common/CcxReg.h>
#include <CcxCmn2Rev.h>
#include <RAS/RasDefs.h>
#include <DF/DfIp2Ip.h>

#define SNP_DISABLE                            0   // SNP is disable no need to allocate memory for RMP table
#define SNP_ENTIRE_MEMORY_ENABLE               1   // SNP is enable need to allocate entire memory size for RMP table
#define SNP_CUSTOM_MEMORY_ENABLE               2   // SNP is enable need to allocate custom memory size for RMP table
#define CONVERT_MB_INTO_BYTES                 20   // covert MB into bytes
#define SIZE_1MB                      0x00100000
#define SIZE_16KB                     0x00004000
#define SIZE_16MB                     0x01000000

static void UpdateCcxOutputData(CCXCLASS_DATA_BLK *CcxData);

/// Default values for Ccx configuration
static const CCXCLASS_DATA_BLK gCcxConfigData = {
  .CcxInputBlock = {
    .AmdApicMode                    = CONFIG_CCX_APIC_MODE,
    .AmdDownCoreMode                = 0x0,
    .AmdSmtMode                     = CONFIG_CCX_SMT_MODE,
    .AmdCcdMode                     = CONFIG_CCX_CCD_MODE,
    .ApSyncFlag                     = 0x0,
    .AmdCoreDisCcd[0]               = 0x0,
    .AmdCoreDisCcd[1]               = 0x0,
    .AmdCoreDisCcd[2]               = 0x0,
    .AmdCoreDisCcd[3]               = 0x0,
    .AmdCoreDisCcd[4]               = 0x0,
    .AmdCoreDisCcd[5]               = 0x0,
    .AmdCoreDisCcd[6]               = 0x0,
    .AmdCoreDisCcd[7]               = 0x0,
    .AmdCoreDisCcd[8]               = 0x0,
    .AmdCoreDisCcd[9]               = 0x0,
    .AmdCoreDisCcd[10]              = 0x0,
    .AmdCoreDisCcd[11]              = 0x0,
    .AmdGameMode                    = false,
    .AmdIbrsEn                      = false,
    .AmdEnvironmentFlag             = 0x0,
    .AmdBranchSampling              = false,
    .AmdVmplEnable                  = true,
    .AmdCStateMode                  = CONFIG_CCX_CSTATE_ENABLE,       //bool, enables CStates
    .AmdCc6Ctrl                     = CONFIG_CCX_CSTATE_CC6_ENABLE,   //bool, enables CC6
    .AmdCStateIoBaseAddress         = CONFIG_CCX_CSTATE_IO_ADDR,      //IO address for Cstate regs
    .AmdCpbMode                     = CONFIG_CCX_CPB_ENABLE,
    .AmdSmee                        = CONFIG_CCX_SMEE_ENABLE,
    .AmdReserved                    = 0xFF,
    .AmdReserved1                   = 0xFF,
    .AmdReserved2                   = 0xFF,
    .EnSpecStFill                   = true,
    .EnableSvmAVIC                  = false,
    .IbsHardwareEn                  = false,
    .EnableAvx512                   = 0xFF,
    .EnableSvmX2AVIC                = false,
    .MonMwaitDis                    = 0xFF,
    .AmdFixedMtrr250                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr258                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr259                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr268                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr269                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26A                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26B                = 0xFFFFFFFFFFFFFFFF,
    .AmdFixedMtrr26C                = 0x1818181818181818,
    .AmdFixedMtrr26D                = 0x1818181818181818,
    .AmdFixedMtrr26E                = 0x1818181818181818,
    .AmdFixedMtrr26F                = 0x1818181818181818,
    .AmdSnpMemCover                 = 0x0,
    .DisableWcSpecConfig            = 0x1,
    .AmdPstatePolicy                = 0x0,
    .AmdSplitRmpTable               = 0x0,
    .AmdReserved3                   = 0xFF,
    .AmdCpuPauseDelay               = 0xFF,
  },
  .CcxOutputBlock = {
    .AmdApicMode                    = 0xFF,
    .AmdDownCoreMode                = 0x0,
    .AmdIsSnpSupported              = false,
    .AmdRmpTableSize                = 0x0,
    .ProcessorId                    = 0x0000,
  }
};

uint64_t mApStartupVector;
uint8_t  mMemoryContentCopy[AP_TEMP_BUFFER_SIZE] = {0};
volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA mApLaunchGlobalData = {0};

static volatile AP_MTRR_SETTINGS ApMtrrSyncList[] =
{
  { MSR_MMIO_Cfg_Base,        0x0000000000000000  },
  { AMD_AP_MTRR_FIX64k_00000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX16k_80000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX16k_A0000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_C0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_C8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_D0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_D8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_E0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_E8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_F0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_F8000,  0x0000000000000000  },
  { 0x00000200,               0x0000000000000000  },
  { 0x00000201,               0x0000000000000000  },
  { 0x00000202,               0x0000000000000000  },
  { 0x00000203,               0x0000000000000000  },
  { 0x00000204,               0x0000000000000000  },
  { 0x00000205,               0x0000000000000000  },
  { 0x00000206,               0x0000000000000000  },
  { 0x00000207,               0x0000000000000000  },
  { 0x00000208,               0x0000000000000000  },
  { 0x00000209,               0x0000000000000000  },
  { 0x0000020A,               0x0000000000000000  },
  { 0x0000020B,               0x0000000000000000  },
  { 0x0000020C,               0x0000000000000000  },
  { 0x0000020D,               0x0000000000000000  },
  { 0x0000020E,               0x0000000000000000  },
  { 0x0000020F,               0x0000000000000000  },
  { 0x000002FF,               0x0000000000000000  },
  { CPU_LIST_TERMINAL                             }
};

static volatile AP_MSR_SYNC ApMsrSyncList[] =
{
  { MSR_CPUID_NAME_STRING0,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4Brandstring
  { MSR_CPUID_NAME_STRING1,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4Brandstring
  { MSR_CPUID_NAME_STRING2,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4Brandstring
  { MSR_CPUID_NAME_STRING3,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4Brandstring
  { MSR_CPUID_NAME_STRING4,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4Brandstring
  { MSR_CPUID_NAME_STRING5,         0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4BrandString
  { SIL_RESERVED2_926,              0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { SIL_RESERVED2_899,              0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { MSR_SYS_CFG,                    0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { SIL_RESERVED2_912,              0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },
  { MSR_CSTATE_ADDRESS,             0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4InitializeC6
  { MSR_HWCR,                       0x0000000000000000, 0xFFFFFFFFFFFFFFFF          },  // CcxZen4InitializeCpb
  { SIL_RESERVED2_919,              0x0000000000000000, BIT_64(15)                  },
  { 0xC00110DD,                     0x0000000000000000, BIT_64(13)                  },  // AmdEnableSvmAVIC
  { 0xC00110DD,                     0x0000000000000000, BIT_64(18)                  },  // AmdEnableSvmX2AVIC
  { MSR_HWCR,                       0x0000000000000000, BIT_64(9)                   },  // AmdMonMwaitDis
  { CPU_LIST_TERMINAL,              0,                  0                           }
};

/**
 * CcxProgramTablesAtReset
 *
 * @brief   Program Ccx register Tables at different time points
 *
 * @param   SleepType             Input of sleep type resuming (S3/S0)
 * @param   InitializedCriteria   entry criteria if already exists
 * @param   CcxConfigData         Ccx input block data
 *
 */
static void
CcxProgramTablesAtReset (
  uint8_t           SleepType,
  ENTRY_CRITERIA    *InitializedCriteria,
  CCXCLASS_DATA_BLK *CcxConfigData
  )
{
  REGISTER_TABLE_TIME_POINT Timepoint;

  Timepoint = (SleepType == 0x03) ?
    AmdRegisterTableTpAfterApLaunchSecureS3 : AmdRegisterTableTpAfterApLaunch;

  SetRegistersFromTablesAtGivenTimePoint (
    (REGISTER_TABLE_AT_GIVEN_TP *) &mApLaunchGlobalData.CcxRegTableListAtGivenTP[0],
    Timepoint,
    InitializedCriteria,
    CcxConfigData);
}

/**
 * RestoreResetVector
 * @brief This routine restores the code in the AP reset vector once all the
 * APs that were launched are done running AP code.
 *
 * @param[in] ApLaunchGlobalData Global data for AP launch
 * @param[in] TotalCoresLaunched The number of cores that were launched by the BSC
 * @param[in] ApStartupVector pointer to AP startup code
 * @param[in] MemoryContentCopy Temporarily buffer to hold reset vector to start AP.
 *
 */
static
void
RestoreResetVector (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  uint16_t TotalApCoresLaunched,
  uint64_t *ApStartupVector,
  uint8_t  *MemoryContentCopy
  )
{
  uint16_t ApSyncCoreNumber;
  ApSyncCoreNumber = 0xFFFF;
  if((ApLaunchGlobalData == NULL) || (ApStartupVector == NULL) || (MemoryContentCopy == NULL)) {
    return;
  }
  // Check whether the last core has completed running the AP Startup code
  do {
    ApSyncCoreNumber = (*(uint16_t *)(size_t)(ApLaunchGlobalData->AllowToLaunchNextThreadLocation));
  } while (ApSyncCoreNumber != TotalApCoresLaunched);

  memcpy ((void *) ((uintptr_t)((*ApStartupVector) - (AP_STARTUP_CODE_OFFSET))),
    MemoryContentCopy,
    AP_TEMP_BUFFER_SIZE);
}

/**
 * InitializeCcxAndLaunchAps
 *
 * @brief   Initialize Ccx IP
 *
 * @details Based Ccx config data initializes Ccx Cache, downcore, resetTables.
 *
 * @param   CcxConfigData   Ccx input and output data block
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  SilDeviceError if current CPU is not BSP
 * @retval  SilResetRequestColdImm if ccx requested immediate cold reset
 * @retval  SilResetRequestWarmImm if ccx requested immediate warm reset
 * @retval  SilNotFound if IP transfer table was not found
 */
static SIL_STATUS
InitializeCcxAndLaunchAps (
  CCXCLASS_DATA_BLK* CcxConfigData,
  uint32_t CcdDisMask,
  uint32_t DesiredCcdCount
  )
{
  SIL_STATUS        Status = SilPass;
  uint32_t          Socket;
  uint32_t          Die;
  uint32_t          Ccd;
  uint32_t          Ccx;
  uint32_t          Core;
  uint32_t          Thread;
  uint32_t          NumberOfDies;
  uint32_t          NumberOfSockets;
  uint32_t          NumberOfCcds;
  uint32_t          NumberOfCoreComplexes;
  uint32_t          NumberOfCores;
  uint32_t          NumberOfThreads;
  uint8_t           ApicMode;
  uint32_t          ApNumBfLaunch = 0x0;
  volatile uint16_t *ApSyncFlag = NULL;
  uint8_t           i = 0;
  SMU_IP2IP_API     *SmuApi;
  DF_IP2IP_API      *DfApi;
  CCX_XFER_TABLE    *CcxXfer;

  if (SilGetCommon2RevXferTable (SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return SilNotFound;
  }

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    return Status;
  }
  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfApi);
  if (Status != SilPass) {
    return Status;
  }

  CCX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (xUslIsBsp()) {

    if (mApLaunchGlobalData.SleepType != 3) {
      CcxCacheInit ();
      CCX_TRACEPOINT (SIL_TRACE_INFO, "CcxCacheInit Done.\n");

      CcxSetBrandString ();
      CCX_TRACEPOINT (SIL_TRACE_INFO, "CcxSetBrandString Done\n");
    }

    DfApi->DfGetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);
    //Get NumberOfCoreComplexes, NumberOfCores, NumberOfThreads of Socket 0
    GetCoreTopologyOnDie (0,
                          0,
                          &NumberOfCcds,
                          &NumberOfCoreComplexes,
                          &NumberOfCores,
                          &NumberOfThreads
                        );

    CCX_TRACEPOINT (SIL_TRACE_INFO, "NumberOfCcds=%d\n", NumberOfCcds);
    CCX_TRACEPOINT (SIL_TRACE_INFO, "NumberOfCoreComplexes=%d\n", NumberOfCoreComplexes);
    CCX_TRACEPOINT (SIL_TRACE_INFO, "NumberOfCores=%d\n", NumberOfCores);
    CCX_TRACEPOINT (SIL_TRACE_INFO, "NumberOfThreads=%d\n", NumberOfThreads);

    switch (CcxConfigData->CcxInputBlock.AmdApicMode) {
      case xApicMode:
        // xApic mode and total thread count is >= 255,
        // downcore to support xApic mode
        if ((NumberOfSockets * NumberOfCcds * NumberOfCoreComplexes *
             NumberOfCores * NumberOfThreads) >= XAPIC_ID_MAX) {
          CCX_TRACEPOINT (SIL_TRACE_INFO,
              "AmdDownCoreMode is changed to CCX_DOWN_CORE_7_0\n");

          CcxConfigData->CcxInputBlock.AmdDownCoreMode  =
          CcxConfigData->CcxOutputBlock.AmdDownCoreMode = CCX_DOWN_CORE_7_0;
        }
        break;
      case x2ApicMode:
        break;
      case ApicAutoMode:
        ApicMode = CcxXfer->GetAmdApicMode(NumberOfSockets,
                                              NumberOfCcds,
                                              NumberOfCoreComplexes,
                                              NumberOfCores,
                                              NumberOfThreads);

        CcxConfigData->CcxInputBlock.AmdApicMode  =
        CcxConfigData->CcxOutputBlock.AmdApicMode = ApicMode;
        CCX_TRACEPOINT (SIL_TRACE_INFO,
            " ApicAutoMode AmdApicMode is changed to 0x%X\n",ApicMode);
        break;
      case ApicCompatibilityMode:
        ApicMode = ApicCompatibilityMode;
        // set xApic mode when total thread count is < 255,
        // this must be done before CcxProgramTablesAtReset ()
        if ((NumberOfSockets * NumberOfCcds * NumberOfCoreComplexes *
             NumberOfCores * NumberOfThreads) < XAPIC_ID_MAX) {
          ApicMode = xApicMode;
        }

        CCX_TRACEPOINT (SIL_TRACE_INFO,
            "AmdApicMode is changed to 0x%X\n",ApicMode);

        CcxConfigData->CcxInputBlock.AmdApicMode  =
        CcxConfigData->CcxOutputBlock.AmdApicMode = ApicMode;
        break;
      default:
        // ERROR - Undefined Apic mode
        assert (false);
        break;
    }

    Status = CcxDownCoreInit (CcxConfigData, CcdDisMask, DesiredCcdCount);
    CCX_TRACEPOINT (SIL_TRACE_INFO, "DownCoreInit Status 0x%X\n", Status);

    mApLaunchGlobalData.ApMtrrSyncList = ApMtrrSyncList;
    mApLaunchGlobalData.SizeOfApMtrr = sizeof (ApMtrrSyncList);
    CCX_TRACEPOINT (
        SIL_TRACE_INFO,
        "SizeOfApMtrr=%d\n",
        mApLaunchGlobalData.SizeOfApMtrr
        );
    mApLaunchGlobalData.ApMsrSyncList = ApMsrSyncList;

    // Load microcode on the BSP
    LoadMicrocodePatch ((uint64_t *) &mApLaunchGlobalData.UcodePatchAddr);
    if (xUslRdMsr (MSR_PATCH_LEVEL) == 0) {
      CCX_TRACEPOINT (SIL_TRACE_WARNING, "CPU microcode is not loaded.\n");
    }

    // Reset Table
    GetPlatformFeatures (
        (PLATFORM_FEATS *) &mApLaunchGlobalData.ResetTableCriteria.PlatformFeats,
        CcxConfigData->CcxInputBlock.AmdApicMode,
        CcxConfigData->CcxInputBlock.AmdIbrsEn,
        CcxConfigData->CcxInputBlock.AmdEnvironmentFlag,
        CcxConfigData->CcxInputBlock.AmdBranchSampling
        );
    GetPerformanceFeatures (
        (PROFILE_FEATS *) &mApLaunchGlobalData.ResetTableCriteria.ProfileFeats);
    GetSocLogicalIdOnCurrentCore (
      (SOC_LOGICAL_ID *) &mApLaunchGlobalData.ResetTableCriteria.SocLogicalId);
    GetCoreLogicalIdOnCurrentCore (
      (CORE_LOGICAL_ID *) &mApLaunchGlobalData.ResetTableCriteria.CoreLogicalId);

    CcxProgramTablesAtReset (
        mApLaunchGlobalData.SleepType,
        (ENTRY_CRITERIA *) &(mApLaunchGlobalData.ResetTableCriteria),
        CcxConfigData);

    RegSettingBeforeLaunchingNextThread ();

    // MSR misc setting
    CcxSetMiscMsrs (&(CcxConfigData->CcxInputBlock));

    // Mca initialization
    CcxSetMca ();

    // CacWeights initialization is split into two routines as APs will not be getting
    // CacWeigts, but will be setting up CacWeights in AP flow
    CcxGetCacWeights ((uint64_t*) &mApLaunchGlobalData.CacWeights[0]);
    CcxSetCacWeights ((uint64_t*) &mApLaunchGlobalData.CacWeights[0]);

    CcxInitializeC6 (&(CcxConfigData->CcxInputBlock));

    CcxInitializeCpb (CcxConfigData->CcxInputBlock.AmdCpbMode);

    for (i = 0; mApLaunchGlobalData.ApMsrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
      mApLaunchGlobalData.ApMsrSyncList[i].MsrData =
          xUslRdMsr (mApLaunchGlobalData.ApMsrSyncList[i].MsrAddr);
    }

    SetupApStartupRegion (
        &mApLaunchGlobalData,
        &mApStartupVector,
        mMemoryContentCopy,
        CcxConfigData);

    ApSyncFlag = (volatile uint16_t *)(uintptr_t) mApLaunchGlobalData.AllowToLaunchNextThreadLocation;

    CCX_TRACEPOINT (SIL_TRACE_INFO, "Launching APs\n");

    for (Socket = 0; Socket < NumberOfSockets; Socket++) {
      DfApi->DfGetProcessorInfo (Socket, &NumberOfDies, NULL);
      for (Die = 0; Die < NumberOfDies; Die++) {
        GetCoreTopologyOnDie (Socket, Die, &NumberOfCcds,
          &NumberOfCoreComplexes, &NumberOfCores, &NumberOfThreads);
        for (Ccd = 0; Ccd < NumberOfCcds; Ccd++) {
          for (Ccx = 0; Ccx < NumberOfCoreComplexes; Ccx++) {
            for (Core = 0; Core < NumberOfCores; Core++) {
              for (Thread = 0; Thread < NumberOfThreads; Thread++) {
                if (!((Socket == 0) &&
                      (Die == 0)    &&
                      (Ccd == 0)    &&
                      (Ccx == 0)    &&
                      (Core == 0)   &&
                      (Thread == 0))) {
                  CCX_TRACEPOINT (SIL_TRACE_INFO,
                      "Launch socket %X die %X ccd %X complex %X core %X thread %X\n",
                      Socket,
                      Die,
                      Ccd,
                      Ccx,
                      Core,
                      Thread);
                  ApNumBfLaunch++;
                  SmuApi->SmuLaunchThread (
                    Socket,
                    Die,
                    Ccd,
                    Ccx,
                    Core,
                    Thread
                    );
                  // Wait until the core launch
                  if (ApSyncFlag != NULL) {
                    while (*ApSyncFlag != ApNumBfLaunch);
                    CCX_TRACEPOINT (SIL_TRACE_INFO, "going to launch next AP.\n");
                  }
                }
              }
            }
          }
        }
      }
    }

    // Enable SMEE
    CcxEnableSmee (CcxConfigData->CcxInputBlock.AmdSmee);

    // Restore the data located at the reset vector
    if (mApLaunchGlobalData.SleepType != 3) {
      RestoreResetVector (&mApLaunchGlobalData,
        (uint16_t) ApNumBfLaunch,
        &mApStartupVector,
        mMemoryContentCopy);
    }
  } else {
    CCX_TRACEPOINT (SIL_TRACE_INFO, "SilDeviceError. \n");
    Status = SilDeviceError;
  }

  UpdateCcxOutputData(CcxConfigData);
  CCX_TRACEPOINT (SIL_TRACE_EXIT, "Status: 0x%X\n", Status);

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
 * @param   CcxRegTableListAtGivenTP  Input Ccx Register config table
 * @param   CcdDisMask
 * @param   DesiredCcdCount
 *
 * @return  SIL_STATUS
 *
 * @retval  SilDeviceError if InitialzeCcxData returns error
 * @retval  SilResetRequestColdImm is ccx requested immediate cold reset
 * @retval  SilResetRequestWarmImm is ccx requested immediate warm reset
 *
 */
SIL_STATUS
InitializeCcx (
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP,
  uint32_t                          CcdDisMask,
  uint32_t                          DesiredCcdCount
  )
{
  SIL_STATUS        Status;
  CCXCLASS_DATA_BLK *LclCcxDataBlk = NULL;

  CCX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclCcxDataBlk = (CCXCLASS_DATA_BLK *) SilFindStructure (SilId_CcxClass, 0);
  CCX_TRACEPOINT (SIL_TRACE_INFO,
    "openSIL Ccx found blk at: 0x%x \n",
    LclCcxDataBlk);

  if (LclCcxDataBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  mApLaunchGlobalData.CcxRegTableListAtGivenTP = CcxRegTableListAtGivenTP;
  Status = InitializeCcxAndLaunchAps (LclCcxDataBlk, CcdDisMask, DesiredCcdCount);
  return Status;
}

/**
 * CcxClassSetInputBlk
 * @brief Establish CCX config data
 *
 *
 * @return SIL_STATUS
 */
SIL_STATUS CcxClassSetInputBlk (void)
{
  CCXCLASS_DATA_BLK *CcxConfigData;

  CcxConfigData = (CCXCLASS_DATA_BLK *)SilCreateInfoBlock (SilId_CcxClass,
                                  sizeof(CCXCLASS_DATA_BLK),
                                  CCXCLASS_INSTANCE,
                                  CCXCLASS_MAJOR_REV,
                                  CCXCLASS_MINOR_REV);
  CCX_TRACEPOINT (SIL_TRACE_INFO,
    "openSIL CcxClassSetInputBlk at: 0x%x \n",
    CcxConfigData);

  if (CcxConfigData == NULL) {
    return SilAborted;
  }

  // fill CCX structure with defaults
  memcpy ((void *)CcxConfigData, &gCcxConfigData, sizeof(CCXCLASS_DATA_BLK));
  CcxConfigData->CcxOutputBlock.ProcessorId = xUslGetProcessorId();

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
  void
  )
{
  bool IsSmtDisabled;

  IsSmtDisabled = (xUslIsSmtDisabled() == 0);

  if (IsSmtDisabled || !xUslIsComputeUnitPrimary ()) {
    xUslMsrOr (SIL_RESERVED_9, BIT_64(49));
  }

  if ((mApLaunchGlobalData.ResetTableCriteria.CoreLogicalId.CoreFamily & AMD_CORE_FAMILY_19_ZEN4) != 0) {
    if (!xUslIsComputeUnitPrimary ()) {
      // MSR0000_0048[2] = 1
      xUslMsrOr (MSR_SPEC_CTRL, BIT_64(2));
    }
  }
}

/**
 * ApEntryPointInC
 * @brief This routine is the C entry point for APs and is called from ApAsmCode
 * @param ApLaunchGlobalData AP launch global data
 *
 */
void
ApEntryPointInC (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  // Skip loading microcode patch on AP if BSP's patch level is 0.
  if (ApLaunchGlobalData->BspPatchLevel != 0) {
    // Using the address saved by BSP previously
    if (ApLaunchGlobalData->UcodePatchAddr != 0) {
      if (xUslIsComputeUnitPrimary ()) {
        xUslWrMsr (MSR_PATCH_LOADER, ApLaunchGlobalData->UcodePatchAddr);
      }
    }
  }

  CcxProgramTablesAtReset (ApLaunchGlobalData->SleepType,
      (ENTRY_CRITERIA *) &(ApLaunchGlobalData->ResetTableCriteria),
      NULL);

  // Mca initialization
  CcxSetMca ();

  // Cac Weights initialization
  CcxSetCacWeights ((uint64_t*) &ApLaunchGlobalData->CacWeights[0]);

  // Last step: Sync up MSRs with BSP
  CcxSyncMiscMsrs (ApLaunchGlobalData);
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

  SecureEncryptionEax.Value = xUslGetSecureEncryption();
  if (SecureEncryptionEax.Field.SevSnp == 1) {
    SnpSupported = true;
  } else {
    SnpSupported = false;
  }

  CCX_TRACEPOINT (SIL_TRACE_INFO, "SEV SNP Support Status=%x\n", SnpSupported);

  return (SnpSupported);
}

/**
 * GetRmpTableSize
 *
 * @brief   Get the Total RAM Table Memory Size
 *
 * @param   CcxDataConfigData   Ccx input and output data block
 *
 */
static
void
GetRmpTableSize (
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

  if (SilGetCommon2RevXferTable (SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return;
  }

  RmpTableSize         = 0;
  SnpMemSizeToCover    = 0;
  SnpMemSizeToCoverMax = 0;
  BytesPer4kPage       = 16;
  RmpTableBase         = 0;

  if (CcxDataConfigData->CcxInputBlock.AmdSnpMemCover != SNP_DISABLE) {
    if (CcxDataConfigData->CcxInputBlock.AmdVmplEnable) {
      SecureEncryptionEax.Value = xUslGetSecureEncryption();
      if (SecureEncryptionEax.Field.VMPL != 0) {
        CCX_TRACEPOINT (SIL_TRACE_INFO, "VMPL Enabled\n");
        xUslWrMsr (MSR_SYS_CFG, (xUslRdMsr (MSR_SYS_CFG) | BIT_64(25)));
      } else {
        CCX_TRACEPOINT (SIL_TRACE_INFO, "VMPL Disabled - not supported, return.\n");
        return;
      }

      SnpMemSizeToCover = xUslRdMsr (MSR_TOM2);

      // if custom memory size, use specified size
      if (CcxDataConfigData->CcxInputBlock.AmdSnpMemCover == SNP_CUSTOM_MEMORY_ENABLE) {
        SnpMemSizeToCoverMax = SnpMemSizeToCover;
        // AmdSnpMemSize is in MB, so convert to bytes
        SnpMemSizeToCover = (uint64_t) CcxDataConfigData->CcxInputBlock.AmdSnpMemSize;
        SnpMemSizeToCover = (SnpMemSizeToCover << CONVERT_MB_INTO_BYTES);
        if ((SnpMemSizeToCover == 0)||(SnpMemSizeToCover < SIZE_16MB)) {
          SnpMemSizeToCover = SIZE_16MB;
        } else if (SnpMemSizeToCover > SnpMemSizeToCoverMax) {
          SnpMemSizeToCover = SnpMemSizeToCoverMax;
        }
      }
    }

    // 16KB + ((Bytes/4K-page) * (# of pages [SnpMemSizeToCover/4K]))
    RmpTableSize = SIZE_16KB + ((uint64_t) BytesPer4kPage * (SnpMemSizeToCover >> 12));

    // round RMP table size up to nearest 1MB
    RmpTableSize = (RmpTableSize + SIZE_1MB - 1) & ~(SIZE_1MB - 1);

    CcxXfer->CalcRMPTableBase(&RmpTableSize,&RmpTableBase,&SnpMemSizeToCover,
                                    CcxDataConfigData->CcxInputBlock.AmdSplitRmpTable);

    CCX_TRACEPOINT (SIL_TRACE_INFO, "RMP Table Memory Size =%x\n", RmpTableSize);
  } else {
    CCX_TRACEPOINT (SIL_TRACE_INFO, "SNP DISABLE \n");
  }

  CcxDataConfigData->CcxOutputBlock.AmdRmpTableSize = RmpTableSize;
  CcxDataConfigData->CcxOutputBlock.AmdRmpTableBase = RmpTableBase;
}

/**
 * UpdateCcxOutputData
 *
 * @brief   Update the Ccx Output Data
 *
 * @param   CcxData   Ccx input and output data block
 *
 */
static
void
UpdateCcxOutputData (
  CCXCLASS_DATA_BLK *CcxData
  )
{
  CcxData->CcxOutputBlock.AmdIsSnpSupported = GetSnpSupportStatus();
  GetRmpTableSize(CcxData);
}
