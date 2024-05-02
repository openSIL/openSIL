/* Copyright 2022-2024 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file Zen4.h
 * @brief Zen4 core specific headers and function declarations
 *
 */


#include <SilCommon.h>
#include <CcxClass-api.h>
#include <Ccx.h>
#include <CcxReg.h>
#include <CcxApic.h>
#include <CcxCmn2Rev.h>
#include <CcxIp2Ip.h>
#include <MsrReg.h>
#include <Zen4Reg.h>
#include <CommonLib/CpuLib.h>
#include <CommonLib/SmnAccess.h>
#include <DF/DfIp2Ip.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>
#include <DF/Common/SilFabricInfo.h>


/**************** Common-2-Rev Transfer Table for Ccx Zen4 ******************/

/*
 *  prototypes for forward references of functions used in the table
 */

// This MUST match the typedef   SET_MISC_MSRS       in CcxCmn2Rev.h
void (Zen4SetMiscMsrs) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

// This MUST match the typedef   GET_AMD_APIC_MODE   in CcxCmn2Rev.h
APIC_MODE (SocZen4GetAmdApicMode) (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  );

// This MUST match the typedef   CALC_LOCAL_APIC     in CcxCmn2Rev.h
uint32_t (Zen4CalcLocalApic) (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Ccd,
  uint32_t Complex,
  uint32_t Core,
  uint32_t Thread
  );

// This MUST match the typedef   CALC_RMP_TABLEBASE  in CcxCmn2Rev.h
void (Zen4CalculateRMPTableBase) (
  uint64_t    *RmpTableSize,
  uint64_t    *RmpTableBase,
  uint64_t    *SnpMemSizeToCover,
  uint8_t     SplitRmpTableValue
  );

/** ------------------------ Table ------------------------------
 * @details This is the internal common-2-Rev transfer table for Zen4
 */
CCX_XFER_TABLE CcxXferZen4 = {
    .Header = {
      .Version        = CcxZen4
    },
    .SetMiscMsrs      = Zen4SetMiscMsrs,
    .GetAmdApicMode   = SocZen4GetAmdApicMode,
    .CalcLocalApic    = Zen4CalcLocalApic,
    .CalcRMPTableBase = Zen4CalculateRMPTableBase
  };

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for Zen4
 */
CCX_IP2IP_API CcxIp2IpZen4 = {
    .CalcLocalApic    = Zen4CalcLocalApic
};

/*********** Functions used in the Common-2-Rev transfer table *************/
/* Note the prototypes (typedefs) for these functions are declared in the
 * common file CcxCmn2Rev.h . These functions must match those prototypes.
 */

/**
 * Zen4CalcLocalApic
 *
 * @brief   Returns the APIC Id based on the provided inputs
 *
 * @param[in]  Socket     Socket # of APIC Id to calculate
 * @param[in]  Die        Die # of APIC Id to calculate
 * @param[in]  Ccd        CCD # of APIC Id to calculate
 * @param[in]  Complex    Complex # of APIC Id to calculate
 * @param[in]  Core       Core # of APIC Id to calculate
 * @param[in]  Thread     Thread # of APIC Id to calculate
 *
 * @retval     uint32_t   Calculate Local APIC Id
 *
 */
