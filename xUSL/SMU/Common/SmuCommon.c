/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuCommon.c
 * @brief OpenSIL SMU initialization
 *
 */


#include <xSIM.h>
#include "SmuCommon.h"
#include "SmuCmn2Rev.h"
#include <CommonLib/CpuLib.h>
#include <Nbio/NbioIp2Ip.h>
#include <string.h>

/**
 * SmuServiceInitArgumentsCommon
 *
 * @brief Routine to Init argument to 0
 *
 * @param  SmuArg Input Request Argument buffer to initialize
 */
void
SmuServiceInitArgumentsCommon (
  uint32_t *SmuArg
  )
{
  memset(SmuArg, 0x0, SMU_ARGUMENT_SIZE);
}

/**
 * SmuFirmwareTestCommon
 *
 * @brief   SMU firmware test
 *
 * @param   NbioHandle  Pointer to GNB_HANDLE
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             Firmware test completed
 * @retval  SilInvalidParameter Invalid Smu Argument
 * @retval  SilNotFound         IP transfer table was not found
 */
SIL_STATUS
SmuFirmwareTestCommon (
  SIL_CONTEXT   *SilContext,
  GNB_HANDLE    *NbioHandle
  )
{
  uint32_t                     SmuArg[6];
  uint32_t                     RequestId;
  SMU_COMMON_2_REV_XFER_BLOCK  *SmuXfer;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = 0xAA55AA55;
  RequestId = SMC_MSG_TestMessage;
  if (SilGetCommon2RevXferTable(SilContext, SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }
  SmuXfer->SmuServiceRequest(NbioHandle->Address,
    RequestId,
    SmuArg,
    0
    );
  SMU_TRACEPOINT(SIL_TRACE_INFO, "Service Test Return 0x%x\n", SmuArg[0]);

  if (SmuArg[0] == 0) {
    SMU_TRACEPOINT(SIL_TRACE_EXIT, "Exit Status = SilInvalidParameter.\n");
    return SilInvalidParameter;
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * SmuGetGnbHandleCommon
 *
 * @brief   Return GNB_HANDLE of Nbio InstanceId
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket (for GN, the socket number)
 * @param   Handle      Pointer to GNB_HANDLE* to return GNB_HANDLE of this
 *                      InstanceId
 *
 * @retval  SilPass             GnbHandle is found associated with socket
 *                              number
 * @retval  SilInvalidParameter InstanceId did not reference a valid NBIO
 *                              instance
 **/
SIL_STATUS
SmuGetGnbHandleCommon (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  GNB_HANDLE     **Handle
  )
{
  SIL_STATUS            Status;
  bool                  InstanceFound;
  GNB_HANDLE            *GnbHandle;
  NBIO_IP2IP_API        *NbioIp2Ip;

  Status = SilInvalidParameter;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  GnbHandle = NbioIp2Ip->GetGnbHandle(SilContext);

  InstanceFound = false;
  while ((InstanceFound == false) && (GnbHandle != NULL)) {
    if (GnbHandle->SocketId == InstanceId) {
      InstanceFound = true;
      SMU_TRACEPOINT(SIL_TRACE_INFO, "Instance found.\n");
      Status = SilPass;
    } else {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "Gnb next handle.\n");
      GnbHandle = GnbGetNextHandle(GnbHandle);
    }
  }

  *Handle = GnbHandle;
  SMU_TRACEPOINT(SIL_TRACE_INFO, "Exit Status: 0x%x\n", Status);
  return Status;
}

/**
 * SmuRegisterWrite
 *
 * @brief Write the Value to an SMN register
 *
 *
 * @param[in] GnbPciAddress      PCI_ADDR
 * @param[in] Address            Register offset, but PortDevice
 * @param[in] Value              The value to write
 * @retval    AGESA_STATUS
 */
void
SmuRegisterWrite (
  PCI_ADDR    PciAddress,
  uint32_t    RegisterIndex,
  uint32_t    Value,
  uint32_t    Flags
  )
{
  UNUSED(Flags);

  xUSLIndirectPciWrite32(PciAddress.AddressValue, RegisterIndex, Value);
  return;
}

/**
 * SmuRegisterRead
 *
 * @brief   Read an SMU register and return the value in Value
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RegisterIndex   The index of the register to be read
 * @param   Value           Pointer to a UINT32 to store the value read from the register
 * @param   Flags           See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SMC_RESULT
 */
void
SmuRegisterRead (
  PCI_ADDR    PciAddress,
  uint32_t    RegisterIndex,
  uint32_t    *Value,
  uint32_t    Flags
  )
{
  UNUSED(Flags);

  xUSLIndirectPciRead32(PciAddress.AddressValue, RegisterIndex, Value);
  return;
}

/**
 * SmuRegisterReadDie
 *
 * @brief   Reads a 32-bit register from SMU register space (Generally intended
 *          for NBIO internal requests)
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param   InstanceId    The unique identifier of the NBIO instance associated
 *                        with this socket/die
 * @param   RegisterIndex The index of the register to be read
 * @param   RegisterValue Pointer to a UINT32 to store the value read from the
 *                        register
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass             The specified register has been read
 * @retval  SilInvalidParameter One of the input parameters was invalid
 *                              InstanceId did not reference a valid NBIO
 *                              instance
 * @retval  SilUnsupported      This function is not supported by this version
 *                              of the driver
 *
 **/
SIL_STATUS
SmuRegisterReadDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon(SilContext, InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciRead32(PcieAddress.AddressValue, RegisterIndex, RegisterValue);
  }
  return Status;
}

/**
 * SmuRegisterWriteDie
 *
 * @brief   Writes a 32-bit register in SMU register space (Generally intended
 *          for NBIO internal requests)
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param   InstanceId    The unique identifier of the NBIO instance associated
 *                        with this socket/die
 * @param   RegisterIndex The index of the register to write
 * @param   RegisterValue Pointer to a UINT32 to store the value read from the
 *                        register
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass             The specified register has been updated with the requested value
 * @retval  SilInvalidParameter One of the input parameters was invalid
 *                              InstanceId did not reference a valid NBIO
 *                              instance
 * @retval  SilUnsupported      This function is not supported by this version
 *                              of the driver
 *
 **/
SIL_STATUS
SmuRegisterWriteDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon(SilContext, InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciWrite32(PcieAddress.AddressValue, RegisterIndex, *RegisterValue);
  }
  return Status;
}

