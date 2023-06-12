/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxZen5.c
 * @brief CCX Zen5 specific implementation.
 *
 */

#include <SilCommon.h>
#include <MsrReg.h>
#include <Ccx.h>
#include <CcxApic.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <CoreTopologyService.h>
#include <Pstates.h>
#include "Zen5Reg.h"
#include "CcxZen5.h"
#include <APOB/ApobIp2Ip.h>

extern volatile AP_MSR_SYNC ApMsrSyncListZen5[];

/**
 * Zen5StoreDowncoreCoreCount
 *
 * @brief This function will set the set CcxOutputBlock.AmdCcxCoreCount which can be consumed by host firmware.
 *
 * @param CcxConfigData   A pointer to the CCX class data block containing the output block structure.
 */
void
Zen5StoreDowncoreCoreCount (
  CCXCLASS_DATA_BLK   *CcxConfigData
  )
{
  return;
}

/**
 * Zen5SetMiscMsrs
 *
 * @brief This routine sets miscellaneous MSRs and is only executed on the BSP.
 * APs will sync the applicable MSRs settings through ApMsrSyncList
 *
 * @param CcxInputBlock CCX input data block
 *
 */
void
Zen5SetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint8_t     DisFstStrErmsb;

  DisFstStrErmsb = CcxInputBlock->DisFstStrErmsb;

  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // MSR_C001_102D[56]
  if (DisFstStrErmsb != 0xFF) {
    xUslMsrAndThenOr(MSR_LS_CFG2, ~(uint64_t) BIT_64(56), (((uint64_t)DisFstStrErmsb & 1) << 56));
  }

  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * Zen5CcxPrefetcher
 *
 * @brief This routine initializes L1/L2 prefetcher per user configurations.
 *        This routine is only executed on the BSP.
 *        APs will sync the applicable MSRs settings through ApMsrSyncList.
 *
 * @param CcxInputBlock Ccx input block data
 *
 */
void
Zen5CcxPrefetcher (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint64_t  LocalMsr;

  if (CcxInputBlock->L1BurstPrefetch) {
    // MSR C001_102B[22] = 1
    xUslMsrOr(MSR_L2_PREFETCH_CFG, BIT_64(22));

    // MSR C001_10E7[12] = 1
    // MSR C001_10E7[10] = 0
    // MSR C001_10E7[6] = 1
    LocalMsr = xUslRdMsr(MSR_DC_PF_CFG);
    LocalMsr &= ~(BIT_64(6) | BIT_64(10) | BIT_64(12));
    LocalMsr |= BIT_64(6) | BIT_64(12);
    CCX_TRACEPOINT(SIL_TRACE_INFO, "MSR_DC_PF_CFG updated value is 0x%lX\n", LocalMsr);
    xUslWrMsr(MSR_DC_PF_CFG, LocalMsr);

    LocalMsr = xUslRdMsr(MSR_DC_PF_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, "MSR_DC_PF_CFG current value is 0x%lX\n", LocalMsr);
  } else {
    // MSR C001_102B[22] = 0
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~(BIT_64(22)));

    // MSR C001_10E7[12] = 0
    // MSR C001_10E7[10] = 1
    // MSR C001_10E7[6] = 0
    LocalMsr = xUslRdMsr(MSR_DC_PF_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_PF_CFG current value is 0x%lX\n", LocalMsr);

    LocalMsr &= ~(BIT_64(6) | BIT_64(10) | BIT_64(12));
    LocalMsr |= BIT_64(10);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_PF_CFG updated value is 0x%lX\n", LocalMsr);
    xUslWrMsr(MSR_DC_PF_CFG, LocalMsr);

    LocalMsr = xUslRdMsr(MSR_DC_PF_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_PF_CFG current value is 0x%lX\n", LocalMsr);
  }

  // L1 Stream HW Prefetcher
  if (!CcxInputBlock->AmdL1StreamPrefetcher) {
    // MSR C001_10E7[16] = 1
    xUslMsrOr(MSR_DC_PF_CFG, BIT_64(16));
  }

  // L1 Stride Prefetcher
  if (!CcxInputBlock->AmdL1StridePrefetcher) {
    // MSR C001_10E7[17] = 1
    xUslMsrOr(MSR_DC_PF_CFG, BIT_64(17));
  }

  // L1 Region Prefetcher
  if (!CcxInputBlock->AmdL1RegionPrefetcher) {
    // MSR C001_10E7[18] = 1
    xUslMsrOr(MSR_DC_PF_CFG, BIT_64(18));
  }

  // L2 Stream Prefetcher
  if (!CcxInputBlock->AmdL2StreamPrefetcher) {
    // MSR C001_102B[0] = 0
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~((uint64_t)BIT_64(0)));
  }

  // L2 Up/Down Prefetcher
  if (!CcxInputBlock->AmdL2UpDownPrefetcher) {
    // MSR C001_102B[2] = 0
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~((uint64_t)BIT_64(2)));
  }
}

