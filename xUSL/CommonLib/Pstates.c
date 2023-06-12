/**
 * @file  Pstates.c
 * @brief OpenSIL Pstates Library
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "Pstates.h"
#include <CommonLib/CpuLib.h>
#include <MsrReg.h>

/**--------------------------------------------------------------------
 *  GetPstateInfo
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
 *  @param  PstateStatus  Output status for the Pstate.  True if Get
 *                        Pstate information successful.  False if
 *                        Pstate is disabled.  Not valid if function
 *                        returns unsuccessfully.
 *
 *  @retval SilPass       GetPstateInfo completed successfully
 *  @retval SilAborted    CpuDfsId (core frequency divisor) is invalid
 *  @retval SilAborted    IddDiv value is undefined (misfused CPU or
 *                        invalid PState MSR write)
 *
 */
SIL_STATUS
GetPstateInfo (
  PSTATE            Pstate,
  uint32_t          *Frequency,
  uint32_t          *VoltageInuV,
  uint32_t          *PowerInmW,
  bool              *PstateStatus
  )
{
  uint32_t          CpuFid;
  uint32_t          CpuDfsId;
  uint32_t          CpuVid;
  uint32_t          IddValue;
  uint32_t          IddDiv;
  uint32_t          PstateNumber;
  PSTATE_DEF_STRUCT PstateMsr;

  PstateNumber = (uint32_t) Pstate;
  PstateMsr.Value = xUslRdMsr (MSR_PSTATE_DEF_ADDRESS + PstateNumber);

  if (PstateMsr.Field.PstateEn == 0) {
    *PstateStatus = false;
    return SilPass;
  }

  XUSL_TRACEPOINT (SIL_TRACE_INFO, "  GetPstateFrequency - P%d\n", PstateNumber);

  CpuFid   = (uint32_t) PstateMsr.Field.CpuFid;
  CpuDfsId = (uint32_t) PstateMsr.Field.CpuDfsId;
  CpuVid   = (uint32_t) PstateMsr.Field.CpuVid;
  IddValue = (uint32_t) PstateMsr.Field.IddValue;
  IddDiv   = (uint32_t) PstateMsr.Field.IddDiv;

  // Values over 0x1A are Reserved if bit 0 is 1.
  if ((CpuDfsId > 0x1A) && ((CpuDfsId & 0x1) == 1)) {
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "Invalid CpuDfsId = 0x%x, bit 0 reserved\n", CpuDfsId);
    assert ((CpuDfsId <= 0x1A) || ((CpuDfsId & BIT_32(0)) == 0));
    return SilAborted;
  }

  if (CpuDfsId == 0) {
    *Frequency = 0;
  } else if ((CpuDfsId >= 8) && (CpuDfsId <= 0x3C)) {
    *Frequency = (size_t) ((200 * CpuFid) / CpuDfsId);
  } else {
    XUSL_TRACEPOINT (SIL_TRACE_INFO,
      "    Undefined CpuDfsId %d used. Force it to div 1.\n", CpuDfsId);
    *Frequency = (size_t) (25 * CpuFid);
  }

  XUSL_TRACEPOINT (SIL_TRACE_INFO,
    "    FrequencyInMHz=%d, CpuFid=%d, CpuVid=%d, CpuDfsId=%d\n", *Frequency,
    CpuFid, CpuVid, CpuDfsId);

  /*
   * Voltage:
   * CpuVid of 0xF8:0xFF would result in a 0 or negative voltage value.
   * Reduce to 0 for these cases.
   */
  if ((CpuVid >= 0xF8) && (CpuVid <= 0xFF)) {
    *VoltageInuV = 0;
  } else {
    *VoltageInuV = 1550000L - (6250 * CpuVid);
  }

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
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "    IddDiv value undefined = 0d%d\n",
      IddDiv);
    assert (IddDiv < 3);
    *PowerInmW = 0;
    return SilAborted;
  }

  *PstateStatus = true;
  return SilPass;
}

/**--------------------------------------------------------------------
 * GetPstateNumber
 *
 * @brief   Get number of Pstates
 *
 * @details Returns the maximum number of Pstates in NumberOfPstate
 *
 * @param   NumberOfBoostPstate Output number of boost Pstate
 * @param   NumberOfPstate      Output number of Pstates
 *
 * @retval  None
 */
void
GetPstateNumber (
  uint32_t             *NumberOfBoostPstates,
  uint32_t             *NumberOfPstates
  )
{
  PSTATE_CURLIM_STRUCT  CurLim;

  *NumberOfBoostPstates = 0;

  CurLim.Value = xUslRdMsr (MSR_PSTATE_CURLIM_ADDRESS);
  *NumberOfPstates = (uint32_t) CurLim.Field.PstateMaxVal;
}

/**--------------------------------------------------------------------
 * SwitchPstate
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
SwitchPstate (
  SWITCH_PSTATE           *SwitchPstate
  )
{
  PSTATE_DEF_STRUCT        PstateMsr;
  PSTATE_CTL_STRUCT        PstateCtl;
  SIL_RESERVED2_913_STRUCT SilReserved2913;
  uint32_t                 PstateNumber;

  PstateNumber = (uint32_t)SwitchPstate->Pstate;
  PstateMsr.Value = xUslRdMsr (MSR_PSTATE_DEF_ADDRESS + PstateNumber);
  if (PstateMsr.Field.PstateEn == 0) {
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "Pstate 0x%x not enabled.\n",
      SwitchPstate->Pstate);
    return SilInvalidParameter;
  }

  PstateCtl.Field.PstateCmd = (uint64_t)SwitchPstate->Pstate;
  xUslWrMsr (MSR_PSTATE_CTL_ADDRESS, PstateCtl.Value);

  if (SwitchPstate->WaitForCompletion) {
    if (SwitchPstate->Pstate != 0) {
      do {
        SilReserved2913.Value = xUslRdMsr (SIL_RESERVED2_913);
      } while ((SilReserved2913.Field.Reserved1 != PstateMsr.Field.CpuFid) ||
               (SilReserved2913.Field.Reserved2 != PstateMsr.Field.CpuDfsId));
    } else {
      do {
        SilReserved2913.Value = xUslRdMsr (SIL_RESERVED2_913);
      } while (((200 * SilReserved2913.Field.Reserved1) / SilReserved2913.Field.Reserved2) <
              ((200 * PstateMsr.Field.CpuFid) / PstateMsr.Field.CpuDfsId));
    }
  }

  return SilPass;
}

/**--------------------------------------------------------------------
 * GetCurrentPstate
 *
 * @brief   Get current Pstate
 *
 * @details Return the value of the current pstate CurPstate.
 *
 * @retval  uint8_t  current Pstate
 */
uint8_t
GetCurrentPstate (void)
{
  PSTATE_STATUS_STRUCT PstateSts;

  PstateSts.Value = xUslRdMsr (MSR_PSTATE_STATUS_ADDRESS);
  return (uint8_t) PstateSts.Field.CurPstate;
}

/**--------------------------------------------------------------------
 * GetPstateTransLatency
 *
 * @brief   call to get Pstate Transition Latency
 *
 * @param[out] TransitionLatency  The transition latency.
 *
 * @retval  SIL_STATUS  Always succeeds.
 */
SIL_STATUS
GetPstateTransLatency (
    uint32_t   *TransitionLatency
  )
{
  *TransitionLatency = 0;

  return (SilPass);
}
