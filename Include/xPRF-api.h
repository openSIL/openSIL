/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPRF-api.h
 * @brief This file is used to declare the API functions used by the Host for
 *        initialization of platform specific content.
 *
 * @details This file is used to declare the API functions, variables and
 * macros needed by the Host to perform the platform initialization
 * of the AMD silicon.
 */
/**
 * @cond API_Doc
 * @page TopXprf   Platform Reference Firmware (xPRF)
 *
 *   'xPRF' is the AMD x86 Platform Configuration Module.
 *   This module is responsible for providing features to the Host
 *   for selecting and configuring conditions specific to the platform.
 *   While xSIM establishes base operation of the silicon, xPRF
 *   'fine tunes' the silicon capabilities to the needs of the platform.
 *
 *   xPRF is a collection of service routines used to accomplish a goal,
 *   directed by the Host.
 *
 * See the 'Files - @ref xprf-api.h' section of this document for
 * further details.
 *
 * @endcond
 */
#pragma once

#include <stddef.h>
#include <stdint.h>   // needed for declarations later in this file
#include <stdbool.h>
#include <CCX/CcxClass-api.h>
#include <Nbio/NbioClass-api.h>
#include <MEM/MemClass-api.h>
#include <DF/DfClass-api.h>

/// DMI Type 16 offset 04h - Location
typedef enum {
  SilOtherLocation = 0x01,                                 ///< Assign 01 to Other
  SilUnknownLocation,                                      ///< Assign 02 to Unknown
  SilSystemboardOrMotherboard,                             ///< Assign 03 to systemboard or motherboard
  SilIsaAddonCard,                                         ///< Assign 04 to ISA add-on card
  SilEisaAddonCard,                                        ///< Assign 05 to EISA add-on card
  SilPciAddonCard,                                         ///< Assign 06 to PCI add-on card
  SilMcaAddonCard,                                         ///< Assign 07 to MCA add-on card
  SilPcmciaAddonCard,                                      ///< Assign 08 to PCMCIA add-on card
  SilProprietaryAddonCard,                                 ///< Assign 09 to proprietary add-on card
  SilNuBus,                                                ///< Assign 0A to NuBus
  SilPc98C20AddonCard,                                     ///< Assign 0A0 to PC-98/C20 add-on card
  SilPc98C24AddonCard,                                     ///< Assign 0A1 to PC-98/C24 add-on card
  SilPc98EAddoncard,                                       ///< Assign 0A2 to PC-98/E add-on card
  SilPc98LocalBusAddonCard                                 ///< Assign 0A3 to PC-98/Local bus add-on card
} SIL_DMI_T16_LOCATION;

/// DMI Type 16 offset 05h - Memory Error Correction
typedef enum {
  SilOtherUse = 0x01,                                      /// Assign 01 to Other
  SilUnknownUse,                                           /// Assign 02 to Unknown
  SilSystemMemory,                                         /// Assign 03 to system memory
  SilVideoMemory,                                          /// Assign 04 to video memory
  SilFlashMemory,                                          /// Assign 05 to flash memory
  SilNonvolatileRam,                                       /// Assign 06 to non-volatile RAM
  SilCacheMemory                                           /// Assign 07 to cache memory
} SIL_DMI_T16_USE;

/// DMI Type 16 offset 07h - Maximum Capacity
typedef enum {
  SilDmi16OtherErrCorrection = 0x01,                       /// Assign 01 to Other
  SilDmi16UnknownErrCorrection,                            /// Assign 02 to Unknown
  SilDmi16NoneErrCorrection,                               /// Assign 03 to None
  SilDmi16Parity,                                          /// Assign 04 to parity
  SilDmi16SingleBitEcc,                                    /// Assign 05 to Single-bit ECC
  SilDmi16MultiBitEcc,                                     /// Assign 06 to Multi-bit ECC
  SilDmi16Crc                                              /// Assign 07 to CRC
} SIL_DMI_T16_ERROR_CORRECTION;

/// DMI Type 16 - Physical Memory Array
typedef struct {
  SIL_DMI_T16_LOCATION          Location;                 ///< The physical location of the Memory Array,
  ///< whether on the system board or an add-in board.
  SIL_DMI_T16_USE               Use;                      ///< Identifies the function for which the array
  ///< is used.
  SIL_DMI_T16_ERROR_CORRECTION  MemoryErrorCorrection;    ///< The primary hardware error correction or
  ///< detection method supported by this memory array.
  uint16_t                  NumberOfMemoryDevices;    ///< The number of slots or sockets available
                                                      ///< for memory devices in this array.
} SIL_TYPE16_DMI_INFO;

