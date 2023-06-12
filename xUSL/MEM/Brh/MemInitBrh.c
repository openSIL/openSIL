/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemInitBrh.c
 * @brief OpenSIL MEM I2I and C2R API Function initialization.
 *
 */

#include <Mem.h>
#include <SilCommon.h>
#include <MEM/Common/MemCmn2Rev.h>
#include <MEM/Common/MemSpd5.h>
#include <MEM/MemClass-api.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/BRH/Apob-BRH.h>
#include <string.h>
#include "MemCmn2RevBrh.h"
#include "MemTableBrh.h"

#define SPD_BUFFER_SIZE 1024

/**
 * BuildMemoryInfoData
 *
 * @details    This is a helper function to populate
 *             AMD_MEMORY_SUMMARY structure
 *
 * @param    *MemSummary   - Pointer to AMD_MEMORY_SUMMARY
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
  SIL_STATUS                                          Status;
  uint8_t                                             Socket;
  uint8_t                                             Channel;
  uint8_t                                             ChanNumOnPlatf;  // Channel Number on platform
  uint16_t                                            IndexSocketDie, IndexSocketDieCh;
  uint8_t                                             Dimm;
  uint16_t                                            Instance;
  uint16_t                                            DieLoop;
  APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_BRH *MemCfgInfo;
  uint8_t                                             MemInfoSize;

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
      Status = AmdGetApobEntryInstance(APOB_MEM,
        APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE,
        Instance,
        0,
        (APOB_TYPE_HEADER **)&MemCfgInfo
        );
      if (Status == SilPass) {
        MEM_TRACEPOINT(SIL_TRACE_INFO,
          "Successfully get APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE of Socket %d\n",
          Socket
          );
        IndexSocketDie = Socket * MemSummary->MaxDiePerSocket + DieLoop;
        // DramEcc
        memcpy((void *)&MemSummary->DramEcc[IndexSocketDie], (void *)&MemCfgInfo->DramEcc, MemInfoSize);
        // DramParity


        memcpy((void *)&MemSummary->DramParity[IndexSocketDie], (void *)&MemCfgInfo->DramParity, MemInfoSize);
        // AutoRefFineGranMode
        memcpy((void *)&MemSummary->AutoRefFineGranMode[IndexSocketDie],
          (void *)&MemCfgInfo->AutoRefFineGranMode,
          MemInfoSize
          );

        if (Instance == 0) {
          /// Status report per system
          // MbistTestEnable
          memcpy((void *)&MemSummary->MbistTestEnable, (void *)&MemCfgInfo->MbistTestEnable, MemInfoSize);
          // MbistAggressorEnable
          memcpy((void *)&MemSummary->MbistAggressorEnable, (void *)&MemCfgInfo->MbistAggressorEnable, MemInfoSize);
          // MbistPerBitSlaveDieReport
          memcpy((void *)&MemSummary->MbistPerBitSlaveDieReport,
            (void *)&MemCfgInfo->MbistPerBitSlaveDieReport,
            MemInfoSize
            );
          // DramTempControlledRefreshEn
          memcpy((void *)&MemSummary->DramTempControlledRefreshEn,
            (void *)&MemCfgInfo->DramTempControlledRefreshEn,
            MemInfoSize
            );
          // UserTimingMode
          memcpy((void *)&MemSummary->UserTimingMode, (void *)&MemCfgInfo->UserTimingMode, MemInfoSize);
          // UserTimingValue
          memcpy((void *)&MemSummary->UserTimingValue, (void *)&MemCfgInfo->UserTimingValue, MemInfoSize);
          // MemBusFreqLimit
          memcpy((void *)&MemSummary->MemBusFreqLimit, (void *)&MemCfgInfo->MemBusFreqLimit, MemInfoSize);
          // EnablePowerDown
          memcpy((void *)&MemSummary->EnablePowerDown, (void *)&MemCfgInfo->EnablePowerDown, MemInfoSize);
          // DramDoubleRefreshRate
          memcpy((void *)&MemSummary->DramDoubleRefreshRate, (void *)&MemCfgInfo->DramDoubleRefreshRate, MemInfoSize);
          // PmuTrainMode
          memcpy((void *)&MemSummary->PmuTrainMode, (void *)&MemCfgInfo->PmuTrainMode, MemInfoSize);
          // EccSymbolSize
          memcpy((void *)&MemSummary->EccSymbolSize, (void *)&MemCfgInfo->EccSymbolSize, MemInfoSize);
          // UEccRetry
          memcpy((void *)&MemSummary->UEccRetry, (void *)&MemCfgInfo->UEccRetry, MemInfoSize);
          // IgnoreSpdChecksum
          memcpy((void *)&MemSummary->IgnoreSpdChecksum, (void *)&MemCfgInfo->IgnoreSpdChecksum, MemInfoSize);
          // EnableBankGroupSwapAlt
          memcpy((void *)&MemSummary->EnableBankGroupSwapAlt,
            (void *)&MemCfgInfo->EnableBankGroupSwapAlt,
            MemInfoSize
            );
          // EnableBankGroupSwap
          memcpy((void *)&MemSummary->EnableBankGroupSwap, (void *)&MemCfgInfo->EnableBankGroupSwap, MemInfoSize);
          // DdrRouteBalancedTee
          memcpy((void *)&MemSummary->DdrRouteBalancedTee, (void *)&MemCfgInfo->DdrRouteBalancedTee, MemInfoSize);
          // NvdimmPowerSource
          memcpy((void *)&MemSummary->NvdimmPowerSource, (void *)&MemCfgInfo->NvdimmPowerSource, MemInfoSize);
          // OdtsCmdThrotEn
          memcpy((void *)&MemSummary->OdtsCmdThrotEn, (void *)&MemCfgInfo->OdtsCmdThrotEn, MemInfoSize);
          // OdtsCmdThrotCyc
          memcpy((void *)&MemSummary->OdtsCmdThrotCyc, (void *)&MemCfgInfo->OdtsCmdThrotCyc, MemInfoSize);
        }

        for (Channel = 0; Channel < MemSummary->MaxChannelPerDie; Channel++) {
          /*
           * In the following code an UMC channel number is converted into Channel Number on platform
           * e.g.
           *  Channel A (Channel 0) - UMC 0
           *  Channel B (Channel 1) - UMC 1
           *
           * For specific socket type, please refer to Processor PPR documentation
           *
           * If MemChanXLatTab is NULL, then no address translation required.
           */
          ChanNumOnPlatf = Channel; // Start initially with UMC and channel on platform as same

          TranslateChannelInfo(Channel, &ChanNumOnPlatf);


          IndexSocketDieCh = Socket * MemSummary->MaxDiePerSocket * MemSummary->MaxChannelPerDie +
            DieLoop * MemSummary->MaxChannelPerDie + ChanNumOnPlatf;
          // ChipselIntlv
          memcpy((void *)&MemSummary->ChipselIntlv[IndexSocketDieCh],
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

  DumpMemoryInfoData(MemSummary);
  return SilPass; // Return success regardless the value of Status. It may be set to error in case when not
                  // all CPUs are populated.
}

