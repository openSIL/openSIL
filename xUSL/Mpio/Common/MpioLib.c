/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

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
 */
#ifndef INVALID_MPIO_MSG
  #define INVALID_MPIO_MSG  0xFF
#endif
#ifndef BIOSMPIO_Result_FATAL
  #define BIOSMPIO_Result_FATAL      ((uint8_t)0xFC)     // i.e. -4
#endif

/**--------------------------------------------------------------------
 * @defgroup BIOS/MPIO Result Types
 *
 */
// MPIO Response Codes:
#define BIOSMPIO_Result_OK                    0x1

#define MPIO_C2PMSG_RESPONSE_ADDRESS                        0xC9109C8ul
#define MPIO_C2PMSG_ARGUMENT_0_ADDRESS                      0xC9109CCul
#define MPIO_C2PMSG_ARGUMENT_1_ADDRESS                      0xC9109D0ul
#define MPIO_C2PMSG_ARGUMENT_2_ADDRESS                      0xC9109D4ul
#define MPIO_C2PMSG_ARGUMENT_3_ADDRESS                      0xC9109D8ul
#define MPIO_C2PMSG_ARGUMENT_4_ADDRESS                      0xC9109DCul
#define MPIO_C2PMSG_ARGUMENT_5_ADDRESS                      0xC9109E0ul
#define MPIO_C2PMSG_RESPONSE_STATUS_MASK                    0xFFul


/**--------------------------------------------------------------------
 *
 * MpioServiceRequestCommon
 *
 * @brief Mpio service request
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   NbioPciAddress      PCI_ADDR of this NBIO
 * @param   RequestId           Request ID
 * @param   RequestArgument     Request Argument
 * @param   AccessFlags         See GNB_ACCESS_FLAGS_* definitions
 *
 * @returns Mpio Service Request Response
 * @retval 32 Bit Enumerated Value
 **/
uint32_t
MpioServiceRequestCommon (
  SIL_CONTEXT              *SilContext,
  PCI_ADDR                 NbioPciAddress,
  uint32_t                 RequestId,
  uint32_t                 *RequestArgument,
  uint32_t                 AccessFlags
  )
{
  uint32_t                      RxMpioMessageResponse;
  uint32_t                      DoorBellValue;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MpioServiceRequest Enter\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  DoorBellValue = 0xFFFFFFFF;

  if (RequestId == INVALID_MPIO_MSG) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIOServiceRequest INVALID!!\n");
    return BIOSMPIO_Result_OK;
  }

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Service Request 0x%x\n", RequestId);
  MPIO_TRACEPOINT(SIL_TRACE_INFO,
    "  MPIO Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    RequestArgument[0],
    RequestArgument[1],
    RequestArgument[2],
    RequestArgument[3],
    RequestArgument[4],
    RequestArgument[5]
    );


  /*
   * 1 Wait until the MPIO is ready
   */
  do {
    RxMpioMessageResponse = xUSLSmnRead(NbioPciAddress.Address.Segment,
      NbioPciAddress.Address.Bus,
      MPIO_C2PMSG_RESPONSE_ADDRESS
      );
    RxMpioMessageResponse = (RxMpioMessageResponse & BIT_32(31));
  } while (RxMpioMessageResponse == 0x0);

  /*
   * 2 Write message arguments
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
   * 3 Write message ID
   */
  RequestId = (RequestId & (0xFFF)) << 8;
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_RESPONSE_ADDRESS,
    RequestId
    );

  /*
   * 4 Write value to doorbell for MPIO to begin work
   */
  xUSLSmnWrite(NbioPciAddress.Address.Segment,
    NbioPciAddress.Address.Bus,
    MpioXferTable->MpioGetC2pmsgDoorbellAddress (),
    DoorBellValue
    );

  /*
   * 5 Waiting MPIO until ready
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
   * 7. Read the response and return function
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   RegisterIndex       SMN address
 * @param   RegisterValue       Value of the register which is read
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegRead (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));

  MpioArg[0] = RegisterIndex;
  MpioArg[1] = 0;

  MpioServiceRequestCommon(SilContext,
    NbioIp2Ip->GetHostPciAddress(GnbHandle),
    BIOS_MPIO_MSG_PCIE_PROXY_WRITE,
    MpioArg,
    0
    );
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   RegisterIndex       Register SMN address
 * @param   RegisterValue       Value to write to the register
 * @param   Flags               Register access flags
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegWrite (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue,
  uint32_t               Flags
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));
  MpioArg[0] = RegisterIndex;
  MpioArg[1] = 0xFFFFFFFF;
  MpioArg[2] = *RegisterValue;

  MpioServiceRequestCommon(SilContext,
    NbioIp2Ip->GetHostPciAddress(GnbHandle),
    BIOS_MPIO_MSG_PCIE_PROXY_WRITE,
    MpioArg,
    Flags
    );
}

/**--------------------------------------------------------------------
 *
 * MpioSmnPrivateRegRMW
 *
 * @brief Read, write SMN register
 *
 * @details MPIO handles the read/write request
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   RegisterIndex       Register SMN address
 * @param   AndMask             TBD
 * @param   OrValue             TBD
 * @param   Flags               Register access flags
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioSmnPrivateRegRMW (
  SIL_CONTEXT        *SilContext,
  GNB_HANDLE         *GnbHandle,
  uint32_t           RegisterIndex,
  uint32_t           AndMask,
  uint32_t           OrValue,
  uint32_t           Flags
  )
{
  uint32_t  MpioArg[6];
  NBIO_IP2IP_API    *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(MpioArg, 0x0, sizeof (MpioArg));

  MpioArg[0] = RegisterIndex;
  MpioArg[1] = ~AndMask;
  MpioArg[2] = OrValue;

  MpioServiceRequestCommon(SilContext,
    NbioIp2Ip->GetHostPciAddress(GnbHandle),
    BIOS_MPIO_MSG_PCIE_PROXY_WRITE,
    MpioArg,
    Flags
    );
}
