/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXAcpiTables.c
 * @brief Locality data behind SRAT, CRAT, SLIT, CDIT and MSCT
 *
 * @details Provides fabric state through xPRF while leaving table formatting
 *          and installation to host firmware.
 *
 *          Nothing is cached across calls. Output goes into caller supplied
 *          buffers so that openSIL carries none of the storage and no structure
 *          has to keep a matching layout on both sides of the boundary.
 */

#include <SilCommon.h>
#include <string.h>
#include <DF/Df.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfClass-api.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include "DfXAcpiDomainInfo.h"

/// Distance a domain reports to itself, fixed by the ACPI specification
#define DISTANCE_TO_SELF  10

/// Smallest distance that is still meaningful once scaled against DISTANCE_TO_SELF
#define DISTANCE_MINIMUM  11

/// Distance value meaning unreachable
#define DISTANCE_UNREACHABLE  0xFF

/// Defaults applied when the host does not override the SLIT distances
#define DEFAULT_DISTANCE_LOCAL       12
#define DEFAULT_DISTANCE_REMOTE      28
#define DEFAULT_DISTANCE_REMOTE_FAR  32
#define DEFAULT_DISTANCE_VIRTUAL     11
#define DEFAULT_DISTANCE_CXL_LOCAL   18
#define DEFAULT_DISTANCE_CXL_REMOTE  28

/// Memory ranges are tracked in units of 64KB, so a 2MB grain is this many
#define SIZE_2MB_RSH16       (0x200000u >> 16)
#define SIZE_2MB_RSH16_MASK  (SIZE_2MB_RSH16 - 1u)

/**
 * DfXAcpiGetBlocks
 *
 * @brief Resolve the built domain info block and the DF input block together.
 *
 * @param[out] DomainInfo  Receives the domain info block
 * @param[out] DfIpBlock   Receives the DF input block, may be NULL if not wanted
 *
 * @retval SilPass         Both resolved and the domain data is valid
 * @retval SilUnsupported  A block is missing or the domain data was never built
 */
static
SIL_STATUS
DfXAcpiGetBlocks (
  DF_DOMAIN_INFO_BLK  **DomainInfo,
  DFCLASS_INPUT_BLK   **DfIpBlock
  )
{
  DF_DOMAIN_INFO_BLK  *Domains;

  Domains = (DF_DOMAIN_INFO_BLK *) xUslFindStructure (SilId_DfClass, DF_DOMAIN_INSTANCE);
  if (Domains == NULL) {
    DF_TRACEPOINT (SIL_TRACE_ERROR, "Df domain info data blk not found\n");
    return SilUnsupported;
  }

  if (!Domains->DomainInfoValid) {
    DF_TRACEPOINT (SIL_TRACE_ERROR, "Df domain info has not been built\n");
    return SilUnsupported;
  }

  if (DfIpBlock != NULL) {
    *DfIpBlock = (DFCLASS_INPUT_BLK *) xUslFindStructure (SilId_DfClass, DFCLASS_INSTANCE);
    if (*DfIpBlock == NULL) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, "Df input data blk not found\n");
      return SilUnsupported;
    }
  }

  *DomainInfo = Domains;
  return SilPass;
}

