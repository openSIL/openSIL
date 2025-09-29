/* SPDX-License-Identifier: MIT */

/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemInitPhx.c
 * @brief OpenSIL MEM I2I and C2R API Function initialization.
 *
 */
#include <MEM/Common/Mem.h>
#include <SilCommon.h>
#include <MEM/Common/MemCmn2Rev.h>
#include <MEM/MemClass-api.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/PHX/Apob-PHX.h>
#include <MEM/Common/MemSpd4.h>
#include <MEM/Common/MemSpd5.h>
#include <Include/Utils.h>
#include "MemCmn2Phx.h"
#include "MemTablePhx.h"

#define SPD_BUFFER_SIZE        1024

/**
 * GetChannelXlatTablePhx
 *
 * @brief    Get ChannelXlatTable for PHX program
 *
 * @details  This function returns the program specific ChannelXlatTable for PHX
 *
 * @retval   HOST_TO_APCB_CHANNEL_XLAT
 */
HOST_TO_APCB_CHANNEL_XLAT *
GetChannelXlatTablePhx (void)
{
  return (HOST_TO_APCB_CHANNEL_XLAT *) gDummyChannelXlatTable;
}


/**
 * SilReadSpd
 *
 * @param    SocketId              - The socket ID
 * @param    MemChannelId          - The Memory channel ID
 * @param    DimmId                - The Dimm ID
 * @param    SpdBufPtr             - Pointer to the SPD Buffer
 *
 * @return   SIL_STATUS
 */
static
SIL_STATUS
SilReadSpd (
  uint8_t    SocketId,
  uint8_t    MemChannelId,
  uint8_t    DimmId,
  uint8_t    *SpdBufPtr
  )
{
  SIL_STATUS Status;

  if (SpdBufPtr == NULL) {
    Status = SilInvalidParameter;
  } else {
    SpdBufPtr = NULL;
    Status = SilUnsupported;
  }

  return Status;
}

/**
 * IsNvHybridDimm
 *
 * @brief     Checks if this is a NV hybrid DIMM
 *
 * @param     *SpdData - Pointer to the SPD data array
 *
 * @returns   bool
 * @retval    true if it is NV Hybrid DIMM, false if it isn't
 */
static
bool
IsNvHybridDimm (
  uint8_t                       *SpdData
  )
{
  assert(SpdData != NULL);

  if ((SpdData[SPD_BASE_MODULE_TYPE] & MOD_TYPE_HYBRID) != 0 &&
    (SpdData[SPD_BASE_MODULE_TYPE] & MOD_TYPE_HYBRID_TYPE) == 0x10) {
    return true;
  } else {
    return false;
  }
}

/**
 *  GetDimmOperatingModeCap
 *
 *  @brief Get the operating mode capability of DIMM
 *
 *  @param[in]       *SpdData        - Pointer to the SPD data array
 *
 *  @retval          uint16_t
 */
static
uint16_t
GetDimmOperatingModeCap (
  uint8_t                       *SpdData
  )
{
  SIL_DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY_VAR  Capability;

  assert(SpdData != NULL);

  Capability.AsUint16 = 0;
  Capability.AsBitmap.VolatileMemory = 1;

  return Capability.AsUint16;
}

/**
 * GetDimmTypePhx
 *
 * @brief    Gets the type of DIMM
 *
 * @param    *SpdData - Pointer to the SPD data array
 *
 * @returns  uint8_t Dram Type value
 */
static
uint8_t
GetDimmType (
  uint8_t                       *SpdData
  )
{
  return SilDramType;
}

/**
 * GetDimmModuleManufacturerId
 *
 * @brief      Gets the module manufacturer ID
 *
 * @param      *SpdData - Pointer to the SPD data array
 *
 * @returns    uint16_t - The Manufacturer Identifier
 */
