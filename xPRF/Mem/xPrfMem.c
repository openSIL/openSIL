/**
 * @file  xPrfMem.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        Mem
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <ApobCmn.h>
#include <xPRF-api.h>
#include <string.h>
#include "xPrfMem.h"
#include "xPrfMemSpd5.h"

/**
 *  IsNvHybridDimm
 *
 * @brief    Check if this is a NV hybrid DIMM
 *
 * @param    *SpdData    - Pointer to the SPD data array
 *
 * @retval    bool
 *
 */
static
bool
IsNvHybridDimm (
  uint8_t    *SpdData
  )
{
  SPD_BASE_CONFIG_0_S*  BaseConfig0;
  assert (SpdData != NULL);

  BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  return (BaseConfig0->KeyByte2.Field.Hybrid == SpdHybrid);
}

/**
 *  GetDimmType
 *
 * @brief  Get the type of NVDIMM
 *
 * @param  *SpdData        - Pointer to the SPD data array
 *
 * @retval  uint8_t
 *
 */
static
uint8_t
GetDimmType (
  uint8_t    *SpdData
  )
{
  SPD_BASE_CONFIG_0_S*  BaseConfig0;

  assert (SpdData != NULL);
  BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm (SpdData)) {
    switch (BaseConfig0->KeyByte2.Field.HybridMedia) {
      case SpdHybridMediaNvdimmN:
        return NvDimmNType;
      case SpdHybridMediaNvdimmP:
        return NvDimmPType;
    }
  }
  return DramType;
}


/**
 *  GetDimmModuleManufacturerId
 *
 * @brief  Get the module manufacturer ID
 *
 * @param  *SpdData     - Pointer to the SPD data array
 *
 * @retval  uint16_t    - The Manufacture Identifier
 *
 */
