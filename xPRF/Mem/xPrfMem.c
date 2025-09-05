/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfMem.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        Mem
 */

#include <SilCommon.h>
#include <xPRF-api.h>
#include <MEM/MemIp2Ip.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>

// 1 Gigabyte
#define GIGB (1 << 30)

/**
 * xPrfGetSystemMemoryMap
 *
 * @brief  Get top of memory (Tom2) for the UEFI Host along with
 *         memory map, and number of holes.
 *
 * @param   NumberOfHoles      Number of memory holes
 * @param   TopOfSystemMemory  Top of memory address
 * @param   MemHoleDescPtr     Memory descriptor structure
 *
 * @retval SilPass                Info extracted successfully.
 * @retval SilInvalidParameter    Failure.
 **/
SIL_STATUS
xPrfGetSystemMemoryMap (
  uint32_t                             *NumberOfHoles,
  uint64_t                             *TopOfSystemMemory,
  void                                 **MemHoleDescPtr
  )
{
  SIL_STATUS                           Status;
  uint32_t                             NumOfMemHoleMap;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT   *ApobEntry;
  APOB_IP2IP_API                       *ApobIp2IpApi;

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_FABRIC,
    APOB_SYS_MAP_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );
  if (Status != SilPass) {
    return Status;
  }

  //
  // Get the number of holes
  //
  NumOfMemHoleMap = ApobEntry->ApobSystemMap.NumberOfHoles;

  //
  // Get Memory Descriptor and TOM2
  //
  if (NumOfMemHoleMap != 0) {
    *MemHoleDescPtr = (void *)&ApobEntry->ApobSystemMap.HoleInfo;
    *TopOfSystemMemory = ApobEntry->ApobSystemMap.TopOfSystemMemory;
    *NumberOfHoles = NumOfMemHoleMap;
  }

  return SilPass;
}

/**
 *  xPrfTranslateChannelInfo
 *
 *  @brief   Translates given channel ID with channel translation table
 *
 *  @details This function is a wrapper to TranslateChannelInfo in xUSL and
 *           it calls TranslateChannelInfo through the I2I API.
 *           TranslateChannelInfo will then translate the channel ID with
 *           the translate table.
 *
 *  @param   RequestedChannelId - The requested channel Id
 *  @param   *TranslatedChannelId - Pointer to the translated Id
 *
 *  @return  SIL_STATUS
 */
SIL_STATUS
xPrfTranslateChannelInfo (
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    Status = SilNotFound;
  } else {
    Status = MemIp2Ip->TranslateChannelInfo(RequestedChannelId, TranslatedChannelId);
  }

  return Status;
}

/**
 * xPrfGetLowUsableDramAddress
 *
 * @brief   Get top of low(<4Gb) usable DRAM accounting for pre-assigned
 *          buffers for system use.
 *          This is used by the host firmware to locate low usable
 *          DRAM top. The ApobBaseAddress is optional if set in kconfig
 *          properly.
 *
 * @param[in] ApobBaseAddress     location of the APOB
 *
 * @retval    uint32_t   Top address of available DRAM region below 4GB
 * @retval    0          Failure.
 **/
uint32_t
xPrfGetLowUsableDramAddress (
  uint32_t                            ApobBaseAddress
  )
{
  SIL_STATUS                          Status;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT  *ApobEntry;
  MEMORY_HOLE_DESCRIPTOR              *HoleMapPtr;
  uint8_t                             MemRangeIndex;
  uint64_t                            TopofLowUsableDRAM;

  //
  // The Caller can pass the APOB base address as a parameter but
  // This will not set the address beyond this Time Point. If the APOB address
  // has been initialized in this time point then this argument will not be used
  //
  Status = AmdGetApobEntryInstance(APOB_FABRIC,
    APOB_SYS_MAP_INFO_TYPE,
    0,
    ApobBaseAddress,
    (APOB_TYPE_HEADER **) &ApobEntry
    );
  /* Error case , needs to be handled by caller. */
  if (Status != SilPass || ApobEntry->ApobSystemMap.NumberOfHoles == 0) {
    return 0;
  }

  /* Start with Max out assignment.*/
  TopofLowUsableDRAM = ApobEntry->ApobSystemMap.TopOfSystemMemory;

  /*
   * scan through all Mem ranges to find the base address of
   * lowest mem range below 4Gib.And the Base of this region is
   * top of low usable DRAM.  Below this region is the low
   * usable DRAM.
   */
  for (MemRangeIndex = 0; MemRangeIndex < ApobEntry->ApobSystemMap.NumberOfHoles; MemRangeIndex++) {
    HoleMapPtr = &ApobEntry->ApobSystemMap.HoleInfo[MemRangeIndex];
    if (HoleMapPtr->Base < 4ULL * GIGB && HoleMapPtr->Size > 0) {
      if (HoleMapPtr->Base < TopofLowUsableDRAM) {
        TopofLowUsableDRAM = HoleMapPtr->Base;
      }
    }
  }

  // Since the low usable DRAM address is below 4GiB, It is safe to truncate.
  return (uint32_t)TopofLowUsableDRAM;
}

/**
 * xPrfGetSmbiosMemInfo
 *
 * @details    This routine gets DMI Type 16, Type 17,
 *             Type 19 and Type 20 related information.
 *
 * @param      DmiInfoTable - DMI records
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
xPrfGetSmbiosMemInfo (
  SIL_DMI_INFO    *DmiInfoTable
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    assert(Status == SilPass);
  } else if (DmiInfoTable == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "xPrfGetSmbiosMemInfo: Failed invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = MemIp2Ip->GetSmbiosMemInfo(DmiInfoTable);
  }

  return Status;
}

/**
 * xPrfGetMemInfo
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
xPrfGetMemInfo (
  AMD_MEMORY_SUMMARY          *MemSummaryTable
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    assert(Status == SilPass);
  } else if (MemSummaryTable == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "xPrfGetMemInfo: Failed invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = MemIp2Ip->GetMemInfo(MemSummaryTable);
  }

  return Status;
}