/**
 * Zen5GetApMsrSyncList
 *
 * @brief Return the AP MSR Sync List for BRH
 *
 * @return Pointer to the ApMsrSyncList
 */
volatile AP_MSR_SYNC *
Zen5GetApMsrSyncList (void)
{
  return ApMsrSyncListZen5;
}

/**
 * Zen5InitializePrefetchMode
 *
 * @brief   Initialize Prefetch Mode
 *
 * @details This function provides for performance tuning to
 *          optimize for specific workloads. For general performance
 *          use the recommended settings.
 *
 * @param   CcxInputBlock
 *
 * @return  nothing
 */
void
Zen5InitializePrefetchMode (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  CCX_PREFETCH_MODE PrefetchMode;

  PrefetchMode.SoftwarePrefetchMode = CcxInputBlock->AmdSoftwarePrefetchMode;
  if (PrefetchMode.SoftwarePrefetchMode != SOFTWARE_PREFETCHES_AUTO) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Zen5InitializePrefetchMode:\n");

    CCX_TRACEPOINT(SIL_TRACE_INFO,
      "SoftwarePrefetchMode - %d\n",
      PrefetchMode.SoftwarePrefetchMode
      );

    // DISABLE_SOFTWARE_PREFETCHES
    if (PrefetchMode.SoftwarePrefetchMode == DISABLE_SOFTWARE_PREFETCHES) {
      // MSR_DE_CFG (MSR_C001_1029)
      //  [7:2] = 0x3F
      if (xUslIsComputeUnitPrimary ()) {
        xUslMsrOr(MSR_DE_CFG, 0xFC);
      }
    }
  }
}

/**--------------------------------------------------------------------
 * Zen5SwitchPstate
 *
 * @brief   Switch to a certain Pstate
 *
 * @details Switch to the Pstate specified in SwitchPstate.
 *
 * @param   SwitchPstate        Input containing Target Pstate [0:7]
 *                              & wait for completion flag.
 *
 * @retval  SilPass             Pstate Switch was successful
 * @retval  SilInvalidParameter Requested Pstate is not enabled
 */
SIL_STATUS
Zen5SwitchPstate (
  SWITCH_PSTATE *SwitchPstate
  )
{
  SIL_RESERVED_UNION_0039       PstateMsr;
  PSTATE_CTL_STRUCT            PstateCtl;
  SIL_RESERVED_UNION_0020    HwPstateSts;
  uint32_t                     PstateNumber;

  PstateNumber = (uint32_t)SwitchPstate->Pstate;
  PstateMsr.Value = xUslRdMsr(MSR_PSTATE_DEF_ADDRESS + PstateNumber);
  if (PstateMsr.Field.field_bit_63 == 0) {
    XUSL_TRACEPOINT(SIL_TRACE_ERROR,
      "Pstate 0x%x not enabled.\n",
      SwitchPstate->Pstate
      );
    return SilInvalidParameter;
  }

  PstateCtl.Field.PstateCmd = (uint64_t)SwitchPstate->Pstate;
  xUslWrMsr(MSR_PSTATE_CTL_ADDRESS, PstateCtl.Value);

  if (SwitchPstate->WaitForCompletion) {
    if (SwitchPstate->Pstate != 0) {
      do {
        HwPstateSts.Value = xUslRdMsr(SIL_RESERVED_0382);
      } while ((HwPstateSts.Field.field_bits_0_to_11 != PstateMsr.Field.field_bits_0_to_11));
    } else {
      do {
        HwPstateSts.Value = xUslRdMsr(SIL_RESERVED_0382);
      } while ((HwPstateSts.Field.field_bits_0_to_11 < PstateMsr.Field.field_bits_0_to_11));
    }
  }

  return SilPass;
}