HOST_TO_APCB_CHANNEL_XLAT *
GetChannelXlatTableBrh (void)
{
  return (HOST_TO_APCB_CHANNEL_XLAT *) gSp5ChannelXlatTable;
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
  SPD_BASE_CONFIG_0_S *BaseConfig0;
  assert(SpdData != NULL);

  BaseConfig0 = (SPD_BASE_CONFIG_0_S *) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  return (BaseConfig0->KeyByte2.Field.Hybrid == SpdHybrid);
}

/**
 *  GetDimmOperatingModeCap
 *
 *  @brief Get the operating mode capability of NVDIMM
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
 * GetNvDimmFirmwareVersion
 *
 * @brief   GetNvDimmFirmwareVersion
 *
 * @details Get the Firmware Version of NVDIMM Subsystem Controller
 *
 * @param   *SpdData - Pointer to the SPD data array
 * @param   *FirmwareVersion - Firmware version
 *
 * @returns void
 */
static
void
GetNvDimmFirmwareVersion (
  uint8_t  *SpdData,
  char     *FirmwareVersion
  )
{
  assert(SpdData != NULL);
  assert(FirmwareVersion != NULL);

  FirmwareVersion[0] = '\0';
}

/**
 * GetNvDimmModuleProductId
 *
 * @brief    GetNvDimmModuleProductId
 *
 * @details  Gets the module product ID of NVDIMM
 *
 * @param    *SpdData - Pointer to the SPD data array
 *
 * @returns   uint16_t - The Module Product Identifier
 */
static
uint16_t
GetNvDimmModuleProductId (
  uint8_t  *SpdData
  )
{
  assert(SpdData != NULL);

  return 0;
}

/**
 * GetNvDimmSubsystemControllerId
 *
 * @brief    GetNvDimmSubsystemControllerId
 *
 * @details  Gets the ID of NVDIMM Subsystem Controller
 *
 * @param    *SpdData - Pointer to the SPD data array
 *
 * @returns  uint16_t - The Identifier of NVDIMM Subsystem Controller
 */
