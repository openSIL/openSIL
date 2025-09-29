/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxInitPhx.c
 * @brief PHX Zen4 core silicon initialization code.
 *
 */

#include <SilCommon.h>
#include <CCX/CcxClass-api.h>
#include <CCX/Common/CcxApic.h>
#include <CCX/Common/CcxCmn2Rev.h>
#include <CCX/Common/CcxCacheInit.h>
#include <CCX/Zen4/CcxZen4.h>
#include <CCX/Zen4/Zen4Reg.h>
#include <CommonLib/SmnAccess.h>
#include <DF/DfIp2Ip.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/PHX/DfSilFabricRegistersPhx.h>
#include <DF/DfX/PHX/DfSilFabricInfoPhx.h>
#include <MsrReg.h>
#include <CoreTopologyService.h>
#include <SMU/SmuIp2Ip.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>
#include <Nbio/Phx/includePHX/PHX_NBIOBASEREG.h>
#include "CcxCmn2Phx.h"
#include "CcxInitPhx.h"
#include "CcxBrandString.h"

/*
 * CalcLocalApicPhx
 *
 * @brief   Returns the APIC Id based on the provided inputs
 *
 * @param  SilContext   A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 * @param  Socket       Socket # of APIC Id to calculate
 * @param  Die          Die # of APIC Id to calculate
 * @param  Ccd          CCD # of APIC Id to calculate
 * @param  Complex      Complex # of APIC Id to calculate
 * @param  Core         Core # of APIC Id to calculate
 * @param  Thread       Thread # of APIC Id to calculate
 *
 * @retval     uint32_t   Calculate Local APIC Id
 *
 */
uint32_t
CalcLocalApicPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Complex,
  uint32_t     Core,
  uint32_t     Thread
  )
{
  PMREG_INITPKG7_STRUCT    InitPkg7;
  uint32_t                 CoreEnable[4];
  uint8_t                  NumberOfSockets;
  uint8_t                  NumberOfCcds;
  uint8_t                  NumberOfComplexes;
  uint8_t                  NumberOfCores;
  uint8_t                  NumberOfThreads;
  uint8_t                  Apic16TMode;
  uint8_t                  SocketShift;
  uint8_t                  CcdShift;
  uint8_t                  ComplexShift;
  uint8_t                  CoreShift;
  uint32_t                 ApicId;
  uint8_t                  CcdIdx;
  uint8_t                  CoreIdx;
  uint8_t                  CcxIdx;
  uint8_t                  MaxCcxPerCcd;
  uint32_t                 Index;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS               Status;
  APOB_IP2IP_API           *ApobIp2IpApi;
  APOB_SOC_DIE_INFO        SocMaxDieInfo;

  MaxCcxPerCcd = 0;
  assert(Socket < 2);
  assert(Die == 0);
  assert(Ccd < 12);
  assert(Complex < 2);
  assert(Core < 8);
  assert(Thread < 2);

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  assert(Status == SilPass);

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  // some CCDs/Cores may be disabled, so find enabled one to read from
  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);
  for (Index = 0; Index < sizeof (CoreEnable) / sizeof (CoreEnable[0]); Index++) {
    CoreEnable[Index] = DfIp2IpApi->DfFabricRegisterAccRead(SilContext,
      0,
      Die,
      DFXFABx01,
      (DFXFABx144 - DFXFABx140) * Index + DFXFABx140,
      FABRIC_REG_ACC_BC
      );
  }
  for (CoreIdx = 0; (CoreIdx < ZEN4_MAX_CORES_PER_SKT) && ((CoreEnable[CoreIdx / 32]
    & (0x1 << (CoreIdx % 32))) == 0); CoreIdx++) {;
  }
  assert(CoreIdx < ZEN4_MAX_CORES_PER_SKT);

  MaxCcxPerCcd = SocMaxDieInfo.MaxSocCcxPerCcdValue;
  CcxIdx = CoreIdx / ZEN4_MAX_CORES_PER_CCD;
  CoreIdx = CoreIdx % ZEN4_MAX_CORES_PER_CCD;
  CcdIdx = CcxIdx / MaxCcxPerCcd;
  CcxIdx = CcxIdx % MaxCcxPerCcd;
  InitPkg7.Value = xUSLSmnRead(0,
    0,
    (SMN_PMREG_INITPKG7_ADDRESS + (SMN_PMREG_INITPKG7_CDD_OFFSET * CcdIdx)
    + (SMN_PMREG_INITPKG7_CCX_OFFSET * CcxIdx) + (SMN_PMREG_INITPKG7_CORE_OFFSET * CoreIdx))
    );


  NumberOfSockets = (uint8_t) InitPkg7.Field.NumOfSockets;
  NumberOfCcds = (uint8_t) InitPkg7.Field.NumOfCcds;
  NumberOfComplexes = (uint8_t) InitPkg7.Field.NumOfComplexes;
  Apic16TMode = (uint8_t) InitPkg7.Field.Apic16TMode;
  NumberOfCores = (uint8_t) InitPkg7.Field.NumOfCores;
  NumberOfThreads = xUslGetThreadsPerCore () - 1;

  assert(Socket <= NumberOfSockets);
  assert(Ccd <= NumberOfCcds);
  assert(Complex <= NumberOfComplexes);
  assert(Core <= NumberOfCores);
  assert(Thread <= NumberOfThreads);

  CoreShift = NumberOfThreads;
  ComplexShift = (xUslBitWidth(NumberOfCores)) + CoreShift;

  if (Apic16TMode == 1 && ComplexShift < 4) {
    ComplexShift = 4;
  }

  CcdShift = (xUslBitWidth(NumberOfComplexes)) + ComplexShift;
  SocketShift = (xUslBitWidth(NumberOfCcds)) + CcdShift;

  ApicId = (uint32_t) ((Socket << SocketShift) | (Ccd << CcdShift) |
    (Complex << ComplexShift) | (Core << CoreShift) | Thread);

  return ApicId;
}

