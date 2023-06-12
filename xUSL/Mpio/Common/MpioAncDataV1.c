/**
 *  @file MpioAncDataV1.c
 *  @brief Ancillary Data related functions for MPIO
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <SilPcie.h>
#include "MpioStructs.h"
#include "MpioLibLocal.h"

/**
 * Function to count the ancillary data size for type
 *
 * @param   Type             Ancillary data type
 * @param   PortPointer      Pointer to the platform Host Firmware supplied platform configuration
 *
 * @retval   uint32_t        ancillary data size for this type
 *
 */
static
uint32_t
GetPortAncDataTypeSize (
  uint8_t                 Type,
  MPIO_PORT_DESCRIPTOR    *PortPointer
  )
{
  uint32_t            i;
  uint32_t            Size = 0;
  MPIO_ANC_DATA_PARAM *AncDataOvrd;

  if (PortPointer->AncData.Ovrd == 0) {
    return Size;
  }

  AncDataOvrd = (MPIO_ANC_DATA_PARAM *)((uintptr_t)(PortPointer->AncData.Ovrd));

  for (i = 0; i < PortPointer->AncData.Count; i++) {
    if (AncDataOvrd[i].Type == Type) {
      switch (AncDataOvrd[i].Type) {
        case ANC_PHY_OVRD:
          Size += sizeof (ANC_PHY_OVRD_DATA);
          break;
        case ANC_SPEED_OVRD:
          Size += sizeof (ANC_SPD_OVRD_DATA);
          break;
      }
    }
  }
  return Size;
}

/**
 * Function to calculate the ancillary data size for a PCIe/SATA engine
 *
 * @param   GnbHandle        Pointer to the Silicon Descriptor for this node
 * @param   PortPointer      Pointer to the platform Host Firmware supplied platform configuration
 *
 * @retval  uint32_t         Total size for this ancillary data
 *
 */
uint32_t
MpioAncDataCalcSize (
  /* GNB_HANDLE              *GnbHandle, */
  MPIO_PORT_DESCRIPTOR    *PortPointer
  )
{
  uint32_t            DataSize;
  uint32_t            PhyOvrdSize;
  uint32_t            SpdOvrdSize;
  /* static uint8_t      GenHeader = 0xFF; */

  DataSize = 0;
  PhyOvrdSize = GetPortAncDataTypeSize(ANC_PHY_OVRD, PortPointer);
  SpdOvrdSize = GetPortAncDataTypeSize(ANC_SPEED_OVRD, PortPointer);

  if (!(PhyOvrdSize + SpdOvrdSize)) {
    return DataSize;
  }

  /* if (GenHeader != GnbHandle->SocketId) { */
  /*   /\* */
  /*    * Add Generic ANC header size for each socket */
  /*    *\/ */
  /*   DataSize += sizeof (GENERIC_ANC_HEADER); */
  /*   GenHeader = GnbHandle->SocketId; */
  /* } */

  /*
   * Add PHY override data size
   */
  if (PhyOvrdSize) {
    DataSize += sizeof (ANC_PHY_OVRD_HEADER);
    DataSize += PhyOvrdSize;
  }

  /*
   * Add speed override data size
   */
  if (SpdOvrdSize) {
    DataSize += sizeof (ANC_SPD_OVRD_HEADER);
    DataSize += SpdOvrdSize;
  }

  DataSize += sizeof (ANC_LIST_TYPE);
  DataSize += sizeof (ANC_DATA_LIST);

  return DataSize;
}

static
uint32_t
AddPhyOvrdData (
  uint8_t                **DataEntry,
  ANC_PHY_OVRD_HEADER    **HeaderAddr,
  uint32_t               Value,
  uint8_t                *Id
  )
{
  uint32_t                  DataSize = 0;
  ANC_PHY_OVRD_HEADER       *PhyHeader;
  ANC_PHY_OVRD_DATA         *PhyOvrdData = NULL;

  if (*HeaderAddr == NULL) {
    PhyHeader = (ANC_PHY_OVRD_HEADER *)*DataEntry;
    *HeaderAddr = PhyHeader;
    PhyHeader->type = ANC_PHY_OVRD;
    PhyHeader->id = *Id;
    PhyHeader->phytype = E32;
    DataSize += sizeof (ANC_PHY_OVRD_HEADER);
    *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_PHY_OVRD_HEADER));
    ++*Id;
  }
  PhyOvrdData = (ANC_PHY_OVRD_DATA *)*DataEntry;
  PhyOvrdData->PhyOverride = Value;
  DataSize += sizeof (ANC_PHY_OVRD_DATA);
  *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_PHY_OVRD_DATA));
  return DataSize;
}

