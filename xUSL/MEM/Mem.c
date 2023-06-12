/**
 * @file  Mem.c
 * @brief This file contains the Mem IPclass init code.
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @ingroup xUSL_group
 * @defgroup Mem_group Memory Class IP block
 * The Memory code provides information about the DIMMs installed to the host.
 *
 * The 'class' element is the top most part of the silicon block and handles
 * the general controls.
 *
 * There are 2 primary entry points:
 * - @ref MemSetInputBlk
 * - @ref InitializeMemBlk
 */

#include "Mem.h"

#define MEM_CLASS_MAJOR_REV   0
#define MEM_CLASS_MINOR_REV   1
#define MEM_CLASS_INSTANCE    0



/**
 * Establish MEM input block defaults
 *
 * This function is called by the openSIL core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
MemSetInputBlk (void)
{
  MEM_CLASS_INPUT_BLK *MemInput;
  uint8_t                          Socket;
  uint8_t                          Channel;
  uint8_t                          Dimm;

  MemInput = (MEM_CLASS_INPUT_BLK *)SilCreateInfoBlock (SilId_MemClass,
                                 sizeof(MEM_CLASS_INPUT_BLK),
                                 MEM_CLASS_INSTANCE,
                                 MEM_CLASS_MAJOR_REV,
                                 MEM_CLASS_MINOR_REV);
  MEM_TRACEPOINT(SIL_TRACE_INFO, "openSIL MemSetInputBlk at: 0x%x \n", MemInput);
  if (MemInput == NULL) {
    return SilAborted;
  }

  // Init Maximum Values
  MemInput->MaxSocketsSupported = ABL_APOB_MAX_SOCKETS_SUPPORTED;
  MemInput->MaxDimmsPerChannel = ABL_APOB_MAX_CHANNELS_PER_SOCKET;
  MemInput->MaxChannelsPerSocket = ABL_APOB_MAX_DIMMS_PER_CHANNEL;

  // Init Dimm/Channel Data
  for (Socket = 0; Socket < ABL_APOB_MAX_SOCKETS_SUPPORTED; Socket++) {
    for (Channel = 0; Channel < ABL_APOB_MAX_CHANNELS_PER_SOCKET; Channel++) {
      for (Dimm = 0; Dimm < ABL_APOB_MAX_DIMMS_PER_CHANNEL; Dimm++) {
        MemInput->DimmInfo[Dimm].SocketNumber = Socket;
        MemInput->DimmInfo[Dimm].ChannelNumber = Channel;
        MemInput->DimmInfo[Dimm].DimmNumber = Dimm;
        MemInput->DimmInfo[Dimm].DimmPresent = false;
        MemInput->DimmInfo[Dimm].DeviceWidth = 0;
        MemInput->DimmInfo[Dimm].Size = 0;
        MemInput->DimmInfo[Dimm].ClockSpeed = 0;

      }
    }
  }

  // fill MEM structure with defaults
  return SilPass;
}


/**
 * Initialize the Mem IP Block
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMemBlk (void)
{
  MEM_CLASS_INPUT_BLK *MemInpBlk;    // pointer to input blk

  SIL_STATUS Status = SilPass;
  MEM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  MemInpBlk = (MEM_CLASS_INPUT_BLK *)SilFindStructure( SilId_MemClass,  0); //Instance 0
  MEM_TRACEPOINT (SIL_TRACE_INFO, "openSIL MEM found blk at: 0x%x \n", MemInpBlk);
  if (MemInpBlk == NULL) {
    // Coult not find the IP input block
    return SilNotFound;
  }

  MEM_TRACEPOINT(SIL_TRACE_EXIT, "Status = [%x]\n", Status);
  return Status;
}

