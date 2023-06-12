/**
 * @file  SmuCommon.c
 * @brief OpenSIL SMU initialization
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "SmuCommon.h"
#include "SmuCmn2Rev.h"
#include <CommonLib/CpuLib.h>
#include <NBIO/NbioIp2Ip.h>
#include <ApobCmn.h>
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
  memset (SmuArg, 0x0, SMU_ARGUMENT_SIZE);
}

/**
 * SmuServiceRequest
 *
 * @brief   SMU service request
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RequestId       Host Firmware to SMU Message ID
 * @param   RequestArgument Request Argument
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SMC_RESULT
 */
SMC_RESULT
SmuServiceRequestCommon (
  PCI_ADDR PciAddress,
  SMC_MSG  RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  uint32_t SmuMessageResponse;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (RequestId > SMC_Message_Count) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "SmuServiceRequest INVALID!!.\n");
    return SMC_Result_OK;
  }

  SMU_TRACEPOINT (SIL_TRACE_INFO, "Service Request 0x%x\n", RequestId);
  SMU_TRACEPOINT (SIL_TRACE_INFO, "Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
                  RequestArgument[0], RequestArgument[1], RequestArgument[2],
                  RequestArgument[3], RequestArgument[4], RequestArgument[5]);

  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse);

  // 1 Clear Response
  SmuMessageResponse = 0;
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_RESPONSE_ADDRESS, SmuMessageResponse);

  // 2 Write message arguments
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_0_ADDRESS, RequestArgument[0]);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_1_ADDRESS, RequestArgument[1]);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_2_ADDRESS, RequestArgument[2]);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_3_ADDRESS, RequestArgument[3]);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_4_ADDRESS, RequestArgument[4]);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_5_ADDRESS, RequestArgument[5]);

  // 3 Write message ID
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_MESSAGE_ADDRESS, (uint32_t)RequestId);

  // 4 Poll Response until non-zero
  do {
    xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse);
    SMU_TRACEPOINT (SIL_TRACE_INFO, "Poll SMU Message Response until non-zero!! Current SMU Message Response 0x%x\n",
      SmuMessageResponse);
  } while (SmuMessageResponse == 0x0);
  SMU_TRACEPOINT (SIL_TRACE_INFO, "After SMU Message Responded!!\n");

  assert ((SMC_RESULT)SmuMessageResponse != SMC_Result_Fatal);

  // 5 Read updated SMU message arguments
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0]);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1]);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2]);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3]);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4]);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5]);

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SmuMessageResponse;
}

/**
 * SmuFirmwareTestCommon
 *
 * @brief   SMU firmware test
 *
 * @param   NbioHandle  Pointer to GNB_HANDLE
 *
 * @retval  SilPass             Firmware test completed
 * @retval  SilInvalidParameter Invalid Smu Argument
 * @retval  SilNotFound         IP transfer table was not found
 */