/// DMI Type 17 offset 0Eh - Form Factor
typedef enum {
  SilOtherFormFactor = 0x01,                               /// Assign 01 to Other
  SilUnknowFormFactor,                                     /// Assign 02 to Unknown
  SilSimmFormFactor,                                       /// Assign 03 to SIMM
  SilSipFormFactor,                                        /// Assign 04 to SIP
  SilChipFormFactor,                                       /// Assign 05 to Chip
  SilDipFormFactor,                                        /// Assign 06 to DIP
  SilZipFormFactor,                                        /// Assign 07 to ZIP
  SilProprietaryCardFormFactor,                            /// Assign 08 to Proprietary Card
  SilDimmFormFactorFormFactor,                             /// Assign 09 to DIMM
  SilTsopFormFactor,                                       /// Assign 10 to TSOP
  SilRowOfChipsFormFactor,                                 /// Assign 11 to Row of chips
  SilRimmFormFactor,                                       /// Assign 12 to RIMM
  SilSodimmFormFactor,                                     /// Assign 13 to SODIMM
  SilSrimmFormFactor,                                      /// Assign 14 to SRIMM
  SilFbDimmFormFactor                                      /// Assign 15 to FB-DIMM
} SIL_DMI_T17_FORM_FACTOR;

/// DMI Type 17 offset 12h - Memory Type
typedef enum {
  SilOtherMemType = 0x01,                                  ///< Assign 01 to Other
  SilUnknownMemType,                                       ///< Assign 02 to Unknown
  SilDramMemType,                                          ///< Assign 03 to DRAM
  SilEdramMemType,                                         ///< Assign 04 to EDRAM
  SilVramMemType,                                          ///< Assign 05 to VRAM
  SilSramMemType,                                          ///< Assign 06 to SRAM
  SilRamMemType,                                           ///< Assign 07 to RAM
  SilRomMemType,                                           ///< Assign 08 to ROM
  SilFlashMemType,                                         ///< Assign 09 to Flash
  SilEepromMemType,                                        ///< Assign 10 to EEPROM
  SilFepromMemType,                                        ///< Assign 11 to FEPROM
  SilEpromMemType,                                         ///< Assign 12 to EPROM
  SilCdramMemType,                                         ///< Assign 13 to CDRAM
  SilThreeDramMemType,                                     ///< Assign 14 to 3DRAM
  SilSdramMemType,                                         ///< Assign 15 to SDRAM
  SilSgramMemType,                                         ///< Assign 16 to SGRAM
  SilRdramMemType,                                         ///< Assign 17 to RDRAM
  SilDdrMemType,                                           ///< Assign 18 to DDR
  SilDdr2MemType,                                          ///< Assign 19 to DDR2
  SilDdr2FbdimmMemType,                                    ///< Assign 20 to DDR2 FB-DIMM
  SilDdr3MemType = 0x18,                                   ///< Assign 24 to DDR3
  SilFbd2MemType,                                          ///< Assign 25 to FBD2
  SilDdr4MemType,                                          ///< Assign 26 to DDR4
  SilLpDdrMemType,                                         ///< Assign 27 to LPDDR
  SilLpDdr2MemType,                                        ///< Assign 28 to LPDDR2
  SilLpDdr3MemType,                                        ///< Assign 29 to LPDDR3
  SilLpDdr4MemType,                                        ///< Assign 30 to LPDDR4
  SilDdr5MemType = 0x22,                                   ///< Assign 34 to DDR5
  SilLpDdr5MemType,
  SilLpDdr5xMemType,
} SIL_DMI_T17_MEMORY_TYPE;

/// DMI Type 17 offset 13h - Type Detail
typedef struct {
  uint16_t                    Reserved1:1;            ///< Reserved
  uint16_t                    Other:1;                ///< Other
  uint16_t                    Unknown:1;              ///< Unknown
  uint16_t                    FastPaged:1;            ///< Fast-Paged
  uint16_t                    StaticColumn:1;         ///< Static column
  uint16_t                    PseudoStatic:1;         ///< Pseudo-static
  uint16_t                    Rambus:1;               ///< RAMBUS
  uint16_t                    Synchronous:1;          ///< Synchronous
  uint16_t                    Cmos:1;                 ///< CMOS
  uint16_t                    Edo:1;                  ///< EDO
  uint16_t                    WindowDram:1;           ///< Window DRAM
  uint16_t                    CacheDram:1;            ///< Cache Dram
  uint16_t                    NonVolatile:1;          ///< Non-volatile
  uint16_t                    Registered:1;           ///< Registered (Buffered)
  uint16_t                    Unbuffered:1;           ///< Unbuffered (Unregistered)
  uint16_t                    LRDIMM:1;               ///< LRDIMM
} SIL_DMI_T17_TYPE_DETAIL;

/// DMI Type 17 offset 28h - Memory Technology
typedef enum {
  SilOtherType = 0x01,                                     ///< Assign 01 to Other
  SilUnknownType = 0x02,                                   ///< Assign 02 to Unknown
  SilDramType = 0x03,                                      ///< Assign 03 to DRAM
  SilIntelPersistentMemoryType = 0x07,                     ///< Assign 07 to Intel persistent memory
} SIL_DMI_T17_MEMORY_TECHNOLOGY;

/// DMI Type 17 offset 29h - Memory Operating Mode Capability
typedef struct {
  uint16_t                    Reserved1:1;                       ///< Reserved, set to 0
  uint16_t                    Other:1;                           ///< Other
  uint16_t                    Unknown:1;                         ///< Unknown
  uint16_t                    VolatileMemory:1;                  ///< Volatile memory
  uint16_t                    ByteAccessiblePersistentMemory:1;  ///< Byte-accessible persistent memory
  uint16_t                    BlockAccessiblePersistentMemory:1; ///< Block-accessible persistent memory
  uint16_t                    Reserved2:10;                      ///< Reserved, set to 0
} SIL_DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY;

