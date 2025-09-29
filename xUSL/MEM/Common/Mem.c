/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  Mem.c
 * @brief This file contains the Mem IPclass init code.
 */

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
#include <SilCommon.h>
#include "MemCmn2Rev.h"
#include "MemTableTypes.h"
#include "Mem.h"

/**
 * MemSetInputBlk
 *
 * @brief Establish MEM input block defaults
 *
 * @details This function is called by the SIL core to populate the IP's input
 *          block with its default values. Each block established by the IP must
 *          have a unique header.ID and its header.size set to the single block
 *          only - so as to allow the FindStruct() to traverse the block chain.
 *          This is an IP private function, not visible to the Host.
 *
 * @return  SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
MemSetInputBlk (void)
{
  // Fill MEM structure with defaults
  return SilPass;
}

/**
 * IntToString
 *
 * @brief  Translate uint8_t array to char array.
 *
 * @param   *String       Pointer to char array
 * @param   *Integer      Pointer to uint8_t array
 * @param   SizeInByte    The size of uint8_t array
 *
 * @return  void
 */
void
IntToString (
  char       *String,
  uint8_t    *Integer,
  uint8_t    SizeInByte
  )
{
  uint8_t Index;

  for (Index = 0; Index < SizeInByte; Index++) {
    *(String + Index * 2) = (*(Integer + Index) >> 4) & 0x0F;
    *(String + Index * 2 + 1) = *(Integer + Index) & 0x0F;
  }
  for (Index = 0; Index < (SizeInByte * 2); Index++) {
    if (*(String + Index) >= 0x0A) {
      *(String + Index) += 0x37;
    } else {
      *(String + Index) += 0x30;
    }
  }
  *(String + SizeInByte * 2) = 0x0;
}

/**
 * SilInitSmbios32Type17
 *
 * @brief  Initialize the SMBIOS 3.2 fields of Type 17
 *
 * @param   *DimmPresent  - DIMM Present
 * @param   *SpdData      - Pointer to the SPD data array
 * @param   *T17          - Pointer to TYPE17_DMI_INFO
 *
 * @return  void
 */
void
SilInitSmbios32Type17 (
  SIL_CONTEXT            *SilContext,
  bool                   DimmPresent,
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  )
{
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  SIL_STATUS                        Status;

  assert(SpdData != NULL);
  assert(T17 != NULL);

  Status = SilGetCommon2RevXferTable(SilContext, SilId_MemClass, (void **)(&MemXferTable));
  if (Status != SilPass) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to Get C2R Table for MEM\n");
  } else {
    if (DimmPresent) {
      MemXferTable->ConfigureTable17DimmPresent(SpdData, T17);
    } else {
      T17->MemoryTechnology = SilUnknownType;
      T17->MemoryOperatingModeCapability.AsBitmap.Unknown = 1;
      T17->FirmwareVersion[0] = '\0';
      T17->ModuleManufacturerId = 0;
      T17->ModuleProductId = 0;
      T17->MemorySubsystemControllerManufacturerId = 0;
      T17->MemorySubsystemControllerProductId = 0;
      T17->NonvolatileSize = 0;
      T17->VolatileSize = 0;
      T17->CacheSize = 0;
      T17->LogicalSize = 0;
    }
  }
}

/**
 * SilInitSmbios33Type17
 *
 * @brief Initialize the SMBIOS 3.3 fields of Type 17
 *
 * @param *DimmPresent       DIMM Present
 * @param *SpdData           Pointer to the SPD data array
 * @param *T17               Pointer to TYPE17_DMI_INFO
 *
 * @return nothing
 */
void
SilInitSmbios33Type17 (
  bool                       DimmPresent,
  uint8_t                    *SpdData,
  SIL_TYPE17_DMI_INFO        *T17
  )
{
  if ((SpdData == NULL) || (T17 == NULL)) {
    assert(false);
    return;
  }

  //
  // For compatibility with older SMBIOS parsers, memory devices slower than
  // 65,535 MT/s should represent their speed using the Speed and Configured Memory Speed fields,
  // leaving the Extended Speed and Extended Configured Memory Speed fields set to 0.
  //
  T17->ExtendedSpeed = 0;
  T17->ExtendedConfiguredMemorySpeed = 0;
}

/**
 *  TranslateChannelInfo
 *
 *  @brief Translate the channel Id depending upon the channel translation table.
 *
 *  @param RequestedChannelId - The requested channel Id
 *  @param *TranslatedChannelId - Pointer to the translated Id
 *
 *  @return   SIL_STATUS
 */
SIL_STATUS
TranslateChannelInfo (
  SIL_CONTEXT                *SilContext,
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  HOST_TO_APCB_CHANNEL_XLAT         *XlatTable;
  Status = SilGetCommon2RevXferTable(SilContext, SilId_MemClass, (void **)(&MemXferTable));
  if (Status != SilPass) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to Get C2R Table for MEM\n");
  } else {
    XlatTable = MemXferTable->GetChannelXlatTable ();
    *TranslatedChannelId = RequestedChannelId;
    if (XlatTable != NULL) {
      while (XlatTable->RequestedChannelId != 0xFF) {
        if (RequestedChannelId == XlatTable->RequestedChannelId) {
          *TranslatedChannelId = XlatTable->TranslatedChannelId;
          break;
        }
        XlatTable++;
      }
    }
    Status = SilPass;
  }
  return Status;
}

/**
 * GetMemInfo
 *
 * @brief    populate AMD_MEMORY_SUMMARY
 *
 * @details  This function is a wrapper for populating program specific AMD_MEMORY_SUMMARY struct
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   *MemSummaryTable    - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
GetMemInfo (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummaryTable
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  if (MemSummaryTable == NULL) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "Memory Table is an invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = SilGetCommon2RevXferTable(SilContext, SilId_MemClass, (void **)(&MemXferTable));
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for MEM\n");
    } else {
      Status = MemXferTable->ConfigureMemInfo(SilContext, MemSummaryTable);
    }
  }

  return Status;
}

/**
 * GetSmbiosTable
 *
 * @brief    populate SIL_DMI_INFO
 *
 * @details  This function is a wrapper for populating the SMBIOS table
 *
 * @param    SilContext       A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible for initializing
 *                            the SIL_CONTEXT structure.
 * @param    *DmiInfoTable  - Pointer to SIL_DMI_INFO
 * @param    DramType -       DRAM Type
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
GetSmbiosTable (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  if (DmiInfoTable == NULL) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "DMI INFO table is an invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = SilGetCommon2RevXferTable(SilContext, SilId_MemClass, (void **)(&MemXferTable));
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for MEM\n");
    } else {
      Status = MemXferTable->PopulateSmbiosMemInfo(SilContext, DmiInfoTable, MemType);
    }
  }

  return Status;
}