static
uint32_t
AddSpdOvrdData (
  uint8_t                **DataEntry,
  ANC_SPD_OVRD_HEADER    **HeaderAddr,
  uint32_t               Value,
  uint8_t                Lane,
  uint8_t                OvrdType
  )
{
  uint32_t                  DataSize = 0;
  ANC_SPD_OVRD_HEADER       *SpdHeader;
  ANC_SPD_OVRD_DATA         *SpdOvrdData = NULL;

  if (*HeaderAddr == NULL) {
    SpdHeader = (ANC_SPD_OVRD_HEADER *)*DataEntry;
    *HeaderAddr = SpdHeader;
    SpdHeader->type = ANC_SPEED_OVRD;
    SpdHeader->lane = Lane;
    SpdHeader->ovrdtype = OvrdType;
    DataSize += sizeof (ANC_SPD_OVRD_HEADER);
    *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_SPD_OVRD_HEADER));
  }
  SpdOvrdData = (ANC_SPD_OVRD_DATA *)*DataEntry;
  SpdOvrdData->Value = Value;
  DataSize += sizeof (ANC_SPD_OVRD_DATA);
  *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_SPD_OVRD_DATA));
  return DataSize;
}

static
uint32_t
AddAncListData (
  uint8_t                  **DataEntry,
  uint16_t                 PhyHeaderofs,
  uint16_t                 SpdHeaderofs,
  uint8_t                  Id
  )
{
  uint32_t                  DataSize = 0;
  ANC_LIST_TYPE             *AncListHeader;
  ANC_DATA_LIST             *AncListData = NULL;

  AncListHeader = (ANC_LIST_TYPE *)*DataEntry;
  AncListHeader->type = ANC_LIST;
  AncListHeader->id = Id;
  AncListHeader->length = 2;
  DataSize += sizeof (ANC_LIST_TYPE);
  *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_LIST_TYPE));

  AncListData = (ANC_DATA_LIST *)*DataEntry;
  if (PhyHeaderofs) {
    AncListData->ElementOfs0 = PhyHeaderofs;
    AncListData->ElementOfs1 = SpdHeaderofs;
  } else {
    AncListData->ElementOfs0 = SpdHeaderofs;
  }
  DataSize += sizeof (ANC_DATA_LIST);
  *DataEntry = (uint8_t *)((uint32_t)((uintptr_t)*DataEntry) + sizeof (ANC_DATA_LIST));
  return DataSize;
}

/**
 * Function to populate the ancillary data for a PCIe/SATA engine
 *
 * @param GnbHandle        Pointer to the Silicon Descriptor for this node
 * @param AncillaryData    Pointer to next available location in ancillary data
 * @param PortList         Pointer to the platform Host Firmware supplied platform configuration
 *
 * @retval      uint32_t           Total size for this ancillary data
 *
 */