static
uint16_t
GetNvDimmSubsystemControllerId (
  uint8_t *SpdData
  )
{
  assert(SpdData != NULL);

  return 0;
}

/**
 * GetDimmTypePhx
 *
 * @brief    Gets the type of NVDIMM
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
  SPD_BASE_CONFIG_0_S *BaseConfig0;

  assert(SpdData != NULL);
  BaseConfig0 = (SPD_BASE_CONFIG_0_S *) &(SpdData[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm(SpdData)) {
    switch (BaseConfig0->KeyByte2.Field.HybridMedia) {
    case SpdHybridMediaNvdimmN:
      return SilNvDimmNType;
    case SpdHybridMediaNvdimmP:
      return SilNvDimmPType;
    }
  }
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
  SPD_MANUFACTURING_INFO_S *MfgInfo;

  assert(SpdData != NULL);
  MfgInfo = (SPD_MANUFACTURING_INFO_S *)&(SpdData[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
  return MfgInfo->ModuleMfgId.Value;
}


/**
 * GetNvDimmSubsystemControllerManufacturerId
 *
 * @brief Gets the manufacturer ID of NVDIMM Subsystem Controller
 *
 * @param  *SpdData - Pointer to the SPD data array
 *
 * @returns uint16_t - The Manufacturer Identifier of NVDIMM Subsystem Controller
 */