typedef union {
  SIL_DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY  AsBitmap;
  uint16_t                                    AsUint16;
} SIL_DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY_VAR;

#pragma pack (push, 1)
/// DMI Type 17 - Memory Device
typedef struct {
  uint16_t                    Handle;             ///< The temporary handle, or instance number, associated with the structure
  uint16_t                    TotalWidth;         ///< Total Width, in bits, of this memory device,
                                                  ///< including any check or error-correction bits.
  uint16_t                    DataWidth;          ///< Data Width, in bits, of this memory device.
  uint16_t                    MemorySize;         ///< The size of the memory device.
  SIL_DMI_T17_FORM_FACTOR       FormFactor;       ///< The implementation form factor for this memory device.
  uint8_t                     DeviceSet;          ///< Identifies when the Memory Device is one of a set of
                                                  ///< Memory Devices that must be populated with all devices of
                                                  ///<  the same type and size, and the set to which this device
                                                  ///<  belongs.
  char                     DeviceLocator[8];      ///< The string number of the string that identifies the physically
                                                  ///< labeled socket or board position where the memory device is
                                                  ///< located.
  char                     BankLocator[13];       ///< The string number of the string that identifies the physically
                                                  ///< labeled bank where the memory device is located.
  SIL_DMI_T17_MEMORY_TYPE       MemoryType;       ///< The type of memory used in this device.
  SIL_DMI_T17_TYPE_DETAIL       TypeDetail;       ///< Additional detail on the memory device type
  uint16_t                    Speed;              ///< Identifies the speed of the device, in megahertz (MHz).
  uint64_t                    ManufacturerIdCode; ///< Manufacturer ID code.
  char                     SerialNumber[9];       ///< Serial Number.
  char                     PartNumber[21];        ///< Part Number.
  uint8_t                     Attributes;         ///< Bits 7-4: Reserved, Bits 3-0: rank.
  uint32_t                    ExtSize;            ///< Extended Size.
  uint16_t                    ConfigSpeed;        ///< Configured memory clock speed
  uint16_t                    MinimumVoltage;     ///< Minimum operating voltage for this device, in millivolts
  uint16_t                    MaximumVoltage;     ///< Maximum operating voltage for this device, in millivolts
  uint16_t                    ConfiguredVoltage;  ///< Configured voltage for this device, in millivolts
  // SMBIOS 3.2
  uint8_t                     MemoryTechnology;                                     ///< Memory technology type for this memory device
  SIL_DMI_T17_MEMORY_OPERATING_MODE_CAPABILITY_VAR   MemoryOperatingModeCapability; ///< The operating modes
  ///< supported by this memory device
  char                     FirmwareVersion[10];                        ///< String number for the firmware version of this memory device
  uint16_t                    ModuleManufacturerId;                    ///< The two-byte module manufacturer ID found in the SPD
                                                                       ///< of this memory device; LSB first.
  uint16_t                    ModuleProductId;                         ///< The two-byte module product ID found in the SPD of this
                                                                       ///< memory device; LSB first
  uint16_t                    MemorySubsystemControllerManufacturerId; ///< The two-byte memory subsystem controller
                                                                       ///< manufacturer ID found in the SPD of this
                                                                       ///< memory device; LSB first
  uint16_t                    MemorySubsystemControllerProductId;      ///< The 2-byte memory subsystem controller productID
  uint64_t                    NonvolatileSize;                         ///< Size of the Non-volatile portion of the
  uint64_t                    VolatileSize;                            ///< Size of the Volatile portion of the memory
  uint64_t                    CacheSize;                               ///< Size of the Cache portion of the memory device in Bytes
  uint64_t                    LogicalSize;                             ///< Size of the Logical memory device in Bytes.
  // SMBIOS 3.3
  uint32_t                    ExtendedSpeed;                 ///< Extended Speed
  uint32_t                    ExtendedConfiguredMemorySpeed; ///< Extended Configured memory speed
} SIL_TYPE17_DMI_INFO;
#pragma pack (pop)

/// DMI Type 19 - Memory Array Mapped Address
typedef struct {
  uint32_t                    StartingAddr;      ///< The physical address, in kilobytes,
                                                 ///< of a range of memory mapped to the
                                                 ///< specified physical memory array.
  uint32_t                    EndingAddr;        ///< The physical ending address of the
                                                 ///< last kilobyte of a range of addresses
                                                 ///< mapped to the specified physical memory array.
  uint16_t                    MemoryArrayHandle; ///< The handle, or instance number, associated
                                                 ///< with the physical memory array to which this
                                                 ///< address range is mapped.
  uint8_t                     PartitionWidth;    ///< Identifies the number of memory devices that
                                                 ///< form a single row of memory for the address
                                                 ///< partition defined by this structure.
  uint64_t                    ExtStartingAddr;   ///< The physical address, in bytes, of a range of
                                                 ///< memory mapped to the specified Physical Memory Array.
  uint64_t                    ExtEndingAddr;     ///< The physical address, in bytes, of a range of
                                                 ///< memory mapped to the specified Physical Memory Array.
} SIL_TYPE19_DMI_INFO;