/*
 * CalculateRmpTableBasePhx
 *
 * @brief   Calculate RMP Table Base address using Socket boundary.
 *
 * @param  SilContext          A context structure through which host firmware defined data
 *                             can be passed to openSIL. The host firmware is responsible
 *                             for initializing the SIL_CONTEXT structure.
 * @param  RmpTableSize        RMP table size
 * @param  RmpTableBase        RMP table Base
 * @param  SnpMemSizeToCover
 *
 */
void CalculateRmpTableBasePhx (
  SIL_CONTEXT  *SilContext,
  uint64_t     *RmpTableSize,
  uint64_t     *RmpTableBase,
  uint64_t     *SnpMemSizeToCover,
  uint8_t      SplitRmpTableValue
  )
{
  // find enough memory for RMP table to fit on MB boundary
  *RmpTableBase = *SnpMemSizeToCover;
  CCX_TRACEPOINT(SIL_TRACE_INFO, "No RMP Split. RmpTableBase = 0x%llx\n", *RmpTableBase);

}

/*
 * GetAmdApicModePhx
 *
 * @brief Get The APIC Mode
 *
 * @param NumberOfSockets   Unused for PHX
 * @param NumberOfCcds      Unused for PHX
 * @param NumberOfComplexes Unused for PHX
 * @param NumberOfCores     Unused for PHX
 * @param NumberOfThreads   Unused for PHX
 *
 * @return APIC_MODE Always x2ApicMode for Zen4
 */
APIC_MODE
GetAmdApicModePhx (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  )
{
  UNUSED(NumberOfSockets);
  UNUSED(NumberOfCcds);
  UNUSED(NumberOfComplexes);
  UNUSED(NumberOfCores);
  UNUSED(NumberOfThreads);

  // Force the APIC mode to x2Apic as it is proven to improve performance
  return x2ApicMode;
}

/**
 * CcxLaunchThreadPhx
 *
 * @brief This will start a core to fetch its first instructions from the reset
 * vector.  This service may only be called from the BSP.
 *
 * @param   Socket          Zero-based socket number of the target thread.
 * @param   Die             Zero-based die number within Socket of the target
 *                          thread.
 * @param   LogicalCcd      Zero-based logical core complex die of the target
 *                          thread.
 * @param   LogicalComplex  Zero-based logical complex number of the target
 *                          thread.
 * @param   LogicalCore     Zero-based logical core number of the target
 *                          thread.
 * @param   LogicalThread   Zero-based logical thread number of the target
 *                          thread.
 *
 * @retval SilPass              The thread was successfully launched.
 * @retval SilDeviceError       The thread has already been launched.
 * @retval SilInvalidParameter  Socket is non-existent.
 * @retval SilInvalidParameter  Die is non-existent.
 * @retval SilInvalidParameter  LogicalComplex is non-existent.
 * @retval SilInvalidParameter  LogicalThread is non-existent.
 *
 **/
