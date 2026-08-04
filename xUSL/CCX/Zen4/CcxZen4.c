/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxZen4.c
 * @brief CCX Zen4 specific implementation.
 *
 */

#include <SilCommon.h>
#include <MsrReg.h>
#include <Ccx.h>
#include <CcxApic.h>
#include <CommonLib/CpuLib.h>
#include <xUslCcxRoles.h>
#include <Pstates.h>
#include "CcxZen4.h"
#include "Zen4Reg.h"

extern volatile AP_MSR_SYNC ApMsrSyncListZen4[];

/**
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
  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  switch (CcxInputBlock->AmdCpuSpeculativeStoreMode) {
  case 0:
    // Balanced
    xUslMsrOr(MSRxC00110E5, (uint64_t) BIT_64(26));
    xUslMsrAnd(MSRxC00110E6, ~(uint64_t) BIT_64(38));
    xUslMsrAnd(MSRxC00110EC, ~(uint64_t) BIT_64(0));
    xUslMsrAnd(MSRxC00110E2, ~(uint64_t) BIT_64(30));
    break;
  case 1:
    // More Speculative
    xUslMsrOr(MSRxC00110E5, (uint64_t) BIT_64(26));
    xUslMsrOr(MSRxC00110E6, (uint64_t) BIT_64(38));
    xUslMsrOr(MSRxC00110EC, (uint64_t) BIT_64(0));
    xUslMsrOr(MSRxC00110E2, (uint64_t) BIT_64(30));
    break;
  case 2:
    // Less Speculative
    xUslMsrAnd(MSRxC00110E5, ~(uint64_t) BIT_64(26));
    xUslMsrOr(MSRxC00110E6, (uint64_t) BIT_64(38));
    xUslMsrOr(MSRxC00110EC, (uint64_t) BIT_64(0));
    xUslMsrOr(MSRxC00110E2, (uint64_t) BIT_64(30));
    break;
  }

  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * Zen4CcxPrefetcher
 *
 * @brief This routine initializes L1/L2 prefetcher per user configurations.
 *        This routine is only executed on the BSP.
 *        APs will sync the applicable MSRs settings through ApMsrSyncList.
 *
 * @param CcxInputBlock Ccx input block data
 *
 */
void
Zen4CcxPrefetcher (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  uint64_t  LocalMsr;

  if (CcxInputBlock->L1BurstPrefetch) {
    xUslMsrOr(MSR_L2_PREFETCH_CFG, BIT_64(22));

    LocalMsr = xUslRdMsr(MSR_DC_CFG);
    LocalMsr &= ~(BIT_64(12) | BIT_64(55) | BIT_64(57) | BIT_64(58) | BIT_64(59) | BIT_64(63));
    LocalMsr |= BIT_64(55) | BIT_64(57) | BIT_64(58) | BIT_64(63);
    CCX_TRACEPOINT(SIL_TRACE_INFO, "MSR_DC_CFG updated value is 0x%lX\n", LocalMsr);
    xUslWrMsr(MSR_DC_CFG, LocalMsr);

    LocalMsr = xUslRdMsr(MSR_DC_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, "MSR_DC_CFG current value is 0x%lX\n", LocalMsr);
  } else {
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~(BIT_64(22)));

    LocalMsr = xUslRdMsr(MSR_DC_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_CFG current value is 0x%lX\n", LocalMsr);

    LocalMsr &= ~(BIT_64(12) | BIT_64(55) | BIT_64(57) | BIT_64(58) | BIT_64(59) | BIT_64(63));
    LocalMsr |= BIT_64(12) | BIT_64(59);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_CFG updated value is 0x%lX\n", LocalMsr);
    xUslWrMsr(MSR_DC_CFG, LocalMsr);

    LocalMsr = xUslRdMsr(MSR_DC_CFG);
    CCX_TRACEPOINT(SIL_TRACE_INFO, " MSR_DC_CFG current value is 0x%lX\n", LocalMsr);
  }

  // L1 Stream HW Prefetcher
  if (!CcxInputBlock->AmdL1StreamPrefetcher) {
    xUslMsrOr(MSR_DC_CFG, BIT_64(16));
  }

  // L1 Stride Prefetcher
  if (!CcxInputBlock->AmdL1StridePrefetcher) {
    xUslMsrOr(MSR_DC_CFG, BIT_64(17));
  }

  // L1 Region Prefetcher
  if (!CcxInputBlock->AmdL1RegionPrefetcher) {
    xUslMsrOr(MSR_DC_CFG, BIT_64(18));
  }

  // L2 Stream Prefetcher
  if (!CcxInputBlock->AmdL2StreamPrefetcher) {
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~((uint64_t)BIT_64(0)));
  }

  // L2 Up/Down Prefetcher
  if (!CcxInputBlock->AmdL2UpDownPrefetcher) {
    xUslMsrAnd(MSR_L2_PREFETCH_CFG, ~((uint64_t)BIT_64(2)));
  }
}