///DMI Type 20 - Memory Device Mapped Address
typedef struct {
  uint32_t                    StartingAddr;                   ///< The physical address, in kilobytes, of a range
                                                              ///< of memory mapped to the referenced Memory Device.
  uint32_t                    EndingAddr;                     ///< The handle, or instance number, associated with
                                                              ///< the Memory Device structure to which this address
                                                              ///< range is mapped.
  uint16_t                    MemoryDeviceHandle;             ///< The handle, or instance number, associated with
                                                              ///< the Memory Device structure to which this address
                                                              ///< range is mapped.
  uint16_t                    MemoryArrayMappedAddressHandle; ///< The handle, or instance number, associated
                                                              ///< with the Memory Array Mapped Address structure to
                                                              ///< which this device address range is mapped.
  uint8_t                     PartitionRowPosition;           ///< Identifies the position of the referenced Memory
                                                              ///< Device in a row of the address partition.
  uint8_t                     InterleavePosition;             ///< The position of the referenced Memory Device in
                                                              ///< an interleave.
  uint8_t                     InterleavedDataDepth;           ///< The maximum number of consecutive rows from the
                                                              ///< referenced Memory Device that are accessed in a
                                                              ///< single interleaved transfer.
  uint64_t                    ExtStartingAddr;                ///< The physical address, in bytes, of a range of
                                                              ///< memory mapped to the referenced Memory Device.
  uint64_t                    ExtEndingAddr;                  ///< The physical ending address, in bytes, of the last of
                                                              ///< a range of addresses mapped to the referenced Memory Device.
} SIL_TYPE20_DMI_INFO;

/**
 * @brief MAXIMUM VALUES
 *
 * These Max values are used to define array sizes and associated loop
 * counts in the code.
 */
#define SIL_MAX_SOCKETS_SUPPORTED     1    /// Max number of sockets supported
#define SIL_MAX_CHANNELS_PER_SOCKET   4    /// Max Channels per sockets supported
#define SIL_MAX_DIMMS_PER_CHANNEL     2    /// Max DIMMs on a memory channel supported
#define SIL_MAX_T19_REGION_SUPPORTED  3    /// Max SMBIOS T19 Memory Region count

/// Collection of pointers to the DMI records
typedef struct {
  SIL_TYPE16_DMI_INFO  T16;
  SIL_TYPE17_DMI_INFO  T17[SIL_MAX_SOCKETS_SUPPORTED][SIL_MAX_CHANNELS_PER_SOCKET][SIL_MAX_DIMMS_PER_CHANNEL];
  SIL_TYPE19_DMI_INFO  T19[SIL_MAX_T19_REGION_SUPPORTED];
  SIL_TYPE20_DMI_INFO  T20[SIL_MAX_SOCKETS_SUPPORTED][SIL_MAX_CHANNELS_PER_SOCKET][SIL_MAX_DIMMS_PER_CHANNEL];
} SIL_DMI_INFO;


/*********************************************************************
 * API Function prototypes
 *********************************************************************/
SIL_STATUS DummyXprfFunction (void);

/**
 * xPrfSetSnpRmp
 *
 * @brief   Program Snp Rmp table MSRs
 *
 * @details This function will program the SNP RMP table base and size MSRs
 *          with the base and size provided by Host-FW.
 *
 * @param   SilContext      Input buffer for the Sil Context structure.
 *                          The Host is responsible for initializing
 *                          the data in the SIL_CONTEXT structure.
 * @param   SnpRmpTableBase Base address of the SNP RMP buffer allocated by
 *                          host firmware
 * @param   SnpRmpTableSize The size of the SNP RMP buffer allocated by host
 *                          firmware.
 *
 */
void
xPrfSetSnpRmp (
  SIL_CONTEXT   *SilContext,
  uint64_t      SnpRmpTableBase,
  uint64_t      SnpRmpTableSize
  );

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
 *  @param   SilContext           Input buffer for the Sil Context structure.
 *                                The Host is responsible for initializing
 *                                the data in the SIL_CONTEXT structure.
 *  @param   RequestedChannelId   The requested channel Id
 *  @param   *TranslatedChannelId Pointer to the translated Id
 *
 *  @return  SIL_STATUS
 */
SIL_STATUS
xPrfTranslateChannelInfo (
  SIL_CONTEXT                *SilContext,
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  );

/**
 * xPrfGetSystemMemoryMap
 * @details Get top of memory (Tom2) for the Host along with
 *          memory map, and number of holes
 *
 * @param   SilContext         Input buffer for the Sil Context structure.
 *                             The Host is responsible for initializing
 *                             the data in the SIL_CONTEXT structure.
 * @param   NumberOfHoles      Number of memory holes
 * @param   TopOfSystemMemory  Top of memory address
 * @param   MemHoleDescPtr     Memory descriptor structure
 *
 * @retval  SilPass                Info extracted successfully.
 * @retval  SilInvalidParameter    Failure.
 **/
SIL_STATUS
xPrfGetSystemMemoryMap (
  SIL_CONTEXT             *SilContext,
  uint32_t                *NumberOfHoles,
  uint64_t                *TopOfSystemMemory,
  void                    **MemHoleDescPtr
  );

