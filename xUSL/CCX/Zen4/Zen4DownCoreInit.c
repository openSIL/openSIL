/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Zen4DownCoreInit.c
 * @brief CCX down core and SMT initialization
 * Reduces the number of threads in the system, if desired.
 *
 */
#include <SilCommon.h>
#include <DF/DfIp2Ip.h>
#include <SMU/SmuIp2Ip.h>
#include "Zen4DownCoreInit.h"
#include <SocServices.h>
#include <CcxClass-api.h>
#include "Ccx.h"
#include <APOB/Common/ApobCmn.h>
#include <CoreTopologyService.h>
#include <string.h>
#include <APOB/ApobIp2Ip.h>
#include <ProjSocConst.h>
#include <CCX/Common/CcxCmn2Rev.h>



/**
 * GetZen4DesiredCcdCount
 *
 * @brief   Get the desired ccd count and the ccd disable mask
 *
 * @details This function will use the CcdMode input to calculate and return
 *          desired ccd count and ccd disable mask.
 *
 * @param   CcdMode         Input of the CcdMode requested.
 * @param   CcdDisMask      Output of the disable mask for the requested ccd mode
 * @param   DesiredCcdCount Output of the core count for the requested ccd mode
 *
 * @returns void
 */
static
void
GetZen4DesiredCcdCount (
  SIL_CONTEXT   *SilContext,
  uint8_t       CcdMode,
  uint32_t      *CcdDisMask,
  uint32_t      *DesiredCcdCount
  )
{
  CCX_XFER_TABLE     *CcxXfer;

  if (SilGetCommon2RevXferTable(SilContext, SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return;
  }

  switch (CcdMode) {
  case CCD_MODE_1_CCD:
    if (CcxXfer->IsOneCcdModeSupported ()) {
      *DesiredCcdCount = 1;      // 1 CCD enabled
      *CcdDisMask = 0x00000FFE;
    }
    break;
  default:
    if (CcdMode != CCD_MODE_AUTO) {
      CCX_TRACEPOINT(SIL_TRACE_INFO, "Unsupported CCD Mode %d\n", CcdMode);
    }
    *DesiredCcdCount = 0xFFFFFFFF;
    *CcdDisMask = 0x0;
    break;
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "Zen4: DesiredCcdCount 0x%X CcdDisMask 0x%X\n",
    *DesiredCcdCount,
    *CcdDisMask
    );
}

/**
 * CcxDownCoreInit
 *
 * @brief   This function initializes the software downcoring feature.
 *
 * @details CCDMode defines the desired CCD count, DownCoreMode defines desired core count.
 *          ccd_down and ccd_presence is read via SocServices, and based on desired core and ccd count,
 *          downing coring is performed by writing into ccd0 downcore address and physical ccd number as offset.
 *          there are below conditions for downcoring:
 *          CCDs/Cores can be removed, if:
 *          - it has not been previously downcored
 *          - number of CCDs/Cores requested is less than current enabled CCDs/Cores.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   CcxConfigData       Ccx input block data
 *
 * @return  SIL_STATUS
 */
static
SIL_STATUS
CcxDownCoreInit (
  SIL_CONTEXT       *SilContext,
  CCXCLASS_DATA_BLK *CcxConfigData
  )
{
  uint32_t           Socket;
  uint32_t           Die;
  uint32_t           CcdCount;
  uint32_t           ComplexCount;
  uint32_t           CoreCount;
  uint32_t           ThreadCount;
  uint32_t           DieCount;
  uint32_t           SocketCount;
  uint32_t           DesiredCoreCount;
  uint32_t           AvailableCoreMap;
  uint32_t           AvailableCoreCount;
  uint32_t           DownCoreBitMap[PROJ_CCX_MAX_DIES_PER_SOCKET * PROJ_MAX_CCD_DIES_PER_SOCKET];
  bool               DownCoreBitMapAvailable;
  uint32_t           SystemDieLoop;
  uint32_t           CoreDisByFuseCount;
  uint32_t           CoreDisByFuseCountInCcx;
  uint32_t           CcdPresentByFuseCount;
  uint32_t           CcdDownByFuseCount;
  uint32_t           CoreDisMask;
  uint32_t           MaxCcdsPerSocket;
  uint32_t           CcdLoop;
  uint32_t           CcxLoop;
  uint32_t           CoreLoop;
  uint32_t           DisMask;
  uint32_t           ApobInstanceId;
  uint32_t           SmuArg[6];
  uint32_t           ThrdPerCoreFuse;
  uint32_t           CoreDisFuse[PROJ_CCX_MAX_DIES_PER_SOCKET * PROJ_MAX_CCD_DIES_PER_SOCKET];
  uint32_t           CoreDisFuseSize;
  uint32_t           CoreDisMapLength;
  uint32_t           CcdDownFuse;
  uint32_t           CcdPresentFuse;
  uint32_t           CcdDisMask;
  uint32_t           DesiredCcdCount;
  uint32_t           SoftDowncoreAddr;
  uint32_t           EnCoreLimit;
  uint32_t           MaxCorePerCcx;
  uint8_t            CcdMode;
  uint8_t            DownCoreMode;
  uint8_t            SmtMode;
  uint8_t            PhyCcx;
  uint8_t            PhyCore;
  uint8_t            PhysCcdNumber[PROJ_MAX_SOCKETS_SUPPORTED]
  [PROJ_CCX_MAX_DIES_PER_SOCKET][PROJ_CCX_MAX_DIES_PER_SOCKET * PROJ_MAX_CCD_DIES_PER_SOCKET];
  bool               CcdAttainable;
  bool               CoreAttainable;
  bool               IssueReset;
  bool               AlreadyDownCored;
  bool               Met;
  bool               SmtEnabledByFuse[PROJ_CCX_MAX_DIES_PER_SOCKET * PROJ_MAX_CCD_DIES_PER_SOCKET];
  uint32_t           SmtEnabledByFuseSize;
  bool               GameMode;
  bool               SocHasPhysicalCcds;
  PCI_ADDR           PciAddress;
  SMC_RESULT         SmcStatus;
  SMU_IP2IP_API      *SmuApi;
  DF_IP2IP_API       *DfApi;
  SIL_STATUS         SilStatus;
  APOB_IP2IP_API     *ApobIp2IpApi;
  APOB_SOC_DIE_INFO  SocMaxDieInfo;
  CCX_XFER_TABLE     *CcxXfer;

  if (SilGetCommon2RevXferTable(SilContext, SilId_CcxClass, (void **)(&CcxXfer)) != SilPass) {
    return SilNotFound;
  }

  SilStatus = SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi);
  if (SilStatus != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    return SilStatus;
  }
  SilStatus = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)&DfApi);
  if (SilStatus != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "DF API not found!\n");
    return SilStatus;
  }
  SilStatus = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if (SilStatus != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "APOB API not found!\n");
    return SilStatus;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  MaxCcdsPerSocket = (SocMaxDieInfo.MaxSocDiesPerSocketValue *
    SocMaxDieInfo.MaxSocCcdsPerDieValue);
  MaxCorePerCcx = SocMaxDieInfo.MaxSocCoresPerComplexValue;
  IssueReset = false;
  CoreDisMask = 0;
  DesiredCoreCount = 0xFFFFFFFF;
  ThrdPerCoreFuse = 2;
  ThreadCount = ThrdPerCoreFuse;
  AlreadyDownCored = false;
  Met = true;
  DownCoreBitMapAvailable = false;
  SocHasPhysicalCcds = true;

  memset(DownCoreBitMap, 0, sizeof (DownCoreBitMap));
  memset(PhysCcdNumber, 0xFF, sizeof (PhysCcdNumber));// Init all values to 0xFF
  memset(SmuArg, 0, sizeof (SmuArg));

  DownCoreMode = CcxConfigData->CcxInputBlock.AmdDownCoreMode;
  SmtMode = CcxConfigData->CcxInputBlock.AmdSmtMode;
  CcdMode = CcxConfigData->CcxInputBlock.AmdCcdMode;

  assert(DownCoreMode < CCX_DOWN_CORE_MAX);
  assert(SmtMode <= 1);

  GameMode = false;
  CCX_TRACEPOINT(SIL_TRACE_INFO, "CCD mode: 0x%x\n", CcdMode);

  GetZen4DesiredCcdCount(SilContext,
    CcxConfigData->CcxInputBlock.AmdCcdMode,
    &CcdDisMask,
    &DesiredCcdCount
    );

  if (SocHasPhysicalCcds && CcxXfer->IsGameModeSupported () && CcxConfigData->CcxInputBlock.AmdGameMode) {
    DesiredCcdCount = 1;
    CcdDisMask = 0x00FB;
    GameMode = true;
  } else if ((CcdMode != CCD_MODE_AUTO) && SocHasPhysicalCcds) {

    if (0xFFFFFFFF == DesiredCcdCount) {
      CCX_TRACEPOINT(SIL_TRACE_ERROR, "Invalid CCD mode requested!\n");
    }
  }

  if (DownCoreMode != CCX_DOWN_CORE_AUTO) {
    switch (DownCoreMode) {
    case CCX_DOWN_CORE_1:
      DesiredCoreCount = 1;
      CoreDisMask = 0x000000FE;
      break;
    case CCX_DOWN_CORE_2:
      DesiredCoreCount = 2;
      CoreDisMask = 0x000000FC;
      break;
    case CCX_DOWN_CORE_3:
      DesiredCoreCount = 3;
      CoreDisMask = 0x000000F8;
      break;
    case CCX_DOWN_CORE_4:
      DesiredCoreCount = 4;
      CoreDisMask = 0x000000F0;
      break;
    case CCX_DOWN_CORE_5:
      DesiredCoreCount = 5;
      CoreDisMask = 0x000000E0;
      break;
    case CCX_DOWN_CORE_6:
      DesiredCoreCount = 6;
      CoreDisMask = 0x000000C0;
      break;
    case CCX_DOWN_CORE_7:
      DesiredCoreCount = 7;
      CoreDisMask = 0x00000080;
      break;
    default:
      CCX_TRACEPOINT(SIL_TRACE_ERROR, "Invalid CCX downcore mode requested!\n");
      break;
    }
  }

  CcdAttainable = true;
  CoreAttainable = true;
  SystemDieLoop = 0;

  EnCoreLimit = MaxCorePerCcx;

  DfApi->DfGetSystemInfo(SilContext, &SocketCount, NULL, NULL, NULL, NULL);

  for (Socket = 0; Socket < SocketCount; Socket++) {
    DfApi->DfGetProcessorInfo(SilContext, Socket, &DieCount, NULL);
    for (Die = 0; Die < DieCount; Die++) {
      CcdDownByFuseCount = 0;
      CcdPresentByFuseCount = 0;
      CoreDisByFuseCount = 0;

      // Get actual topology
      GetCoreTopologyOnDieMax(SilContext, Socket, Die, &CcdCount, &ComplexCount, &CoreCount, &ThreadCount);

      // Get PhysCcdNumber
      ApobInstanceId = ((uint32_t) Socket << 8) | (uint32_t) Die;
      for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
        ApobIp2IpApi->ApobGetPhysCcdNumber(SilContext,
          ApobInstanceId,
          CcdLoop,
          &PhysCcdNumber[Socket][Die][CcdLoop]
          );
      }

      // Get CCD_Down, CCD_Present, and a valid core_disable fuse
      CoreDisFuseSize = sizeof (CoreDisFuse);
      SmtEnabledByFuseSize = sizeof (SmtEnabledByFuse);
      SilStatus = SmuApi->SmuGetOpnCorePresenceEx(SystemDieLoop,
        &CcdPresentFuse,
        &CcdDownFuse,
        &CoreDisFuse[0],
        &CoreDisFuseSize,
        &CoreDisMapLength,
        &SmtEnabledByFuse[0],
        &SmtEnabledByFuseSize
        );
      if (SilStatus != SilPass) {
        SilStatus = SmuApi->SmuGetOpnCorePresence(SilContext,
          SystemDieLoop,
          &CcdPresentFuse,
          &CcdDownFuse,
          &CoreDisFuse[0],
          &CoreDisMapLength,
          &SmtEnabledByFuse[0]
          );
        assert(SilStatus == SilPass);
        for (CcdLoop = 1; CcdLoop < MaxCcdsPerSocket; CcdLoop++) {
          CoreDisFuse[CcdLoop] = CoreDisFuse[0];
          SmtEnabledByFuse[CcdLoop] = SmtEnabledByFuse[0];
        }
      }

      CcdDownByFuseCount = xUslGetSetBitCount(CcdDownFuse & CcdPresentFuse);
      CcdPresentByFuseCount = xUslGetSetBitCount(CcdPresentFuse);
      for (CcdLoop = 0; CcdLoop < MaxCcdsPerSocket; CcdLoop++) {
        CoreDisByFuseCount += xUslGetSetBitCount(CoreDisFuse[CcdLoop]);
      }
      ThrdPerCoreFuse = SmtEnabledByFuse[0] ? 2 : 1;

      for (CcdLoop = 0; CcdLoop < MaxCcdsPerSocket; CcdLoop++) {
        for (CcxLoop = 0; CcxLoop < ComplexCount; CcxLoop++) {
          CoreDisByFuseCountInCcx = xUslGetSetBitCount((CoreDisFuse[CcdLoop] >> (CcxLoop * MaxCorePerCcx)) & 0xFF);
          if (MaxCorePerCcx - CoreDisByFuseCountInCcx < EnCoreLimit) {
            EnCoreLimit = MaxCorePerCcx - CoreDisByFuseCountInCcx;
          }
        }
      }
      CCX_TRACEPOINT(SIL_TRACE_INFO, "  Socket 0x%X Die 0x%X\n", Socket, Die);
      CCX_TRACEPOINT(SIL_TRACE_INFO, "  CcdDisMask %X CoreDisMask %X\n", CcdDisMask, CoreDisMask);
      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "  CoreDisByFuseCount %X CcdDownByFuseCount %X CcdPresentByFuseCount %X\n",
        CoreDisByFuseCount,
        CcdDownByFuseCount,
        CcdPresentByFuseCount
        );
      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "  CcdCount %X ComplexCount %X CoreCount %X\n",
        CcdCount,
        ComplexCount,
        CoreCount
        );

      if (DesiredCcdCount == 0xFFFFFFFF) {
        DesiredCcdCount = CcdCount;
      }

      if (DesiredCoreCount == 0xFFFFFFFF) {
        DesiredCoreCount = CoreCount;
      }

      AvailableCoreCount = 0;
      for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
        for (CcxLoop = 0; CcxLoop < ComplexCount; CcxLoop++) {
          SilStatus = ApobIp2IpApi->ApobGetPhysComplexNumber(SilContext, ApobInstanceId, CcdLoop, CcxLoop, &PhyCcx);
          if (SilStatus != SilPass || PhyCcx == CCX_NOT_PRESENT) {
            break;
          }
          for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
            SilStatus = ApobIp2IpApi->ApobGetPhysCoreNumber(SilContext,
              ApobInstanceId,
              CcdLoop,
              CcxLoop,
              CoreLoop,
              &PhyCore
              );
            if (SilStatus != SilPass || PhyCore == CCX_NOT_PRESENT) {
              break;
            }
            AvailableCoreCount++;
          }
        }
      }

      if (((CcdCount + CcdDownByFuseCount) < CcdPresentByFuseCount) ||
        ((AvailableCoreCount + CoreDisByFuseCount) < (MaxCorePerCcx * ComplexCount * CcdCount))) {
        AlreadyDownCored = true;
        CcdDisMask = 0;
        CoreDisMask = 0;
      }

      // Get down core bitmap
      // BitMap downcore mode is only supported on 1 socket system.
      for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
        DownCoreBitMap[CcdLoop] = CcxConfigData->CcxInputBlock.AmdCoreDisCcd[CcdLoop];
        CCX_TRACEPOINT(SIL_TRACE_INFO, "DownCoreBitMap[0x%x]: 0x%x\n", CcdLoop, DownCoreBitMap[CcdLoop]);
      }

      for (CcdLoop = 0; CcdLoop < (sizeof (DownCoreBitMap) / sizeof (DownCoreBitMap[0])); CcdLoop++) {
        if (DownCoreBitMap[CcdLoop] != 0) {
          DownCoreBitMapAvailable = true;
          break;
        }
      }

      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "  DesiredCcdCount %X DesiredCoreCount %X\n",
        DesiredCcdCount,
        DesiredCoreCount
        );

      if (!DownCoreBitMapAvailable && ((CcdDisMask != 0) || (CoreDisMask != 0))) {
        // CCDs/Cores can be removed, if:
        // - it has not been previously down cored
        // - number of CCDs/Cores requested is less than current enabled CCDs/Cores
        if (DesiredCcdCount > CcdCount) {
          CCX_TRACEPOINT(SIL_TRACE_ERROR, "  Invalid CCD config requested!\n");
          CcdAttainable = false;
          CcdDisMask = 0;
        }

        if (DesiredCcdCount == CcdCount) {
          CCX_TRACEPOINT(SIL_TRACE_INFO, "  CCD config met\n");
          CcdDisMask = 0;
        } else {
          Met = false;
        }

        if (DesiredCoreCount > CoreCount) {
          CCX_TRACEPOINT(SIL_TRACE_ERROR, "  Invalid core config requested!\n");
          CoreAttainable = false;
          CoreDisMask = 0;
        }

        if (DesiredCoreCount == CoreCount) {
          CCX_TRACEPOINT(SIL_TRACE_INFO, "  Core config met\n");
          CoreDisMask = 0;
        } else {
          Met = false;
        }
      } else if (DownCoreBitMapAvailable) {
        Met = true;
        for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
          AvailableCoreMap = 0;

          for (CcxLoop = 0; CcxLoop < ComplexCount; CcxLoop++) {
            SilStatus = ApobIp2IpApi->ApobGetPhysComplexNumber(SilContext, ApobInstanceId, CcdLoop, CcxLoop, &PhyCcx);
            if (SilStatus != SilPass || PhyCcx == CCX_NOT_PRESENT) {
              break;
            }
            for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
              SilStatus = ApobIp2IpApi->ApobGetPhysCoreNumber(SilContext,
                ApobInstanceId,
                CcdLoop,
                CcxLoop,
                CoreLoop,
                &PhyCore
                );
              if (SilStatus != SilPass || PhyCore == CCX_NOT_PRESENT) {
                break;
              }
              AvailableCoreMap |= ((1 << (uint32_t) PhyCore) << (((uint32_t) PhyCcx) * MaxCorePerCcx));
            }
          }

          if (AvailableCoreMap != (~DownCoreBitMap[CcdLoop] & 0xFFFF)) {
            Met = false;
            break;
          }
        }
      }

      if (!AlreadyDownCored && !Met && CoreAttainable && CcdAttainable &&
        ((CcdDisMask != 0) || (CoreDisMask != 0) || DownCoreBitMapAvailable)) {
        for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
          SoftDowncoreAddr = SocGetSoftDowncoreAddress(PhysCcdNumber[Socket][Die][CcdLoop]);

          if (((CcdDisMask & (1 << PhysCcdNumber[Socket][Die][CcdLoop])) == 0)) {
            if (DownCoreBitMap[PhysCcdNumber[Socket][Die][CcdLoop]] != 0) {
              DisMask = DownCoreBitMap[PhysCcdNumber[Socket][Die][CcdLoop]] & 0xFFFF;
            } else {
              DisMask = CoreDisMask;
            }
          } else {
            DisMask = 0;
            for (CcxLoop = 0; CcxLoop < ComplexCount; CcxLoop++) {
              DisMask |= (0xFF << (MaxCorePerCcx * CcxLoop));
            }
          }

          if (DisMask != 0) {
            CCX_TRACEPOINT(SIL_TRACE_INFO, "  Set CCD %x# down core register %x\n", CcdLoop, DisMask);

            PciAddress.AddressValue = MAKE_SBDFO(0, Socket, 0, 0, 0);
            SmuApi->SmuRegisterWriteDie(SilContext, Socket, SoftDowncoreAddr, &DisMask);
            SmuArg[0] = 0xAA55AA55;
            SmcStatus = SmuApi->SmuServiceRequest(PciAddress, 1, SmuArg, 0);
            if (SmcStatus != SMC_Result_OK) {
              CCX_TRACEPOINT(SIL_TRACE_ERROR, "CcxDownCoreInit Smu Service Request failed.\n");
            }
            IssueReset = true;
          }
        }
      }

      SystemDieLoop++;
    }
  }

  CcxConfigData->CcxOutputBlock.AmdCcxCoreCount = EnCoreLimit;

  if (SmtMode == 0) {
    if (ThreadCount > 1) {
      if (SmuApi->SmuDisableSmt () == SilPass) {
        CCX_TRACEPOINT(SIL_TRACE_INFO, "  Disabling SMT\n");
        IssueReset = true;
      } else {
        CCX_TRACEPOINT(SIL_TRACE_INFO, "  SMT cannot be disabled\n");
      }
    }
  }

  if (AlreadyDownCored || (ThreadCount != ThrdPerCoreFuse) || !(CcxXfer->IsS3Supported ())) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "  Setting AmdAcpiS3Support to false\n");
    CcxConfigData->CcxOutputBlock.AmdAcpiS3Support = false;
  }

  /*
   * If we are already downcored and core config is still not met, something
   * is wrong.
   *
   * If CcdMode and DownCoreMode are auto (no downcore request), downcore
   * bitmap is 0, and we are already downcored, something is wrong.
   *
   * If simultaneous multi threading (SMT) is enabled and ThreadCount does
   * not match the threads per core fuse, something is wrong.
   *
   * If any of these cases are true, request cold reset.
   */
  CCX_TRACEPOINT(SIL_TRACE_INFO, "Met = %x, AlreadyDownCored = %x\n", Met, AlreadyDownCored);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "GameMode = %x, CcdMode = %x\n", GameMode, CcdMode);
  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "DownCoreMode = %x, DownCoreBitMapAvailable = %x\n",
    DownCoreMode,
    DownCoreBitMapAvailable
    );
  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "SmtMode = %x, ThreadCount = %x, ThrdPerCoreFuse = %x\n",
    SmtMode,
    ThreadCount,
    ThrdPerCoreFuse
    );
  if ((!Met && AlreadyDownCored) ||
    (!GameMode && (CcdMode == CCD_MODE_AUTO) && (DownCoreMode == CCX_DOWN_CORE_AUTO) && AlreadyDownCored &&
    !DownCoreBitMapAvailable) ||
    ((SmtMode == 1) && (ThreadCount != ThrdPerCoreFuse))) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxDownCoreInit: Requesting an immediate cold reset.\n");
    /*
     * There is no timing requirement on this reset.  It could be a deferred reset.
     * However, currently there is no other reset request originating in openSIL.
     * Therefore, we should request it immediately to avoid unnecessary code
     * execution time.
     */
    return SilResetRequestColdImm;
  }

  if (IssueReset) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxDownCoreInit: Requesting an immediate warm reset.\n");
    /*
     * There is no timing requirement on this reset.  It could be a deferred reset.
     * However, currently there is no other reset request originating in openSIL.
     * Therefore, we should request it immediately to avoid unnecessary code
     * execution time.
     */
    return SilResetRequestWarmImm;
  }

  return SilPass;
}

/**
 * Zen4StoreDowncoreCoreCountZen4
 *
 * @brief This function will set the set CcxOutputBlock.AmdCcxCoreCount which can be consumed by host firmware.
 *
 * @details For Zen4:
 *            1. First handle downcore events
 *            2. Then set AmdCcxCoreCount based on the number of cores that will remain enabled.
 *
 * @param SilContext      A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param CcxConfigData   A pointer to the CCX class data block containing the output block structure.
 */
void
Zen4StoreDowncoreCoreCount (
  SIL_CONTEXT       *SilContext,
  CCXCLASS_DATA_BLK *CcxConfigData
  )
{
  SIL_STATUS Status;

  Status = CcxDownCoreInit(SilContext, CcxConfigData);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "DownCoreInit Status 0x%X\n", Status);
}