/**
 * DfXAcpiGetDomainCounts
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiGetDomainCounts (
  uint32_t  *ReportedDomainCount,
  uint32_t  *PhysicalDomainCount,
  uint32_t  *PhysNodesPerSocket,
  uint32_t  *SystemCxlCount,
  uint32_t  *MaxDomains,
  bool      *CcxAsNuma
  )
{
  DF_DOMAIN_INFO_BLK  *DomainInfo;
  DFCLASS_INPUT_BLK   *DfIpBlock;
  SIL_STATUS          Status;
  uint32_t            Sockets;

  Status = DfXAcpiGetBlocks (&DomainInfo, &DfIpBlock);
  if (Status != SilPass) {
    return Status;
  }

  if (ReportedDomainCount != NULL) {
    *ReportedDomainCount = DomainInfo->NumberOfReportedDomains;
  }

  if (PhysicalDomainCount != NULL) {
    *PhysicalDomainCount = DomainInfo->NumberOfPhysicalDomains;
  }

  if (PhysNodesPerSocket != NULL) {
    *PhysNodesPerSocket = DomainInfo->PhysNps;
  }

  if (SystemCxlCount != NULL) {
    *SystemCxlCount = DomainInfo->SystemCxlCount;
  }

  if (CcxAsNuma != NULL) {
    *CcxAsNuma = DomainInfo->CcxAsNuma;
  }

  if (MaxDomains != NULL) {
    //
    // Return a count, not the largest domain index. NPS0 interleaves both
    // sockets into one DRAM domain. MSCT subtracts one from this count.
    //
    Sockets = DfIpBlock->AmdNumberOfPhysicalSocket;
    if (Sockets == 0) {
      Sockets = 1;
    }
    if (DomainInfo->CcxAsNuma) {
      *MaxDomains = Sockets * DomainInfo->CcdCount[0] * DomainInfo->CcxPerCcd[0];
    } else {
      *MaxDomains = DomainInfo->PhysNps == 0 ? 1 : Sockets * DomainInfo->PhysNps;
    }
    *MaxDomains += DomainInfo->SystemCxlCount;
    if (*MaxDomains < DomainInfo->NumberOfReportedDomains) {
      *MaxDomains = DomainInfo->NumberOfReportedDomains;
    }
  }

  return SilPass;
}

/**
 * DfXAcpiGetReportedDomains
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiGetReportedDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  DF_DOMAIN_INFO_BLK  *DomainInfo;
  SIL_STATUS          Status;
  uint32_t            Domain;
  uint32_t            Word;

  if ((Count == NULL) || (Buffer == NULL)) {
    return SilInvalidParameter;
  }

  Status = DfXAcpiGetBlocks (&DomainInfo, NULL);
  if (Status != SilPass) {
    return Status;
  }

  if (DomainInfo->NumberOfReportedDomains > MaxDomains) {
    return SilOutOfBounds;
  }

  Word = 0;
  for (Domain = 0; Domain < DomainInfo->NumberOfReportedDomains; Domain++) {
    Buffer[Word++] = (uint32_t) DomainInfo->ReportedDomainInfo[Domain].Type;
    Buffer[Word++] = DomainInfo->ReportedDomainInfo[Domain].SocketMap;
    Buffer[Word++] = DomainInfo->ReportedDomainInfo[Domain].PhysicalDomain;
  }

  *Count = DomainInfo->NumberOfReportedDomains;
  return SilPass;
}

/**
 * DfXAcpiGetPhysicalDomains
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiGetPhysicalDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  DF_DOMAIN_INFO_BLK  *DomainInfo;
  SIL_STATUS          Status;
  uint32_t            Domain;
  uint32_t            Word;

  if ((Count == NULL) || (Buffer == NULL)) {
    return SilInvalidParameter;
  }

  Status = DfXAcpiGetBlocks (&DomainInfo, NULL);
  if (Status != SilPass) {
    return Status;
  }

  if (DomainInfo->NumberOfPhysicalDomains > MaxDomains) {
    return SilOutOfBounds;
  }

  Word = 0;
  for (Domain = 0; Domain < DomainInfo->NumberOfPhysicalDomains; Domain++) {
    Buffer[Word++] = DomainInfo->PhysicalDomainInfo[Domain].NormalizedCsMap;
    Buffer[Word++] = DomainInfo->PhysicalDomainInfo[Domain].SharingEntityCount;
    Buffer[Word++] = DomainInfo->PhysicalDomainInfo[Domain].SharingEntityMap;
    Buffer[Word++] = DomainInfo->PhysicalDomainInfo[Domain].ExtendInfo;
  }

  *Count = DomainInfo->NumberOfPhysicalDomains;
  return SilPass;
}

/**
 * DfXAcpiGetNthReportedDomain
 *
 * @brief Find the Nth reported domain sitting on a given physical domain.
 *
 * @param[in]  DomainInfo      Domain info block
 * @param[in]  PhysicalDomain  Physical domain to search for
 * @param[in]  N               Zero based occurrence to return
 * @param[out] ReportedDomain  Receives the reported domain number
 *
 * @retval SilPass             Found
 * @retval SilNotFound         Fewer than N+1 reported domains share it
 */
