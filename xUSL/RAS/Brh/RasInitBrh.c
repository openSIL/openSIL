/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RasInitBrh.c
 * @brief RAS silicon initialization code for BRH.
 *
 */

#include <xSIM.h>
#include <MsrReg.h>
#include <stdint.h>
#include <SilCommon.h>
#include <RAS/RasDefs.h>
#include <RAS/Common/Ras.h>
#include <RAS/Common/RasReg.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfIp2Ip.h>
#include "RasInitBrh.h"

/**
 * CollectDimmMapBrh
 *
 * @brief   Not supported in openSIL for BRH.
 *
 * @param   DimmMap     Input buffer for the dimm map.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilUnsupported
 */
SIL_STATUS
CollectDimmMapBrh (
  SIL_ADDR_DATA *DimmMap
  )
{
  UNUSED(DimmMap);
  // This function is not supported for BRH
  return SilUnsupported;
}

/**
 * SetDimmPresenceBrh
 *
 * @brief   Populate the openSIL RAS data block buffer with dimm presence information.
 *
 * @return  SIL_STATUS
 *
 */
SIL_STATUS
SetDimmPresenceBrh (
  RAS_DATA_BLOCK_BRH  *RasDataBlock
  )
{
  SIL_STATUS    Status;
  uint32_t      NumberOfInstalledProcessors;
  uint32_t      RootBridgeOnDie;
  uint32_t      TotalNumberOfDie;
  uint32_t      BusNumberBase;
  uint32_t      SocketId;
  uint32_t      DieId;
  uint32_t      ChannelId;
  DF_IP2IP_API  *DfApi;
  uint8_t       UmcPerDie;
  uint32_t      Data1;
  uint32_t      Data2;

  XPRF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  UmcPerDie = GetUmcPerDieBrh ();

  NumberOfInstalledProcessors = 0;
  TotalNumberOfDie = 0;
  RootBridgeOnDie = 0;
  BusNumberBase = 0;

  DfApi->DfGetSystemInfo(&NumberOfInstalledProcessors, NULL, NULL, NULL, NULL);

  for (SocketId = 0; SocketId < NumberOfInstalledProcessors; SocketId++) {
    DfApi->DfGetProcessorInfo((uint32_t)SocketId, &TotalNumberOfDie, NULL);
    for (DieId = 0; DieId < TotalNumberOfDie; DieId++) {
      DfApi->DfGetRootBridgeInfo(SocketId,
        DieId,
        RootBridgeOnDie,
        NULL,
        &BusNumberBase,
        NULL,
        NULL,
        NULL,
        NULL
        );
      for (ChannelId = 0; ChannelId < UmcPerDie; ChannelId++) {
        // Set Dimm.Present for each Socket/Die/Channel
        Data1 = xUSLSmnRead(0, BusNumberBase, UMC_SMN_INDEX(ChannelId, 0x90));
        Data2 = xUSLSmnRead(0, BusNumberBase, UMC_SMN_INDEX(ChannelId, 0x94));
        RasDataBlock->Dimm.Present[SocketId][(DieId * UmcPerDie) + ChannelId] =
          ((Data1 & BIT_32(4)) || (Data2 & BIT_32(4))) ? true : false;

        RAS_TRACEPOINT(SIL_TRACE_INFO,
          "S0x%x:D0x%x:CH0x%x:B0x%x DimmPresent = %d\n",
          SocketId,
          DieId,
          ChannelId,
          BusNumberBase,
          RasDataBlock->Dimm.Present[SocketId][(DieId * UmcPerDie) + ChannelId]
          );
      }
    }
  }

  return SilPass;
}

/**
 * GetCoreMcaSmnAddrByteBrh
 *
 * @brief Get the address byte for the core mca bank of the given type
 *
 * @param   McaType   The MCA Bank Type
 * @return uint8_t    The corresponding SMN address byte
 */
uint8_t
GetCoreMcaSmnAddrByteBrh (
  uint16_t    McaType
  )
{
  uint8_t CoreMcaSmnAddrByte1;

  switch (McaType) {
  case LS_MCA_TYPE_BRH:
    CoreMcaSmnAddrByte1 = LS_THR0_SMNADDR_BYTE1_BRH;
    break;
  case IF_MCA_TYPE:
    CoreMcaSmnAddrByte1 = IF_THR0_SMNADDR_BYTE1;
    break;
  case L2_MCA_TYPE:
    CoreMcaSmnAddrByte1 = L2_THR0_SMNADDR_BYTE1;
    break;
  case DE_MCA_TYPE:
    CoreMcaSmnAddrByte1 = DE_THR0_SMNADDR_BYTE1;
    break;
  case EX_MCA_TYPE:
    CoreMcaSmnAddrByte1 = EX_THR0_SMNADDR_BYTE1;
    break;
  case FP_MCA_TYPE:
    CoreMcaSmnAddrByte1 = FP_THR0_SMNADDR_BYTE1;
    break;
  default: //Should not be here
    CoreMcaSmnAddrByte1 = 0;
    break;
  }
  return CoreMcaSmnAddrByte1;
}