uint32_t
Zen4CalcLocalApic (
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    Ccd,
  uint32_t    Complex,
  uint32_t    Core,
  uint32_t    Thread
  )
{
  PMREG_INITPKG7_STRUCT    InitPkg7;
  uint32_t                 CoreEnable[4];
  uint8_t                  NumberOfSockets;
  uint8_t                  NumberOfCcds;
  uint8_t                  NumberOfComplexs;
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
  DF_IP2IP_API*            DfIp2IpApi;
  SIL_STATUS               Status;

  MaxCcxPerCcd = 0;
  assert (Socket < MAX_SOCKETS_SUPPORTED);
  assert (Die == 0);
  assert (Ccd < MAX_CCDS_PER_DIE);
  assert (Complex < MAX_COMPLEXES_PER_CCD);
  assert (Core < CCX_MAX_CORES_PER_COMPLEX);
  assert (Thread < MAX_THREADS_PER_CORE);

  UNUSED(Die);
  Status = SilGetIp2IpApi (SilId_DfClass, (void*) &DfIp2IpApi);
  assert (Status == SilPass);
  for (Index = 0; Index < sizeof (CoreEnable) / sizeof (CoreEnable[0]); Index++) {
    CoreEnable[Index] = DfIp2IpApi->DfFabricRegisterAccRead (0, SIL_RESERVED_824,
     (SIL_RESERVED_823 - SIL_RESERVED_822) * Index + SIL_RESERVED_822, FABRIC_REG_ACC_BC);
  }
  for (CoreIdx = 0; (CoreIdx < ZEN4_MAX_CORES_PER_SKT) && ((CoreEnable[CoreIdx / 32]
                                       & (0x1 << (CoreIdx % 32))) == 0); CoreIdx++);
  assert(CoreIdx < ZEN4_MAX_CORES_PER_SKT);

  MaxCcxPerCcd   = ((xUslGetRawIdOnExecutingCore() & RAW_FAMILY_ID_MASK) == AMD_F19_GENOA_RAW_ID) ? MAX_CCX_PER_CCD : MAX_CCX_PER_CCD_GENOA;
  CcxIdx         = CoreIdx / ZEN4_MAX_CORES_PER_CCD;
  CoreIdx        = CoreIdx % ZEN4_MAX_CORES_PER_CCD;
  CcdIdx         = CcxIdx / MaxCcxPerCcd;
  CcxIdx         = CcxIdx % MaxCcxPerCcd;
  InitPkg7.Value = xUSLSmnRead (0, 0, (SMN_PMREG_INITPKG7_ADDRESS + (SMN_PMREG_INITPKG7_CDD_OFFSET * CcdIdx)
               + (SMN_PMREG_INITPKG7_CCX_OFFSET * CcxIdx) + (SMN_PMREG_INITPKG7_CORE_OFFSET * CoreIdx)));


  NumberOfSockets  = (uint8_t) InitPkg7.Field.NumOfSockets;
  NumberOfCcds     = (uint8_t) InitPkg7.Field.NumOfCcds;
  NumberOfComplexs = (uint8_t) InitPkg7.Field.NumOfComplexes;
  Apic16TMode      = (uint8_t) InitPkg7.Field.Apic16TMode;
  NumberOfCores    = (uint8_t) InitPkg7.Field.NumOfCores;
  NumberOfThreads  = xUslGetThreadsPerCore() - 1;

  assert (Socket  <= NumberOfSockets);
  assert (Complex <= NumberOfComplexs);
  assert (Core    <= NumberOfCores);
  assert (Thread  <= NumberOfThreads);

  CoreShift    = NumberOfThreads;
  ComplexShift = (xUslBitWidth (NumberOfCores)) + CoreShift;

  if (Apic16TMode == 1 && ComplexShift < 4) {
    ComplexShift = 4;
  }

  CcdShift    = (xUslBitWidth (NumberOfComplexs)) + ComplexShift;
  SocketShift = (xUslBitWidth (NumberOfCcds)) + CcdShift;

  ApicId = (uint32_t) ((Socket << SocketShift) | (Ccd << CcdShift) |
                       (Complex << ComplexShift) | (Core << CoreShift) | Thread);

  return ApicId;
}

/** ----------------------------------------------------------------------------
 * Zen4CalculateRMPTableBase
 *
 * @brief   Calculate RMP Table Base address using Socket boundary.
 *
 * @param[in]  RmpTableSize        RMP table size
 * @param[in]  RmpTableBase        RMP table Base
 * @param[in]  SnpMemSizeToCover
 *
 */