static
SIL_STATUS
DfXAcpiGetNthReportedDomain (
  DF_DOMAIN_INFO_BLK  *DomainInfo,
  uint32_t            PhysicalDomain,
  uint32_t            N,
  uint32_t            *ReportedDomain
  )
{
  uint32_t  Current;
  uint32_t  Found;

  Found = 0;
  for (Current = 0; Current < DomainInfo->NumberOfReportedDomains; Current++) {
    if (DomainInfo->ReportedDomainInfo[Current].PhysicalDomain != PhysicalDomain) {
      continue;
    }
    if (Found == N) {
      *ReportedDomain = Current;
      return SilPass;
    }
    Found++;
  }

  return SilNotFound;
}

/**
 * DfXAcpiStoreMemEntry
 *
 * @brief Write one memory range in the split form SRAT and CRAT expect.
 *
 * @param[out] Entry   Receives the range
 * @param[in]  Domain  Owning reported domain
 * @param[in]  Base    Range base, in units of 64KB
 * @param[in]  Size    Range size, in units of 64KB
 */
static
void
DfXAcpiStoreMemEntry (
  DF_ACPI_MEM_ENTRY  *Entry,
  uint32_t           Domain,
  uint32_t           Base,
  uint32_t           Size
  )
{
  Entry->Domain = Domain;
  Entry->BaseLo = (Base & 0x0000FFFFu) << 16;
  Entry->BaseHi = (Base & 0xFFFF0000u) >> 16;
  Entry->SizeLo = (Size & 0x0000FFFFu) << 16;
  Entry->SizeHi = (Size & 0xFFFF0000u) >> 16;
}