static
uint16_t
GetDimmModuleManufacturerId (
  uint8_t    *SpdData
  )
{
  SPD_MANUFACTURING_INFO_S *MfgInfo;

  assert (SpdData != NULL);
  MfgInfo     = (SPD_MANUFACTURING_INFO_S *)&(SpdData[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
  return MfgInfo->ModuleMfgId.Value;
}

/**
 *  GetNvDimmSubsystemControllerManufacturerId
 *
 * @brief  Get the manufacturer ID of NVDIMM Subsystem
 *         Controller
 *
 * @param   *SpdData     - Pointer to the SPD data array
 *
 * @retval   uint16_t    - The Manufacturer Identifier of NVDIMM
 *                         Subsystem Controller
 *
 */
static
uint16_t
GetNvDimmSubsystemControllerManufacturerId (
 uint8_t    *SpdData
  )
{
  SPD_NVDIMM_N_ANNEX_S   *ModuleParms;

  assert (SpdData != NULL);
  ModuleParms = (SPD_NVDIMM_N_ANNEX_S *)&(SpdData[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm (SpdData)) {
    return ModuleParms->DbMfgId.Value;
  } else {
    return 0;
  }
}

/**
 *  InitSmbios32Type17
 *
 * @brief  Initialize the SMBIOS 3.2 fields of Type 17
 *
 * @param   *DimmPresent  - DIMM Present
 * @param   *SpdData      - Pointer to the SPD data array
 * @param   *T17          - Pointer to TYPE17_DMI_INFO
 *
 */
static
void
InitSmbios32Type17 (
  bool                   DimmPresent,
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  )
{
  uint32_t         MemorySize;

  assert (SpdData != NULL);
  assert (T17 != NULL);

  if (DimmPresent) {
    T17->MemoryTechnology                           = GetDimmType (SpdData);
    T17->MemoryOperatingModeCapability.AsUint16     = 0;
    T17->FirmwareVersion[0]                         ='\0';
    T17->ModuleManufacturerId                       = GetDimmModuleManufacturerId (SpdData);
    T17->ModuleProductId                            = 0;
    T17->MemorySubsystemControllerManufacturerId    = GetNvDimmSubsystemControllerManufacturerId (SpdData);
    T17->MemorySubsystemControllerProductId         = 0;
    if (0x7FFF == T17->MemorySize) {
      MemorySize = T17->ExtSize;
    } else {
      MemorySize = T17->MemorySize;
    }
    if (IsNvHybridDimm (SpdData)) {
      T17->NonvolatileSize                          =  MemorySize * 0x100000; // Convert Mega size to Byte size
      T17->VolatileSize                             = 0;
      T17->CacheSize                                = 0;
      T17->LogicalSize                              = 0;
    } else {
      T17->NonvolatileSize                          = 0;
      T17->VolatileSize                             = MemorySize * 0x100000; // Convert Mega size to Byte size
      T17->CacheSize                                = 0;
      T17->LogicalSize                              = 0;
    }
  } else {
    T17->MemoryTechnology                           = UnknownType;
    T17->MemoryOperatingModeCapability.AsBitmap.Unknown = 1;
    T17->FirmwareVersion[0]                         = '\0';
    T17->ModuleManufacturerId                       = 0;
    T17->ModuleProductId                            = 0;
    T17->MemorySubsystemControllerManufacturerId    = 0;
    T17->MemorySubsystemControllerProductId         = 0;
    T17->NonvolatileSize                            = 0;
    T17->VolatileSize                               = 0;
    T17->CacheSize                                  = 0;
    T17->LogicalSize                                = 0;
  }
}

/**
 *
 * IntToString
 *
 * @brief  Translate uint8_t array to char array.
 *
 * @param   *String       Pointer to char array
 * @param   *Integer      Pointer to uint8_t array
 * @param   SizeInByte    The size of uint8_t array
 *
 */
static
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
 * TranslateChannelInfo
 *
 * @brief   This function dTranslate the channel Id depending
 *          upon the channel translation table.
 *
 * @param   RequestedChannelId     The requested channel Id
 * @param   *TranslatedChannelId   Pointer to the translated Id
 * @param   *XlatTable             Pointer to the memory channel
 *                                 translation table
 *
 */
static
void
TranslateChannelInfo (
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId,
  HOST_TO_APCB_CHANNEL_XLAT  *XlatTable
  )
{
  *TranslatedChannelId = RequestedChannelId;

  if (XlatTable != NULL) {
    while (XlatTable->RequestedChannelId != 0xFF) {
      if (RequestedChannelId == XlatTable->RequestedChannelId) {
        *TranslatedChannelId = XlatTable->TranslatedChannelId;
        return;
      }
      XlatTable++;
    }
  }
}

/**
 * MemSpdDecodeDensity
 *
 * @brief   This function decode SDRAM density per Die to its
 *          corresponding capacity.
 *
 * @param   SpdDensity   SDRAM density per Die
 *
 * @return  uint8_t      spd capcity
 *
 */
static
uint8_t
MemSpdDecodeDensity (
   uint8_t    SpdDensity
  )
{
  uint8_t SpdDensityTable[] = SPD_DENSITY_TABLE;
  uint8_t Density;

  if (SpdDensity >= sizeof (SpdDensityTable)) {
    Density = 0;
  } else {
    Density = SpdDensityTable[SpdDensity];
  }
  return Density;
}

/**
 * MemSpdDecodeDiesPerPackage
 *
 * @brief   This function gets the Die per package from
 *          the Spd Die per package table.
 *
 * @param   SpdDies   the Spd Die per package index in the table
 *
 * @return  uint8_t   the Spd Die per package
 *
 */
static
uint8_t
MemSpdDecodeDiesPerPackage (
   uint8_t    SpdDies
  )
{
  uint8_t SpdDiePerPkgTable[] = SPD_DIE_PER_PKG_TABLE;
  uint8_t Dies;

  if (SpdDies >= sizeof (SpdDiePerPkgTable)) {
    Dies = 0;
  } else {
    Dies = SpdDiePerPkgTable[SpdDies];
  }
  return Dies;
}

/**
 * SilReadSpd
 *
 * @brief  Call the host environment interface, once, to read an
 *         SPD's* content.**
 *
 * @param    SocketId              - The socket ID
 * @param    MemChannelId          - The Memory channel ID
 * @param    DimmId                - The Dimm ID
 * @param    SpdBufPtr             - Pointer to the SPD Buffer
 *
 * @return   SIL_STATUS
 *
 */
static
SIL_STATUS
SilReadSpd  (
  uint8_t    SocketId,
  uint8_t    MemChannelId,
  uint8_t    DimmId,
  uint8_t    *SpdBufPtr
  )
{
    if (SpdBufPtr == NULL) {
      return SilInvalidParameter;
    }
    if ((SocketId == 0) && (MemChannelId == 0) && (DimmId == 1)) {
    SpdBufPtr = MemSpdDdr4;
    return SilPass;
  } else {
    SpdBufPtr = NULL;
    return SilUnsupported;
  }
}

/**
 *  GetPhysicalDimmInfoD5
 *
 * @brief   Obtain the physical DIMM information from SPD data.
 *
 * @param    *T17               - Pointer to TYPE17_DMI_INFO
 * @param    *PhysicalDimm      - Pointer to the physical
 *                                DIMM info
 * @param    TranslatedChannel  - Translated Channel Number
 *
 * @retval   boolean
 *
 */
static
uint8_t
GetPhysicalDimmInfoD5 (
  SIL_TYPE17_DMI_INFO           *T17,
  APOB_MEM_DMI_PHYSICAL_DIMM    *PhysicalDimm,
  uint8_t                       TranslatedChannel
  )
{
  uint16_t                  i;
  uint8_t                   Socket;
  uint8_t                   Channel;
  uint8_t                   Dimm;
  uint8_t                   DimmSpd[SPD_BUFFER_SIZE] = {0,};
  SIL_STATUS                SilStatus;
  uint8_t                   IoWidth;
  uint16_t                  BusWidth;
  uint16_t                  TckPs;
  uint32_t                  MemorySize;
  uint32_t                  FreqTableIndex;
  SIL_STATUS                ApobStatus;
  uint16_t                  Instance;
  uint16_t                  DieLoop;
  uint8_t                   NumRanks;
  uint8_t                   Rank;
  uint8_t                   SpdCapacity;
  uint8_t                   Asymetric;
  SPD_BASE_CONFIG_0_S       *BaseConfig0;
  SPD_ANNEX_COMMON_S        *ModuleParms;
  SPD_MANUFACTURING_INFO_S  *MfgInfo;
  uint8_t                   ChannelsPerDimm;
  uint8_t                   DiePerPkg;

//  uint32_t ManufacturerIdCode;

  Socket  = PhysicalDimm->Socket;
  Channel = PhysicalDimm->Channel;
  Dimm    = PhysicalDimm->Dimm;
  T17->Handle = PhysicalDimm->Handle;
  T17->DeviceSet = 0;

  T17->DeviceLocator[0] = 'D';
  T17->DeviceLocator[1] = 'I';
  T17->DeviceLocator[2] = 'M';
  T17->DeviceLocator[3] = 'M';
  T17->DeviceLocator[4] = ' ';
  T17->DeviceLocator[5] = Dimm + 0x30;
  T17->DeviceLocator[6] = '\0';
  T17->DeviceLocator[7] = '\0';

  T17->BankLocator[0] = 'P';
  T17->BankLocator[1] = Socket + 0x30;
  T17->BankLocator[2] = ' ';
  T17->BankLocator[3] = 'C';
  T17->BankLocator[4] = 'H';
  T17->BankLocator[5] = 'A';
  T17->BankLocator[6] = 'N';
  T17->BankLocator[7] = 'N';
  T17->BankLocator[8] = 'E';
  T17->BankLocator[9] = 'L';
  T17->BankLocator[10] = ' ';
  T17->BankLocator[11] = TranslatedChannel + 0x41;
  T17->BankLocator[12] = '\0';

  if (PhysicalDimm->DimmPresent) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO, "SPD Socket %d Channel %d Dimm %d: %08x\n", Socket, Channel, Dimm, DimmSpd);
    // Get SPD Data from APOB
    DieLoop = 0;
    SilStatus = SilUnsupported;
    for (DieLoop = 0; DieLoop < ABL_APOB_MAX_DIES_PER_SOCKET; DieLoop++) {
      Instance = DieLoop;
      Instance |= ((Socket & 0x000000FF) << 8);
      XPRF_TRACEPOINT (SIL_TRACE_INFO,
                       "Get Spd Data from APOB for Socket %d, Die %d , Channel %d Instance %d\n",
                       Socket,
                       DieLoop,
                       TranslatedChannel,
                       Instance
                       );
      ApobStatus = ApobGetDimmSpdData (Instance, Socket, Channel, Dimm, SPD_BUFFER_SIZE, DimmSpd);
      if (ApobStatus == SilPass) {
        SilStatus = SilPass;
        break;
      }
    }
    if (SilStatus != SilPass) {
      XPRF_TRACEPOINT (SIL_TRACE_INFO, "Get Spd Data from SMBUS\n");
      SilStatus = SilReadSpd (Socket, TranslatedChannel, Dimm, (uint8_t *)DimmSpd);
    }

    assert (SilPass == SilStatus);
    BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(DimmSpd[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
    ModuleParms = (SPD_ANNEX_COMMON_S*) &(DimmSpd[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
    MfgInfo     = (SPD_MANUFACTURING_INFO_S*) &(DimmSpd[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
    MemorySize = 0;
    Asymetric = (ModuleParms->ModuleOrg.Field.RankMix == RankMixAsymmetrical) ? true : false;
    NumRanks = SPD_PACKAGE_RANKS_DECODE(ModuleParms->ModuleOrg.Field.RanksPerChannel);
    for (Rank = 0; Rank < ((NumRanks < 2) ? NumRanks : 2); Rank ++ ) { // Minimum of (NumRanks and 2)
      if (Asymetric && (Rank & 0x01) ) {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->SecondIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->SecondDensity.Field.DiePerPkg);
      } else {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->FirstIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->FirstDensity.Field.DiePerPkg);
      }
      BusWidth = SPD_CHANNEL_BUS_WIDTH_DECODE(ModuleParms->ChBusWidth.Field.Width);
      T17->DataWidth = BusWidth * 2;  // RDIMM data width = 2 x data width per sub-channel
      T17->TotalWidth = T17->DataWidth +
                        (SPD_CHANNEL_BUS_WIDTH_EXT_DECODE (ModuleParms->ChBusWidth.Field.WidthExt) * 2);
      SpdCapacity = MemSpdDecodeDensity(BaseConfig0->FirstDensity.Field.Density);
      ChannelsPerDimm = SPD_CHANNELS_PER_DIMM_DECODE(ModuleParms->ChBusWidth.Field.NumChannels);
      MemorySize += ChannelsPerDimm * BusWidth / IoWidth * DiePerPkg * SpdCapacity / 8; // MemorySize in GB
    }

    MemorySize = MemorySize << 10; // MemorySize in MB
    if (MemorySize < 0x7FFF) {
      T17->MemorySize = (uint16_t)MemorySize;
      T17->ExtSize = 0;
    } else {
      T17->MemorySize = 0x7FFF;
      T17->ExtSize = MemorySize;
    }

    T17->TypeDetail.Synchronous = 1;

    switch (BaseConfig0->KeyByte2.Field.BaseModuleType) {
      case SPD_BASEMODULE_RDIMM:
        T17->TypeDetail.Registered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_LRDIMM:
        T17->TypeDetail.Registered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_UDIMM:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_SODIMM:
      case SPD_BASEMODULE_SORDIMM:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = SodimmFormFactor;
        break;
      case SPD_BASEMODULE_SOLDEREDDOWN:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = OtherFormFactor;
        break;
      default:
        T17->TypeDetail.Unknown = 1;
        T17->FormFactor = UnknowFormFactor;
    }

    T17->MemoryType = Ddr5MemType;
    T17->MinimumVoltage = 1100;
    T17->MaximumVoltage = 1100;
    TckPs = BaseConfig0->TckAvgMin.Value;

    for (FreqTableIndex = 0;
         FreqTableIndex < sizeof (MemFreqToTckTable) / sizeof (MemFreqToTckTable[0]); FreqTableIndex ++) {
      if (MemFreqToTckTable[FreqTableIndex].TckPs <= TckPs) {
        T17->Speed = MemFreqToTckTable[FreqTableIndex].Memclk;
        break;
      }
    }

    T17->ManufacturerIdCode = MfgInfo->ModuleMfgId.Value;
    IntToString ((char *)T17->SerialNumber,
                 (uint8_t *)MfgInfo->ModuleSerialNumber.String,
                 SPD_MODULE_SERIAL_NUMBER_LEN
                 );

    for (i = 0; i < sizeof (T17->PartNumber) - 1; i ++) {
      T17->PartNumber[i] = MfgInfo->ModulePartNumber.String[i];
    }
    T17->PartNumber[i] = 0;

    T17->Attributes = NumRanks;

    T17->ConfigSpeed = PhysicalDimm->ConfigSpeed;
    T17->ConfiguredVoltage = PhysicalDimm->ConfigVoltage;

  } else {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,
                     "Dummy Type 17 Created for Socket %d, Channel %d, Dimm %d\n",
                     Socket,
                     Channel,
                     Dimm
                     );
    T17->DataWidth = 0xFFFF;
    T17->TotalWidth = 0xFFFF;
    T17->MemorySize = 0;
    T17->ExtSize = 0;
    T17->TypeDetail.Unknown = 1;
    T17->FormFactor = UnknowFormFactor;
    T17->MemoryType = UnknownMemType;
    T17->Speed = 0;
    T17->ManufacturerIdCode = 0;

    for (i = 0; i < sizeof (T17->SerialNumber); i++) {
      T17->SerialNumber[i] = 0x0;
    }

    for (i = 0; i < sizeof (T17->PartNumber); i++) {
      T17->PartNumber[i] = 0x0;
    }

    T17->Attributes = 0;
    T17->MinimumVoltage = 0;
    T17->MaximumVoltage = 0;
    T17->ConfigSpeed = 0;
    T17->ConfiguredVoltage = 0;
  }

  InitSmbios32Type17 (PhysicalDimm->DimmPresent, DimmSpd, T17);

  if (PhysicalDimm->DimmPresent) {
    return true;
  } else {
    return false;
  }
}

/**
 *  GetLogicalDimmInfo
 *
 * @brief  Obtain the logical DIMM information for Type 20.
 *
 * @param   T20           - Pointer to TYPE20_DMI_INFO
 * @param   *LogicalDimm  - Pointer to the logical DIMM
 *                         info
 *
 * @retval  boolean
 *
 */
static
uint8_t
GetLogicalDimmInfo (
  SIL_TYPE20_DMI_INFO          *T20,
  APOB_MEM_DMI_LOGICAL_DIMM    *LogicalDimm
  )
{
  if (1 == LogicalDimm->DimmPresent) {
    T20->MemoryDeviceHandle = LogicalDimm->MemoryDeviceHandle;
    T20->PartitionRowPosition = 0xFF;
    T20->InterleavePosition   = 0;
    T20->InterleavedDataDepth = 0;
    if (1 == LogicalDimm->Interleaved) {
      T20->InterleavePosition = 0xFF;
      T20->InterleavedDataDepth = 0xFF;
    }
    T20->StartingAddr       = LogicalDimm->StartingAddr;
    T20->EndingAddr         = LogicalDimm->EndingAddr;
    T20->ExtStartingAddr    = LogicalDimm->UnifiedExtStartingAddr.ExtStartingAddr;
    T20->ExtEndingAddr      = LogicalDimm->UnifiedExtEndingAddr.ExtEndingAddr;
  }

  return true;
}

/**
 * xPrfGetSystemMemoryMap
 *
 * @brief  Get top of memory (Tom2) for the Host along with
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
  uint32_t    *NumberOfHoles,
  uint64_t    *TopOfSystemMemory,
  void        **MemHoleDescPtr
  )
{
  SIL_STATUS                           Status;
  uint32_t                             NumOfMemHoleMap;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT   *ApobEntry;

  Status = AmdGetApobEntryInstance (APOB_FABRIC,
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
  uint32_t    ApobBaseAddress
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
  Status = AmdGetApobEntryInstance (APOB_FABRIC,
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
    HoleMapPtr =  &ApobEntry->ApobSystemMap.HoleInfo[MemRangeIndex];
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
  uint8_t                      Socket;
  uint8_t                      Channel;
  uint8_t                      Dimm;
  uint16_t                     DimmIndex;
  uint8_t                      MaxPhysicalDimms;
  uint8_t                      MaxLogicalDimms;
  uint64_t                     Value64;
  uint32_t                     TotalMemSize;
  uint8_t                      NumActiveDimms;
  uint8_t                      TranslatedChannel;
  SIL_STATUS                   Status;
  SIL_DMI_INFO                 *DmiBuffer;
  APOB_MEM_DMI_HEADER          *ApobMemDmiHeader;
  APOB_MEM_DMI_PHYSICAL_DIMM   *PhysicalDimm;
  APOB_MEM_DMI_LOGICAL_DIMM    *LogicalDimm;
  APOB_TYPE_HEADER             *ApobSmbiosInfo;

  DmiBuffer = DmiInfoTable;


  XPRF_TRACEPOINT (SIL_TRACE_INFO, "\tDMI enabled\n");

  Status = AmdGetApobEntryInstance (APOB_SMBIOS, APOB_MEM_SMBIOS_TYPE, 0, 0, &ApobSmbiosInfo);
  assert (ApobSmbiosInfo != NULL);
  ApobMemDmiHeader = (APOB_MEM_DMI_HEADER *)ApobSmbiosInfo;
  MaxPhysicalDimms = ApobMemDmiHeader->MaxPhysicalDimms;
  MaxLogicalDimms = ApobMemDmiHeader->MaxLogicalDimms;
  TotalMemSize = 0;
  PhysicalDimm = (APOB_MEM_DMI_PHYSICAL_DIMM *)&ApobMemDmiHeader[1];

  // Type 16 construction
  DmiBuffer->T16.Location = 0x03;
  DmiBuffer->T16.Use = 0x03;
  DmiBuffer->T16.NumberOfMemoryDevices = MaxPhysicalDimms;
  DmiBuffer->T16.MemoryErrorCorrection = (ApobMemDmiHeader->EccCapable != 0)
                                         ? Dmi16MultiBitEcc : Dmi16NoneErrCorrection;

  NumActiveDimms = 0;
  // TYPE 17 entries are organized by physical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxPhysicalDimms; DimmIndex ++, PhysicalDimm ++) {
    Socket = PhysicalDimm->Socket;
    TranslateChannelInfo (PhysicalDimm->Channel, &TranslatedChannel, Sp5ChannelXlatTable);
    Dimm = PhysicalDimm->Dimm;
    if (GetPhysicalDimmInfoD5 (&DmiBuffer->T17[Socket][TranslatedChannel][Dimm], PhysicalDimm, TranslatedChannel)) {
      NumActiveDimms ++;
    }
    TotalMemSize += (DmiBuffer->T17[Socket][TranslatedChannel][Dimm].MemorySize != 0x7FFF) ?
                      DmiBuffer->T17[Socket][TranslatedChannel][Dimm].MemorySize :
                      DmiBuffer->T17[Socket][TranslatedChannel][Dimm].ExtSize;
  }

  // Pointer to DMI info of Logical DIMMs
  LogicalDimm = (APOB_MEM_DMI_LOGICAL_DIMM *)PhysicalDimm;

  // TYPE 20 entries are organized by logical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxLogicalDimms; DimmIndex ++, LogicalDimm ++) {
    Socket = LogicalDimm->Socket;
    TranslateChannelInfo (LogicalDimm->Channel, &Channel, Sp5ChannelXlatTable);
    if (Channel != 0xFF) {
      Dimm = LogicalDimm->Dimm;
      GetLogicalDimmInfo (&DmiBuffer->T20[Socket][Channel][Dimm], LogicalDimm);
    }
  }

  // TYPE 19
  DmiBuffer->T19[0].StartingAddr    = 0;
  DmiBuffer->T19[0].ExtStartingAddr = 0;
  DmiBuffer->T19[0].ExtEndingAddr   = 0;

  // If Ending Address >= 0xFFFFFFFF, update Starting Address (offset 04h) & Ending Address (offset 08h) to 0xFFFFFFFF,
  // and use the Extended Starting Address (offset 0Fh) & Extended Ending Address (offset 17h) instead.
  Value64 = (TotalMemSize << 10) - 1;
  if (Value64 >= ((uint64_t) 0xFFFFFFFF)) {
    DmiBuffer->T19[0].StartingAddr = 0xFFFFFFFFUL;
    DmiBuffer->T19[0].EndingAddr = 0xFFFFFFFFUL;
    // In Byte
    DmiBuffer->T19[0].ExtEndingAddr = Value64 << 10;
  } else {
    // In KByte
    DmiBuffer->T19[0].EndingAddr = (uint32_t) Value64;
  }

  DmiBuffer->T19[0].PartitionWidth = NumActiveDimms;

  return Status;
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
  uint8_t        DimmPresence;
  DimmPresence = 0;
  if (MemSummary != NULL) {
    DimmPresence = (uint8_t)((MemSummary->DimmPresentMap [Socket * (MemSummary->MaxDiePerSocket) + Die] \
                            >> (Channel * (MemSummary->MaxDimmPerChannel) + Dimm)) & 1);
  }
  return DimmPresence;

}

/**
 * DumpMemoryInfoData
 *
 * @brief    This is a function prints the memory Data Struct
 *
 * @param    *MemSummary   - Pointer to AMD_MEMORY_SUMMARY
 *
 */
static
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

  XPRF_TRACEPOINT (SIL_TRACE_INFO, "Dump Memory Config. Info.\n");
  // Print MbistTestEnable
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--MbistTestEnable.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->MbistTestEnable.Status.Enabled, MemSummary->MbistTestEnable.StatusCode);
  // Print MbistAggressorEnable
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--MbistAggressorEnable.Status.Enabled        = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->MbistAggressorEnable.Status.Enabled, MemSummary->MbistAggressorEnable.StatusCode);
  // Print MbistPerBitSecondaryDieReport
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--MbistPerBitSecondaryDieReport.Status.Value     = 0x%04x (StatusCode = 0x%04x)\n",
                   MemSummary->MbistPerBitSecondaryDieReport.Status.Value,
                   MemSummary->MbistPerBitSecondaryDieReport.StatusCode);
  // Print DramTempControlledRefreshEn
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--DramTempControlledRefreshEn.Status.Enabled = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->DramTempControlledRefreshEn.Status.Enabled,
                   MemSummary->DramTempControlledRefreshEn.StatusCode);
  // Print UserTimingMode
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--UserTimingMode.Status.Enabled              = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->UserTimingMode.Status.Enabled, MemSummary->UserTimingMode.StatusCode);
  // Print UserTimingValue
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--UserTimingValue.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->UserTimingValue.Status.Enabled, MemSummary->UserTimingValue.StatusCode);
  // Print MemBusFreqLimit
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--MemBusFreqLimit.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->MemBusFreqLimit.Status.Enabled, MemSummary->MemBusFreqLimit.StatusCode);
  // Print EnablePowerDown
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--EnablePowerDown.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->EnablePowerDown.Status.Enabled, MemSummary->EnablePowerDown.StatusCode);
  // Print DramDoubleRefreshRate
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--DramDoubleRefreshRate.Status.Enabled       = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->DramDoubleRefreshRate.Status.Enabled, MemSummary->DramDoubleRefreshRate.StatusCode);
  // Print PmuTrainMode
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--PmuTrainMode.Status.Value                  = 0x%04x (StatusCode = 0x%04x)\n",
                   MemSummary->PmuTrainMode.Status.Value, MemSummary->PmuTrainMode.StatusCode);
  // Print EccSymbolSize
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--EccSymbolSize.Status.Value                 = 0x%04x (StatusCode = 0x%04x)\n",
                   MemSummary->EccSymbolSize.Status.Value, MemSummary->EccSymbolSize.StatusCode);
  // Print UEccRetry
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--UEccRetry.Status.Enabled                   = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->UEccRetry.Status.Enabled, MemSummary->UEccRetry.StatusCode);
  // Print IgnoreSpdChecksum
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--IgnoreSpdChecksum.Status.Enabled           = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->IgnoreSpdChecksum.Status.Enabled, MemSummary->IgnoreSpdChecksum.StatusCode);
  // Print EnableBankGroupSwapAlt
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--EnableBankGroupSwapAlt.Status.Enabled      = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->EnableBankGroupSwapAlt.Status.Enabled, MemSummary->EnableBankGroupSwapAlt.StatusCode);
  // Print EnableBankGroupSwap
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--EnableBankGroupSwap.Status.Enabled         = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->EnableBankGroupSwap.Status.Enabled, MemSummary->EnableBankGroupSwap.StatusCode);
  // Print DdrRouteBalancedTee
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--DdrRouteBalancedTee.Status.Enabled         = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->DdrRouteBalancedTee.Status.Enabled, MemSummary->DdrRouteBalancedTee.StatusCode);
  // Print NvdimmPowerSource
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--NvdimmPowerSource.Status.Value             = 0x%04x (StatusCode = 0x%04x)\n",
                   MemSummary->NvdimmPowerSource.Status.Value, MemSummary->NvdimmPowerSource.StatusCode);
  // Print OdtsCmdThrotEn
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--OdtsCmdThrotEn.Status.Enabled              = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->OdtsCmdThrotEn.Status.Enabled, MemSummary->OdtsCmdThrotEn.StatusCode);
  // Print OdtsCmdThrotCyc
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "--OdtsCmdThrotCyc.Status.Enabled             = %d      (StatusCode = 0x%04x)\n",
                   MemSummary->OdtsCmdThrotCyc.Status.Enabled, MemSummary->OdtsCmdThrotCyc.StatusCode);
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "\n");

  for (Socket = 0; Socket < MemSummary->MaxSocketSupported; Socket++) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO, "[Socket %d]\n", Socket);
    for (DieLoop = 0; DieLoop < MemSummary->MaxDiePerSocket; DieLoop++) {
      XPRF_TRACEPOINT (SIL_TRACE_INFO, "     [Die %d]\n", DieLoop);
      Index = Socket * MemSummary->MaxDiePerSocket + DieLoop;
      // Print DramEcc
      XPRF_TRACEPOINT (SIL_TRACE_INFO, "     --DramEcc.Status.Enabled = %d (StatusCode = 0x%04x)\n",
                       MemSummary->DramEcc[Index].Status.Enabled, MemSummary->DramEcc[Index].StatusCode);
      // Print DramParity
      XPRF_TRACEPOINT (SIL_TRACE_INFO, "     --DramParity.Status.Enabled = %d (StatusCode = 0x%04x)\n",
                       MemSummary->DramParity[Index].Status.Enabled, MemSummary->DramParity[Index].StatusCode);
      // Print AutoRefFineGranMode
      XPRF_TRACEPOINT (SIL_TRACE_INFO, "     --AutoRefFineGranMode.Status.Value = 0x%04x (StatusCode = 0x%04x)\n",
                       MemSummary->AutoRefFineGranMode[Index].Status.Value,
                       MemSummary->AutoRefFineGranMode[Index].StatusCode);

      for (Channel = 0; Channel < MemSummary->MaxChannelPerDie; Channel++) {
        XPRF_TRACEPOINT (SIL_TRACE_INFO, "          [Channel %d]\n", Channel);
        Index = Socket * MemSummary->MaxDiePerSocket * MemSummary->MaxChannelPerDie +
                DieLoop * MemSummary->MaxChannelPerDie + Channel;
        // Print ChipselIntlv
        XPRF_TRACEPOINT (SIL_TRACE_INFO, "          --ChipselIntlv.Status.Enabled = %d (StatusCode = 0x%04x)\n",
                         MemSummary->ChipselIntlv[Index].Status.Enabled, MemSummary->ChipselIntlv[Index].StatusCode);

        for (Dimm = 0; Dimm < MemSummary->MaxDimmPerChannel; Dimm++) {
          XPRF_TRACEPOINT (SIL_TRACE_INFO, "               [Dimm %d]\n", Dimm);
          // Print DimmPresentMap
          XPRF_TRACEPOINT (SIL_TRACE_INFO, "               --DimmPresence: %s\n", (GetDimmPresence
                          (MemSummary, Socket, DieLoop, Channel, Dimm) != 0) ? L"Present" : L"Absent");
        }
      }
    }
  }
}