/**
 *  Zen5GetPstateInfo
 *
 *  @brief    Get Pstate information
 *
 *  @details  Get the Pstate information including Freq, Voltage, Power.
 *            It is important to note that if the function does not
 *            return successfully, the outputs should be considered
 *            invalid.
 *
 *  @param  Pstate        Input of the P-state to analyze
 *  @param  Frequency     Output of the P-State's frequency in MegaHertz
 *  @param  VoltageInuV   Output of the P-State's voltage in uV
 *  @param  PowerInmW     Output of The Power in milliWatts of that P-State.
 *  @param  PstateStatus  Output status for the Pstate.  True if Get Pstate information successful.  False if Pstate is
 *                        disabled.  Not valid if function returns unsuccessfully.
 *
 *  @retval SilPass       GetPstateInfo completed successfully
 *  @retval SilAborted    CpuDfsId (core frequency divisor) is invalid
 *  @retval SilAborted    IddDiv value is undefined (misfused CPU or
 *                        invalid PState MSR write)
 *
 */
SIL_STATUS
Zen5GetPstateInfo (
  PSTATE    Pstate,
  uint32_t  *Frequency,
  uint32_t  *VoltageInuV,
  uint32_t  *PowerInmW,
  bool      *PstateStatus
  )
{
  uint32_t                CpuFid;
  uint32_t                CpuVid;
  uint32_t                IddValue;
  uint32_t                IddDiv;
  uint32_t                PstateNumber;
  uint32_t                P0Vid;
  uint32_t                P0Freq;
  uint32_t                MaxCpuCofMhz;
  SIL_RESERVED_UNION_0039  PstateMsr;
  SIL_RESERVED_UNION_0037   PmgtMisc;
  CCXCLASS_DATA_BLK       *CcxData;

  PstateNumber = (uint32_t) Pstate;
  PstateMsr.Value = xUslRdMsr(MSR_PSTATE_DEF_ADDRESS + PstateNumber);

  if (PstateMsr.Field.field_bit_63 == 0) {
    *PstateStatus = false;
    return SilPass;
  }

  CcxData = (CCXCLASS_DATA_BLK *)xUslFindStructure(SilId_CcxClass, CCXCLASS_INSTANCE);
  if (CcxData == NULL) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO, "  GetPstateFrequency - P%d\n", PstateNumber);

  CpuFid = (uint32_t) PstateMsr.Field.field_bits_0_to_11;
  CpuVid = (uint32_t) PstateMsr.Field.field_bits_14_to_21;
  IddValue = (uint32_t) PstateMsr.Field.field_bits_22_to_29;
  IddDiv = (uint32_t) PstateMsr.Field.field_bits_30_to_31;

  *Frequency = (size_t) (5 * CpuFid);

  if ((PstateNumber == 0) && (CcxData->CcxInputBlock.P0Setting == SIL_PSTATE0_CUSTOM)) {
    P0Freq = CcxData->CcxInputBlock.P0Freq;
    P0Vid = CcxData->CcxInputBlock.P0Vid32;
    if ((P0Vid != 0) && (P0Vid != CcxData->CcxInputBlock.PxAutoVid)) {
      CpuVid = P0Vid & 0x1FF;
    }
    if ((P0Freq != 0) && (P0Freq != CcxData->CcxInputBlock.PxAutoFreq)) {
      PmgtMisc.Value = xUslRdMsr(MSR_PMGT_MISC_ADDRESS);
      MaxCpuCofMhz = (uint32_t) ((PmgtMisc.Field.field_bits_15_to_20 * 100) + (PmgtMisc.Field.field_bit_30 * 50));

      if ((PmgtMisc.Field.field_bits_15_to_20 != 0) && (P0Freq > MaxCpuCofMhz)) {
        CCX_TRACEPOINT(SIL_TRACE_INFO, "  P0Freq %d exceeds Frequency Max limit of %d\n", P0Freq, MaxCpuCofMhz);
        P0Freq = MaxCpuCofMhz;
      }
      *Frequency = P0Freq;
    }
  }
  CCX_TRACEPOINT(SIL_TRACE_INFO, "    FrequencyInMHz=%d, CpuFid=%d, CpuVid=%d\n", *Frequency, CpuFid, CpuVid);

  // Voltage:
  if (CpuVid == 0) {
    *VoltageInuV = 0;
  } else {
    *VoltageInuV = 250000L + (5000L * (CpuVid - 1));
  }
  CCX_TRACEPOINT(SIL_TRACE_INFO, "    SVI3 VoltageInuV=0x%x\n", *VoltageInuV);

  // PowerInmW
  *PowerInmW = (*VoltageInuV) / 10 * IddValue;

  switch (IddDiv) {
  case 0:
    *PowerInmW = *PowerInmW / 100L;
    break;
  case 1:
    *PowerInmW = *PowerInmW / 1000L;
    break;
  case 2:
    *PowerInmW = *PowerInmW / 10000L;
    break;
  default:
    /*
     * IddDiv is set to an undefined value.  This is due to either a misfused
     * CPU, or an invalid P-state MSR write.
     */
    CCX_TRACEPOINT(SIL_TRACE_ERROR,
      "    IddDiv value undefined = 0d%d\n",
      IddDiv
      );
    assert(IddDiv < 3);
    *PowerInmW = 0;
    return SilAborted;
  }

  *PstateStatus = true;
  return SilPass;
}