/**
 * xPrfGetLowUsableDramAddress
 *
 * @details Get top of low(<4Gb) usable DRAM accounting for pre-assigned
 *          buffers for system use.
 *          This is used by the host firmware to locate low usable
 *          DRAM top. The ApobBaseAddress is optional if set in kconfig
 *          properly.
 *
 * @param   SilContext        Input buffer for the Sil Context structure.
 *                            The Host is responsible for initializing
 *                            the data in the SIL_CONTEXT structure.
 *
 * @retval  uint32_t   Top address of available DRAM region below 4GB
 * @retval  0          Failure.
 **/
uint32_t
xPrfGetLowUsableDramAddress (
  SIL_CONTEXT   *SilContext
  );


/**
 * xPrfGetSmbiosMemInfo
 *
 *  Description:
 *     This routine gets DMI Type 16, Type 17, Type 19 and Type 20 related information.
 *
 * @details    This routine gets DMI Type 16, Type 17,
 *             Type 19 and Type 20 related information.
 *
 * @param   SilContext           Input buffer for the Sil Context structure.
 *                               The Host is responsible for initializing
 *                               the data in the SIL_CONTEXT structure.
 * @param   DmiInfoTable         DMI records
 * @param   MemType -           DRAM Type
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
xPrfGetSmbiosMemInfo (
  SIL_CONTEXT  *SilContext,
  SIL_DMI_INFO         *DmiInfoTable,
  SIL_TECHNOLOGY_TYPE  MemType
  );

/**
 * xPrfGetMemInfo
 *
 * @brief    populate AMD_MEMORY_SUMMARY
 *
 * @details  This function populate AMD_MEMORY_SUMMARY
 *           structure which is used to report DRAM info
 *           to the Host
 *
 * @param   SilContext           Input buffer for the Sil Context structure.
 *                               The Host is responsible for initializing
 *                               the data in the SIL_CONTEXT structure.
 * @param    *MemSummaryTable  - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
xPrfGetMemInfo (
  SIL_CONTEXT        *SilContext,
  AMD_MEMORY_SUMMARY *MemSummaryTable
  );

/**
 * xPrfGetThreadsPerCore
 *
 * @brief   This Function is responsible to provide the threads per core values.
 *          Threads per core read from the MSR Register CPUID Fn0x8000001E_EBX
 *          xPRF wrapper function call xPrfGetThreadsPerCore to
 *          get the threads per core values
 *
 * @param   SilContext        Input buffer for the Sil Context structure.
 *                            The Host is responsible for initializing
 *                            the data in the SIL_CONTEXT structure.
 * @return  uint8_t           Threads per Core values
 */
uint8_t
xPrfGetThreadsPerCore (
  SIL_CONTEXT   *SilContext
  );

/**
 * xPrfGetPStatePower
 *
 * @brief   This Function is responsible to provide the Power in mW of the specified PState.
 *          GetPstateInfo take input has current PState value and provide the Power in mW.
 *          xPRF wrapper function call xPrfGetPStatePower to
 *          get Power in mW of the specified PState.
 *
 * @param   SilContext           Input buffer for the Sil Context structure.
 *                               The Host is responsible for initializing
 *                               the data in the SIL_CONTEXT structure.
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfGetPStatePower (
  SIL_CONTEXT   *SilContext,
  uint32_t      PstateSts,
  uint32_t *PowerInmW
  );

/**
 * xPrfGetNbiotopologyStructure
 *
 * @brief   Returns the base address of Pcie Topology Structure and its size
 *
 * @param   SilContext           Input buffer for the Sil Context structure.
 *                               The Host is responsible for initializing
 *                               the data in the SIL_CONTEXT structure.
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetNbiotopologyStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoBlockDataSize
  );

/**
 * xPrfGetGfxN6InfoStructure
 *
 * @brief   Returns the base address of Gfx Info Structure and its size
 *
 * @param   *InfoBlockDataSize   - Output block size
 * @param   SilContext      Input buffer for the Sil Context structure.
 *                          The Host is responsible for initializing
 *                          the data in the SIL_CONTEXT structure.
 * @return  void*           The address from openSIL Instance.
 *
 */
void *
xPrfGetGfxN6InfoStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoBlockDataSize
  );

/**
 * xPrfGetGfxDdiInfoStructure
 *
 * @brief   Returns the base address of Gfx Info Structure and its size
 *
 * @param   SilContext              Input buffer for the Sil Context structure.
 *                                  The Host is responsible for initializing
 *                                  the data in the SIL_CONTEXT structure.
 * @param   *InfoDdiBlockDataSize  - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetGfxDdiInfoStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoDdiBlockDataSize
  );

/**
 * xPrfCreateSratApicEntry
 *
 * @brief create SRAT Local APIC structure
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 * @param   SratApic      Input buffer for the SRAT Local APIC structure.  The Host is responsible
 *                        for ensuring the buffer size is sufficient to contain
 *                        SIL_SRAT_APIC or SIL_SRAT_x2APIC structure.
 *                        On output, the buffer is populated with SIL_SRAT_APIC or SIL_SRAT_x2APIC.
 * @param   SratApicSize  The size of the SratApicSize input buffer from the Host. This
 *                        is used by openSIL to ensure the input buffer size is
 *                        sufficient to contain SIL_SRAT_APIC or SIL_SRAT_x2APIC.
 *
 * @param   ApicModeValue APIC operation modes. Options are:
 *                        ApicMode              - Small systems (< 128 cores).
 *                        x2ApicMode            - Large systems.
 *                        ApicCompatibilityMode - Allow the code to choose depending on the quantity
 *                        of cores present. X2 mode is preferred.
 *
 * @param  SratTableLength xPrfCreateSratApicEntry xPrf Service update Total SRAT Table Length into
 *                        "SratTableLength" variable.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The SRAT APIC Entry was successfully populated.
 * @retval  SilOutOfBounds  The SRAT APIC Entry input buffer size was not sufficient.
 */