/**
 * SmuRegisterRMWDie
 *
 * @brief   Read/Modify/Write a 32-bit register from SMU register space
 *          (Generally intended for NBIO internal requests)
 *
 * @param   SilContext        A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param   InstanceId        The unique identifier of the NBIO instance
 *                            associated with this socket/die
 * @param   RegisterIndex     The index of the register read/modify/write
 * @param   RegisterANDValue  Holds the value to be "ANDed" with the data read
 *                            from the register (i.e. mask of data that will
 *                            not be modified)
 * @param   RegisterORValue   Holds the value to be "ORed" with the data read
 *                            from the register (i.e. data bits to be "set" by
 *                            this function)
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass             The specified register has been updated with the requested value
 * @retval  SilInvalidParameter One of the input parameters was invalid
 *                              InstanceId did not reference a valid NBIO
 *                              instance
 * @retval  SilUnsupported      This function is not supported by this version
 *                              of the driver
 *
 **/
SIL_STATUS
SmuRegisterRMWDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       RegisterANDValue,
  uint32_t       RegisterORValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  uint32_t   RegisterValue;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon(SilContext, InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciRead32(PcieAddress.AddressValue, RegisterIndex, &RegisterValue);
    RegisterValue = (RegisterValue & RegisterANDValue) | RegisterORValue;
    xUSLIndirectPciWrite32(PcieAddress.AddressValue, RegisterIndex, RegisterValue);
  }
  return Status;
}

/**
 * SmuDisableSmt
 *
 * @brief   Disable simultaneous multithreading (SMT) on all dies in the system.
 *
 * @details This routine should only be called from the BSP.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilUnsupported if SmuDisableSmtCommon returned an error
 * @retval  SilPass otherwise
 **/
SIL_STATUS
SmuDisableSmtCommon (void)
{
  return SilUnsupported;
}