/**
 * Zen5ReOrderLogicalCcdWithNumaDomainOrder
 *
 * Re-order the logical CCD number array with the CCD NUMA domain order for Zen5.
 *
 * @param  LogicalSocket      Zero-based logical socket number.
 * @param  LogicalDie         Zero-based logical die number.
 * @param  NumberOfCcds       The number of logical CCDs.
 * @param  OrderedLogicalCcd  Output the ordered logical CCD array. Caller must ensure the output buffer contains
 *                            at least NumberOfCcds entries.
 *
 * @retval SilPass              Operation successfully completed.
 * @retval SilInvalidParameter  Specified location is not available.
 * @retval SilInvalidParameter  OrderedLogicalCcd output buffer is NULL.
 *
 **/
SIL_STATUS
Zen5ReOrderLogicalCcdWithNumaDomainOrder (
  uint32_t          LogicalSocket,
  uint32_t          LogicalDie,
  uint32_t          NumberOfCcds,
  uint32_t          *OrderedLogicalCcd
  )
{
  // CcdNum ensures SRAT entries are entered in increasing order of NUMA domain
  uint32_t   PhyCcdOrder[] = {0, 4, 8, 12, 2, 6, 10, 14, 3, 7, 11, 15, 1, 5, 9, 13};
  uint32_t          i;
  uint32_t          j;
  uint32_t          k;
  uint32_t          PhySocketNum;
  uint32_t          PhyDieNum;
  uint32_t          PhyCcdNum;
  SIL_STATUS        Status;
  CCD_MAP           CcdMap[SIL_ARRAY_SIZE(PhyCcdOrder)];
  APOB_IP2IP_API    *ApobIp2IpApi;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;


  if (OrderedLogicalCcd == NULL) {
    return SilInvalidParameter;
  }
  if (NumberOfCcds > SIL_ARRAY_SIZE(PhyCcdOrder)) {
    assert(NumberOfCcds <= SIL_ARRAY_SIZE(PhyCcdOrder));
    return SilInvalidParameter;
  }
  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  ApobIp2IpApi->ApobGetCcdLogToPhysMap(0, 0, &ApobCcdLogToPhysMap);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "Logical CCD    Physical CCD\n");
  for (i = 0; i < NumberOfCcds; i++) {
    PhySocketNum = LogicalSocket;
    PhyDieNum = LogicalDie;
    PhyCcdNum = i;
    Status = LogicalToPhysicalLocation(&ApobCcdLogToPhysMap, &PhySocketNum, &PhyDieNum, &PhyCcdNum, NULL, NULL);
    assert(Status == SilPass);
    CcdMap[i].LogicalId = i;
    CcdMap[i].PhysicalId = PhyCcdNum;
    CCX_TRACEPOINT(SIL_TRACE_INFO, "        %02d             %02d\n", i, PhyCcdNum);
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO, "Ordered Logical CCD: ");
  k = 0;
  for (i = 0; i < SIL_ARRAY_SIZE(PhyCcdOrder); i++) {
    for (j = 0; j < NumberOfCcds; j++) {
      if (PhyCcdOrder[i] == CcdMap[j].PhysicalId) {
        OrderedLogicalCcd[k] = CcdMap[j].LogicalId;
        k++;
        CCX_TRACEPOINT(SIL_TRACE_RAW, "%d ", CcdMap[j].LogicalId);
      }
    }
  }
  CCX_TRACEPOINT(SIL_TRACE_RAW, "\n");
  assert(k == NumberOfCcds);

  return SilPass;
}