SIL_STATUS
CcxLaunchThreadPhx (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       LogicalCcd,
  uint32_t       LogicalComplex,
  uint32_t       LogicalCore,
  uint32_t       LogicalThread
  )
{
  SIL_STATUS           Status;
  GNB_HANDLE           *GnbHandle;
  SIL_RESERVED_UNION_0023 ThreadEnable;
  uint32_t             NumberOfCcds;
  uint32_t             NumberOfComplexes;
  uint32_t             NumberOfCores;
  uint32_t             NumberOfLogicalThreads;
  uint32_t             ApobInstanceId;
  uint32_t             WhichBit;
  uint8_t              PhysCcdNumber;
  uint8_t              PhysComplexNumber;
  uint8_t              PhysCoreNumber;
  bool                 IsThreadEnabled;
  APOB_IP2IP_API       *ApobIp2IpApi;
  APOB_SOC_DIE_INFO    SocMaxDieInfo;
  SMU_IP2IP_API       *SmuApi;

  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "Smu API not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  Status = SmuApi->SmuGetGnbHandle(SilContext, Socket, &GnbHandle);
  if (GnbHandle != NULL) {
    ApobInstanceId = ((uint32_t) Socket << 8) | (uint32_t) Die;

    ApobIp2IpApi->ApobGetPhysCcdNumber(SilContext, ApobInstanceId, LogicalCcd, &PhysCcdNumber);
    if ((LogicalCcd >= SocMaxDieInfo.MaxSocCcdsPerDieValue) ||
      (PhysCcdNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobIp2IpApi->ApobGetPhysComplexNumber(SilContext,
      ApobInstanceId,
      LogicalCcd,
      LogicalComplex,
      &PhysComplexNumber
      );

    if ((LogicalComplex >= SocMaxDieInfo.MaxSocCcxPerCcdValue) ||
      (PhysComplexNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobIp2IpApi->ApobGetPhysCoreNumber(SilContext,
      ApobInstanceId,
      LogicalCcd,
      LogicalComplex,
      LogicalCore,
      &PhysCoreNumber
      );
    if ((LogicalCore >= SocMaxDieInfo.MaxSocCoresPerComplexValue) ||
      (PhysCoreNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobIp2IpApi->ApobGetIsThreadEnabled(SilContext,
      ApobInstanceId,
      LogicalCcd,
      LogicalComplex,
      LogicalCore,
      LogicalThread,
      &IsThreadEnabled
      );
    if ((LogicalThread >= SocMaxDieInfo.MaxSocThreadPerCore) ||
      IsThreadEnabled == false) {
      return SilInvalidParameter;
    }

    Status = GetCoreTopologyOnDieMax(SilContext,
      Socket,
      Die,
      &NumberOfCcds,
      &NumberOfComplexes,
      &NumberOfCores,
      &NumberOfLogicalThreads
      );

    if (Status == SilPass) {
      WhichBit = LogicalComplex * 8;
      WhichBit += (LogicalCore * NumberOfLogicalThreads) + LogicalThread;

      ThreadEnable.Value =
        xUSLSmnRead(0,
        GnbHandle->Address.Address.Bus,
        0x5a818UL
        );

      if ((ThreadEnable.Field.field0 & (1 << WhichBit)) == 0) {
        ThreadEnable.Field.field0 |= 1 << WhichBit;
        xUSLSmnWrite(0,
          GnbHandle->Address.Address.Bus,
          0x5a818UL,
          ThreadEnable.Value
          );

      } else {
        Status = SilDeviceError;
      }
    }
  } else {
    Status = SilInvalidParameter;
  }

  CCX_TRACEPOINT(SIL_TRACE_EXIT, "Status:0x%X\n", Status);

  return Status;
}

/*
 * CcxCacheInitPhx
 *
 * @brief cache initialization
 *
 * @param Null
 *
 * @return Null
 */
void CcxCacheInitPhx (void) {
  uint64_t   LocalMsrRegister;
  uint8_t    MtrrIndex = 0;

  // For common Cache setting
  CcxCacheInitCommon ();

  // For Phoenix Cache setting
  //======================================================================
  // vMTRR Registers
  //======================================================================
  //

  LocalMsrRegister = xUslRdMsr(MSR_TOM2);
  if (LocalMsrRegister >= 0x10000000000ull) {
    for (MtrrIndex = 0; MtrrIndex < 8; MtrrIndex++) {
      if (((xUslRdMsr(BIT_11_AAAABHA + (MtrrIndex << 1))) & MTRR_VARIABLE_MASK0_VALID) == 0) {
        break;
      }
    }
    assert(MtrrIndex < 8);
    xUslWrMsr((MTRR_VARIABLE_BASE0 + (MtrrIndex << 1)), 0x000FD00000000ull);
    xUslWrMsr((BIT_11_AAAABHA + (MtrrIndex << 1)), 0xFFFFF00000800ull);

    for (; MtrrIndex < 8; MtrrIndex++) {
      if (((xUslRdMsr(BIT_11_AAAABHA + (MtrrIndex << 1))) & MTRR_VARIABLE_MASK0_VALID) == 0) {
        break;
      }
    }
    assert(MtrrIndex < 8);
    xUslWrMsr((MTRR_VARIABLE_BASE0 + (MtrrIndex << 1)), 0x000FE00000000ull);
    xUslWrMsr((BIT_11_AAAABHA + (MtrrIndex << 1)), 0xFFFFE00000800ull);
  }
}

/*
 * SetBrandStringPhx
 *
 * @brief Set Brand String
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 *
 * @return Null
 */
void
SetBrandStringPhx (
  SIL_CONTEXT    *SilContext
  )
{
  // For Phoenix brand string setting
  uint64_t                  String;
  uint8_t                   BaseModel = 0;
  uint8_t                   ExtModel = 0;
  uint8_t                   PkgType = 0;
  CPUID_DATA                Regs = {0};
  NATIVE_DIE_CPUID_REGISTER FMSEReg;
  BRANDID_CPUID_REGISTER    BrandIdReg;

  // For common brand string setting
  CcxSetBrandStringCommon(SilContext);

  // CPU Brand String Overrides PHX2
  xUslCpuId(AMD_CPUID_FMF, 0, &Regs);
  *((uint32_t *) &FMSEReg) = Regs.EaxReg;
  *((uint32_t *) &BrandIdReg) = Regs.EbxReg;
  BaseModel = FMSEReg.BaseModel & 0xF;
  ExtModel = FMSEReg.ExtModel & 0xF;
  PkgType = BrandIdReg.PkgType & 0xF;
  CCX_TRACEPOINT(SIL_TRACE_INFO, "BaseMode = 0x%x, ExtModel = 0x%x, PkgType = 0x%x\n", BaseModel, ExtModel, PkgType);
  // PHX2 FP7, FP7r2
  if (((ExtModel == 7) && (BaseModel >= 8) && (BaseModel <= 0xB)) && ((PkgType == 4) || (PkgType == 5))) {
    String = xUslRdMsr(MSR_CPUID_NAME_STRING1);
    if (String == NAME_STRING_0457_5_n) { // 0457 5 n
      // non PRO
      // Program the ProcNameString to "5457 5 n".
      xUslWrMsr(MSR_CPUID_NAME_STRING1, NAME_STRING_5457_5_n);
    } else {
      String = xUslRdMsr(MSR_CPUID_NAME_STRING2);
      if (String == NAME_STRING_U0457) { // /w U0457
        // PRO
        // program the ProcNameString to "/w U5457".
        xUslWrMsr(MSR_CPUID_NAME_STRING2, NAME_STRING_U5457);
      }
    }
  }
}

/**
 * IsOneCcdModeSupportedPhx
 * @brief This service returns whether or not the SoC supports disabling all but one CCD.
 *
 *
 * @retval true                               Downing to 1 CCD is supported
 * @retval false                              Downing to 1 CCD is not supported
 *
 **/
bool
IsOneCcdModeSupportedPhx (
  void
  )
{
  return true;
}

/**
 * IsS3SupportedPhx
 * @brief Returns whether or not the SoC supports the S3 sleep state.
 *
 * @retval true       S3 is supported by the SoC
 * @retval false      S3 is not supported by the SoC
 */
bool
IsS3SupportedPhx (
  void
  )
{
  return true;
}

/**
 * IsGameModeSupportedPhx
 * @brief Returns whether or not the SoC supports game mode.
 *
 * @retval true
 * @retval false
 */
bool
IsGameModeSupportedPhx (
  void
  )
{
  return false;
}