/**
 * Zen4GetApMsrSyncList
 *
 * @brief Return the AP MSR Sync List for Zen4
 *
 * @return Pointer to the ApMsrSyncList
 */
volatile AP_MSR_SYNC *
Zen4GetApMsrSyncList (void)
{
  return ApMsrSyncListZen4;
}

/**
 * Zen4InitializePrefetchMode
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
Zen4InitializePrefetchMode (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  )
{
  CCX_PREFETCH_MODE PrefetchMode;

  PrefetchMode.HardwarePrefetchMode = CcxInputBlock->AmdHardwarePrefetchMode;
  PrefetchMode.SoftwarePrefetchMode = CcxInputBlock->AmdSoftwarePrefetchMode;
  if ((PrefetchMode.HardwarePrefetchMode != HARDWARE_PREFETCHER_AUTO) ||
    (PrefetchMode.SoftwarePrefetchMode != SOFTWARE_PREFETCHES_AUTO)) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Zen4InitializePrefetchMode:\n");
    CCX_TRACEPOINT(SIL_TRACE_INFO,
      "HardwarePrefetchMode - %d\n",
      PrefetchMode.HardwarePrefetchMode
      );

    CCX_TRACEPOINT(SIL_TRACE_INFO,
      "SoftwarePrefetchMode - %d\n",
      PrefetchMode.SoftwarePrefetchMode
      );

    switch (PrefetchMode.HardwarePrefetchMode) {
    case DISABLE_HARDWARE_PREFETCH:
      xUslMsrOr(MSR_DC_CFG, (BIT_64(13) | BIT_64(15)));
      if (xUslIsComputeUnitPrimary ()) {
        xUslMsrOr(MSR_L2_PREFETCH_CFG, (BIT_64(3) | BIT_64(16) | BIT_64(17) | BIT_64(18)));
      }
      break;
    case DISABLE_L2_STRIDE_PREFETCHER:
    case DISABLE_L1_PREFETCHER:
      if (xUslIsComputeUnitPrimary ()) {
        xUslMsrOr(MSR_L2_PREFETCH_CFG, BIT_64(3));
      }
      break;
    case DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES:
      xUslMsrOr(MSR_DC_CFG, BIT_64(15));
      break;
    default:
      break;
    }
    // DISABLE_SOFTWARE_PREFETCHES
    if (PrefetchMode.SoftwarePrefetchMode == DISABLE_SOFTWARE_PREFETCHES) {
      if (xUslIsComputeUnitPrimary ()) {
        xUslMsrOr(MSR_DE_CFG, 0xFC);
      }
    }
  }
}

/**--------------------------------------------------------------------
 * Zen4SwitchPstate
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
Zen4SwitchPstate (
  SWITCH_PSTATE *SwitchPstate
  )
{
  PSTATE_DEF_STRUCT_ZEN4  PstateMsr;
  PSTATE_CTL_STRUCT       PstateCtl;
  SIL_RESERVED_UNION_0026 PstateSts;
  uint32_t                PstateNumber;

  PstateNumber = (uint32_t)SwitchPstate->Pstate;
  PstateMsr.Value = xUslRdMsr(MSR_PSTATE_DEF_ADDRESS + PstateNumber);
  if (PstateMsr.Field.PstateEn == 0) {
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
      /*
       * Switch to non-P0, check FID & DID.  If current Fid and DfsId do not
       * match, the switch has not yet completed.
       */
      do {
        PstateSts.Value = xUslRdMsr(MSRxC0010293);
      } while ((PstateSts.Field.field_bits_0_to_7 != PstateMsr.Field.CpuFid) ||
               (PstateSts.Field.field_bits_8_to_13 != PstateMsr.Field.CpuDfsId));
    } else {
      /*
       * Switch to P0, check current frequency >= P0.  If current frequency is
       * lower than P0 frequency, the switch has not yet completed.
       */
      do {
        PstateSts.Value = xUslRdMsr(MSRxC0010293);
      } while (((200 * PstateSts.Field.field_bits_0_to_7) / PstateSts.Field.field_bits_8_to_13) <
        ((200 * PstateMsr.Field.CpuFid) / PstateMsr.Field.CpuDfsId));
    }
  }

  return SilPass;
}