SIL_STATUS
SmuFirmwareTestCommon (
  GNB_HANDLE *NbioHandle
  )
{
  uint32_t                     SmuArg[6];
  uint32_t                     RequestId;
  SMU_COMMON_2_REV_XFER_BLOCK  *SmuXfer;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  SmuServiceInitArgumentsCommon (SmuArg);
  SmuArg[0] = 0xAA55AA55;
  RequestId = SMC_MSG_TestMessage;
  if (SilGetCommon2RevXferTable (SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }
  SmuXfer->SmuServiceRequest (
    NbioHandle->Address,
    RequestId,
    SmuArg,
    0
    );
  SMU_TRACEPOINT (SIL_TRACE_INFO, "Service Test Return 0x%x\n", SmuArg[0]);

  if (SmuArg[0] == 0) {
    SMU_TRACEPOINT (SIL_TRACE_EXIT, "Exit Status = SilInvalidParameter.\n");
    return SilInvalidParameter;
  }

  SMU_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * DxioServiceRequestCommon
 *
 * @brief   DXIO firmware request - Common function
 *
 * @param   NbioHandle      Pointer to GNB_HANDLE
 * @param   DxioMBoxRequest DXIO Mailbox Request ID
 * @param   DxioMBoxArg     SMU Argument structure - [0] entry reserved for
 *                          DXIO Mailbox Request ID
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  RetVal  Smu Response
 */
SMC_RESULT
DxioServiceRequestCommon (
  GNB_HANDLE  *NbioHandle,
  uint32_t    DxioMBoxRequest,
  uint32_t    *DxioMBoxArg,
  uint32_t    AccessFlags
  )
{
  SMC_RESULT                  RetVal;
  SMU_COMMON_2_REV_XFER_BLOCK *SmuXfer;

  if (SilGetCommon2RevXferTable (SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SMC_Result_Failed;
  }

  *DxioMBoxArg = DxioMBoxRequest;
  SMU_TRACEPOINT (
    SIL_TRACE_INFO,
    "Input Args = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    DxioMBoxRequest,
    DxioMBoxArg[1],
    DxioMBoxArg[2],
    DxioMBoxArg[3],
    DxioMBoxArg[4],
    DxioMBoxArg[5]
    );

  RetVal = SmuXfer->SmuServiceRequest  (
    NbioHandle->Address,
    SMC_MSG_DxioTestMessage,
    DxioMBoxArg,
    AccessFlags
    );

  SMU_TRACEPOINT (
    SIL_TRACE_INFO,
    "Return Args = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    DxioMBoxArg[0],
    DxioMBoxArg[1],
    DxioMBoxArg[2],
    DxioMBoxArg[3],
    DxioMBoxArg[4],
    DxioMBoxArg[5]
    );

  return RetVal;
}

/**
 * SmuNotifyS3EntryCommon
 *
 * @brief SMU S3 Entry Notification
 *
 */
void
SmuNotifyS3EntryCommon (void)
{
  PCI_ADDR PciAddress;
  uint32_t RequestId;
  uint32_t RxSmuMessageResponse;
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  xUSLIndirectPciRead32 (PciAddress.AddressValue, MP1_C2PMSG_RESPONSE_ADDRESS, &RxSmuMessageResponse);

  // 1 Clear Response
  RxSmuMessageResponse = 0;
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_RESPONSE_ADDRESS, RxSmuMessageResponse);

  // 2 Clear Message Arguments
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_0_ADDRESS, RxSmuMessageResponse);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_1_ADDRESS, RxSmuMessageResponse);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_2_ADDRESS, RxSmuMessageResponse);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_3_ADDRESS, RxSmuMessageResponse);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_4_ADDRESS, RxSmuMessageResponse);
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_ARGUMENT_5_ADDRESS, RxSmuMessageResponse);

  // 3 Write message ID
  RequestId = SMC_MSG_SleepEntry;
  xUSLIndirectPciWrite32 (PciAddress.AddressValue, MP1_C2PMSG_MESSAGE_ADDRESS, RequestId);

  // Halt execution
  xUslCpuSleep ();
}

/**
 * SmuGetGnbHandleCommon
 *
 * @brief   Return GNB_HANDLE of Nbio InstanceId
 *
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket
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
  uint32_t InstanceId,
  GNB_HANDLE **Handle
  )
{
  SIL_STATUS            Status;
  bool                  InstanceFound = false;
  GNB_HANDLE            *GnbHandle;
  NBIO_IP2IP_API        *NbioIp2Ip;

  Status = SilInvalidParameter;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }
  GnbHandle = NbioIp2Ip->GetGnbHandle ();

  while ((InstanceFound == false) && (GnbHandle != NULL)) {
    if (GnbHandle->SocketId == InstanceId) {
      InstanceFound = true;
      SMU_TRACEPOINT (SIL_TRACE_INFO, "Instance found.\n");
      Status = SilPass;
    } else {
      SMU_TRACEPOINT (SIL_TRACE_INFO, "Gnb next handle.\n");
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }

  *Handle = GnbHandle;
  SMU_TRACEPOINT (SIL_TRACE_INFO, "Exit Status: 0x%x\n", Status);
  return Status;
}

/**
 * SmuReadBrandStringCommon
 *
 * @brief   Returns the brand string read from SMN space.
 *
 * @param   InstanceId        The unique identifier of the SMU instance
 *                            associated with this socket
 * @param   BrandStringLength Number of characters to be read. This value does
 *                            NOT have to be a multiple of 4. NBIO driver code
 *                            will be responsible for alignment, endianness,
 *                            and string length.
 * @param   BrandString       Pointer to a CHAR array to store the brand string.
 *
 * @return  SIL_STATUS
 * @retval  SilNotFound       IP transfer table was not found
 **/