static
uint16_t
GetDimmModuleManufacturerId (
  uint8_t  *SpdData
  )
{
  assert(SpdData != NULL);

  return (SpdData[SPD_MANUFACTURER_ID_MSB] << 8) + SpdData[SPD_MANUFACTURER_ID_LSB];
}

/**
 *  GetLogicalDimmInfoPhx
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
GetLogicalDimmInfoPhx (
  SIL_TYPE20_DMI_INFO          *T20,
  APOB_MEM_DMI_LOGICAL_DIMM_PHX    *LogicalDimm
  )
{
  if (1 == LogicalDimm->DimmPresent) {
    T20->MemoryDeviceHandle = LogicalDimm->MemoryDeviceHandle;
    T20->PartitionRowPosition = 0xFF;
    T20->InterleavePosition = 0;
    T20->InterleavedDataDepth = 0;
    if (1 == LogicalDimm->Interleaved) {
      T20->InterleavePosition = 0xFF;
      T20->InterleavedDataDepth = 0xFF;
    }
    T20->StartingAddr = LogicalDimm->StartingAddr;
    T20->EndingAddr = LogicalDimm->EndingAddr;
    T20->ExtStartingAddr = LogicalDimm->UnifiedExtStartingAddr.ExtStartingAddr;
    T20->ExtEndingAddr = LogicalDimm->UnifiedExtEndingAddr.ExtEndingAddr;
  }

  return true;
}

/**
 * ConfigureTable17DimmPresentPhx
 *
 * @brief   Configures the T17 of the SMBIOS Table when the DIMM is present
 *
 * @param   *SpdData - Pointer to the SPD data array
 * @param   *T17     - Pointer to Table 17
 *
 * @returns void
 */