void Zen4CalculateRMPTableBase (
  uint64_t   *RmpTableSize,
  uint64_t   *RmpTableBase,
  uint64_t   *SnpMemSizeToCover,
  uint8_t    SplitRmpTableValue
  )
{
  uint32_t                     Sockets;
  uint32_t                     DramMapIndex;
  DRAM_ADDRESS_CTL_REGISTER    DramAddressCtl;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr;
  DRAM_LIMIT_ADDRESS_REGISTER  DramLimitAddr;
  uint8_t                      PrevSocketId;
  uint8_t                      NextSocketId;
  DF_IP2IP_API*                DfIp2IpApi;
  SIL_STATUS                   Status;

  PrevSocketId = 0u;
  NextSocketId = 0u;
  Sockets      = 0u;

  Status= SilGetIp2IpApi (SilId_DfClass, (void*) &DfIp2IpApi);
  assert (Status == SilPass);
  Sockets = DfIp2IpApi->DfGetNumberOfProcessorsPresent();

  if ((Sockets > 1u) && (SplitRmpTableValue > 0u)) {
    // Collect information about the memory ranges discovered by ABL
    for (DramMapIndex = 0; DramMapIndex < DFX_NUMBER_OF_DRAM_REGIONS; DramMapIndex++) {
      DramAddressCtl.Value = DfIp2IpApi->DfFabricRegisterAccRead (0, DRAMADDRESSCTL_0_FUNC,
                                (DRAMADDRESSCTL_0_REG + (DramMapIndex *
                                 DFX_DRAM_REGION_REGISTER_OFFSET)),DFX_IOMS2_INSTANCE_ID);
      if (DramAddressCtl.Field.AddrRngVal == 1) {
        NextSocketId = (DramAddressCtl.Field.DstFabricID >> DFX_FABRIC_ID_SOCKET_SHIFT) &
                                                            DFX_FABRIC_ID_SOCKET_SIZE_MASK;
        if (DramMapIndex == 0) {
          PrevSocketId = NextSocketId;
        } else {
          if (NextSocketId != PrevSocketId) {
            DramBaseAddr.Value = DfIp2IpApi->DfFabricRegisterAccRead (0, DRAMBASEADDRESS_0_FUNC,
                                   (DRAMBASEADDRESS_0_REG + (DramMapIndex *
                                    DFX_DRAM_REGION_REGISTER_OFFSET)),DFX_IOMS2_INSTANCE_ID);
            DramLimitAddr.Value = DfIp2IpApi->DfFabricRegisterAccRead (0, DRAMLIMITADDRESS_0_FUNC,
                                   (DRAMLIMITADDRESS_0_REG + (DramMapIndex *
                                    DFX_DRAM_REGION_REGISTER_OFFSET)), DFX_IOMS2_INSTANCE_ID);
            break;
          }
        }
      }
    }

    if (PrevSocketId == NextSocketId) {
      // No socket boundary found. Find enough memory for RMP table to fit on MB boundary
      *RmpTableBase = *SnpMemSizeToCover;
      CCX_TRACEPOINT (SIL_TRACE_INFO,
      "No Socket boundary found.No RMP Split. Use RmpTableBase as the Max RMP Limit = 0x%llx\n",
                      *SnpMemSizeToCover);
    } else {
      // Socket boundary detected. Find the max address of RMP Table,
      // that can be used to find new base of RMP table.
      *RmpTableBase = ((uint64_t)DramBaseAddr.Value << 28) + (*RmpTableSize / 2);

      if (*RmpTableBase > *SnpMemSizeToCover) {
        //Always make sure, RMP Max address (Limit address) is below the SNP Coverage Limit
        *RmpTableBase = *SnpMemSizeToCover;
        CCX_TRACEPOINT (SIL_TRACE_INFO,
        "Reset RmpTableBase to SnpMemSizeToCover to get the final RMP base = 0x%llx\n",
                        *RmpTableBase);
      }

      CCX_TRACEPOINT (SIL_TRACE_INFO,
      "DRAM Range that covers the socket boundary between socket 0&1 = 0x%x,0x%x,0x%llx,0x%llx\n",
                            DramBaseAddr.Value,DramLimitAddr.Value,*RmpTableBase,*RmpTableSize);
    }
  } else {
    // find enough memory for RMP table to fit on MB boundary
    *RmpTableBase = *SnpMemSizeToCover;
    CCX_TRACEPOINT (SIL_TRACE_INFO, "No RMP Split. RmpTableBase = 0x%llx\n", *RmpTableBase);
  }
}