/**
 * BuildMemoryInfoData
 *
 * @details    This is a helper function to populate
 *           AMD_MEMORY_SUMMARY structure
 *
 * @param    *MemSummary   -Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 *
 */
static
SIL_STATUS
BuildMemoryInfoData (
  AMD_MEMORY_SUMMARY   *MemSummary
 )
{
   SIL_STATUS                                      Status;
   uint8_t                                         Socket;
   uint8_t                                         Channel;
   uint8_t                                         ChanNumOnPlatf;  // Channel Number on platform
   uint16_t                                        IndexSocketDie, IndexSocketDieCh;
   uint8_t                                         Dimm;
   uint16_t                                        Instance;
   uint16_t                                        DieLoop;
   APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *MemCfgInfo;
   uint8_t                                         MemInfoSize;

  //
  // Fill in data
  //
  Status = SilPass;
  DieLoop = 0;
  MemSummary->DimmPresentMap[0] = 0;  // Clear both sockets DimmPresentMap = 0
  MemSummary->DimmPresentMap[1] = 0;
  MemInfoSize = sizeof (SIL_MEM_CFG_INFO);

  for (Socket = 0; Socket < MemSummary->MaxSocketSupported; Socket++) {
    Instance = DieLoop;
    Instance |= ((Socket & 0x000000FF) << 8);
    for (DieLoop = 0; DieLoop < MemSummary->MaxDiePerSocket; DieLoop++) {
      Instance = DieLoop;
      Instance |= ((Socket & 0x000000FF) << 8);
      Status = AmdGetApobEntryInstance (APOB_MEM,
                                        APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE,
                                        Instance,
                                        0,
                                        (APOB_TYPE_HEADER **)&MemCfgInfo
                                        );
      if (Status == SilPass) {
        XPRF_TRACEPOINT (SIL_TRACE_INFO, "Successfully get APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE of Socket %d\n",
                         Socket);
        IndexSocketDie = Socket * MemSummary->MaxDiePerSocket + DieLoop;
        // DramEcc
        memcpy ((void *)&MemSummary->DramEcc[IndexSocketDie], (void *)&MemCfgInfo->DramEcc, MemInfoSize);
        // DramParity


        memcpy ((void *)&MemSummary->DramParity[IndexSocketDie], (void *)&MemCfgInfo->DramParity, MemInfoSize);
        // AutoRefFineGranMode
        memcpy ((void *)&MemSummary->AutoRefFineGranMode[IndexSocketDie],
               (void *)&MemCfgInfo->AutoRefFineGranMode,
               MemInfoSize
               );

        if (Instance == 0) {
          /// Status report per system
          // MbistTestEnable
          memcpy ((void *)&MemSummary->MbistTestEnable, (void *)&MemCfgInfo->MbistTestEnable, MemInfoSize);
          // MbistAggressorEnable
          memcpy ((void *)&MemSummary->MbistAggressorEnable, (void *)&MemCfgInfo->MbistAggressorEnable, MemInfoSize);
          // MbistPerBitSecondaryDieReport
          memcpy ((void *)&MemSummary->MbistPerBitSecondaryDieReport,
                 (void *)&MemCfgInfo->MbistPerBitSecondaryDieReport,
                 MemInfoSize
                 );
          // DramTempControlledRefreshEn
          memcpy ((void *)&MemSummary->DramTempControlledRefreshEn,
                 (void *)&MemCfgInfo->DramTempControlledRefreshEn,
                 MemInfoSize
                 );
          // UserTimingMode
          memcpy ((void *)&MemSummary->UserTimingMode, (void *)&MemCfgInfo->UserTimingMode, MemInfoSize);
          // UserTimingValue
          memcpy ((void *)&MemSummary->UserTimingValue, (void *)&MemCfgInfo->UserTimingValue, MemInfoSize);
          // MemBusFreqLimit
          memcpy ((void *)&MemSummary->MemBusFreqLimit, (void *)&MemCfgInfo->MemBusFreqLimit, MemInfoSize);
          // EnablePowerDown
          memcpy ((void *)&MemSummary->EnablePowerDown, (void *)&MemCfgInfo->EnablePowerDown, MemInfoSize);
          // DramDoubleRefreshRate
          memcpy ((void *)&MemSummary->DramDoubleRefreshRate, (void *)&MemCfgInfo->DramDoubleRefreshRate, MemInfoSize);
          // PmuTrainMode
          memcpy ((void *)&MemSummary->PmuTrainMode, (void *)&MemCfgInfo->PmuTrainMode, MemInfoSize);
          // EccSymbolSize
          memcpy ((void *)&MemSummary->EccSymbolSize, (void *)&MemCfgInfo->EccSymbolSize, MemInfoSize);
          // UEccRetry
          memcpy ((void *)&MemSummary->UEccRetry, (void *)&MemCfgInfo->UEccRetry, MemInfoSize);
          // IgnoreSpdChecksum
          memcpy ((void *)&MemSummary->IgnoreSpdChecksum, (void *)&MemCfgInfo->IgnoreSpdChecksum, MemInfoSize);
          // EnableBankGroupSwapAlt
          memcpy ((void *)&MemSummary->EnableBankGroupSwapAlt,
                 (void *)&MemCfgInfo->EnableBankGroupSwapAlt,
                 MemInfoSize
                 );
          // EnableBankGroupSwap
          memcpy ((void *)&MemSummary->EnableBankGroupSwap, (void *)&MemCfgInfo->EnableBankGroupSwap, MemInfoSize);
          // DdrRouteBalancedTee
          memcpy ((void *)&MemSummary->DdrRouteBalancedTee, (void *)&MemCfgInfo->DdrRouteBalancedTee, MemInfoSize);
          // NvdimmPowerSource
          memcpy ((void *)&MemSummary->NvdimmPowerSource, (void *)&MemCfgInfo->NvdimmPowerSource, MemInfoSize);
          // OdtsCmdThrotEn
          memcpy ((void *)&MemSummary->OdtsCmdThrotEn, (void *)&MemCfgInfo->OdtsCmdThrotEn, MemInfoSize);
          // OdtsCmdThrotCyc
          memcpy ((void *)&MemSummary->OdtsCmdThrotCyc, (void *)&MemCfgInfo->OdtsCmdThrotCyc, MemInfoSize);
        }

        for (Channel = 0; Channel < MemSummary->MaxChannelPerDie; Channel++) {
          //
          // In the following code a UMC channel number is converted into Channel Number on platform
          // e.g.
          //  Channel A (Channel 0) - UMC 0
          //  Channel B (Channel 1) - UMC 1
          //
          // If MemChanXLatTab is NULL, then no address translation required.
          //
          ChanNumOnPlatf = Channel;     // Start initially with UMC and channel on platform as same

          TranslateChannelInfo (Channel, &ChanNumOnPlatf, Sp5ChannelXlatTable);


          IndexSocketDieCh = Socket * MemSummary->MaxDiePerSocket * MemSummary->MaxChannelPerDie +
                                            DieLoop * MemSummary->MaxChannelPerDie + ChanNumOnPlatf;
          // ChipselIntlv
          memcpy ((void *)&MemSummary->ChipselIntlv[IndexSocketDieCh],
                 (void *)&MemCfgInfo->ChipselIntlv[Channel],
                 MemInfoSize
                 );

          for (Dimm = 0; Dimm < MemSummary->MaxDimmPerChannel; Dimm++) {
            // Dimm Present Map
            if (MemCfgInfo->DimmSize[Channel][Dimm] != 0) {
              MemSummary->DimmPresentMap[IndexSocketDie] |= 1 <<
                (ChanNumOnPlatf * MemSummary->MaxDimmPerChannel + Dimm);
            }
          }
        }
      }
    }
  }

  DumpMemoryInfoData (MemSummary);
  return Status;
}