SIL_STATUS
xPrfCreateSratApicEntry (
  SIL_CONTEXT   *SilContext,
  uint8_t       *SratApic,
  uint32_t      SratApicSize,
  uint8_t       ApicModeValue,
  uint32_t      *SratTableLength
  );

/**
 * xPrfPStateGatherData
 *
 * @brief   This xPrf Service Provide the  PState information.
 *
 * @details This function will collect all PState information from the MSRs and fill up the
 *          Output Buffer.
 *          PState Informations are  : Physical socket number
 *                                     Logical core number in this socket
 *                           Pstate enable status
 *                           Core Frequency in MHz
 *                           Power in MilliWatts
 *                           Software P-state number
 *
 * @param   SilContext      Input buffer for the Sil Context structure.
 *                          The Host is responsible for initializing
 *                          the data in the SIL_CONTEXT structure.
 * @param   PStateData      Input buffer for the PState System Info structure.  The Host is responsible
 *                          for ensuring the buffer size is sufficient to contain
 *                          SIL_PSTATE_SYS_INFO  structure.
 *                          On output, the buffer is populated with SIL_PSTATE_SYS_INFO.
 * @param   PStateDataSize  The size of the PStateDataSize input buffer from the Host. This
 *                          is used by openSIL to ensure the input buffer size is
 *                          sufficient to contain SIL_PSTATE_SYS_INFO .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 */
SIL_STATUS
xPrfPStateGatherData (
  SIL_CONTEXT           *SilContext,
  SIL_PSTATE_SYS_INFO   *PStateData,
  uint32_t              PStateDataSize
  );

/**
 * xPrfGetCratHsaProcInfo
 *
 * @brief   This xPrf Service Provide the CRAT information about the HSA.
 *
 * @details This function will collect CRAT information about the HSA and fill up the
 *          Output Buffer.
 *          CRAT Informations are  : proximity node
 *                                  logical processor included in this HSA proximity domain
 *                                  count of execution units present in the APU node.
 *
 * @param   SilContext          Input buffer for the Sil Context structure.
 *                              The Host is responsible for initializing
 *                              the data in the SIL_CONTEXT structure.
 * @param  CratHsaProcData      Input buffer for the CRAT Info structure about the HSA. The
 *                              Host is responsible for ensuring the buffer size is
 *                              sufficient to contain SIL_CRAT_HSA_PROC_INFO  structure.
 *                              On output, the buffer is populated with SIL_CRAT_HSA_PROC_INFO.
 * @param  CratHsaProcDataSize  The size of the CratHsaProcDataSize input buffer from the Host.
 *                              This is used by openSIL to ensure the input buffer size is
 *                              sufficient to contain SIL_CRAT_HSA_PROC_INFO .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 */
SIL_STATUS
xPrfGetCratHsaProcInfo (
  SIL_CONTEXT   *SilContext,
  uint8_t       *CratHsaProcData,
  uint32_t      CratHsaProcDataSize
  );

/**
 * xPrfCratCacheEntry
 *
 * @brief   This xPrf Service Provide the CRAT  Cache information .
 *
 * @details This function will collect CRAT Cache information and fill up the
 *          Output Buffer.
 *          CRAT  Cache Informations are  : Low value of a logical processor
 *                                                    which includes this component
 *
 * @param   SilContext          Input buffer for the Sil Context structure.
 *                              The Host is responsible for initializing
 *                              the data in the SIL_CONTEXT structure.
 * @param  CratCacheEntry       Input buffer for the CRAT Cache Info structure. The
 *                              Host is responsible for ensuring the buffer size is
 *                              sufficient to contain SIL_CRAT_CACHE  structure.
 *                              On output, the buffer is populated with SIL_CRAT_CACHE.
 * @param  CratCacheEntrySize   The size of the CratCacheEntrySize input buffer from the Host.
 *                              This is used by openSIL to ensure the input buffer size is
 *                              sufficient to contain SIL_CRAT_CACHE .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 */
SIL_STATUS
xPrfCratCacheEntry (
  SIL_CONTEXT   *SilContext,
  uint8_t       *CratCacheEntry,
  uint32_t      CratCacheEntrySize
  );

