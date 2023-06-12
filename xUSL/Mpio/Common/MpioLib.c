/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

/**
 * @file  MpioLib.c
 * @brief MPIO firmware messaging interface library
 *
 */

#include <xSIM.h>
#include <string.h>
#include <CommonLib/SmnAccess.h>
#include "MpioLib.h"
#include "MpioStructs.h"
#include "MpioCmn2Rev.h"

/**--------------------------------------------------------------------
 * @defgroup MPIO Invalid / Fatal Errors
 *
 * @{
 */
#ifndef INVALID_MPIO_MSG
  #define INVALID_MPIO_MSG  0xFF
#endif
#ifndef BIOSMPIO_Result_FATAL
  #define BIOSMPIO_Result_FATAL      ((uint8_t)0xFC)     // i.e. -4
#endif
/** @} */

/**--------------------------------------------------------------------
 * @defgroup BIOS/MPIO Result Types
 *
 * @{
 */
// MPIO Response Codes:
#define BIOSMPIO_Result_OK                    0x1

/**--------------------------------------------------------------------
 * @defgroup C2P Message Argument Addresses
 *
 * @{
 */
/** Address C2PMSG 5 */
#define MPIO_C2PMSG_RESPONSE_ADDRESS                        0xC9109C8ul
/** Address C2PMSG 51 to 56 */
#define MPIO_C2PMSG_ARGUMENT_0_ADDRESS                      0xC9109CCul
#define MPIO_C2PMSG_ARGUMENT_1_ADDRESS                      0xC9109D0ul
#define MPIO_C2PMSG_ARGUMENT_2_ADDRESS                      0xC9109D4ul
#define MPIO_C2PMSG_ARGUMENT_3_ADDRESS                      0xC9109D8ul
#define MPIO_C2PMSG_ARGUMENT_4_ADDRESS                      0xC9109DCul
#define MPIO_C2PMSG_ARGUMENT_5_ADDRESS                      0xC9109E0ul
#define MPIO_C2PMSG_RESPONSE_STATUS_MASK                    0xFFul
/** @} */

/**--------------------------------------------------------------------
 *
 * MpioServiceRequestCommon
 *
 * @brief Mpio service request
 *
 * @param[in]      NbioPciAddress  PCI_ADDR of this NBIO
 * @param[in]      RequestId       Request ID
 * @param[in]      RequestArgument Request Argument
 * @param[in]      AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @returns Mpio Service Request Response
 * @retval 32 Bit Enumerated Value
 **/
