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
 *
 * This is an IP private function, not visible to the Host.
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
 * GetDimmPresence
 *
 * @details  a helper function used to check the presence of the dimm
 *
 * @param    *MemSummary   Pointer to AMD_MEMORY_SUMMARY
 * @param    Socket        System Socket
 * @param    Die           System Die
 * @param    Channel       System Channel
 * @param    Dimm          System Dimm
 *
 */
static
uint8_t
GetDimmPresence (
  AMD_MEMORY_SUMMARY         *MemSummary,
  uint8_t                    Socket,
  uint16_t                   Die,
  uint8_t                    Channel,
  uint8_t                    Dimm
  )
{
  uint8_t DimmPresence;
  DimmPresence = 0;

  if (MemSummary != NULL) {
    DimmPresence = (uint8_t)((MemSummary->DimmPresentMap [Socket * (MemSummary->MaxDiePerSocket) + Die]
      >> (Channel * (MemSummary->MaxDimmPerChannel) + Dimm)) & 1);
  }

  return DimmPresence;
}

/**
 * DumpMemoryInfoData
 *
 * @brief    This is a function prints the AMD_MEMORY_SUMMARY Struct
 *
 * @param    *MemSummary   - Pointer to AMD_MEMORY_SUMMARY
 *
 */
void
DumpMemoryInfoData (
  AMD_MEMORY_SUMMARY   *MemSummary
  )
{
  uint8_t     Socket;
  uint8_t     Channel;
  uint8_t     Dimm;
  uint16_t    DieLoop;
  uint16_t    Index;

  DieLoop = 0;

  MEM_TRACEPOINT(SIL_TRACE_INFO, "Dump Memory Config. Info.\n");
  // Print MbistTestEnable
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--MbistTestEnable.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
    MemSummary->MbistTestEnable.Status.Enabled,
    MemSummary->MbistTestEnable.StatusCode
    );
  // Print MbistAggressorEnable
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--MbistAggressorEnable.Status.Enabled        = %d      (StatusCode = 0x%04x)\n",
    MemSummary->MbistAggressorEnable.Status.Enabled,
    MemSummary->MbistAggressorEnable.StatusCode
    );
  // Print MbistPerBitSlaveDieReport
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--MbistPerBitSlaveDieReport.Status.Value     = 0x%04x (StatusCode = 0x%04x)\n",
    MemSummary->MbistPerBitSlaveDieReport.Status.Value,
    MemSummary->MbistPerBitSlaveDieReport.StatusCode
    );
  // Print DramTempControlledRefreshEn
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--DramTempControlledRefreshEn.Status.Enabled = %d      (StatusCode = 0x%04x)\n",
    MemSummary->DramTempControlledRefreshEn.Status.Enabled,
    MemSummary->DramTempControlledRefreshEn.StatusCode
    );
  // Print UserTimingMode
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--UserTimingMode.Status.Enabled              = %d      (StatusCode = 0x%04x)\n",
    MemSummary->UserTimingMode.Status.Enabled,
    MemSummary->UserTimingMode.StatusCode
    );
  // Print UserTimingValue
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--UserTimingValue.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
    MemSummary->UserTimingValue.Status.Enabled,
    MemSummary->UserTimingValue.StatusCode
    );
  // Print MemBusFreqLimit
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--MemBusFreqLimit.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
    MemSummary->MemBusFreqLimit.Status.Enabled,
    MemSummary->MemBusFreqLimit.StatusCode
    );
  // Print EnablePowerDown
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--EnablePowerDown.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
    MemSummary->EnablePowerDown.Status.Enabled,
    MemSummary->EnablePowerDown.StatusCode
    );
  // Print DramDoubleRefreshRate
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--DramDoubleRefreshRate.Status.Enabled       = %d      (StatusCode = 0x%04x)\n",
    MemSummary->DramDoubleRefreshRate.Status.Enabled,
    MemSummary->DramDoubleRefreshRate.StatusCode
    );
  // Print PmuTrainMode
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--PmuTrainMode.Status.Value                  = 0x%04x (StatusCode = 0x%04x)\n",
    MemSummary->PmuTrainMode.Status.Value,
    MemSummary->PmuTrainMode.StatusCode
    );
  // Print EccSymbolSize
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--EccSymbolSize.Status.Value                 = 0x%04x (StatusCode = 0x%04x)\n",
    MemSummary->EccSymbolSize.Status.Value,
    MemSummary->EccSymbolSize.StatusCode
    );
  // Print UEccRetry
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--UEccRetry.Status.Enabled                   = %d      (StatusCode = 0x%04x)\n",
    MemSummary->UEccRetry.Status.Enabled,
    MemSummary->UEccRetry.StatusCode
    );
  // Print IgnoreSpdChecksum
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--IgnoreSpdChecksum.Status.Enabled           = %d      (StatusCode = 0x%04x)\n",
    MemSummary->IgnoreSpdChecksum.Status.Enabled,
    MemSummary->IgnoreSpdChecksum.StatusCode
    );
  // Print EnableBankGroupSwapAlt
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--EnableBankGroupSwapAlt.Status.Enabled      = %d      (StatusCode = 0x%04x)\n",
    MemSummary->EnableBankGroupSwapAlt.Status.Enabled,
    MemSummary->EnableBankGroupSwapAlt.StatusCode
    );
  // Print EnableBankGroupSwap
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--EnableBankGroupSwap.Status.Enabled         = %d      (StatusCode = 0x%04x)\n",
    MemSummary->EnableBankGroupSwap.Status.Enabled,
    MemSummary->EnableBankGroupSwap.StatusCode
    );
  // Print DdrRouteBalancedTee
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--DdrRouteBalancedTee.Status.Enabled         = %d      (StatusCode = 0x%04x)\n",
    MemSummary->DdrRouteBalancedTee.Status.Enabled,
    MemSummary->DdrRouteBalancedTee.StatusCode
    );
  // Print NvdimmPowerSource
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--NvdimmPowerSource.Status.Value             = 0x%04x (StatusCode = 0x%04x)\n",
    MemSummary->NvdimmPowerSource.Status.Value,
    MemSummary->NvdimmPowerSource.StatusCode
    );
  // Print OdtsCmdThrotEn
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--OdtsCmdThrotEn.Status.Enabled              = %d      (StatusCode = 0x%04x)\n",
    MemSummary->OdtsCmdThrotEn.Status.Enabled,
    MemSummary->OdtsCmdThrotEn.StatusCode
    );
  // Print OdtsCmdThrotCyc
  MEM_TRACEPOINT(SIL_TRACE_INFO,
    "--OdtsCmdThrotCyc.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
    MemSummary->OdtsCmdThrotCyc.Status.Enabled,
    MemSummary->OdtsCmdThrotCyc.StatusCode
    );
  MEM_TRACEPOINT(SIL_TRACE_INFO, "\n");

  for (Socket = 0; Socket < MemSummary->MaxSocketSupported; Socket++) {
    MEM_TRACEPOINT(SIL_TRACE_INFO, "[Socket %d]\n", Socket);
    for (DieLoop = 0; DieLoop < MemSummary->MaxDiePerSocket; DieLoop++) {
      MEM_TRACEPOINT(SIL_TRACE_INFO, "     [Die %d]\n", DieLoop);
      Index = Socket * MemSummary->MaxDiePerSocket + DieLoop;
      // Print DramEcc
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "     --DramEcc.Status.Enabled = %d (StatusCode = 0x%04x)\n",
        MemSummary->DramEcc[Index].Status.Enabled,
        MemSummary->DramEcc[Index].StatusCode
        );
      // Print DramParity
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "     --DramParity.Status.Enabled = %d (StatusCode = 0x%04x)\n",
        MemSummary->DramParity[Index].Status.Enabled,
        MemSummary->DramParity[Index].StatusCode
        );
      // Print AutoRefFineGranMode
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "     --AutoRefFineGranMode.Status.Value = 0x%04x (StatusCode = 0x%04x)\n",
        MemSummary->AutoRefFineGranMode[Index].Status.Value,
        MemSummary->AutoRefFineGranMode[Index].StatusCode
        );

      for (Channel = 0; Channel < MemSummary->MaxChannelPerDie; Channel++) {
        MEM_TRACEPOINT(SIL_TRACE_INFO, "          [Channel %d]\n", Channel);
        Index = Socket * MemSummary->MaxDiePerSocket * MemSummary->MaxChannelPerDie +
          DieLoop * MemSummary->MaxChannelPerDie + Channel;
        // Print ChipselIntlv
        MEM_TRACEPOINT(SIL_TRACE_INFO,
          "          --ChipselIntlv.Status.Enabled = %d (StatusCode = 0x%04x)\n",
          MemSummary->ChipselIntlv[Index].Status.Enabled,
          MemSummary->ChipselIntlv[Index].StatusCode
          );

        for (Dimm = 0; Dimm < MemSummary->MaxDimmPerChannel; Dimm++) {
          MEM_TRACEPOINT(SIL_TRACE_INFO, "               [Dimm %d]\n", Dimm);
          // Print DimmPresentMap
          MEM_TRACEPOINT(SIL_TRACE_INFO,
            "               --DimmPresence: %s\n",
            (GetDimmPresence
              (MemSummary, Socket, DieLoop, Channel, Dimm) != 0) ? L"Present" : L"Absent"
            );
        }
      }
    }
  }
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
  bool                   DimmPresent,
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  )
{
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  SIL_STATUS                        Status;

  assert(SpdData != NULL);
  assert(T17 != NULL);

  Status = SilGetCommon2RevXferTable(SilId_MemClass, (void **)(&MemXferTable));
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
    Status = SilPass;
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
  assert(SpdData != NULL);
  assert(T17 != NULL);

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
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  HOST_TO_APCB_CHANNEL_XLAT         *XlatTable;

  Status = SilGetCommon2RevXferTable(SilId_MemClass, (void **)(&MemXferTable));
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
 * @details  This function populate AMD_MEMORY_SUMMARY structure which is used to report DRAM info to the Host.
 *           The following fields of AMD_MEMORY_SUMMARY are the inputs and must be populated by the caller:
 *              AMD_MEMORY_SUMMARY.MaxSocketSupported, AMD_MEMORY_SUMMARY.MaxDiePerSocket,
 *              AMD_MEMORY_SUMMARY.MaxChannelPerDie, AMD_MEMORY_SUMMARY.MaxDimmPerChannel
 *
 * @param    *MemSummaryTable  - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
GetMemInfo (
  AMD_MEMORY_SUMMARY *MemSummaryTable
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;

  if (MemSummaryTable == NULL) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "Memory Table is an invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = SilGetCommon2RevXferTable(SilId_MemClass, (void **)(&MemXferTable));
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for MEM\n");
    } else {
      Status = MemXferTable->ConfigureMemInfo(MemSummaryTable);
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
 * @param    *DmiInfoTable  - Pointer to SIL_DMI_INFO
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
GetSmbiosTable (
  SIL_DMI_INFO    *DmiInfoTable
  )
{
  SIL_STATUS                        Status;
  MEM_COMMON_2_REV_XFER_BLOCK       *MemXferTable;
  if (DmiInfoTable == NULL) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, "DMI INFO table is an invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = SilGetCommon2RevXferTable(SilId_MemClass, (void **)(&MemXferTable));
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for MEM\n");
    } else {
      Status = MemXferTable->PopulateSmbiosMemInfo(DmiInfoTable);
    }
  }

  return Status;
}