uint32_t
MpioAddAncDataOverride (
  GNB_HANDLE             *GnbHandle,
  void                   *AncillaryData,
  MPIO_PORT_DESCRIPTOR   *PortList
  )
{
  uint32_t               DataSize;
  uint32_t               i;
  uint8_t                *DataEntry;
  ANC_PHY_OVRD_HEADER    *PhyOvrdHeader;
  ANC_SPD_OVRD_HEADER    *SpdOvrdHeader;
  uint32_t               PhyDataSize;
  uint32_t               SpdDataSize;
  uint16_t               PhyHeaderOfs;
  uint16_t               SpdHeaderOfs;
  uint8_t                AncListId;
  uint8_t                PhyId;
  uint8_t                Lane;
  uint8_t                SpdOvrdType;
  MPIO_ANC_DATA_PARAM    *AncDataOvrd;

  DataSize = 0;
  AncListId = 0;
  PhyId = 0;
  DataEntry = (uint8_t *)((uint8_t *)AncillaryData + sizeof(GENERIC_ANC_HEADER));

  while (PortList != NULL) {
    if ((PortList->EngineData.EngineType == MpioPcieEngine ||
      PortList->EngineData.EngineType == MpioSATAEngine) &&
      (PortList->AncData.Ovrd != 0)) {

      PhyOvrdHeader = NULL;
      SpdOvrdHeader = NULL;
      PhyDataSize = SpdDataSize = 0;
      PhyHeaderOfs = SpdHeaderOfs = 0;
      SpdOvrdType = Lane = 0;
      AncDataOvrd = (MPIO_ANC_DATA_PARAM *)((uintptr_t)(PortList->AncData.Ovrd));

      for (i = 0; i < PortList->AncData.Count; i++) {
        if (!(AncDataOvrd[i].Type == ANC_PHY_OVRD ||
            AncDataOvrd[i].Type == ANC_SPEED_OVRD)) {
          continue;
        }
        switch (AncDataOvrd[i].Type) {
          case ANC_PHY_OVRD:
            PhyDataSize += AddPhyOvrdData (&DataEntry, &PhyOvrdHeader, AncDataOvrd[i].Value, &PhyId);
            break;
          case ANC_SPEED_OVRD:
            Lane = ((PortList->EngineData.StartLane < PortList->EngineData.EndLane) ?
                     PortList->EngineData.StartLane : PortList->EngineData.EndLane);
            SpdOvrdType = ((PortList->EngineData.EngineType == MpioPcieEngine) ?
                            PCIE_PCS : SATA_PCS);
            SpdDataSize += AddSpdOvrdData (&DataEntry, &SpdOvrdHeader, AncDataOvrd[i].Value, Lane, SpdOvrdType);
            break;
        }
      }
      if (PhyDataSize) {
        PhyOvrdHeader->length = (uint8_t)(PhyDataSize >> 2);
        PhyHeaderOfs = (uint16_t)(((uint32_t)((uintptr_t)PhyOvrdHeader) - (uint32_t)((uintptr_t)AncillaryData)) >> 2);
        DataSize += PhyDataSize;
      }
      if (SpdDataSize) {
        SpdOvrdHeader->length = (uint8_t)(SpdDataSize >> 2);
        SpdHeaderOfs = (uint16_t)(((uint32_t)((uintptr_t)SpdOvrdHeader) - (uint32_t)((uintptr_t)AncillaryData)) >> 2);
        DataSize += SpdDataSize;
      }
      if (PhyDataSize + SpdDataSize) {
        PortList->Port.MpioAncDataIdx = (uint16_t)(((uint32_t)((uintptr_t)DataEntry) -
                                        (uint32_t)((uintptr_t)AncillaryData)) >> 2);
        DataSize += AddAncListData (&DataEntry, PhyHeaderOfs, SpdHeaderOfs, AncListId++);
      }
    }
    PortList = PcieConfigGetNextDataDescriptor(PortList);
  }

  return DataSize;
}

/**
 * Function to dump the whole ancillary data for this socket
 *
 * @param GnbHandle          Pointer to the Silicon Descriptor for this node
 * @param AncillaryData      Pointer to next available location in ancillary data
 *
 * @param AncillaryDataSize  Total size for this ancillary data
 *
 */
void
MpioAncDataDump (
  GNB_HANDLE                *GnbHandle,
  uint8_t                   *AncillaryData,
  uint32_t                  AncillaryDataSize
  )
{
  uint32_t    i;
  uint32_t    *DWData = NULL;

  DWData = (uint32_t *)AncillaryData;

  if (AncillaryDataSize != 0 && DWData != NULL) {
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "MPIO Socket:%d AncData Dump Addr:0x%08X size:0x%X\n", GnbHandle->SocketId,
      AncillaryData, AncillaryDataSize);
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "DWord Data:");
    for (i = 0; i < (AncillaryDataSize / 4); i++) {
      if (!(i % 4)) {
        MPIO_TRACEPOINT (SIL_TRACE_INFO, "\n-0x%02x-", i);
      }
      MPIO_TRACEPOINT (SIL_TRACE_INFO, " %08X", DWData[i]);
    }
    MPIO_TRACEPOINT (SIL_TRACE_INFO, "\n");
  }
}