void
ConfigureTable17DimmPresentPhx (
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  )
{
  uint64_t MemorySize;

  T17->MemoryTechnology = GetDimmType(SpdData);
  T17->MemoryOperatingModeCapability.AsUint16 = GetDimmOperatingModeCap(SpdData);
  T17->ModuleManufacturerId = GetDimmModuleManufacturerId(SpdData);
  if (0x7FFF == T17->MemorySize) {
    MemorySize = T17->ExtSize;
  } else {
    MemorySize = T17->MemorySize;
  }
  if (IsNvHybridDimm(SpdData)) {
    T17->NonvolatileSize = MemorySize * 0x100000;// Convert MegaByte size to Byte size
    T17->VolatileSize = 0;
    T17->CacheSize = 0;
    T17->LogicalSize = 0;
  } else {
    T17->NonvolatileSize = 0;
    T17->VolatileSize = MemorySize * 0x100000;// Convert MegaByte size to Byte size
    T17->CacheSize = 0;
    T17->LogicalSize = 0;
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
 * @return  uint8_t      spd capacity
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
  uint8_t  SpdDies
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
bool
GetPhysicalDimmInfoD5 (
  SIL_CONTEXT                       *SilContext,
  SIL_TYPE17_DMI_INFO               *T17,
  APOB_MEM_DMI_PHYSICAL_DIMM_PHX    *PhysicalDimm,
  uint8_t                           TranslatedChannel
  )
{
  uint16_t            i;
  uint8_t             Socket;
  uint8_t             Channel;
  uint8_t             Dimm;
  uint8_t             DimmSpd[SPD_BUFFER_SIZE];
  uint8_t             IoWidth;
  uint16_t            BusWidth;
  uint16_t            TckPs;
  uint32_t            MemorySize;
  uint32_t            FreqTableIndex;
  SIL_STATUS          ApobStatus;
  uint16_t            Instance;
  uint16_t            DieLoop;
  uint8_t             NumRanks;
  uint8_t             Rank;
  uint8_t             SpdCapacity;
  bool                Asymmetric;
  SPD_BASE_CONFIG_0_S  *BaseConfig0;
  SPD_ANNEX_COMMON_S   *ModuleParms;
  SPD_MANUFACTURING_INFO_S *MfgInfo;
  uint8_t ChannelsPerDimm;
  uint8_t DiePerPkg;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;
  SIL_STATUS          Status;

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    MEM_TRACEPOINT(SIL_TRACE_INFO, "Error : not found APOB IP2IP API\n");
    assert(false);
    return false;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  Socket = PhysicalDimm->Socket;
  Channel = PhysicalDimm->Channel;
  Dimm = PhysicalDimm->Dimm;
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
    MEM_TRACEPOINT(SIL_TRACE_INFO, "SPD Socket %d Channel %d Dimm %d: %08x\n", Socket, Channel, Dimm, DimmSpd);
    // Get SPD Data from APOB
    DieLoop = 0;
    Status = SilUnsupported;
    for (DieLoop = 0; DieLoop < SocMaxDieInfo.MaxSocDiesPerSocketValue; DieLoop++) {
      Instance = DieLoop;
      Instance |= ((Socket & 0x000000FF) << 8);
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "Get Spd Data from APOB for Socket %d, Die %d , Channel %d Instance %d\n",
        Socket,
        DieLoop,
        TranslatedChannel,
        Instance
        );
      ApobStatus = ApobIp2IpApi->ApobGetDimmSpdData(SilContext,
        Instance,
        Socket,
        Channel,
        Dimm,
        SPD_BUFFER_SIZE,
        DimmSpd
        );
      if (ApobStatus == SilPass) {
        Status = SilPass;
        break;
      }
    }
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_INFO, "Get Spd Data from SMBUS\n");
      Status = SilReadSpd(Socket, TranslatedChannel, Dimm, (uint8_t *)DimmSpd);
    }
    assert(SilPass == Status);

    BaseConfig0 = (SPD_BASE_CONFIG_0_S *) &(DimmSpd[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
    ModuleParms = (SPD_ANNEX_COMMON_S *) &(DimmSpd[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
    MfgInfo = (SPD_MANUFACTURING_INFO_S *) &(DimmSpd[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
    MemorySize = 0;
    Asymmetric = (ModuleParms->ModuleOrg.Field.RankMix == RankMixAsymmetrical) ? true : false;
    NumRanks = SPD_PACKAGE_RANKS_DECODE(ModuleParms->ModuleOrg.Field.RanksPerChannel);
    for (Rank = 0; Rank < ((NumRanks < 2) ? NumRanks : 2); Rank++ ) {// Minimum of (NumRanks and 2)
      if (Asymmetric && (Rank & 0x01)) {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->SecondIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->SecondDensity.Field.DiePerPkg);
      } else {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->FirstIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->FirstDensity.Field.DiePerPkg);
      }
      BusWidth = SPD_CHANNEL_BUS_WIDTH_DECODE(ModuleParms->ChBusWidth.Field.Width);
      ChannelsPerDimm = SPD_CHANNELS_PER_DIMM_DECODE(ModuleParms->ChBusWidth.Field.NumChannels);
      T17->DataWidth = BusWidth * ChannelsPerDimm;
      T17->TotalWidth = (BusWidth +
        SPD_CHANNEL_BUS_WIDTH_EXT_DECODE(ModuleParms->ChBusWidth.Field.WidthExt)) * ChannelsPerDimm;
      SpdCapacity = MemSpdDecodeDensity(BaseConfig0->FirstDensity.Field.Density);
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
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_LRDIMM:
      T17->TypeDetail.Registered = 1;
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_UDIMM:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_SODIMM:
    case SPD_BASEMODULE_SORDIMM:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilSodimmFormFactor;
      break;
    case SPD_BASEMODULE_SOLDEREDDOWN:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilOtherFormFactor;
      break;
    default:
      T17->TypeDetail.Unknown = 1;
      T17->FormFactor = SilUnknowFormFactor;
    }

    switch (BaseConfig0->KeyByte1.Field.ModuleType) {
    case ModTypeLpddr5Sdram:
      T17->MemoryType = SilLpDdr5MemType;
      T17->MinimumVoltage = 1100;
      T17->MaximumVoltage = 1100;
      break;
    default:
      T17->MemoryType = SilDdr5MemType;
      T17->MinimumVoltage = 1100;
      T17->MaximumVoltage = 1100;
      break;
    }
    TckPs = BaseConfig0->TckAvgMin.Value;
    for (FreqTableIndex = 0;
      FreqTableIndex < sizeof (MemFreqToTckTable) / sizeof (MemFreqToTckTable[0]); FreqTableIndex++) {
      if (MemFreqToTckTable[FreqTableIndex].TckPs <= TckPs) {
        T17->Speed = MemFreqToTckTable[FreqTableIndex].Memclk;
        break;
      }
    }

    T17->ManufacturerIdCode = MfgInfo->ModuleMfgId.Value;
    IntToString(T17->SerialNumber, MfgInfo->ModuleSerialNumber.String, SPD_MODULE_SERIAL_NUMBER_LEN);
    for (i = 0; i < sizeof (T17->PartNumber) - 1; i++) {
      T17->PartNumber[i] = MfgInfo->ModulePartNumber.String[i];
    }
    T17->PartNumber[i] = 0;

    T17->Attributes = NumRanks;

    T17->ConfigSpeed = PhysicalDimm->ConfigSpeed;
    T17->ConfiguredVoltage = PhysicalDimm->ConfigVoltage;
  } else {
    MEM_TRACEPOINT(SIL_TRACE_INFO,
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
    T17->FormFactor = SilUnknowFormFactor;
    T17->MemoryType = SilUnknownMemType;
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

  SilInitSmbios32Type17(SilContext, PhysicalDimm->DimmPresent, DimmSpd, T17);

  if (PhysicalDimm->DimmPresent) {
    return true;
  } else {
    return false;
  }
}

/**
 *  GetPhysicalDimmInfoD4
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
bool
GetPhysicalDimmInfoD4 (
  SIL_CONTEXT                       *SilContext,
  SIL_TYPE17_DMI_INFO               *T17,
  APOB_MEM_DMI_PHYSICAL_DIMM_PHX    *PhysicalDimm,
  uint8_t                           TranslatedChannel
  )
{
  uint16_t            i;
  uint8_t             Socket;
  uint8_t             Channel;
  uint8_t             Dimm;
  uint8_t             DimmSpd[SPD_BUFFER_SIZE];
  uint8_t             DataWidth;
  uint16_t            Capacity;
  uint16_t            BusWidth;
  uint16_t            Width;
  uint8_t             Rank;
  uint8_t             DieCount;
  uint8_t             NumChannels;
  uint8_t             NumChannelsPerPkg;
  uint32_t            MemorySize;
  int32_t             MTB_ps;
  int32_t             FTB_ps;
  int32_t             Value32;
  uint8_t             VoltageMap;
  SIL_STATUS          ApobStatus;
  uint16_t            Instance;
  uint16_t            DieLoop;
  uint8_t             SpdCapacity = 0;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;
  SIL_STATUS          Status;

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return false;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  Socket = PhysicalDimm->Socket;
  Channel = PhysicalDimm->Channel;
  Dimm = PhysicalDimm->Dimm;
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
    MEM_TRACEPOINT(SIL_TRACE_INFO, "SPD Socket %d Channel %d Dimm %d: %08x\n", Socket, Channel, Dimm, DimmSpd);
    // Get SPD Data from APOB
    DieLoop = 0;
    Status = SilUnsupported;
    for (DieLoop = 0; DieLoop < SocMaxDieInfo.MaxSocDiesPerSocketValue; DieLoop++) {
      Instance = DieLoop;
      Instance |= ((Socket & 0x000000FF) << 8);
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "Get Spd Data from APOB for Socket %d, Die %d , Channel %d Instance %d\n",
        Socket,
        DieLoop,
        TranslatedChannel,
        Instance
        );
      ApobStatus = ApobIp2IpApi->ApobGetDimmSpdData(SilContext,
        Instance,
        Socket,
        Channel,
        Dimm,
        SPD_BUFFER_SIZE,
        DimmSpd
        );
      if (ApobStatus == SilPass) {
        Status = SilPass;
        break;
      }
    }
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_INFO, "Get Spd Data from SMBUS\n");
      Status = SilReadSpd(Socket, TranslatedChannel, Dimm, (uint8_t *)DimmSpd);
    }

    assert(SilPass == Status);
    DataWidth = 1 << ((DimmSpd[SPD_BUS_WIDTH] & BUSWIDTH_MASK) + 3);
    T17->DataWidth = DataWidth;
    T17->TotalWidth = T17->DataWidth + 8 * ((DimmSpd[SPD_BUSWIDTH_EXT] & BUSWIDTH_EXT_ECC) >> 3);
    SpdCapacity = (DimmSpd[SPD_CAPACITY] & CAPACITY_MASK);
    assert(SpdCapacity < sizeof (SpdCapacityTable) / sizeof (SPD_CAPACITY_TABLE));
    Capacity = SpdCapacityTable[SpdCapacity];
    assert(Capacity <= 0x8000);
    BusWidth = 8 << (DimmSpd[SPD_BUS_WIDTH] & BUSWIDTH_MASK);
    assert(BusWidth <= 64);
    Width = 4 << (DimmSpd[SPD_DEVICE_WIDTH] & DEVICE_WIDTH_MASK);
    assert(Width <= 32);
    Rank = (1 + (DimmSpd[SPD_RANKS] >> RANKS_SHIFT)) & RANKS_MASK;
    assert(Rank <= 4);

    switch (DimmSpd[SPD_DRAM_DEVICE_TYPE]) {
    case DEVICE_TYPE_LPDDR5:
    case DEVICE_TYPE_LPDDR5X:
      DieCount = 1 + ((DimmSpd[SPD_DIE_COUNT] >> DIE_COUNT_SHIFT) & DIE_COUNT_MASK);
      assert(DieCount <= 8);
      NumChannels = 1 + ((DimmSpd[SPD_CHANNELS_PER_SYS] >> CHANNELS_PER_SYS_SHIFT) & CHANNELS_PER_SYS_MASK);
      assert(NumChannels <= 4);
      NumChannelsPerPkg = 1 << ((DimmSpd[SPD_CHANNELS_PER_PKG] >> CHANNELS_PER_PKG_SHIFT) & CHANNELS_PER_PKG_MASK);
      assert(NumChannelsPerPkg <= 4);
      if (Width == 8) {
        Width <<= ((DimmSpd[SPD_BYTE_MODE_ID] >> BYTE_MODE_ID_SHIFT) & BYTE_MODE_ID_MASK);
        assert(Width <= 32);
      }
      MemorySize = (Capacity / 8 * DieCount * BusWidth / (NumChannelsPerPkg * Width)) * NumChannels;
      break;
    default:
      MemorySize = Capacity / 8 * BusWidth / Width * Rank;
      break;
    }

    if (MemorySize < 0x7FFF) {
      T17->MemorySize = (uint16_t)MemorySize;
      T17->ExtSize = 0;
    } else {
      T17->MemorySize = 0x7FFF;
      T17->ExtSize = MemorySize;
    }

    T17->TypeDetail.Synchronous = 1;

    switch (DimmSpd[SPD_BASE_MODULE_TYPE] & MODULE_TYPE_MASK) {
    case SPD_BASEMODULE_RDIMM:
      T17->TypeDetail.Registered = 1;
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_UDIMM:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_LRDIMM:
      T17->TypeDetail.Registered = 1;
      T17->FormFactor = SilDimmFormFactorFormFactor;
      break;
    case SPD_BASEMODULE_SODIMM:
    case SPD_BASEMODULE_SORDIMM:
    case SPD_BASEMODULE_SOUDIMM:
    case SPD_BASEMODULE_DDIMM:
    case SPD_BASEMODULE_SOLDEREDDOWN:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilSodimmFormFactor;
      break;
    case SPD_BASE_NONDIMMSOLUTION:
      T17->TypeDetail.Unbuffered = 1;
      T17->FormFactor = SilOtherFormFactor;
      break;
    default:
      T17->TypeDetail.Unknown = 1;
      T17->FormFactor = SilUnknowFormFactor;
    }

    switch (DimmSpd[SPD_DRAM_DEVICE_TYPE]) {
    case DEVICE_TYPE_LPDDR5:
    case DEVICE_TYPE_LPDDR5X:
      T17->MemoryType = SilLpDdr5MemType;
      break;
    default:
      T17->MemoryType = SilDdr5MemType;
      break;
    }

    MTB_ps = (((DimmSpd[SPD_MEDIUM_TIMEBASE] >> MTB_SHIFT) & MTB_MSK) == 0) ? 125 : 0;
    FTB_ps = ((DimmSpd[SPD_FINE_TIMEBASE] & FTB_MSK) == 0) ? 1 : 0;
    Value32 = (MTB_ps * DimmSpd[SPD_TCK]) + (FTB_ps * (int8_t) DimmSpd[SPD_TCK_FTB]);

    if (Value32 <= 266) {
      T17->Speed = 3750;              // DDR-7500
    } else if (Value32 <= 285) {
      T17->Speed = 3500;              // DDR-7000
    } else if (Value32 <= 312) {
      T17->Speed = 3200;              // DDR-6400
    } else if (Value32 <= 333) {
      T17->Speed = 3000;              // DDR-6000
    } else if (Value32 <= 363) {
      T17->Speed = 2750;              // DDR-5500
    } else if (Value32 <= 416) {
      T17->Speed = 2400;              // DDR-4800
    } else if (Value32 <= 469) {
      T17->Speed = 2133;              // DDR-4267
    } else if (Value32 <= 536) {
      T17->Speed = 1866;              // DDR-3733
    } else if (Value32 <= 625) {
      T17->Speed = 1600;              // DDR-3200
    } else if (Value32 <= 682) {
      T17->Speed = 1467;              // DDR-2933
    } else if (Value32 <= 750) {
      T17->Speed = 1333;              // DDR-2667
    } else if (Value32 <= 834) {
      T17->Speed = 1200;              // DDR-2400
    } else if (Value32 <= 938) {
      T17->Speed = 1067;              // DDR-2133
    } else if (Value32 <= 1071) {
      T17->Speed = 933;               // DDR-1866
    } else if (Value32 <= 1250) {
      T17->Speed = 800;               // DDR-1600
    } else {
      T17->Speed = 667;               // DDR-1333
    }

    T17->ManufacturerIdCode = (DimmSpd[SPD_MANUFACTURER_ID_MSB] << 8) | DimmSpd[SPD_MANUFACTURER_ID_LSB];
    IntToString(T17->SerialNumber, &DimmSpd[SPD_SERIAL_NUMBER], (sizeof (T17->SerialNumber) - 1) / 2);
    for (i = 0; i < sizeof (T17->PartNumber) - 1; i++) {
      T17->PartNumber[i] = DimmSpd[i + SPD_PART_NUMBER];
    }
    T17->PartNumber[i] = 0;

    T17->Attributes = Rank;

    switch (DimmSpd[SPD_DRAM_DEVICE_TYPE]) {
    case DEVICE_TYPE_LPDDR5:
      T17->MinimumVoltage = 500;
      T17->MaximumVoltage = 500;
      break;
    case DEVICE_TYPE_LPDDR5X:
      T17->MinimumVoltage = 500;
      T17->MaximumVoltage = 500;
      break;
    default:
      VoltageMap = DimmSpd[SPD_DRAM_VDD];
      T17->MinimumVoltage = SIL_CONVERT_ENCODED_TO_VDDIO_MILLIVOLTS((xUslHighBitSet32(VoltageMap) & 0xFE),
        SilDdr5Technology
        );
      T17->MaximumVoltage = SIL_CONVERT_ENCODED_TO_VDDIO_MILLIVOLTS((xUslLowBitSet32(VoltageMap) & 0xFE),
        SilDdr5Technology
        );
      break;
    }

    T17->ConfigSpeed = PhysicalDimm->ConfigSpeed;
    T17->ConfiguredVoltage = PhysicalDimm->ConfigVoltage;
  } else {
    MEM_TRACEPOINT(SIL_TRACE_INFO,
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
    T17->FormFactor = SilUnknowFormFactor;
    T17->MemoryType = SilUnknownMemType;
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

  SilInitSmbios32Type17(SilContext, PhysicalDimm->DimmPresent, DimmSpd, T17);
  SilInitSmbios33Type17(PhysicalDimm->DimmPresent, DimmSpd, T17);

  if (PhysicalDimm->DimmPresent) {
    return true;
  } else {
    return false;
  }
}

/**
 * PopulateSmbiosMemInfoPhx
 *
 * @details    This routine gets DMI Type 16, Type 17,
 *             Type 19 and Type 20 related information.
 *
 * @param      DmiInfoTable - DMI records
 * @param      DramType -     DRAM Type
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
PopulateSmbiosMemInfoPhx (
  SIL_CONTEXT          *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  )
{
  uint8_t                          Socket;
  uint8_t                          Channel;
  uint8_t                          Dimm;
  uint16_t                         DimmIndex;
  uint8_t                          MaxPhysicalDimms;
  uint8_t                          MaxLogicalDimms;
  uint64_t                         Value64;
  uint32_t                         TotalMemSize;
  uint8_t                          NumActiveDimms;
  uint8_t                          TranslatedChannel;
  SIL_STATUS                       Status;
  APOB_MEM_DMI_HEADER              *ApobMemDmiHeader;
  APOB_MEM_DMI_PHYSICAL_DIMM_PHX   *PhysicalDimm;
  APOB_MEM_DMI_LOGICAL_DIMM_PHX    *LogicalDimm;
  APOB_TYPE_HEADER                 *ApobSmbiosInfo;
  APOB_IP2IP_API                   *ApobIp2IpApi;

  MEM_TRACEPOINT(SIL_TRACE_INFO, "\tDMI enabled\n");

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
    APOB_SMBIOS,
    APOB_MEM_SMBIOS_TYPE,
    0,
    0,
    &ApobSmbiosInfo
    );
  assert(ApobSmbiosInfo != NULL);
  ApobMemDmiHeader = (APOB_MEM_DMI_HEADER *)ApobSmbiosInfo;
  MaxPhysicalDimms = ApobMemDmiHeader->MaxPhysicalDimms;
  MaxLogicalDimms = ApobMemDmiHeader->MaxLogicalDimms;
  TotalMemSize = 0;
  PhysicalDimm = (APOB_MEM_DMI_PHYSICAL_DIMM_PHX *)&ApobMemDmiHeader[1];

  // Type 16 construction
  DmiInfoTable->T16.Location = 0x03;
  DmiInfoTable->T16.Use = 0x03;
  DmiInfoTable->T16.NumberOfMemoryDevices = MaxPhysicalDimms;
  DmiInfoTable->T16.MemoryErrorCorrection = (ApobMemDmiHeader->EccCapable != 0)
                                         ? SilDmi16MultiBitEcc : SilDmi16NoneErrCorrection;

  NumActiveDimms = 0;
  // TYPE 17 entries are organized by physical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxPhysicalDimms; DimmIndex++, PhysicalDimm++) {
    Socket = PhysicalDimm->Socket;
    TranslateChannelInfo(SilContext, PhysicalDimm->Channel, &TranslatedChannel);
    Dimm = PhysicalDimm->Dimm;
    if (MemType == SilDdr5Technology) {
      if (GetPhysicalDimmInfoD5(SilContext,
        &DmiInfoTable->T17[Socket][TranslatedChannel][Dimm],
        PhysicalDimm,
        TranslatedChannel
        )) {
        NumActiveDimms++;
      }
    } else {
      if (GetPhysicalDimmInfoD4(SilContext,
        &DmiInfoTable->T17[Socket][TranslatedChannel][Dimm],
        PhysicalDimm,
        TranslatedChannel
        )) {
        NumActiveDimms++;
      }
    }
    TotalMemSize += (DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].MemorySize != 0x7FFF) ?
      DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].MemorySize :
      DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].ExtSize;
  }

  // Pointer to DMI info of Logical DIMMs
  LogicalDimm = (APOB_MEM_DMI_LOGICAL_DIMM_PHX *)PhysicalDimm;

  // TYPE 20 entries are organized by logical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxLogicalDimms; DimmIndex++, LogicalDimm++) {
    Socket = LogicalDimm->Socket;
    TranslateChannelInfo(SilContext, LogicalDimm->Channel, &Channel);
    if (Channel != 0xFF) {
      Dimm = LogicalDimm->Dimm;
      GetLogicalDimmInfoPhx(&DmiInfoTable->T20[Socket][Channel][Dimm], LogicalDimm);
    }
  }

  // TYPE 19
  DmiInfoTable->T19[0].StartingAddr = 0;
  DmiInfoTable->T19[0].ExtStartingAddr = 0;
  DmiInfoTable->T19[0].ExtEndingAddr = 0;

  // If Ending Address >= 0xFFFFFFFF, update Starting Address (offset 04h) & Ending Address (offset 08h) to 0xFFFFFFFF,
  // and use the Extended Starting Address (offset 0Fh) & Extended Ending Address (offset 17h) instead.
  Value64 = ((uint64_t)TotalMemSize << 10) - 1;
  if (Value64 >= ((uint64_t) 0xFFFFFFFF)) {
    DmiInfoTable->T19[0].StartingAddr = 0xFFFFFFFFUL;
    DmiInfoTable->T19[0].EndingAddr = 0xFFFFFFFFUL;
    // In Byte
    DmiInfoTable->T19[0].ExtEndingAddr = Value64 << 10;
  } else {
    // In KByte
    DmiInfoTable->T19[0].EndingAddr = (uint32_t) Value64;
  }

  DmiInfoTable->T19[0].PartitionWidth = NumActiveDimms;

  return Status;
}

/**
 * ConfigureMemInfoPhx
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
ConfigureMemInfoPhx (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummary
  )
{
  SIL_STATUS         Status;
  APOB_TYPE_HEADER   *ApobEntry;
  APOB_IP2IP_API     *ApobIp2IpApi;

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **)(&ApobIp2IpApi));
  if (Status != SilPass) {
    MEM_TRACEPOINT(SIL_TRACE_ERROR, " APOB I2I API is not found.\n");
  } else {
    Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
      APOB_MEM,
      APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE,
      0,
      0,
      &ApobEntry
      );
    if ((Status != SilPass) || (ApobEntry == NULL)) {
      MemSummary->AmdMemoryFrequency = 0;
      MemSummary->DdrMaxRate = 0;
    } else {
      MemSummary->AmdMemoryFrequency = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_PHX *)ApobEntry)->MemClkFreq;
      MemSummary->DdrMaxRate = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_PHX *)ApobEntry)->DdrMaxRate;
    }
  }

  return Status;
}