/// PAUSE instruction mode setting.
typedef enum  {
  CPU_PAUSE_DELAY_AUTO     = 0xFF    ///< Use the recommended setting for the processor.
} CPU_PAUSE_DELAY_MODE;
/** -------------------------------------------------------------------------
 * Zen4SetMiscMsrs
 *
 * @brief This routine sets miscellaneous MSRs and is only executed on the BSP.
 * APs will sync the applicable MSRs settings through ApMsrSyncList
 *
 * @param CcxInputBlock CCX input data block
 *
 */
void
Zen4SetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint8_t     MonMwaitDis;
  bool        EnableSvmX2AVIC;
  uint8_t     ReservedVal;
  uint8_t     CpuPauseDelay;

  MonMwaitDis          = CcxInputBlock->MonMwaitDis;
  EnableSvmX2AVIC      = CcxInputBlock->EnableSvmX2AVIC;
  ReservedVal          = CcxInputBlock->AmdReserved3;
  CpuPauseDelay        = CcxInputBlock->AmdCpuPauseDelay;

  CCX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  // MSR_C001_0015[9]
  if (MonMwaitDis != 0xFF) {
    xUslMsrAndThenOr (
      MSR_HWCR,
      ~(uint64_t) BIT_64(9),
      ((MonMwaitDis) ? BIT_64(9) : 0)
      );
  }

  xUslMsrOr (0xC001102E, (uint64_t) BIT_64(35));

  switch (ReservedVal) {
  case 0:
    xUslMsrOr (0xC00110E5, (uint64_t) BIT_64(26));
    xUslMsrAnd (0xC00110E6, ~(uint64_t) BIT_64(38));
    xUslMsrAnd (0xC00110EC, ~(uint64_t) BIT_64(0));
    xUslMsrAnd (0xC00110E2, ~(uint64_t) BIT_64(30));
    break;
  case 1:
    xUslMsrOr (0xC00110E5, (uint64_t) BIT_64(26));
    xUslMsrOr (0xC00110E6, (uint64_t) BIT_64(38));
    xUslMsrOr (0xC00110EC, (uint64_t) BIT_64(0));
    xUslMsrOr (0xC00110E2, (uint64_t) BIT_64(30));
    break;
  case 2:
    xUslMsrAnd (0xC00110E5, ~(uint64_t) BIT_64(26));
    xUslMsrOr (0xC00110E6, (uint64_t) BIT_64(38));
    xUslMsrOr (0xC00110EC, (uint64_t) BIT_64(0));
    xUslMsrOr (0xC00110E2, (uint64_t) BIT_64(30));
    break;
  }

  switch (CpuPauseDelay) {
  case CPU_PAUSE_DELAY_AUTO:
    break;
  default:
    assert (false);
    break;
  }

  if (EnableSvmX2AVIC) {
    xUslMsrOr (0xC00110DD, (uint64_t) BIT_64(18));
  } else {
    xUslMsrAnd (0xC00110DD, ~((uint64_t) BIT_64(18)));
  }

  CCX_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}



/** ******************************************************************
 *  Get The APIC Mode
 *  @brief
 *  @details
 */
APIC_MODE
SocZen4GetAmdApicMode (
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

  // On Genoa, force the APIC mode to x2Apic as it is proven to improve performance
  return x2ApicMode;
}

