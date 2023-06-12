/**
 * @file  MpioLib.c
 * @brief MPIO firmware messaging interface library
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <string.h>
#include "MpioLib.h"
#include "MpioStructs.h"

/**--------------------------------------------------------------------
 * @defgroup MPIO Invalid / Fatal Errors
 *
 * @{
 */
#ifndef INVALID_MPIO_MSG
  /** tbd. */
  #define INVALID_MPIO_MSG  0xFF
#endif
#ifndef BIOSMPIO_Result_FATAL
  /** tbd. */
  #define BIOSMPIO_Result_FATAL      ((uint8_t)0xFC)     // i.e. -4
#endif
/** @} */

/**--------------------------------------------------------------------
 * @defgroup Host Firmware/MPIO Result Types
 *
 * @{
 */
#define BIOSMPIO_Result_OK                    0x1

/**--------------------------------------------------------------------
 * @defgroup C2P Message Argument Addresses
 *
 * @{
 */
/** Address C2PMSG 21 */
#define MPIO_C2PMSG_DOORBELL_ADDRESS                        0xc910554ul
/** Address C2PMSG 5 */
#define MPIO_C2PMSG_RESPONSE_ADDRESS                        0xC9109C8ul
/** Address C2PMSG 51 to 56 */
#define MPIO_C2PMSG_ARGUMENT_0_ADDRESS                      0xC9109CCul
/** tbd. */
#define MPIO_C2PMSG_ARGUMENT_1_ADDRESS                      0xC9109D0ul
/** tbd. */
#define MPIO_C2PMSG_ARGUMENT_2_ADDRESS                      0xC9109D4ul
/** tbd. */
#define MPIO_C2PMSG_ARGUMENT_3_ADDRESS                      0xC9109D8ul
/** tbd. */
#define MPIO_C2PMSG_ARGUMENT_4_ADDRESS                      0xC9109DCul
/** tbd. */
#define MPIO_C2PMSG_ARGUMENT_5_ADDRESS                      0xC9109E0ul
/** tbd. */
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
 //uint32_t                  Address;
 uint32_t                  DoorBellValue;

 MPIO_TRACEPOINT (SIL_TRACE_INFO, "MpioServiceRequest Enter\n");

 DoorBellValue = 0xFFFFFFFF;

 if (RequestId == INVALID_MPIO_MSG) {
   MPIO_TRACEPOINT (SIL_TRACE_INFO, "MPIOServiceRequest INVALID!!\n");
   return BIOSMPIO_Result_OK;
 }

 MPIO_TRACEPOINT (SIL_TRACE_INFO, "  MPIO Service Request 0x%x\n", RequestId);
 MPIO_TRACEPOINT (SIL_TRACE_INFO, "  MPIO Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
  RequestArgument[0], RequestArgument[1], RequestArgument[2], RequestArgument[3],
  RequestArgument[4], RequestArgument[5]);


 /*
  * 1 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
  */
 do {
   RxMpioMessageResponse = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_RESPONSE_ADDRESS);
   RxMpioMessageResponse = (RxMpioMessageResponse & BIT_32(31));
 } while (RxMpioMessageResponse == 0x0);

 /*
  * 2 Write message arguments (Write cmd params to mmMPIO_C2PMSG_31-36)
  */
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_0_ADDRESS, RequestArgument[0]);
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_1_ADDRESS, RequestArgument[1]);
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_2_ADDRESS, RequestArgument[2]);
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_3_ADDRESS, RequestArgument[3]);
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_4_ADDRESS, RequestArgument[4]);
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_5_ADDRESS, RequestArgument[5]);

  /*
   * 3 Write message ID (Write cmd ID to mmMPIO_C2PMSG_30 with Ready_flag cleare)
   */
  RequestId = (RequestId & (0xFF)) << 8;
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_RESPONSE_ADDRESS, RequestId);

  /*
   * 4 Write value to mmMPIO_C2PMSG_21 doorbell for MPIO to begin work
   */
  xUSLSmnWrite (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_DOORBELL_ADDRESS, DoorBellValue);

  /*
   * 5 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
   */
  do {
    RxMpioMessageResponse = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
      MPIO_C2PMSG_RESPONSE_ADDRESS);
  } while ((RxMpioMessageResponse & BIT_32(31)) == 0x0);

  /*
   * 6 Read updated MPIO message arguments
   */
  RequestArgument[0] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_0_ADDRESS);
  RequestArgument[1] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_1_ADDRESS);
  RequestArgument[2] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_2_ADDRESS);
  RequestArgument[3] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_3_ADDRESS);
  RequestArgument[4] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_4_ADDRESS);
  RequestArgument[5] = xUSLSmnRead (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus,
    MPIO_C2PMSG_ARGUMENT_5_ADDRESS);

  /*
   * If the status from MPIO is not okay
   */
  if ( BIOSMPIO_Result_OK != (RxMpioMessageResponse & MPIO_C2PMSG_RESPONSE_STATUS_MASK))
  {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, "  MPIO ERROR! Error ID: 0x%x\n", RequestArgument[0]);
  }

  /*
   * 7. Read the response mmMPIO_C2PMSG_30 [15:0] and return function
   */
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "MpioServiceRequest Exit\n");
  return (RxMpioMessageResponse);
}