/**
 * xPrfGetMemInfo
 *
 * @brief    populate AMD_MEMORY_SUMMARY
 *
 * @details  This function populate AMD_MEMORY_SUMMARY
 *           structure which is used to report DRAM info
 *           to the Host
 *
 * @param    *MemSummaryTable  - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
xPrfGetMemInfo (
  AMD_MEMORY_SUMMARY *MemSummaryTable
  )
{
  SIL_STATUS                  Status;
  AMD_MEMORY_SUMMARY          *MemSummary;
  APOB_TYPE_HEADER            *ApobEntry;

  if (MemSummaryTable == NULL) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "xPrfGetMemInfo: Failed invalid input\n");
    return SilInvalidParameter;
  }
  ApobEntry = NULL;
  MemSummary = MemSummaryTable;

  Status = AmdGetApobEntryInstance (APOB_MEM, APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE, 0, 0, &ApobEntry);
  if ((Status != SilPass) || (ApobEntry == NULL)) {
    MemSummary->AmdMemoryFrequency = 0;
    MemSummary->DdrMaxRate         = 0;
    //Fill in max. number of Socket/Die/Channel/Dimm
    MemSummary->MaxSocketSupported = 0;
    MemSummary->MaxDiePerSocket    = 0;
    MemSummary->MaxChannelPerDie   = 0;
    MemSummary->MaxDimmPerChannel  = 0;
  } else {
    MemSummary->AmdMemoryFrequency = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *)ApobEntry)->MemClkFreq;
    MemSummary->DdrMaxRate         = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *)ApobEntry)->DdrMaxRate;
  }

  //
  // Build Ppi memory config. info data.
  //
  Status = BuildMemoryInfoData (MemSummary);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "xPrfGetMemInfo: Build AMD_MEMORY_SUMMARY Failed!\n");
  }
  return (Status);
}