/**
 * xPrfGetCoreTopologyOnDieMax
 *
 * @brief Retrieves information about the layout of the cores on the given die.
 *
 * @details The xPRF service will return maximum number of Ccd/Complex/Core/Thread.
 *          It is only available after host firmware calls "xSimAssignMemoryTp*".
 *          If host firmware directly links the service, host firmware is responsible
 *          for initializing the openSIL environment.
 *
 * @param   SilContext            Input buffer for the Sil Context structure.
 *                                The Host is responsible for initializing
 *                                the data in the SIL_CONTEXT structure.
 * @param Socket                  Zero-based socket number to check.
 * @param Die                     The target die's identifier within Socket.
 * @param MaxNumberOfCcds         On output, the maximum number of enabled CCDs on the given socket / die.
 * @param MaxNumberOfComplexes    On output, the maximum number of enabled complexes on the given socket / die.
 * @param MaxNumberOfCores        On output, the maximum number of enabled cores per complex on the given socket / die.
 * @param MaxNumberOfThreads      On output, the maximum number of enabled threads per core on the given socket / die.
 *
 * @retval SilPass                The core topology information was successfully retrieved.
 * @retval SilInvalidParameter    All output parameter pointers are not valid.
 * @retval SilNotFound            APOB CCD Logical to Physical Map not found.
 *
 */
SIL_STATUS
xPrfGetCoreTopologyOnDieMax (
  SIL_CONTEXT *SilContext,
  uint32_t    Socket,
  uint32_t    Die,
  uint32_t    *MaxNumberOfCcds,
  uint32_t    *MaxNumberOfComplexes,
  uint32_t    *MaxNumberOfCores,
  uint32_t    *MaxNumberOfThreads
  );

/**
 * xPrfGetGnbHandle
 *
 * @brief   Get base address to NBIO IOHC config data
 *
 * @details This data returned by this function is only valid after openSIL time point 1.
 *          Prior to this time point execution, the data is not available.
 *
 * @param   SilContext     Input buffer for the Sil Context structure.
 *                         The Host is responsible for initializing
 *                         the data in the SIL_CONTEXT structure.
 * @param   GnbHandle      On output, a pointer to the NBIO IOHC config data.
 *
 * @returns SIL_STATUS
 *
 * @retval  SilPass         A pointer to the IOHC config data is returned in GnbHandle
 * @retval  SilNotFound     NBIO API was not found in the API list
 * @retval  SilDeviceError  The GnbHandle is not ready
 *
 */
SIL_STATUS
xPrfGetGnbHandle (
  SIL_CONTEXT     *SilContext,
  SIL_GNB_HANDLE  **GnbHandle
  );

/**
 * xPrfFchEnableSpi
 *
 * @brief This Function is responsible for enabling FCH SPI interface
 *
 * @details SPI interface is required by Data Fabric IP
 * for accessing the SPIROM early during the system boot.
 * xPrfFchEnableSpi enables MMIO access to SPI configuration
 * registers by programming SPI_BASE address to FCH D14F3x0A0
 * register (i.e., FCH::LPCPCICFG::SPI_BASE_ADDR). It also sets
 * FCH SPIx0020 register (i.e, FCH::LPCHOSTSPIREG::SPI100ENABLE_REGISTER)
 * LSB (i.e., usespi100) to 1, enabling the SPI interface.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 */
void
xPrfFchEnableSpi (
  SIL_CONTEXT     *SilContext
  );

/**
 * xPrfDfGetNumberOfProcessorsPresent.
 *
 * @brief Get number of sockets on the system.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 *
 * @return uint32_t       Number of sockets.
 *
 */
uint32_t
xPrfDfGetNumberOfProcessorsPresent (
  SIL_CONTEXT     *SilContext
  );

/**
 * xPrfDfGetNumberOfRootBridgesOnSocket.
 *
 * @brief   Get number of root bridges on the socket.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 * @param   Socket        Socket number.
 *
 *
 * @return uint32_t         Number of root bridges.
 *
 */
uint32_t
xPrfDfGetNumberOfRootBridgesOnSocket (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket
  );

/**
 * xPrfDfGetNumberOfRootBridgesOnDie.
 *
 * @brief Get number of root bridges on the die.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext  A context structure through which host firmware defined data
 *                    can be passed to openSIL. The host firmware is responsible for initializing
 *                    the SIL_CONTEXT structure.
 * @param Socket      Socket number.
 * @param Die         Die number
 *
 * @return uint32_t   Number of root bridges.
 *
 */
uint32_t
xPrfDfGetNumberOfRootBridgesOnDie (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die
  );

/**
 * xPrfDfGetHostBridgeBusBase.
 *
 * @brief Get bus base number on the root bridge device.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 * @param   Socket        Socket number.
 * @param   Die           Die number
 * @param   Index         Root bridge number.
 *
 * @return uint32_t  Bus base Number.
 *
 */
uint32_t
xPrfDfGetHostBridgeBusBase (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die,
  uint32_t        Index
  );

/**
 * xPrfDfGetHostBridgeBusLimit.
 *
 * @brief Get bus base number on the root bridge device.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext  A context structure through which host firmware defined data
 *                    can be passed to openSIL. The host firmware is responsible for initializing
 *                    the SIL_CONTEXT structure.
 * @param Socket      Socket number.
 * @param Die         Die number.
 * @param Index       Root bridge number.
 *
 * @return uint32_t   Bus limit number.
 *
 */
