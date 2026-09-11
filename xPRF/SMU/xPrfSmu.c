/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfSmu.c
 * @brief Platform Reference Firmware - SMU services.
 *
 * @details These wrappers expose SMU Ip-2-Ip operations as host-callable
 *          semantic services.
 *
 *          Fuse reads and throttle thresholds are deliberately absent: openSIL
 *          has no equivalent implementation.
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <xPRF-api.h>
#include <SMU/SmuIp2Ip.h>
#include <Nbio/NbioIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <Pci.h>
#include <xPrfSmu.h>

/**
 * xPrfSmuServiceRequest
 *
 * @brief   Send a service request to the SMU.
 *
 * @param   NbioPciAddress  PCI address of the NBIO whose SMU is targeted
 * @param   RequestId       Service request identifier
 * @param   RequestArgument Six-word argument buffer, updated in place
 * @param   AccessFlags     Register access flags
 *
 * @return  uint32_t        The SMU's response code
 */
uint32_t
xPrfSmuServiceRequest (
  uint32_t  NbioPciAddress,
  uint32_t  RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;
  PCI_ADDR      Address;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return SMC_Result_Failed;
  }

  Address.AddressValue = NbioPciAddress;
  return (uint32_t) SmuApi->SmuServiceRequest (Address, RequestId, RequestArgument, AccessFlags);
}

/**
 * xPrfSmuServiceRequestByInstance
 *
 * @brief   Send an SMU service request addressed by die instance.
 *
 * @details Callers that identify the SMU by instance rather than by PCI
 *          address need the instance resolved to a GNB handle and then to the
 *          host bridge address. Both steps are openSIL's, so they happen here
 *          rather than in host firmware.
 *
 * @param   InstanceId      Die instance
 * @param   RequestId       Service request identifier
 * @param   RequestArgument Six-word argument buffer, updated in place
 *
 * @return  SIL_STATUS
 * @retval  SilInvalidParameter The SMU rejected or failed the request
 */
SIL_STATUS
xPrfSmuServiceRequestByInstance (
  uint32_t  InstanceId,
  uint32_t  RequestId,
  uint32_t  *RequestArgument
  )
{
  SIL_STATUS     Status;
  SMU_IP2IP_API  *SmuApi;
  NBIO_IP2IP_API *NbioApi;
  GNB_HANDLE     *GnbHandle;
  SMC_RESULT     SmuResult;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  Status = SilGetIp2IpApi (SilId_NbioClass, (void **)&NbioApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "NBIO API not found!\n");
    return Status;
  }

  Status = SmuApi->SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "No GNB handle for instance %d\n", InstanceId);
    return Status;
  }

  SmuResult = SmuApi->SmuServiceRequest (
                        NbioApi->GetHostPciAddress (GnbHandle),
                        RequestId,
                        RequestArgument,
                        GNB_REG_ACC_FLAG_S3SAVE
                        );

  if (SmuResult != SMC_Result_OK) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU request 0x%x returned 0x%x\n", RequestId, SmuResult);
    return SilInvalidParameter;
  }

  return SilPass;
}

/**
 * xPrfSmuServiceInitArguments
 *
 * @brief   Clear an SMU service request argument buffer.
 *
 * @param   RequestArgument Argument buffer to initialise
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuServiceInitArguments (
  uint32_t  *RequestArgument
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  SmuApi->SmuServiceInitArguments (RequestArgument);

  return SilPass;
}

/**
 * xPrfSmuRegisterRead
 *
 * @brief   Read an SMU register on the given die.
 *
 * @param   InstanceId      Die instance
 * @param   RegisterIndex   Register to read
 * @param   RegisterValue   Populated with the register contents
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuRegisterRead (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  return SmuApi->SmuRegisterReadDie (InstanceId, RegisterIndex, RegisterValue);
}

/**
 * xPrfSmuRegisterWrite
 *
 * @brief   Write an SMU register on the given die.
 *
 * @param   InstanceId      Die instance
 * @param   RegisterIndex   Register to write
 * @param   RegisterValue   Value to write
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuRegisterWrite (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  return SmuApi->SmuRegisterWriteDie (InstanceId, RegisterIndex, RegisterValue);
}

/**
 * xPrfSmuRegisterRMW
 *
 * @brief   Read-modify-write an SMU register on the given die.
 *
 * @param   InstanceId        Die instance
 * @param   RegisterIndex     Register to modify
 * @param   RegisterANDValue  Mask applied to the current value
 * @param   RegisterORValue   Bits set after masking
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuRegisterRMW (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  RegisterANDValue,
  uint32_t  RegisterORValue
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  return SmuApi->SmuRegisterRMWDie (InstanceId, RegisterIndex, RegisterANDValue, RegisterORValue);
}

/**
 * xPrfSmuReadBrandString
 *
 * @brief   Read the processor brand string from the SMU.
 *
 * @param   InstanceId          Die instance
 * @param   BrandStringLength   Size of the caller's buffer
 * @param   BrandString         Buffer to fill
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuReadBrandString (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  return SmuApi->SmuReadBrandString (InstanceId, BrandStringLength, BrandString);
}

/**
 * xPrfSmuReadCacWeights
 *
 * @brief   Read the APM CAC weights from the SMU.
 *
 * @param   MaxNumWeights   Number of weights the caller's buffer holds
 * @param   ApmWeights      Buffer to fill
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfSmuReadCacWeights (
  uint32_t  MaxNumWeights,
  uint64_t  *ApmWeights
  )
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return Status;
  }

  return SmuApi->SmuReadCacWeights (MaxNumWeights, ApmWeights);
}

/**
 * xPrfSmuEnableNvmSelfRefresh
 *
 * @brief Ask the SMU to preserve NVM state when required for warm restore.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
xPrfSmuEnableNvmSelfRefresh (void)
{
  SIL_STATUS    Status;
  SMU_IP2IP_API *SmuApi;

  Status = SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi);
  if ((Status != SilPass) || (SmuApi == NULL)) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "SMU API not found!\n");
    return (Status == SilPass) ? SilNotFound : Status;
  }

  return SmuApi->SmuEnableNvmSelfRefresh ();
}

/**
 * xPrfSmnRead
 *
 * @brief   Read an SMN register.
 *
 * @details Host firmware cannot link xUSL directly, so SMN access is exposed
 *          here like every other openSIL service.
 *
 * @param   SegmentNumber   PCI segment
 * @param   IohcBus         IOHC bus number
 * @param   SmnAddress      SMN address to read
 *
 * @return  uint32_t        The register contents
 */