static
uint16_t
GetNvDimmSubsystemControllerManufacturerId (
  uint8_t                     *SpdData
  )
{
  SPD_NVDIMM_N_ANNEX_S   *ModuleParms;

  assert(SpdData != NULL);
  ModuleParms = (SPD_NVDIMM_N_ANNEX_S *)&(SpdData[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
  if (IsNvHybridDimm(SpdData)) {
    return ModuleParms->DbMfgId.Value;
  } else {
    return 0;
  }
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
 *  GetLogicalDimmInfoBrh
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
bool
GetLogicalDimmInfoBrh (
  SIL_TYPE20_DMI_INFO              *T20,
  APOB_MEM_DMI_LOGICAL_DIMM_BRH    *LogicalDimm
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
 * ConfigureTable17DimmPresentBrh
 *
 * @brief   Configures the T17 of the SMBIOS Table when the DIMM is present
 *
 * @param   *SpdData - Pointer to the SPD data array
 * @param   *T17     - Pointer to Table 17
 *
 * @returns void
 */
void
ConfigureTable17DimmPresentBrh (
  uint8_t                *SpdData,
  SIL_TYPE17_DMI_INFO    *T17
  )
{
  uint64_t MemorySize;

  T17->MemoryTechnology = GetDimmType(SpdData);
  T17->MemoryOperatingModeCapability.AsUint16 = GetDimmOperatingModeCap(SpdData);
  GetNvDimmFirmwareVersion(SpdData, &T17->FirmwareVersion[0]);
  T17->ModuleManufacturerId = GetDimmModuleManufacturerId(SpdData);
  T17->ModuleProductId = GetNvDimmModuleProductId(SpdData);
  T17->MemorySubsystemControllerManufacturerId = GetNvDimmSubsystemControllerManufacturerId(SpdData);
  T17->MemorySubsystemControllerProductId = GetNvDimmSubsystemControllerId(SpdData);
  if (0x7FFF == T17->MemorySize) {
    MemorySize = T17->ExtSize;
  } else {
    MemorySize = T17->MemorySize;
  }
  if (IsNvHybridDimm(SpdData)) {
    T17->NonvolatileSize = MemorySize * 0x100000;// Convert Mega size to Byte size
    T17->VolatileSize = 0;
    T17->CacheSize = 0;
    T17->LogicalSize = 0;
  } else {
    T17->NonvolatileSize = 0;
    T17->VolatileSize = MemorySize * 0x100000;// Convert Mega size to Byte size
    T17->CacheSize = 0;
    T17->LogicalSize = 0;
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
bool
GetPhysicalDimmInfoD5 (
  SIL_TYPE17_DMI_INFO               *T17,
  APOB_MEM_DMI_PHYSICAL_DIMM_BRH    *PhysicalDimm,
  uint8_t                           TranslatedChannel
  )
{
  uint16_t                      i;
  uint8_t                       Socket;
  uint8_t                       Channel;
  uint8_t                       Dimm;
  uint8_t                       DimmSpd[SPD_BUFFER_SIZE];
  SIL_STATUS                    SilStatus;
  uint8_t                       IoWidth;
  uint16_t                      BusWidth;
  uint16_t                      TckPs;
  uint32_t                      MemorySize;
  uint32_t                      FreqTableIndex;
  SIL_STATUS                    ApobStatus;
  uint16_t                      Instance;
  uint16_t                      DieLoop;
  uint8_t                       NumRanks;
  uint8_t                       Rank;
  uint8_t                       SpdCapacity;
  bool                          Asymmetric;
  SPD_BASE_CONFIG_0_S           *BaseConfig0;
  SPD_ANNEX_COMMON_S            *ModuleParms;
  SPD_MANUFACTURING_INFO_S      *MfgInfo;
  uint8_t                       ChannelsPerDimm;
  uint8_t                       DiePerPkg;
  APOB_IP2IP_API                *ApobIp2IpApi;
  APOB_SOC_DIE_INFO             SocMaxDieInfo;
  SIL_STATUS                    Status;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if (Status != SilPass) {
    MEM_TRACEPOINT(SIL_TRACE_INFO, "Error : not found APOB IP2IP API\n");
    assert(false);
    return 0;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

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
    SilStatus = SilUnsupported;
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
      ApobStatus = ApobIp2IpApi->ApobGetDimmSpdData(Instance, Socket, Channel, Dimm, SPD_BUFFER_SIZE, DimmSpd);
      if (ApobStatus == SilPass) {
        SilStatus = SilPass;
        break;
      }
    }
    if (SilStatus != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_INFO,
        "Error on get Spd Data from APOB for Socket %d, Channel %d (Slot %d), Dimm %d\n",
        Socket,
        Channel,
        TranslatedChannel,
        Dimm
        )
      ;
      assert(false);
    }

    assert(SilPass == SilStatus);
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
      T17->DataWidth = BusWidth * 2;  // RDIMM data width = 2 x data width per sub-channel
      T17->TotalWidth = T17->DataWidth +
        (SPD_CHANNEL_BUS_WIDTH_EXT_DECODE(ModuleParms->ChBusWidth.Field.WidthExt) * 2);
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
    default:
      T17->MemoryType = SilDdr5MemType;
      T17->MinimumVoltage = 1100;
      T17->MaximumVoltage = 1100;
      break;
    }
    TckPs = BaseConfig0->TckAvgMin.Value;
    for (FreqTableIndex = 0;
      FreqTableIndex < sizeof (gMemFreqToTckTable) / sizeof (gMemFreqToTckTable[0]); FreqTableIndex++) {
      if (gMemFreqToTckTable[FreqTableIndex].TckPs <= TckPs) {
        T17->Speed = gMemFreqToTckTable[FreqTableIndex].Memclk;
        break;
      }
    }

    T17->ManufacturerIdCode = MfgInfo->ModuleMfgId.Value;
    IntToString((char *)T17->SerialNumber,
      (uint8_t *)MfgInfo->ModuleSerialNumber.String,
      SPD_MODULE_SERIAL_NUMBER_LEN
      );

    for (i = 0; i < sizeof (T17->PartNumber) - 1; i++) {
      T17->PartNumber[i] = MfgInfo->ModulePartNumber.String[i];
    }
    T17->PartNumber[i] = 0;

    T17->Attributes = NumRanks;

    T17->ConfigSpeed = PhysicalDimm->ConfigSpeed;
    T17->ConfiguredVoltage = PhysicalDimm->ConfigVoltage;

  } else {
    MEM_TRACEPOINT(SIL_TRACE_INFO,
      "Dummy Type 17 Created for Socket %d, Channel %d, (Slot %d), Dimm %d\n",
      Socket,
      Channel,
      TranslatedChannel,
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

  SilInitSmbios32Type17(PhysicalDimm->DimmPresent, DimmSpd, T17);

  if (PhysicalDimm->DimmPresent) {
    return true;
  } else {
    return false;
  }
}

/**
 * PopulateSmbiosMemInfoBrh
 *
 * @details    This routine gets DMI Type 16, Type 17,
 *             Type 19 and Type 20 related information.
 *
 * @param      DmiInfoTable - DMI records
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
PopulateSmbiosMemInfoBrh (
  SIL_DMI_INFO                 *DmiInfoTable
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
  APOB_MEM_DMI_PHYSICAL_DIMM_BRH   *PhysicalDimm;
  APOB_MEM_DMI_LOGICAL_DIMM_BRH    *LogicalDimm;
  APOB_TYPE_HEADER                 *ApobSmbiosInfo;
  APOB_IP2IP_API                   *ApobIp2IpApi;

  MEM_TRACEPOINT(SIL_TRACE_INFO, "\tDMI enabled\n");

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_SMBIOS, APOB_MEM_SMBIOS_TYPE, 0, 0, &ApobSmbiosInfo);
  assert(ApobSmbiosInfo != NULL);
  ApobMemDmiHeader = (APOB_MEM_DMI_HEADER *)ApobSmbiosInfo;
  MaxPhysicalDimms = ApobMemDmiHeader->MaxPhysicalDimms;
  MaxLogicalDimms = ApobMemDmiHeader->MaxLogicalDimms;
  TotalMemSize = 0;
  PhysicalDimm = (APOB_MEM_DMI_PHYSICAL_DIMM_BRH *)&ApobMemDmiHeader[1];

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
    TranslateChannelInfo(PhysicalDimm->Channel, &TranslatedChannel);
    Dimm = PhysicalDimm->Dimm;
    if (GetPhysicalDimmInfoD5(&DmiInfoTable->T17[Socket][TranslatedChannel][Dimm], PhysicalDimm, TranslatedChannel)) {
      NumActiveDimms++;
    }
    TotalMemSize += (DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].MemorySize != 0x7FFF) ?
      DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].MemorySize :
      DmiInfoTable->T17[Socket][TranslatedChannel][Dimm].ExtSize;
  }

  // Pointer to DMI info of Logical DIMMs
  LogicalDimm = (APOB_MEM_DMI_LOGICAL_DIMM_BRH *)PhysicalDimm;

  // TYPE 20 entries are organized by logical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxLogicalDimms; DimmIndex++, LogicalDimm++) {
    Socket = LogicalDimm->Socket;
    TranslateChannelInfo(LogicalDimm->Channel, &Channel);
    if (Channel != 0xFF) {
      Dimm = LogicalDimm->Dimm;
      GetLogicalDimmInfoBrh(&DmiInfoTable->T20[Socket][Channel][Dimm][0], LogicalDimm);
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
ConfigureMemInfoBrh (
  AMD_MEMORY_SUMMARY *MemSummary
  )
{
  SIL_STATUS         Status;
  uint8_t            Socket;
  uint32_t           Instance;
  uint16_t           P1MemPORMaxSpeed;
  APOB_TYPE_HEADER   *ApobEntry;
  APOB_IP2IP_API     *ApobIp2IpApi;

  MEM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    Status = SilNotFound;
  } else {
    Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_MEM,
      APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE,
      0,
      0,
      &ApobEntry
      );

    if ((Status != SilPass) || (ApobEntry == NULL)) {
      MemSummary->AmdMemoryFrequency = 0;
      MemSummary->DdrMaxRate = 0;
      //Fill in max. number of Socket/Die/Channel/Dimm
      MemSummary->MaxSocketSupported = 0;
      MemSummary->MaxDiePerSocket = 0;
      MemSummary->MaxChannelPerDie = 0;
      MemSummary->MaxDimmPerChannel = 0;
      MemSummary->MemPORMaxSpeed = 0;
    } else {
      MemSummary->AmdMemoryFrequency = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_BRH *)ApobEntry)->MemClkFreq;
      MemSummary->DdrMaxRate = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_BRH *)ApobEntry)->DdrMaxRate;
      MemSummary->MemPORMaxSpeed = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_BRH *)ApobEntry)->MemPORMaxSpeed;
      // Get socket 1 APOB APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT
      Socket = 1;
      Instance = (uint32_t) Socket << 8;
      ApobEntry = NULL;

      Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_MEM,
        APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE,
        Instance,
        0,
        &ApobEntry
        );
      if ((Status != SilPass) || (ApobEntry == NULL)) {
        // Socket 1 does not exist, use data of socket 0
      } else {
        // Socket 1 present, fill MemPORMaxSpeed with lower speed of both sockets
        P1MemPORMaxSpeed = ((APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT_BRH *) ApobEntry)->MemPORMaxSpeed;
        if (MemSummary->MemPORMaxSpeed > P1MemPORMaxSpeed) {
          MemSummary->MemPORMaxSpeed = P1MemPORMaxSpeed;
        }
      }
    }

    /*
     * Build memory config. info data.
     */
    Status = BuildMemoryInfoData(MemSummary);
    if (Status != SilPass) {
      MEM_TRACEPOINT(SIL_TRACE_ERROR, "xPrfGetMemInfo: Build AMD_MEMORY_SUMMARY Failed!\n");
    }
  }

  MEM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}