SIL_STATUS
SmuReadBrandStringCommon (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  )
{
  GNB_HANDLE                  *GnbHandle = NULL;
  SIL_STATUS                  Status = SilPass;
  uint32_t                    SmuArg[6];
  uint32_t                    SmuResult;
  uint8_t                     *StringPtr;
  uint32_t                    StringCount;
  uint32_t                    ByteCount;
  uint32_t                    Iteration;
  SMU_COMMON_2_REV_XFER_BLOCK *SmuXfer;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable (SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }

  Status = SmuGetGnbHandleCommon (InstanceId, &GnbHandle);
  if (Status == SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_INFO, "GnbHandle Found.\n");
    StringCount = BrandStringLength;
    StringPtr = BrandString;
    Iteration = 0;
    while (StringCount != 0) {
      SmuServiceInitArgumentsCommon (SmuArg);
      SmuArg[0] = Iteration++;
      SmuResult = SmuXfer->SmuServiceRequest (
        GnbHandle->Address,
        SMC_MSG_GetNameString,
        SmuArg,
        0
        );
      if (SmuResult == SMC_Result_OK) {
        ByteCount = (StringCount < STRING_COUNT_LIMIT) ? StringCount : STRING_COUNT_LIMIT;
        memcpy (StringPtr, SmuArg, ByteCount);
        StringPtr += ByteCount;
        StringCount -= ByteCount;
      } else {
        Status = SilInvalidParameter;
        SMU_TRACEPOINT (SIL_TRACE_INFO, "SmuRequestFail!!\n");
        break;
      }
    }
  }

  SMU_TRACEPOINT (SIL_TRACE_EXIT, "Exit Status:0x%x.\n", Status);
  return Status;
}

/**
 * SmuReadBistInfoCommon
 *
 * @brief Retrieve BIST info from SMU
 *
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket
 * @param   PhysicalCcx Number of the physical CCX
 * @param   BistInfo    Pointer to uint32_t where BIST info should be copied.
 *                      Caller should allocate the memory for BistData.
 *
 * @return  SIL_STATUS  SilNotFound         If unable to make SMU call
 *                      SilNotFound         IP transfer table not found
 *                      SilInvalidParameter If null pointer passed in
 *                      SilPass             If call succeded and return argument is populated. Returns
 *                      SilUnsupported      SMU does not support this call
 *
 **/
SIL_STATUS
SmuReadBistInfoCommon (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  )
{
  uint32_t                    SmuArg[6];
  SIL_STATUS                  Status;
  SMC_RESULT                  SmcStatus;
  GNB_HANDLE                  *GnbHandle;
  SMU_COMMON_2_REV_XFER_BLOCK *SmuXfer;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (BistInfo == NULL) {
    return SilInvalidParameter;
  }

  if (SilGetCommon2RevXferTable (SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }

  Status = SmuGetGnbHandleCommon (InstanceId, &GnbHandle);

  if (Status == SilPass) {
    SmuServiceInitArgumentsCommon (SmuArg);
    SmuArg[0] = PhysicalCcx;
    SmcStatus = SmuXfer->SmuServiceRequest (
      GnbHandle->Address,
      SMC_MSG_GetPerSrcBistPF,
      SmuArg,
      0);
    if (SmcStatus != SMC_Result_OK) {
      SMU_TRACEPOINT (SIL_TRACE_INFO, "BIST call to SMU failed.\n");
      return SilUnsupported;
    }

    *BistInfo = SmuArg[0];

    SMU_TRACEPOINT (SIL_TRACE_INFO, "BIST INFO = 0x%x.\n", *BistInfo);
    Status = SilPass;
  } else {
    SMU_TRACEPOINT (SIL_TRACE_INFO, "Unable to make BIST call to SMU. Status: 0x%x\n", Status);
    Status = SilNotFound;
  }

  SMU_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * SmuRegisterReadCommon
 *
 * @brief   Reads a 32-bit register from SMU register space (Generally intended
 *          for NBIO internal requests)
 *
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
SmuRegisterReadCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon (InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciRead32 (PcieAddress.AddressValue, RegisterIndex, RegisterValue);
  }
  return Status;
}

/**
 * SmuRegisterWriteCommon
 *
 * @brief   Writes a 32-bit register in SMU register space (Generally intended
 *          for NBIO internal requests)
 *
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
SmuRegisterWriteCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon (InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciWrite32 (PcieAddress.AddressValue, RegisterIndex, *RegisterValue);
  }
  return Status;
}

/**
 * SmuRegisterRMWCommon
 *
 * @brief   Read/Modify/Write a 32-bit register from SMU register space
 *          (Generally intended for NBIO internal requests)
 *
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
SmuRegisterRMWCommon (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  RegisterANDValue,
  uint32_t  RegisterORValue
  )
{
  GNB_HANDLE *GnbHandle;
  SIL_STATUS Status;
  uint32_t   RegisterValue;
  PCI_ADDR   PcieAddress;

  Status = SmuGetGnbHandleCommon (InstanceId, &GnbHandle);
  if (Status == SilPass) {
    PcieAddress.AddressValue = GnbHandle->Address.AddressValue;
    xUSLIndirectPciRead32 (PcieAddress.AddressValue, RegisterIndex, &RegisterValue);
    RegisterValue = (RegisterValue & RegisterANDValue) | RegisterORValue;
    xUSLIndirectPciWrite32 (PcieAddress.AddressValue, RegisterIndex, RegisterValue);
  }
  return Status;
}