uint32_t
xPrfSmnRead (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress
  )
{
  return xUSLSmnRead (SegmentNumber, IohcBus, SmnAddress);
}

/**
 * xPrfSmnWrite
 *
 * @brief   Write an SMN register.
 *
 * @param   SegmentNumber   PCI segment
 * @param   IohcBus         IOHC bus number
 * @param   SmnAddress      SMN address to write
 * @param   Value           Value to write
 */
void
xPrfSmnWrite (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress,
  uint32_t  Value
  )
{
  xUSLSmnWrite (SegmentNumber, IohcBus, SmnAddress, Value);
}

/**
 * xPrfSmnReadModifyWrite
 *
 * @brief   Read-modify-write an SMN register.
 *
 * @param   SegmentNumber   PCI segment
 * @param   IohcBus         IOHC bus number
 * @param   SmnAddress      SMN address to modify
 * @param   AndMask         Mask applied to the current value
 * @param   OrMask          Bits set after masking
 */
void
xPrfSmnReadModifyWrite (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress,
  uint32_t  AndMask,
  uint32_t  OrMask
  )
{
  xUSLSmnReadModifyWrite (SegmentNumber, IohcBus, SmnAddress, AndMask, OrMask);
}

/**
 * xPrfPciRead
 *
 * @brief   Read PCI config space.
 *
 * @param   Address   Encoded PCI address
 * @param   Width     Access width
 * @param   Value     Populated with the register contents
 */
void
xPrfPciRead (
  uint32_t      Address,
  uint32_t      Width,
  void          *Value
  )
{
  xUSLPciRead (Address, (ACCESS_WIDTH) Width, Value);
}

/**
 * xPrfPciWrite
 *
 * @brief   Write PCI config space.
 *
 * @details openSIL exposes sized writers rather than a width-dispatching one,
 *          so the width is resolved here.
 *
 * @param   Address   Encoded PCI address
 * @param   Width     Access width
 * @param   Value     Points at the value to write
 */
void
xPrfPciWrite (
  uint32_t      Address,
  uint32_t      Width,
  void          *Value
  )
{
  switch (Width) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    xUSLPciWrite8 (Address, *(uint8_t *)Value);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    xUSLPciWrite16 (Address, *(uint16_t *)Value);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    xUSLPciWrite32 (Address, *(uint32_t *)Value);
    break;
  case AccessWidth64:
  case AccessS3SaveWidth64:
    xUSLPciWrite64 (Address, *(uint64_t *)Value);
    break;
  default:
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "Unsupported PCI access width %d\n", Width);
    break;
  }
}

/**
 * xPrfPciRmw
 *
 * @brief   Read-modify-write PCI config space.
 *
 * @param   Address   Encoded PCI address
 * @param   Width     Access width
 * @param   Mask      Mask applied to the current value
 * @param   OrValue   Bits set after masking
 */
void
xPrfPciRmw (
  uint32_t      Address,
  uint32_t      Width,
  uint32_t      Mask,
  uint32_t      OrValue
  )
{
  xUSLPciRMW (Address, (ACCESS_WIDTH) Width, Mask, OrValue);
}