/**
 *  Zen4GetPstateInfo
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
 *  @retval SilAborted    IddDiv value is undefined
 *
 */
SIL_STATUS
Zen4GetPstateInfo (
  PSTATE    Pstate,
  uint32_t  *Frequency,
  uint32_t  *VoltageInuV,
  uint32_t  *PowerInmW,
  bool      *PstateStatus
  )
{
  uint32_t                CpuFid;
  uint32_t                CpuDfsId;
  uint32_t                CpuVid;
  uint32_t                IddValue;
  uint32_t                IddDiv;
  uint32_t                PstateNumber;
  PSTATE_DEF_STRUCT_ZEN4  PstateMsr;

  PstateNumber = (uint32_t) Pstate;
  PstateMsr.Value = xUslRdMsr(MSR_PSTATE_DEF_ADDRESS + PstateNumber);

  if (PstateMsr.Field.PstateEn == 0) {
    *PstateStatus = false;
    return SilPass;
  }

  XUSL_TRACEPOINT(SIL_TRACE_INFO, "  GetPstateFrequency - P%d\n", PstateNumber);

  CpuFid = (uint32_t) PstateMsr.Field.CpuFid;
  CpuDfsId = (uint32_t) PstateMsr.Field.CpuDfsId;
  CpuVid = (uint32_t) PstateMsr.Field.CpuVid;
  IddValue = (uint32_t) PstateMsr.Field.IddValue;
  IddDiv = (uint32_t) PstateMsr.Field.IddDiv;

  // Values over 0x1A are Reserved if bit 0 is 1.
  if ((CpuDfsId > 0x1A) && ((CpuDfsId & 0x1) == 1)) {
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Invalid CpuDfsId = 0x%x, bit 0 reserved\n", CpuDfsId);
    assert((CpuDfsId <= 0x1A) || ((CpuDfsId & BIT_32(0)) == 0));
    return SilAborted;
  }

  if (CpuDfsId == 0) {
    *Frequency = 0;
  } else if ((CpuDfsId >= 8) && (CpuDfsId <= 0x2C)) {
    *Frequency = (size_t) ((200 * CpuFid) / CpuDfsId);
  } else {
    XUSL_TRACEPOINT(SIL_TRACE_INFO, "    Undefined CpuDfsId %d used. Force it to div 1.\n", CpuDfsId);
    *Frequency = (size_t) (25 * CpuFid);
  }

  XUSL_TRACEPOINT(SIL_TRACE_INFO,
    "    FrequencyInMHz=%d, CpuFid=%d, CpuVid=%d, CpuDfsId=%d\n",
    *Frequency,
    CpuFid,
    CpuVid,
    CpuDfsId
    );

  if (CpuVid == 0) {
    *VoltageInuV = 0;
  } else {
    *VoltageInuV = 250000L + (5000L * (CpuVid - 1));
  }
  XUSL_TRACEPOINT(SIL_TRACE_INFO, "    SVI3 VoltageInuV=0x%x\n", *VoltageInuV);

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
     * IddDiv is set to an undefined value.
     */
    XUSL_TRACEPOINT(SIL_TRACE_ERROR,
      "    IddDiv value undefined = 0d%d\n",
      IddDiv
      );
    assert(IddDiv < 3);
    *PowerInmW = 0;
    return SilAborted;
  }

  XUSL_TRACEPOINT(SIL_TRACE_INFO, "    PowerInmW=0x%x\n", *PowerInmW);
  *PstateStatus = true;
  return SilPass;
}

