/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFabricAcpi.c
 * @brief Platform Reference Firmware - fabric locality services.
 *
 * @details Answers the fabric side of SRAT, CRAT, SLIT, CDIT and MSCT on
 *          demand, using fabric state at the moment the host builds each
 *          table.
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <xPRF-api.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include <xPrfFabricAcpi.h>

/**
 * xPrfFabricAcpiGetDomainCounts
 *
 * @brief   Retrieve the NUMA domain counts the ACPI tables are sized from.
 *
 * @param   ReportedDomainCount  Populated with the OS visible domain count
 * @param   PhysicalDomainCount  Populated with the physical domain count
 * @param   PhysNodesPerSocket   Populated with the ABL determined NPS
 * @param   SystemCxlCount       Populated with the trailing CXL domain count
 * @param   MaxDomains           Populated with the upper bound for the MSCT
 * @param   CcxAsNuma            Populated true when each complex is a domain
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetDomainCounts (
  uint32_t  *ReportedDomainCount,
  uint32_t  *PhysicalDomainCount,
  uint32_t  *PhysNodesPerSocket,
  uint32_t  *SystemCxlCount,
  uint32_t  *MaxDomains,
  bool      *CcxAsNuma
  )
{
  return DfXAcpiGetDomainCounts (
           ReportedDomainCount,
           PhysicalDomainCount,
           PhysNodesPerSocket,
           SystemCxlCount,
           MaxDomains,
           CcxAsNuma
           );
}

/**
 * xPrfFabricAcpiDomainXlat
 *
 * @brief   Translate a core's physical location to its NUMA domain.
 *
 * @param   Socket  Zero based socket the core is attached to
 * @param   Die     DF die on that socket
 * @param   Ccd     Logical CCD the core is on
 * @param   Ccx     Logical core complex
 * @param   Domain  Populated with the domain the core belongs to
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiDomainXlat (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Ccd,
  uint32_t  Ccx,
  uint32_t  *Domain
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  return DfApi->DfDomainXlat (Socket, Die, Ccd, Ccx, Domain);
}

/**
 * xPrfFabricAcpiGetReportedDomains
 *
 * @brief   Retrieve the reported NUMA domain table.
 *
 * @param   MaxDomains  Capacity of Buffer, in domains
 * @param   Count       Populated with the domains written
 * @param   Buffer      Receives type, socket map and physical domain per entry
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetReportedDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  return DfXAcpiGetReportedDomains (MaxDomains, Count, Buffer);
}

/**
 * xPrfFabricAcpiGetPhysicalDomains
 *
 * @brief   Retrieve the physical NUMA domain table.
 *
 * @param   MaxDomains  Capacity of Buffer, in domains
 * @param   Count       Populated with the domains written
 * @param   Buffer      Receives CS map, sharing count, sharing map, extend info
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetPhysicalDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  return DfXAcpiGetPhysicalDomains (MaxDomains, Count, Buffer);
}

/**
 * xPrfFabricAcpiGetMemoryEntries
 *
 * @brief   Produce the SRAT and CRAT memory affinity ranges.
 *
 * @param   MaxEntries      Capacity of both buffers, in entries
 * @param   MemEntryCount   Populated with the ranges written
 * @param   MemEntryBuffer  Receives domain, base and size per range
 * @param   MemInfoCount    Populated with the per-domain totals written
 * @param   MemInfoBuffer   Receives domain and size per total
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetMemoryEntries (
  uint32_t  MaxEntries,
  uint32_t  *MemEntryCount,
  uint32_t  *MemEntryBuffer,
  uint32_t  *MemInfoCount,
  uint32_t  *MemInfoBuffer
  )
{
  return DfXAcpiCollectMemoryEntries (
           MaxEntries,
           MemEntryCount,
           MemEntryBuffer,
           MemInfoCount,
           MemInfoBuffer
           );
}

/**
 * xPrfFabricAcpiGetDistanceInfo
 *
 * @brief   Produce the SLIT and CDIT distance matrix.
 *
 * @param   DomainCount  Matrix order
 * @param   BufferSize   Capacity of Distance, in bytes
 * @param   Distance     Receives the matrix, row major
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetDistanceInfo (
  uint32_t  DomainCount,
  uint32_t  BufferSize,
  uint8_t   *Distance
  )
{
  return DfXAcpiGetDistanceInfo (DomainCount, BufferSize, Distance);
}

/**
 * xPrfFabricAcpiGetPxmDomains
 *
 * @brief   Retrieve the proximity domains behind one host bridge.
 *
 * @param   BusBase   Segment adjusted base bus number of the host bridge
 * @param   MaxCount  Capacity of Domains
 * @param   Count     Populated with the domains written, zero when not fabric owned
 * @param   Domains   Receives the proximity domain numbers
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfFabricAcpiGetPxmDomains (
  uint32_t  BusBase,
  uint32_t  MaxCount,
  uint32_t  *Count,
  uint32_t  *Domains
  )
{
  return DfXAcpiGetPxmDomains (BusBase, MaxCount, Count, Domains);
}

/**
 * xPrfFabricRegisterAccRead
 *
 * @brief   Read a data fabric register.
 *
 * @param   Socket    Socket to address
 * @param   Die       Die on that socket
 * @param   Function  Fabric function number
 * @param   Offset    Register offset
 * @param   Instance  Fabric instance ID, or the broadcast value
 *
 * @return  uint32_t  Register contents, zero when the DF API is unavailable
 */
uint32_t
xPrfFabricRegisterAccRead (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Function,
  uint32_t  Offset,
  uint32_t  Instance
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return 0;
  }

  return DfApi->DfFabricRegisterAccRead (Socket, Die, Function, Offset, Instance);
}

/**
 * xPrfFabricRegisterAccWrite
 *
 * @brief   Write a data fabric register.
 *
 * @details These platforms do not support S3, so no save list is maintained
 *          and the write goes directly to the register.
 *
 * @param   Socket    Socket to address
 * @param   Die       Die on that socket
 * @param   Function  Fabric function number
 * @param   Offset    Register offset
 * @param   Instance  Fabric instance ID, or the broadcast value
 * @param   Value     Value to write
 */
void
xPrfFabricRegisterAccWrite (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Function,
  uint32_t  Offset,
  uint32_t  Instance,
  uint32_t  Value
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfApi;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return;
  }

  DfApi->DfFabricRegisterAccWrite (Socket, Die, Function, Offset, Instance, Value);
}