uint32_t
MpioServiceRequestCommon (
  PCI_ADDR                 NbioPciAddress,
  uint32_t                 RequestId,
  uint32_t                 *RequestArgument,
  uint32_t                 AccessFlags
  )
{
  uint32_t                  RxMpioMessageResponse;
  uint32_t                  DoorBellValue;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "  MPIO Service Request 0x%x Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    RequestId,
    RequestArgument[0],
    RequestArgument[1],
    RequestArgument[2],
    RequestArgument[3],
    RequestArgument[4],
    RequestArgument[5]
    );

  DoorBellValue = 0xFFFFFFFF;

  if (RequestId == INVALID_MPIO_MSG) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIOServiceRequest INVALID!!\n");
    return BIOSMPIO_Result_OK;
  }


  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Wait till MPIO ready bit is 1\n");

  /*
   * 1 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
   */
  do {
    RxMpioMessageResponse = xUSLSmnRead(NbioPciAddress.Address.Segment,
      NbioPciAddress.Address.Bus,
      MPIO_C2PMSG_RESPONSE_ADDRESS
      );
    RxMpioMessageResponse = (RxMpioMessageResponse & BIT_32(31));
  } while (RxMpioMessageResponse == 0x0);

  /*
   * 2 Write message arguments (Write cmd params to mmMPIO_C2PMSG_31-36)
   */
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_0_ADDRESS,
    RequestArgument[0]
    );
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_1_ADDRESS,
    RequestArgument[1]
    );
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_2_ADDRESS,
    RequestArgument[2]
    );
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_3_ADDRESS,
    RequestArgument[3]
    );
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_4_ADDRESS,
    RequestArgument[4]
    );
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_5_ADDRESS,
    RequestArgument[5]
    );

  /*
   * 3 Write message ID (Write cmd ID to mmMPIO_C2PMSG_30 with Ready_flag cleared)
   */
  RequestId = (RequestId & (0xFFF)) << 8;
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_RESPONSE_ADDRESS,
    RequestId
    );

  /*
   * 4 Write value to mmMPIO_C2PMSG_21 doorbell for MPIO to begin work
   */
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MpioXferTable->MpioGetC2pmsgDoorbellAddress (),
    DoorBellValue
    );

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Wait till MPIO service complete\n");
  /*
   * 5 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
   */
  do {
    RxMpioMessageResponse = xUSLSmnRead(NbioPciAddress.Address.Segment,
      NbioPciAddress.Address.Bus,
      MPIO_C2PMSG_RESPONSE_ADDRESS
      );
  } while ((RxMpioMessageResponse & BIT_32(31)) == 0x0);

  /*
   * 6 Read updated MPIO message arguments
   */
  RequestArgument[0] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_0_ADDRESS
    );
  RequestArgument[1] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_1_ADDRESS
    );
  RequestArgument[2] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_2_ADDRESS
    );
  RequestArgument[3] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_3_ADDRESS
    );
  RequestArgument[4] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_4_ADDRESS
    );
  RequestArgument[5] = xUSLSmnRead(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_5_ADDRESS
    );

  /*
   * If the status from MPIO is not okay
   */
  if ( BIOSMPIO_Result_OK != (RxMpioMessageResponse & MPIO_C2PMSG_RESPONSE_STATUS_MASK)) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "  MPIO ERROR! Error ID: 0x%x\n", RequestArgument[0]);
  }

  /*
   * 7. Read the response mmMPIO_C2PMSG_30 [15:0] and return function
   */
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioServiceRequest Exit\n");
  return (RxMpioMessageResponse);
}

/**--------------------------------------------------------------------
 *
 * MpioSmnPrivateRegRead
 *
 * @brief Read SMN register
 *
 * @details MPIO handles the read request
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  RegisterIndex  SMN address
 * @param[in]  RegisterValue  Value of the register which is read
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegRead (
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));

  MpioArg[0] = RegisterIndex;
  MpioArg[1] = 0;

  MpioServiceRequestCommon(NbioIp2Ip->GetHostPciAddress(GnbHandle), BIOS_MPIO_MSG_PCIE_PROXY_WRITE, MpioArg, 0);
  *RegisterValue = MpioArg[0];
}

/**--------------------------------------------------------------------
 *
 * MpioSmnPrivateRegWrite
 *
 * @brief Write SMN register
 *
 * @details MPIO handles the write request
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  RegisterIndex  Register SMN address
 * @param[in]  RegisterValue  Value to write to the register
 * @param[in]  Flags          Register access flags
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegWrite (
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue,
  uint32_t               Flags
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));
  MpioArg[0] = RegisterIndex;
  MpioArg[1] = 0xFFFFFFFF;
  MpioArg[2] = *RegisterValue;

  MpioServiceRequestCommon(NbioIp2Ip->GetHostPciAddress(GnbHandle), BIOS_MPIO_MSG_PCIE_PROXY_WRITE, MpioArg, Flags);
}

/**--------------------------------------------------------------------
 *
 * MpioSmnPrivateRegRMW
 *
 * @brief Read, write SMN register
 *
 * @details MPIO handles the read/write request
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  RegisterIndex  Register SMN address
 * @param[in]  AndMask        TBD
 * @param[in]  OrValue        TBD
 * @param[in]  Flags          Register access flags
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegRMW (
  GNB_HANDLE         *GnbHandle,
  uint32_t           RegisterIndex,
  uint32_t           AndMask,
  uint32_t           OrValue,
  uint32_t           Flags
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));

  MpioArg[0] = RegisterIndex;
  MpioArg[1] = ~AndMask;
  MpioArg[2] = OrValue;

  MpioServiceRequestCommon(NbioIp2Ip->GetHostPciAddress(GnbHandle), BIOS_MPIO_MSG_PCIE_PROXY_WRITE, MpioArg, Flags);
}