/**
 * Zen4ReOrderLogicalCcdWithNumaDomainOrder
 *
 * Re-order is not supported for Zen4.  Just return OrderedLogicalCcd in sequential order.
 *
 * @param  LogicalSocket      Zero-based logical socket number.
 * @param  LogicalDie         Zero-based logical die number.
 * @param  NumberOfCcds       The number of logical CCDs.
 * @param  OrderedLogicalCcd  Output the ordered logical CCD array. Caller must ensure the output buffer contains
 *                            at least NumberOfCcds entries.
 *
 * @retval SilPass              Operation successfully completed.
 * @retval SilInvalidParameter  OrderedLogicalCcd output buffer is NULL.
 *
 **/
SIL_STATUS
Zen4ReOrderLogicalCcdWithNumaDomainOrder (
  uint32_t   LogicalSocket,
  uint32_t   LogicalDie,
  uint32_t   NumberOfCcds,
  uint32_t  *OrderedLogicalCcd
  )
{
  uint32_t   i;
  uint32_t   j;
  uint32_t   PhyCcdOrder[] = {0, 4, 8, 2, 6, 10, 3, 7, 11, 1, 5, 9};

  UNUSED(LogicalSocket);
  UNUSED(LogicalDie);

  if (OrderedLogicalCcd == NULL) {
    return SilInvalidParameter;
  }

  CCX_TRACEPOINT(SIL_TRACE_INFO, "Ordered Logical CCD: ");
  j = 0;
  for (i = 0; i < SIL_ARRAY_SIZE(PhyCcdOrder); i++) {
    if (PhyCcdOrder[i] >= NumberOfCcds) {
      continue;
    }
    OrderedLogicalCcd[j] = PhyCcdOrder[i];
    CCX_TRACEPOINT(SIL_TRACE_RAW, "%d ", OrderedLogicalCcd[j]);
    j++;
  }
  CCX_TRACEPOINT(SIL_TRACE_RAW, "\n");

  return SilPass;
}

/**
 * Zen4CcxInitializeC6
 * @brief This function is called by all threads to enables Core C6.
 * @param[in] CStateBaseAddr Pointer to CState base address
 *
 *
 */
void Zen4CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock)
{
  SIL_RESERVED_UNION_0006      CstateCfg;
  CSTATE_ADDRESS_MSR  CstateAddr;
  uint8_t             Cc6Enable;

  if (CcxInputBlock->AmdCStateMode == 1) {
    if (xUslIsComputeUnitPrimary ()) {
      Cc6Enable = CcxInputBlock->AmdCc6Ctrl & 1;
      CstateCfg.Value = 0;
      CstateCfg.Field.field_bits_0_to_5 = 8;
      CstateCfg.Field.field_bit_6 = Cc6Enable;
      CstateCfg.Field.field_bits_8_to_13 = 8;
      CstateCfg.Field.field_bit_14 = Cc6Enable;
      CstateCfg.Field.field_bits_16_to_21 = 8;
      CstateCfg.Field.field_bit_22 = Cc6Enable;
      xUslWrMsr(MSRxC0010296, CstateCfg.Value);
    }

    CstateAddr.Value = 0;
    CstateAddr.Field.CstateAddr = (uint32_t) CcxInputBlock->AmdCStateIoBaseAddress;
    xUslWrMsr(MSR_CSTATE_ADDRESS, CstateAddr.Value);
  }
}