/**
 * DfXAcpiCollectMemoryEntries
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiCollectMemoryEntries (
  uint32_t  MaxEntries,
  uint32_t  *MemEntryCount,
  uint32_t  *MemEntryBuffer,
  uint32_t  *MemInfoCount,
  uint32_t  *MemInfoBuffer
  )
{
  DF_DOMAIN_INFO_BLK          *DomainInfo;
  DFCLASS_INPUT_BLK           *DfIpBlock;
  DF_COMMON_2_REV_XFER_BLOCK  *DfXfer;
  DF_DRAM_REGION              Regions[DF_MAX_DRAM_REGIONS];
  DF_HOIST_REGION             HoistRegions[DF_MAX_HOIST_REGIONS];
  DF_ACPI_MEM_ENTRY           *Entries;
  DF_ACPI_MEM_INFO            *Infos;
  SIL_STATUS                  Status;
  uint32_t                    NumberOfRegions;
  uint32_t                    NumberOfHoisted;
  uint32_t                    DramMapIndex;
  uint32_t                    PhysDomain;
  uint32_t                    ReportedDomain;
  uint32_t                    Entity;
  uint32_t                    EntitySize;
  uint32_t                    MemoryBase;
  uint32_t                    RegionBase;
  uint32_t                    RegionSize;
  uint32_t                    RegionSizeRemaining;
  uint32_t                    SizeRemaining;
  uint32_t                    ActualSize;
  uint32_t                    EntryCount;
  uint32_t                    InfoCount;
  uint32_t                    i;
  uint32_t                    j;

  if ((MemEntryCount == NULL) || (MemEntryBuffer == NULL) ||
      (MemInfoCount == NULL) || (MemInfoBuffer == NULL)) {
    return SilInvalidParameter;
  }

  Status = DfXAcpiGetBlocks (&DomainInfo, &DfIpBlock);
  if (Status != SilPass) {
    return Status;
  }

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    return Status;
  }

  NumberOfRegions = DfXfer->DfGetNumberOfDramRegions ();
  if (NumberOfRegions > DF_MAX_DRAM_REGIONS) {
    DF_TRACEPOINT (SIL_TRACE_ERROR, "DF reports %d DRAM ranges, more than expected\n", NumberOfRegions);
    return SilOutOfBounds;
  }

  Status = DfXfer->DfCollectDramMap (DfIpBlock->AmdFabric1TbRemap,
    NumberOfRegions,
    Regions,
    &NumberOfHoisted,
    HoistRegions
    );
  if (Status != SilPass) {
    return Status;
  }

  Entries = (DF_ACPI_MEM_ENTRY *) MemEntryBuffer;
  Infos = (DF_ACPI_MEM_INFO *) MemInfoBuffer;
  EntryCount = 0;
  InfoCount = 0;

  //
  // DRAM behind the UMCs first. A range is owned by one physical domain but may
  // be shared by several reported domains, so it is cut into equal pieces and
  // each piece is handed to the next reported domain in turn.
  //
  for (DramMapIndex = 0; DramMapIndex < NumberOfRegions; DramMapIndex++) {
    if ((Regions[DramMapIndex].NormalizedMap & CS_UMC_SYSTEM_MAP) == 0) {
      continue;
    }

    RegionSize = Regions[DramMapIndex].RawSize;
    RegionBase = Regions[DramMapIndex].RawBase;
    if (RegionSize == 0) {
      continue;
    }

    for (PhysDomain = 0; PhysDomain < DomainInfo->NumberOfPhysicalDomains; PhysDomain++) {
      if ((Regions[DramMapIndex].NormalizedMap &
           DomainInfo->PhysicalDomainInfo[PhysDomain].NormalizedCsMap) != 0) {
        break;
      }
    }
    if (PhysDomain >= DomainInfo->NumberOfPhysicalDomains) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, "DRAM range %d matches no physical domain\n", DramMapIndex);
      return SilAborted;
    }

    if (DomainInfo->PhysicalDomainInfo[PhysDomain].SharingEntityCount == 0) {
      DF_TRACEPOINT (SIL_TRACE_ERROR, "Physical domain %d has no sharing entity\n", PhysDomain);
      return SilAborted;
    }

    EntitySize = RegionSize / DomainInfo->PhysicalDomainInfo[PhysDomain].SharingEntityCount;
    if ((EntitySize & SIZE_2MB_RSH16_MASK) != 0) {
      EntitySize &= ~SIZE_2MB_RSH16_MASK;
      EntitySize += SIZE_2MB_RSH16;
    }

    MemoryBase = RegionBase;
    RegionSizeRemaining = RegionSize;
    for (Entity = 0; Entity < DomainInfo->PhysicalDomainInfo[PhysDomain].SharingEntityCount; Entity++) {
      Status = DfXAcpiGetNthReportedDomain (DomainInfo, PhysDomain, Entity, &ReportedDomain);
      if (Status != SilPass) {
        return SilAborted;
      }

      if (RegionSizeRemaining > EntitySize) {
        SizeRemaining = EntitySize;
      } else {
        SizeRemaining = RegionSizeRemaining & ~SIZE_2MB_RSH16_MASK;
      }

      for (i = 0; i < NumberOfHoisted; i++) {
        if (MemoryBase == HoistRegions[i].Base) {
          MemoryBase = HoistRegions[i].Limit;
        }
        if ((MemoryBase < HoistRegions[i].Base) &&
            ((MemoryBase + SizeRemaining) > HoistRegions[i].Base)) {
          if ((EntryCount >= MaxEntries) || (InfoCount >= MaxEntries)) {
            return SilOutOfBounds;
          }
          DfXAcpiStoreMemEntry (&Entries[EntryCount++],
            ReportedDomain,
            MemoryBase,
            HoistRegions[i].Base - MemoryBase
            );
          //
          // A hoisted window that consumes DRAM still costs the domain its
          // capacity, so the per-domain total counts up to the limit while the
          // published range stops at the base.
          //
          ActualSize = HoistRegions[i].MemoryLost ?
            (HoistRegions[i].Limit - MemoryBase) : (HoistRegions[i].Base - MemoryBase);
          RegionSizeRemaining -= ActualSize;
          SizeRemaining -= ActualSize;
          MemoryBase = HoistRegions[i].Limit;

          Infos[InfoCount].Domain = ReportedDomain;
          Infos[InfoCount].RegionSizeLo = ActualSize << 16;
          Infos[InfoCount].RegionSizeHi = ActualSize >> 16;
          InfoCount++;
        }
      }

      if ((EntryCount >= MaxEntries) || (InfoCount >= MaxEntries)) {
        return SilOutOfBounds;
      }
      DfXAcpiStoreMemEntry (&Entries[EntryCount++], ReportedDomain, MemoryBase, SizeRemaining);
      Infos[InfoCount].Domain = ReportedDomain;
      Infos[InfoCount].RegionSizeLo = SizeRemaining << 16;
      Infos[InfoCount].RegionSizeHi = SizeRemaining >> 16;
      InfoCount++;

      MemoryBase += SizeRemaining;
      RegionSizeRemaining -= SizeRemaining;
    }
  }

  //
  // Ranges that live only behind CXL. These are never shared with a core
  // complex, so each physical domain takes as much of the range as it holds.
  //
  for (DramMapIndex = 0; DramMapIndex < NumberOfRegions; DramMapIndex++) {
    if ((Regions[DramMapIndex].NormalizedMap & CS_CMP_SYSTEM_MAP) == 0) {
      continue;
    }

    RegionSize = Regions[DramMapIndex].RawSize;
    if (RegionSize == 0) {
      continue;
    }

    MemoryBase = Regions[DramMapIndex].RawBase;
    RegionSizeRemaining = RegionSize;
    for (PhysDomain = 0; PhysDomain < DomainInfo->NumberOfPhysicalDomains; PhysDomain++) {
      if ((Regions[DramMapIndex].NormalizedMap &
           DomainInfo->PhysicalDomainInfo[PhysDomain].NormalizedCsMap) == 0) {
        continue;
      }
      if (RegionSizeRemaining == 0) {
        break;
      }

      Status = DfXAcpiGetNthReportedDomain (DomainInfo, PhysDomain, 0, &ReportedDomain);
      if (Status != SilPass) {
        return SilAborted;
      }

      if ((EntryCount >= MaxEntries) || (InfoCount >= MaxEntries)) {
        return SilOutOfBounds;
      }
      DfXAcpiStoreMemEntry (&Entries[EntryCount++], ReportedDomain, MemoryBase, RegionSizeRemaining);
      Infos[InfoCount].Domain = ReportedDomain;
      Infos[InfoCount].RegionSizeLo = RegionSizeRemaining << 16;
      Infos[InfoCount].RegionSizeHi = RegionSizeRemaining >> 16;
      InfoCount++;

      MemoryBase += RegionSizeRemaining;
      RegionSizeRemaining = 0;
    }
  }

  //
  // The OS wants affinity entries grouped by domain, so sort before returning.
  // Insertion sort keeps equal domains in discovery order, which preserves the
  // ascending address order within a domain.
  //
  for (i = 1; i < EntryCount; i++) {
    DF_ACPI_MEM_ENTRY Pending = Entries[i];
    j = i;
    while ((j > 0) && (Entries[j - 1].Domain > Pending.Domain)) {
      Entries[j] = Entries[j - 1];
      j--;
    }
    Entries[j] = Pending;
  }

  *MemEntryCount = EntryCount;
  *MemInfoCount = InfoCount;

  DF_TRACEPOINT (SIL_TRACE_INFO,
    "Collected %d ACPI memory ranges over %d domains\n",
    EntryCount,
    DomainInfo->NumberOfReportedDomains
    );

  return SilPass;
}

/**
 * DfXAcpiGetDistanceInfo
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiGetDistanceInfo (
  uint32_t  DomainCount,
  uint32_t  BufferSize,
  uint8_t   *Distance
  )
{
  DF_DOMAIN_INFO_BLK   *DomainInfo;
  DFCLASS_INPUT_BLK    *DfIpBlock;
  FABRIC_DOMAIN_INFO2  *Domains;
  SIL_STATUS           Status;
  uint32_t             i;
  uint32_t             j;
  bool                 UseHostValues;
  uint8_t              D2DLocal;
  uint8_t              D2DRemote;
  uint8_t              D2CLocal;
  uint8_t              D2CRemote;
  uint8_t              V2VLocal;

  if (Distance == NULL) {
    return SilInvalidParameter;
  }

  Status = DfXAcpiGetBlocks (&DomainInfo, &DfIpBlock);
  if (Status != SilPass) {
    return Status;
  }

  if ((DomainCount > DomainInfo->NumberOfReportedDomains) ||
      (BufferSize < (DomainCount * DomainCount))) {
    return SilOutOfBounds;
  }

  UseHostValues = (DfIpBlock->AmdFabricSlitDistancePcdCtrl == 0);

  D2DLocal  = UseHostValues ? DfIpBlock->AmdFabricSlitLocalDistance : DEFAULT_DISTANCE_LOCAL;
  D2DRemote = UseHostValues ? DfIpBlock->AmdFabricSlitRemoteDistance :
                (DfIpBlock->AmdFabricSlitAutoRemoteFar ?
                  DEFAULT_DISTANCE_REMOTE_FAR : DEFAULT_DISTANCE_REMOTE);
  V2VLocal  = UseHostValues ? DfIpBlock->AmdFabricSlitVirtualDistance : DEFAULT_DISTANCE_VIRTUAL;
  D2CLocal  = UseHostValues ? DfIpBlock->AmdFabricSlitCxlLocalDistance : DEFAULT_DISTANCE_CXL_LOCAL;
  D2CRemote = UseHostValues ? DfIpBlock->AmdFabricSlitCxlRemoteDistance : DEFAULT_DISTANCE_CXL_REMOTE;

  // Anything at or below the distance to self would claim to be nearer than local memory
  if (D2DLocal  < DISTANCE_TO_SELF) { D2DLocal  = DISTANCE_MINIMUM; }
  if (D2DRemote < DISTANCE_TO_SELF) { D2DRemote = DISTANCE_MINIMUM; }
  if (V2VLocal  < DISTANCE_TO_SELF) { V2VLocal  = DISTANCE_MINIMUM; }
  if (D2CLocal  < DISTANCE_TO_SELF) { D2CLocal  = DISTANCE_MINIMUM; }
  if (D2CRemote < DISTANCE_TO_SELF) { D2CRemote = DISTANCE_MINIMUM; }

  Domains = DomainInfo->ReportedDomainInfo;

  for (i = 0; i < DomainCount; i++) {
    for (j = 0; j < DomainCount; j++) {
      if (i == j) {
        *Distance = DISTANCE_TO_SELF;
      } else if (Domains[i].PhysicalDomain == Domains[j].PhysicalDomain) {
        // Two reported domains carved out of the same physical domain
        *Distance = V2VLocal;
      } else if ((Domains[i].Type == NumaDram) && (Domains[j].Type == NumaDram)) {
        *Distance = ((Domains[i].SocketMap & Domains[j].SocketMap) != 0) ? D2DLocal : D2DRemote;
      } else if ((Domains[i].Type == NumaDram) && (Domains[j].Type == NumaCxl)) {
        *Distance = ((Domains[i].SocketMap & Domains[j].SocketMap) != 0) ? D2CLocal : D2CRemote;
      } else {
        // Nothing is reachable from CXL attached memory, and no other pairing is defined
        *Distance = DISTANCE_UNREACHABLE;
      }
      Distance++;
    }
  }

  return SilPass;
}

/**
 * DfXAcpiGetPxmDomains
 *
 * See DfXAcpiDomainInfo.h for the interface description.
 */
SIL_STATUS
DfXAcpiGetPxmDomains (
  uint32_t  BusBase,
  uint32_t  MaxCount,
  uint32_t  *Count,
  uint32_t  *Domains
  )
{
  DF_COMMON_2_REV_XFER_BLOCK  *DfXfer;
  SIL_STATUS                  Status;

  if ((Count == NULL) || (Domains == NULL)) {
    return SilInvalidParameter;
  }

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void **) &DfXfer);
  if (Status != SilPass) {
    return Status;
  }

  return DfXfer->DfGetPxmDomains (BusBase, MaxCount, Count, Domains);
}
