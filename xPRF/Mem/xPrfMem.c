/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfMem.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        Mem
 */

#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <xPRF-api.h>
#include <MEM/MemIp2Ip.h>
#include <APOB/ApobIp2Ip.h>

// 1 Gigabyte
#define GIGB (1 << 30)

/**
 * xPrfGetSystemMemoryMap
 *
 * @brief  Get top of memory (Tom2) for the Host along with
 *         memory map, and number of holes.
 *
 * @param   SilContext         A context structure through which host firmware defined data
 *                             can be passed to openSIL. The host firmware is responsible for initializing
 *                             the SIL_CONTEXT structure.
 * @param   NumberOfHoles      Number of memory holes
 * @param   TopOfSystemMemory  Top of memory address
 * @param   MemHoleDescPtr     Memory descriptor structure
 *
 * @retval SilPass                Info extracted successfully.
 * @retval SilInvalidParameter    Failure.
 **/
SIL_STATUS
xPrfGetSystemMemoryMap (
  SIL_CONTEXT *SilContext,
  uint32_t    *NumberOfHoles,
  uint64_t    *TopOfSystemMemory,
  void        **MemHoleDescPtrR
  )
{
  SIL_STATUS                           Status;
  uint32_t                             NumOfMemHoleMap;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT   *ApobEntry;
  APOB_IP2IP_API                       *ApobIp2IpApi;

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
    APOB_FABRIC,
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
    *MemHoleDescPtrR = (void *)&ApobEntry->ApobSystemMap.HoleInfo;
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
  SIL_CONTEXT                *SilContext,
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilContext, SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    Status = SilNotFound;
  } else {
    Status = MemIp2Ip->TranslateChannelInfo(SilContext, RequestedChannelId, TranslatedChannelId);
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
 * @note    This xPRF service is a special case. It will call directly into
 *          APOB service common code to support this service, which is required
 *          prior to openSIL environment configuration (I2I).
 *
 *          Special cases require the following characteristics:
 *
 *          1. The service must be required prior to openSIL environment setup
 *             (e.g. host firmware cannot postpone usage until after calling
 *             xSimAssignMemoryTpn).
 *          2. The callee must be common code. Any program specific call requires
 *             I2I API support.
 *
 * @param   SilContext       A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible for initializing
 *                           the SIL_CONTEXT structure.
 *
 * @retval    uint32_t   Top address of available DRAM region below 4GB
 * @retval    0          Failure.
 **/
uint32_t
xPrfGetLowUsableDramAddress (
  SIL_CONTEXT *SilContext
  )
{
  SIL_STATUS                          Status;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT  *ApobEntry;
  MEMORY_HOLE_DESCRIPTOR              *HoleMapPtr;
  uint8_t                             MemRangeIndex;
  uint64_t                            TopofLowUsableDRAM;

  TopofLowUsableDRAM = 0;

  if (SilContext == NULL) {
    return (uint32_t)TopofLowUsableDRAM;
  }

  /*
   * Direct call to openSIL APOB services AmdGetApobEntryInstance.
   * This is a special case for xPRF services that are required by host firmware
   * prior to openSIL environment initialization.
   */
  Status = AmdGetApobEntryInstance(SilContext,
    APOB_FABRIC,
    APOB_SYS_MAP_INFO_TYPE,
    0,
    SilContext->ApobBaseAddress,
    (APOB_TYPE_HEADER **) &ApobEntry
    );
  /* Error case , needs to be handled by caller. */
  if (Status != SilPass || ApobEntry->ApobSystemMap.NumberOfHoles == 0) {
    return (uint32_t)TopofLowUsableDRAM;
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

  XPRF_TRACEPOINT(SIL_TRACE_INFO, "TopofLowUsableDRAM = 0x%x\n", TopofLowUsableDRAM);
  // Since the low usable DRAM address is below 4GiB, It is safe to truncate.
  return (uint32_t)TopofLowUsableDRAM;
}

/**
 * xPrfGetSmbiosMemInfo
 *
 * @details    This routine gets DMI Type 16, Type 17,
 *             Type 19 and Type 20 related information.
 *
 * @param      SilContext   A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible for initializing
 *                          the SIL_CONTEXT structure.
 * @param      DmiInfoTable - DMI records
 * @param      MemType      - DRAM Type
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
xPrfGetSmbiosMemInfo (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilContext, SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    assert(Status == SilPass);
  } else if (DmiInfoTable == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "xPrfGetSmbiosMemInfo: Failed invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = MemIp2Ip->GetSmbiosMemInfo(SilContext, DmiInfoTable, MemType);
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
 * @param    SilContext          A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible for initializing
 *                               the SIL_CONTEXT structure.
 * @param    *MemSummaryTable  - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
xPrfGetMemInfo (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummaryTable
  )
{
  SIL_STATUS                  Status;
  MEM_IP2IP_API               *MemIp2Ip;

  Status = SilGetIp2IpApi(SilContext, SilId_MemClass, (void **)(&MemIp2Ip));
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, " MEM I2I API is not found.\n");
    assert(Status == SilPass);
  } else if (MemSummaryTable == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "xPrfGetMemInfo: Failed invalid input\n");
    Status = SilInvalidParameter;
  } else {
    Status = MemIp2Ip->GetMemInfo(SilContext, MemSummaryTable);
  }

  return Status;
}
