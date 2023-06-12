/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxInitBrh.c
 * @brief BRH Zen5 specific transfer table and I2I API functions definitions
 *
 */

#include <CCX/Zen5/CcxZen5.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/BRH/SilFabricRegistersBrh.h>
#include <MsrReg.h>
#include <DF/DfX/BRH/SilFabricInfoBrh.h>
#include "CcxCmn2Brh.h"

/**--------------------------------------------------------------------
 * CalcLocalApicBrh
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
CalcLocalApicBrh (
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    Ccd,
  uint32_t    Complex,
  uint32_t    Core,
  uint32_t    Thread
  )
{
  CPUID_DATA          CpuId = {0};
  SIL_RESERVED_UNION_0015 CpuTopoEcx;
  SIL_RESERVED_UNION_0014 CpuTopoEax;
  uint8_t             SubIndex = 0;
  bool                SmtEnable = false;
  uint8_t             CoreBitShift = 0;
  uint8_t             ComplexBitShift = 0;
  uint8_t             CcdBitShift = 0;
  uint8_t             SocketBitShift = 0;
  uint32_t            ApicId = 0;

  assert(Socket < 2);
  assert(Die == 0);
  assert(Ccd < 16);
  assert(Complex < 2);
  assert(Core < 16);
  assert(Thread < 2);

  // CPUID_Fn0000000B [Extended Topology Enumeration]
  if (xUslGetCpuidCoreMaskWidth () == 0x1) {
    SmtEnable = true;
  } else {
    SmtEnable = false;
  }

  for (SubIndex = 0; SubIndex < SIL_RESERVED_0007; SubIndex++) {

    xUslGetCpuidExtCpuTopology(&CpuId, SubIndex);
    CpuTopoEcx.Value = CpuId.EcxReg;
    CpuTopoEax.Value = CpuId.EaxReg;
    switch (CpuTopoEcx.Bits.field_bits_8_to_15) {
    case LEVEL_TYPE_CORE:
      // Core
      CoreBitShift = (uint8_t)CpuTopoEax.Bits.field_bits_0_to_4;
      break;
    case LEVEL_TYPE_COMPLEX:
      // Complex
      ComplexBitShift = (uint8_t)CpuTopoEax.Bits.field_bits_0_to_4;
      break;
    case LEVEL_TYPE_CCD:
      // CCD
      CcdBitShift = (uint8_t)CpuTopoEax.Bits.field_bits_0_to_4;
      break;
    case LEVEL_TYPE_SOCKET:
      // Socket
      SocketBitShift = (uint8_t)CpuTopoEax.Bits.field_bits_0_to_4;
      break;
    default:
      // Should never reach here
      assert(0);
      break;
    }
  }

  ApicId = (uint32_t)((Core << CoreBitShift) + \
    (Complex << ComplexBitShift) + \
    (Ccd << CcdBitShift) + \
    (Socket << SocketBitShift));

  if (SmtEnable) {
    ApicId += (uint32_t)Thread;
  }

  return ApicId;
}

/**--------------------------------------------------------------------
 * CalculateRmpTableBaseBrh
 *
 * @brief   Calculate RMP Table Base address using Socket boundary.
 *
 * @param[in]  RmpTableSize        RMP table size
 * @param[in]  RmpTableBase        RMP table Base
 * @param[in]  SnpMemSizeToCover
 *
 */
void CalculateRmpTableBaseBrh (
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
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS                   Status;

  PrevSocketId = 0u;
  NextSocketId = 0u;
  Sockets = 0u;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);
  Sockets = DfIp2IpApi->DfGetNumberOfProcessorsPresent ();

  if ((Sockets > 1u) && (SplitRmpTableValue > 0u)) {
    // Collect information about the memory ranges discovered by ABL
    for (DramMapIndex = 0; DramMapIndex < SIL_RESERVED_0207; DramMapIndex++) {
      DramAddressCtl.Value = DfIp2IpApi->DfFabricRegisterAccRead(0,
        0,
        DRAMADDRESSCTL_0_FUNC,
        (DRAMADDRESSCTL_0_REG + (DramMapIndex *
        SIL_RESERVED_0176)),
        SIL_RESERVED_0187
        );
      if (DramAddressCtl.Field.AddrRngVal == 1) {
        NextSocketId = (DramAddressCtl.Field.DstFabricID >> BRH_FABRIC_ID_SOCKET_SHIFT) &
          SIL_RESERVED_0178;
        if (DramMapIndex == 0) {
          PrevSocketId = NextSocketId;
        } else {
          if (NextSocketId != PrevSocketId) {
            DramBaseAddr.Value = DfIp2IpApi->DfFabricRegisterAccRead(0,
              0,
              DRAMBASEADDRESS_0_FUNC,
              (DRAMBASEADDRESS_0_REG + (DramMapIndex *
              SIL_RESERVED_0176)),
              SIL_RESERVED_0187
              );
            DramLimitAddr.Value = DfIp2IpApi->DfFabricRegisterAccRead(0,
              0,
              DRAMLIMITADDRESS_0_FUNC,
              (DRAMLIMITADDRESS_0_REG + (DramMapIndex *
              SIL_RESERVED_0176)),
              SIL_RESERVED_0187
              );
            break;
          }
        }
      }
    }

    if (PrevSocketId == NextSocketId) {
      // No socket boundary found. Find enough memory for RMP table to fit on MB boundary
      *RmpTableBase = *SnpMemSizeToCover;
      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "No Socket boundary found.No RMP Split. Use RmpTableBase as the Max RMP Limit = 0x%llx\n",
        *SnpMemSizeToCover
        );
    } else {
      // Socket boundary detected. Find the max address of RMP Table,
      // that can be used to find new base of RMP table.
      *RmpTableBase = ((uint64_t)DramBaseAddr.Value << 28) + (*RmpTableSize / 2);

      if (*RmpTableBase > *SnpMemSizeToCover) {
        //Always make sure, RMP Max address (Limit address) is below the SNP Coverage Limit
        *RmpTableBase = *SnpMemSizeToCover;
        CCX_TRACEPOINT(SIL_TRACE_INFO,
          "Reset RmpTableBase to SnpMemSizeToCover to get the final RMP base = 0x%llx\n",
          *RmpTableBase
          );
      }

      CCX_TRACEPOINT(SIL_TRACE_INFO,
        "DRAM Range that covers the socket boundary between socket 0&1 = 0x%x,0x%x,0x%llx,0x%llx\n",
        DramBaseAddr.Value,
        DramLimitAddr.Value,
        *RmpTableBase,
        *RmpTableSize
        );
    }
  } else {
    // find enough memory for RMP table to fit on MB boundary
    *RmpTableBase = *SnpMemSizeToCover;
    CCX_TRACEPOINT(SIL_TRACE_INFO, "No RMP Split. RmpTableBase = 0x%llx\n", *RmpTableBase);
  }
}

/**--------------------------------------------------------------------
 * GetAmdApicModeBrh
 *
 * @brief Get The APIC Mode
 *
 * @param NumberOfSockets   Unused for BRH
 * @param NumberOfCcds      Unused for BRH
 * @param NumberOfComplexes Unused for BRH
 * @param NumberOfCores     Unused for BRH
 * @param NumberOfThreads   Unused for BRH
 *
 * @return APIC_MODE Always x2ApicMode for BRH
 */
APIC_MODE
GetAmdApicModeBrh (
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

  return x2ApicMode;
}