/**
 * Zen5CcxInitializeC6
 * @brief This function is called by all threads to enables Core C6.
 * @param[in] CStateBaseAddr Pointer to CState base address
 *
 *
 */
void Zen5CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock)
{
  SIL_RESERVED_UNION_0005      CstateCfg;
  SIL_RESERVED_UNION_0004     CstateCfg2;
  CSTATE_ADDRESS_MSR  CstateAddr;
  uint8_t             Cc6Enable;

  if (CcxInputBlock->AmdCStateMode == 1) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "  CcxInitializeC6 - Enable C6\n");
    if (xUslIsComputeUnitPrimary ()) {
      Cc6Enable = CcxInputBlock->AmdCc6Ctrl & 1;
      CstateCfg.Value = xUslRdMsr(SIL_RESERVED_0377);
      CstateCfg2.Value = xUslRdMsr(SIL_RESERVED_0378);
      CstateCfg.Field.field_bits_0_to_5 = 8;
      CstateCfg.Field.field_bit_6 = Cc6Enable;
      CstateCfg.Field.field_bits_8_to_13 = 8;
      CstateCfg.Field.field_bit_14 = Cc6Enable;
      CstateCfg.Field.field_bits_16_to_21 = 8;
      CstateCfg.Field.field_bit_22 = Cc6Enable;
      CstateCfg.Field.field_bits_24_to_29 = 8;
      CstateCfg.Field.field_bit_30 = Cc6Enable;
      CstateCfg2.Field.field_bits_0_to_5 = 8;
      CstateCfg2.Field.field_bit_6 = Cc6Enable;
      CstateCfg2.Field.field_bits_8_to_13 = 8;
      CstateCfg2.Field.field_bit_14 = Cc6Enable;
      CstateCfg2.Field.field_bits_16_to_21 = 8;
      CstateCfg2.Field.field_bit_22 = Cc6Enable;
      CstateCfg2.Field.field_bits_24_to_29 = 8;
      CstateCfg2.Field.field_bit_30 = Cc6Enable;
      xUslWrMsr(SIL_RESERVED_0377, CstateCfg.Value);
      xUslWrMsr(SIL_RESERVED_0378, CstateCfg2.Value);
    }

    CstateAddr.Value = 0;
    CstateAddr.Field.CstateAddr = (uint32_t) CcxInputBlock->AmdCStateIoBaseAddress;
    xUslWrMsr(MSR_CSTATE_ADDRESS, CstateAddr.Value);
  }
}