uint32_t
xPrfDfGetHostBridgeBusLimit (
  SIL_CONTEXT     *SilContext,
  uint32_t        Socket,
  uint32_t        Die,
  uint32_t        Index
  );

/**
 * xPrfDfGetSystemInfo.
 *
 * @brief This function retrieves information about the overall system.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext                    Input buffer for the Sil Context structure.
 *                                      The Host is responsible for initializing
 *                                      the data in the SIL_CONTEXT structure.
 * @param NumberOfInstalledProcessors   On output, the total number of populated
 *                                      processor sockets in the system.
 * @param TotalNumberOfDie              On output, the total number of die in the
 *                                      system.
 * @param TotalNumberOfRootBridges      On output, the total number of root PCI
 *                                      bridges in the system.
 * @param SystemFchRootBridgeLocation   On output, system primary FCH location.
 * @param SystemSmuRootBridgeLocation   On output, system primary SMU location.
 *
 * @retval SilPass                      The system topology information was
 *                                      successfully retrieved.
 * @retval SilInvalidParameter          All output parameter pointers are NULL.
 *
 */
SIL_STATUS
xPrfDfGetSystemInfo (
  SIL_CONTEXT              *SilContext,
  uint32_t                 *NumberOfInstalledProcessors,
  uint32_t                 *TotalNumberOfDie,
  uint32_t                 *TotalNumberOfRootBridges,
  SIL_ROOT_BRIDGE_LOCATION *SystemFchRootBridgeLocation,
  SIL_ROOT_BRIDGE_LOCATION *SystemSmuRootBridgeLocation
  );

/**
 * xPrfDfFabricRegisterAccRMW.
 *
 * @brief Routine to read-modify-write a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext    A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param Socket        Processor socket to read from.
 * @param Die           Socket die to read from.
 * @param Function      Function number to read from.
 * @param Offset        Register to read.
 * @param Instance      Instance ID of the target fabric device.
 * @param NandValue     Value to NAND with the current register value.
 * @param OrValue       Value to OR with the current register value.
 * @retval Value written to target register if interested.
 *
 */
uint32_t
xPrfDfFabricRegisterAccRMW (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Function,
  uint32_t          Offset,
  uint32_t          Instance,
  uint32_t          NandValue,
  uint32_t          OrValue
  );

/**
 * xPrfDfFabricRegisterAccRead.
 *
 * @brief Routine to read a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param SilContext    A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param Socket        Processor socket to read from.
 * @param Die           Socket die to read from.
 * @param Function      Function number to read from.
 * @param Offset        Register to read.
 * @param Instance      Instance ID of the target fabric device.
 * @retval Current value of the target register.
 *
 */
uint32_t
xPrfDfFabricRegisterAccRead (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket,
  uint32_t      Die,
  uint32_t      Function,
  uint32_t      Offset,
  uint32_t      Instance
  );

/**
 * xPrfDfFabricRegisterAccWrite.
 *
 * @brief Routine to write a register.
 *
 * @details The xPRF service is only available after host firmware calls
 *          "xSimAssignMemoryTp*". If host firmware directly links the service,
 *          host firmware is responsible for initializing the openSIL environment.
 *
 * @param   SilContext    Input buffer for the Sil Context structure.
 *                        The Host is responsible for initializing
 *                        the data in the SIL_CONTEXT structure.
 * @param   Socket        Processor socket to read from.
 * @param   Die           Socket die to read from.
 * @param   Function      Function number to read from.
 * @param   Offset        Register to read.
 * @param   Instance      Instance ID of the target fabric device.
 * @param   Value         Value to write.
 *
 */
void
xPrfDfFabricRegisterAccWrite (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket,
  uint32_t      Die,
  uint32_t      Function,
  uint32_t      Offset,
  uint32_t      Instance,
  uint32_t      Value
  );

/**
 * xPrfGetResourceManagerTable
 *
 * @brief   This function is responsible for returning the desired resource manager table to the host firmware.
 *
 * @details It is the responsibility of the caller to:
 *          1. Specify the resource manager type defined in RcManager4-api.h.
 *          2. Cast the void * output to the appropriate resource manager table type defined in RcManager4-api.h.
 *
 * @param   SilContext      A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible for initializing
 *                          the SIL_CONTEXT structure.
 * @param   RcMgrTableType  The resource manager table type to be returned to the host firmware. Refer to
 *                          SIL_RC_MGR_TYPE for supported table types.
 *
 * @return  void *          A pointer to the requested resource manager table.
 *
 * @retval  NULL            If the resource manager table type is invalid or if the resource manager table is not found.
 */
void *
xPrfGetResourceManagerTable (
  SIL_CONTEXT   *SilContext,
  uint32_t      RcMgrTableType
  );

/**
 * xPrfGetCppcMinFrequency
 *
 * @brief   This Function is responsible for reading the minimal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfGetCppcMinFrequency (
  SIL_CONTEXT *SilContext,
  uint32_t *MinFrequency
  );

/**
 * xPrfGetCppcMNomFrequency
 *
 * @brief   This Function is responsible for reading the nominal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfGetCppcNomFrequency (
  SIL_CONTEXT *SilContext,
  uint32_t *NomFrequency
  );
