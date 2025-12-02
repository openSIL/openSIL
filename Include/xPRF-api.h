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
#include <RAS/Common/RasClass-api.h>
#include <CCX/CcxClass-api.h>
#include <MEM/MemClass-api.h>

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

/// Memory DMI Type 17 - for memory use
typedef struct {
  uint8_t                     Socket:3;               ///< Socket ID
  uint8_t                     Channel:2;              ///< Channel ID
  uint8_t                     Dimm:2;                 ///< DIMM ID
  uint8_t                     DimmPresent:1;          ///< Dimm Present
  uint16_t                    Handle;                 ///< The temporary handle, or instance number, associated with
                                                      ///< the structure
  uint16_t                    TotalWidth;             ///< Total Width, in bits, of this memory device, including any
                                                      ///< check or error-correction bits.
  uint16_t                    DataWidth;              ///< Data Width, in bits, of this memory device.
  uint16_t                    MemorySize;             ///< The size of the memory device.
  SIL_DMI_T17_FORM_FACTOR         FormFactor;         ///< The implementation form factor for this memory device.
  uint8_t                     DeviceLocator;          ///< The string number of the string that identifies the
                                                      ///< physically labeled socket or board position where the memory
                                                      ///< device is located.
  uint8_t                     BankLocator;            ///< The string number of the string that identifies the
                                                      ///< physically labeled bank where the memory device is located.
  uint16_t                    Speed;                  ///< Identifies the speed of the device, in megahertz (MHz).
  uint64_t                    ManufacturerIdCode;     ///< Manufacturer ID code.
  uint8_t                     SerialNumber[9];        ///< Serial Number.
  uint8_t                     PartNumber[21];         ///< Part Number.
  uint8_t                     Attributes;             ///< Bits 7-4: Reserved, Bits 3-0: rank.
  uint32_t                    ExtSize;                ///< Extended Size.
  uint16_t                    ConfigSpeed;            ///< Configured memory clock speed
  uint16_t                    MinimumVoltage;         ///< Minimum operating voltage for this device, in millivolts
  uint16_t                    MaximumVoltage;         ///< Maximum operating voltage for this device, in millivolts
  uint16_t                    ConfiguredVoltage;      ///< Configured voltage for this device, in millivolts
} SIL_MEM_DMI_PHYSICAL_DIMM_INFO;

/// DMI Type 17 offset 12h - Memory Type
typedef enum {
  SilOtherMemType = 0x01,                                  ///< Assign 01 to Other
  SilUnknownMemType,                                       ///< Assign 02 to Unknown
  SilDramMemType,                                          ///< Assign 03 to DRAM
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
  SilNvDimmNType = 0x04,                                   ///< Assign 04 to NVDIMM-N
  SilNvDimmFType = 0x05,                                   ///< Assign 05 to NVDIMM-F
  SilNvDimmPType = 0x06,                                   ///< Assign 06 to NVDIMM-P
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
#if defined (CONFIG_SOC_F19M10) && CONFIG_SOC_F19M10
  char                        PartNumber[21];           ///< Part Number.
#else
  char                        PartNumber[31];           ///< Part Number.
#endif
  uint8_t                     Attributes;        ///< Bits 7-4: Reserved, Bits 3-0: rank.
  uint32_t                    ExtSize;           ///< Extended Size.
  uint16_t                    ConfigSpeed;       ///< Configured memory clock speed
  uint16_t                    MinimumVoltage;    ///< Minimum operating voltage for this device, in millivolts
  uint16_t                    MaximumVoltage;    ///< Maximum operating voltage for this device, in millivolts
  uint16_t                    ConfiguredVoltage; ///< Configured voltage for this device, in millivolts
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
 * @brief AGESA MAXIMUM VALUES
 *
 * These Max values are used to define array sizes and associated loop
 * counts in the code.  They reflect the maximum values that AGESA
 * currently supports and does not necessarily reflect the hardware
 * capabilities of configuration.
 */
#define SIL_MAX_SOCKETS_SUPPORTED     2
#define SIL_MAX_CHANNELS_PER_SOCKET   12   /// Max Channels per sockets supported
#define SIL_MAX_DIMMS_PER_CHANNEL     2    /// Max DIMMs on a memory channel supported
#define SIL_MAX_T19_REGION_SUPPORTED  3    /// Max SMBIOS T19 Memory Region count
#define SIL_MAX_T20_REGION_SUPPORTED  3    ///< Max SMBIOS T20 Memory Region count
/// Collection of pointers to the DMI records
typedef struct {
  SIL_TYPE16_DMI_INFO  T16;
  SIL_TYPE17_DMI_INFO  T17[SIL_MAX_SOCKETS_SUPPORTED][SIL_MAX_CHANNELS_PER_SOCKET][SIL_MAX_DIMMS_PER_CHANNEL];
  SIL_TYPE19_DMI_INFO  T19[SIL_MAX_T19_REGION_SUPPORTED];
  SIL_TYPE20_DMI_INFO  T20[SIL_MAX_SOCKETS_SUPPORTED][SIL_MAX_CHANNELS_PER_SOCKET]
  [SIL_MAX_DIMMS_PER_CHANNEL][SIL_MAX_T20_REGION_SUPPORTED];
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
 * @param   SnpRmpTableBase Base address of the SNP RMP buffer allocated by
 *                          host firmware
 * @param   SnpRmpTableSize The size of the SNP RMP buffer allocated by host
 *                          firmware.
 *
 */
void
xPrfSetSnpRmp (
  uint64_t  SnpRmpTableBase,
  uint64_t  SnpRmpTableSize
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
  uint8_t                    RequestedChannelId,
  uint8_t                    *TranslatedChannelId
  );

/**
 * xPrfGetSystemMemoryMap
 * @brief Get top of memory (Tom2) for the UEFI Host along with
 * memory map, and number of holes
 *
 * @param[out] NumberOfHoles      Number of memory holes
 * @param[out] TopOfSystemMemory  Top of memory address
 * @param[out] MemHoleDescPtr     Memory descriptor structure
 *
 * @retval SilPass                Info extracted successfully.
 * @retval SilInvalidParameter    Failure.
 **/
SIL_STATUS
xPrfGetSystemMemoryMap (
  uint32_t                     *NumberOfHoles,
  uint64_t                     *TopOfSystemMemory,
  void                         **MemHoleDescPtr
  );

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
  );

/*
 * Prototypes for RAS xPRF services
 */
/**
 * xPrfCollectDimmMap
 *
 * @brief   Populate the DimmMap buffer input from the Host.
 *
 * @param   DimmMap     Input buffer for the Dimm Map.  The Host is responsible
 *                      for ensuring the buffer size is sufficient to contain
 *                      SIL_ADDR_DATA (defined in RasClass-api.h).
 *                      On output, the buffer is populated with SIL_ADDR_DATA.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The Dimm Map was successfully populated.
 * @retval  SilOutOfBounds  The Dimm Map input buffer size was not sufficient.
 */
SIL_STATUS
xPrfCollectDimmMap (
  SIL_ADDR_DATA *DimmMap
  );

/**
 * xPrfGetMaxCpus
 *
 * @brief   Returns the maximum possible number of CPUs.  The maximum here
 *          represents the absolute maximum supported by any SoC in openSIL.
 *          This value is used to allocate memory for the Cpu map.
 *
 *          When the Host allocate memory for the RasCpuMap buffer, this
 *          function should be used to determine the number of SIL_CPU_INFO
 *          structures that need to be allocated.
 *
 * @return  The maximum number of cpus including logical CPUs (threads).
 */
uint32_t
xPrfGetMaxCpus (void);

/**
 * xPrfCollectCpuMap
 *
 * @brief This function is responsible for building the CPU map used by the host
 *        RAS driver.
 *
 * @param   RasCpuMap   On input, the pointer to the CPU map structure.  The structure contains a SIL_CPU_INFO for
 *                      every processor (thread) in the system.  It is the responsibility of the host to allocate
 *                      sufficient memory for this structure.
 *                      On output, the buffer is filled with instances of
 *                      SIL_CPU_INFO (defined in RasClass-api.h) for each CPU.
 * @param   CpuMapSize  The size of the buffer RasCpuMap.
 * @param   TotalCpus   Return the total number of CPUs.
 *
 * @details This function has dependencies on the following openSIL
 *          Services/IPs:
 *
 *          Df->GetSystemInfo()
 *
 * @retval  SilPass             CPU map created successfully
 * @retval  SilInvalidParameter GetProcessorInfo failed
 * @retval  SilInvalidParameter GetCoreTopologyOnDieMax failed
 * @retval  SilNotFound         Info for current ccd, complex, or core not found
 * @retval  SilOutOfBounds      The input buffer is not sufficient for all CPUs
 */
SIL_STATUS
xPrfCollectCpuMap (
  SIL_CPU_INFO  *RasCpuMap,
  uint32_t      CpuMapSize,
  uint32_t      *TotalCpus
  );

/**
 * xPrfCollectCoreMcaInfo
 *
 * @brief   Save machine check architecture (MCA) IP ID value to CPU MCA Info
 *          buffer
 *
 * @details Read the MCA IP ID for every MCA bank on the current cpu and save it
 *          into the MCA INFO structure buffer provided by the Host. Refer to
 *          chapter 3 of the PPR MCA bank information.
 *
 *          It is the responsibility of the Host to ensure the CpuMcaInfo input
 *          buffer is sufficient to contain the SIL_CPU_MCA_INFO (defined in
 *          RasClass-api.h).
 *
 * @note    ***This function is executed on all processors by the Host in a
 *          multi-processor environment.
 *
 * @param   Buffer  An input buffer that points to a struct containing the empty
 *                  Cpu MCA info buffer of the currently executing processor and
 *                  the buffer size.
 */
void
xPrfCollectCoreMcaInfo (
  void *Buffer
  );

/**
 * xPrfCollectMcaErrorInfo
 *
 * @brief   Function for performing a search for MCA errors through all banks on
 *          a specific thread.
 *
 * @details It is the responsibility of the Host to ensure the input buffer is
 *          sufficient to contain the SIL_RAS_MCA_ERROR_INFO_V2 (defined in
 *          RasClass-api.h).
 *
 * @note    ***This function is executed on all processors by the Host in a
 *          multi-processor environment.
 *
 * @param   RasMcaErrorInfo   RAS MCA Error info buffer that will be populated by openSIL with MCA error information.
 *
 * @returns SIL_STATUS
 */
SIL_STATUS
xPrfCollectMcaErrorInfo (
  SIL_RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo
  );

/**
 * xPrfMcaErrorAddrTranslate
 *
 * @brief   Translate Unified Memory Controller (UMC) local address into
 *          specific memory DIMM information and system address.
 *
 * @param   SystemMemoryAddress Pointer to return the calculated system address
 * @param   NormalizedAddress   UMC memory address Information passed in from
 *                              Host.
 * @param   DimmInfo            Point to a buffer to populate translated
 *                              normalized address data. Host is responsible
 *                              for ensuring the buffer size is sufficient to
 *                              contain SIL_DIMM_INFO (defined in
 *                              RasClass-api.h).
 * @param   AddrData            Dimm information map, created by Host call to
 *                              xPrfCollectDimmMap, used in address translation
 *
 * @return  SIL_STATUS
 *
 */
SIL_STATUS
xPrfMcaErrorAddrTranslate (
  uint64_t                *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  );

/**
 * xPrfTranslateSysAddrToCS
 *
 * @brief Translate system address into specific memory DIMM information and
 *        normalized address information
 *
 * @param  SystemMemoryAddress  System Address
 * @param  NormalizedAddress    UMC memory address Information
 * @param  DimmInfo             DIMM information
 * @param  AddrData             Dimm information map used in address translation
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
xPrfTranslateSysAddrToCS (
  uint64_t                *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  );

/**
 * xPrfTranslateSysAddrToDpa
 *
 * @brief   System Address Translate to DIMM Physical Address (DPA)
 *
 * @details Translate system address into specific memory DIMM information and
 *          normalized address information
 *
 * @param SystemMemoryAddress System Address input
 * @param Dpa                 DIMM Physical Address to output
 * @param AddrData            Dimm information map input used in address
 *                            translation
 *
 * @return SIL_STATUS
 *
 * @retval SilNotFound        RAS API was not found in the API list
 * @retval SilPass            Address translated successfully
 */
SIL_STATUS
xPrfTranslateSysAddrToDpa (
  uint64_t      *SystemMemoryAddress,
  uint64_t      *Dpa,
  SIL_ADDR_DATA *AddrData
  );

/**
 * xPrfMcaIpIdInstanceIdInit
 *
 * @brief Program the Core MCA_IPID MSR Instance ID values for the CPU specified in RasCpuInfo.
 *
 * @param   RasCpuInfo  The CPU info structure for the core to program (RasClass-api.h)
 *
 * @return  SIL_STATUS
 * @retval  SilPass     If the function completed normally
 * @retval  SilNotFound If RAS API was not found in the API list
 */
SIL_STATUS
xPrfMcaIpIdInstanceIdInit (
  SIL_CPU_INFO  *RasCpuInfo
  );

/**
 * xPrfProgramCoreMcaConfigUmc
 *
 * @brief  This function will enable/disable FRU text in MCA based on the input flag.
 *
 * @details When enabled the FRU text is reported in McaSynd1/McaSynd2 MSRs.
 *
 * @param  EnableFruText                Input flag to enable/disable FRU text in MCA.
 * @return  SIL_STATUS
 * @retval  SilPass     If the function completed normally
 * @retval  SilNotFound If RAS API was not found in the API list
 */
SIL_STATUS
xPrfProgramCoreMcaConfigUmc (
  bool  EnableFruText
  );

/**
 * xPrfSetIpMcaCtlMask
 *
 * @brief Set the MCA_CTL_MASK for a given IP.
 *
 * @param HardwareId      Hardware ID input to match with MCA Bank MCA IP ID.  Refer to the Processor Programming
 *                        Reference (PPR) for details on acceptable HardwareID values for each IP.
 * @param McaType         MCA Type input to match with MCA Bank MCA IP ID.  Refer to the Processor Programming
 *                        Reference (PPR) for details on acceptable McaType values for each IP.
 * @param IpMcaPolicyCfg  Host firmware input containing platform policy information
 *                        that is used to configure MCA Control mask settings. The structure is a union
 *                        of all supported IP RAS policy structure. The host is responsible for populating the
 *                        appropriate IP structure with valid policy information. Currently supported IP structures:
 *
 *                        - SIL_NBIO_RAS_POLICY
 *
 * @returns SIL_STATUS
 * @retval  SilNotFound   If RAS API was not found in the API list
 * @retval  SilPass       Function completed normally
 *
 */
SIL_STATUS
xPrfSetIpMcaCtlMask (
  uint16_t          HardwareId,
  uint16_t          McaType,
  SIL_IP_RAS_POLICY *IpMcaPolicyCfg
  );

/**
 * xPrfUpdateDimmFruTextToMca
 *
 * @brief   This function provides the service to add Field Replaceable Unit
 *          (FRU) text to UMC MCA bank
 *
 * @details This function has dependencies on the following openSIL
 *          Services/IPs:
 *
 *          Df->DfGetSystemInfo()
 *          Df->DfGetProcessorInfo()
 *          Df->DfGetRootBridgeInfo()
 *
 * @param   DimmFruTextTable Input of the Dimm Fru Text table.  Host is expected
 *                           to provide the table in a format compatible with
 *                           SIL_DIMM_FRUTEXT_TABLE (defined in RasClass-api.h)
 * @return SIL_STATUS
 *
 * @retval SilNotFound         The RAS IP Api was not found
 * @retval SilInvalidParameter An invalid parameter was passed to
 *                             RasApi->UpdateFruTextToUmc
 * @retval SilPass             FRU text successfully updated
 */
SIL_STATUS
xPrfUpdateDimmFruTextToMca (
  SIL_DIMM_FRUTEXT_TABLE   *DimmFruTextTable
  );

/**
 * xPrfUpdateMcaFruText
 *
 * @brief   This function provides the service to add Field Replaceable Unit
 *          (FRU) text to MCA_SYND1 and MCA_SYND2 for all MCA Banks on a processor
 *
 * @param   RasCpuInfo This input contains cpu information for the processor being configured.
 *
 * @return SIL_STATUS
 *
 * @retval SilNotFound         The RAS IP Api was not found
 * @retval SilPass             FRU text successfully updated
 */
SIL_STATUS
xPrfUpdateMcaFruText (
  SIL_CPU_INFO  *RasCpuInfo
  );

/**
 * xPrfGetLocalSmiStatus
 *
 * @brief   Get the Local Smi Status from the SMM save state area.
 *
 * @details The top 512 bytes (FE00h to FFFFh) of SMRAM memory space are the
 *          default SMM state-save area. When an SMI occurs, the processor saves
 *          its state in the 512-byte SMRAM state-save area during the control
 *          transfer into SMM.
 *
 * @note  ***This function is expected to be executed on a specific processor
 *        by the Host in a multi-processor environment.
 *
 * @return uint32_t      The local SMI status for the processor
 */
uint32_t
xPrfGetLocalSmiStatus (
  void
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
 * @param      DmiInfoTable - DMI records
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
xPrfGetSmbiosMemInfo (
  SIL_DMI_INFO *DmiInfoTable
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
 * @param    *MemSummaryTable  - Pointer to AMD_MEMORY_SUMMARY
 *
 * @retval   SIL_STATUS
 */
SIL_STATUS
xPrfGetMemInfo (
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
 * @return  uint8_t Threads per Core values
 */
uint8_t
xPrfGetThreadsPerCore (void);

/**
 * xPrfGetPStatePower
 *
 * @brief   This Function is responsible to provide the Power in mW of the specified PState.
 *          GetPstateInfo take input has current PState value and provide the Power in mW.
 *          xPRF wrapper function call xPrfGetPStatePower to
 *          get Power in mW of the specified PState.
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfGetPStatePower (
  uint32_t  PstateSts,
  uint32_t *PowerInmW
  );

/**
 * xPrfGetNbiotopologyStructure
 *
 * @brief   Returns the base address of Pcie Topology Structure and its size
 *
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetNbiotopologyStructure (
  uint32_t  *InfoBlockDataSize
  );

/**
 * xPrfCreateSratApicEntry
 *
 * @brief create SRAT Local APIC structure
 *
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
  uint8_t   *SratApic,
  uint32_t  SratApicSize,
  uint8_t   ApicModeValue,
  uint32_t  *SratTableLength);

/**
 * xPrfPStateGatherData
 *
 * @brief   This xPrf Service Provide the  PState information.
 *
 * @details This function will collect all PState information from the MSRs and fill up the
 *          Output Buffer.
 *          PState Information are  : Physical socket number
 *                                     Logical core number in this socket
 *                           Pstate enable status
 *                           Core Frequency in MHz
 *                           Power in MilliWatts
 *                           Software P-state number
 *
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
  SIL_PSTATE_SYS_INFO *PStateData,
  uint32_t            PStateDataSize
  );

/**
 * xPrfGetCratHsaProcInfo
 *
 * @brief   This xPrf Service Provide the CRAT information about the HSA.
 *
 * @details This function will collect CRAT information about the HSA and fill up the
 *          Output Buffer.
 *          CRAT Information are  : proximity node
 *                                  logical processor included in this HSA proximity domain
 *                                  count of execution units present in the APU node.
 *
 * @param  CratHsaProcData      Input buffer for the CRAT Info structureabout the HSA. The
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
  uint8_t   *CratHsaProcData,
  uint32_t  CratHsaProcDataSize
  );

/**
 * xPrfCratCacheEntry
 *
 * @brief   This xPrf Service Provide the CRAT  Cache information .
 *
 * @details This function will collect CRAT Cache information and fill up the
 *          Output Buffer.
 *          CRAT  Cache Information are  : Low value of a logical processor
 *                                                    which includes this component
 *
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
  uint8_t   *CratCacheEntry,
  uint32_t  CratCacheEntrySize
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
 */
void
xPrfFchEnableSpi (
  void
  );

/**
 * xPrfGetCppcMinFrequency
 *
 * @brief   This Function is responsible for reading the minimal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @return  uint8_t Threads per Core values
 */
SIL_STATUS
xPrfGetCppcMinFrequency (
  uint32_t *MinFrequency
  );

/**
 * xPrfGetCppcMNomFrequency
 *
 * @brief   This Function is responsible for reading the nominal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @return  uint8_t Threads per Core values
 */
SIL_STATUS
xPrfGetCppcNomFrequency (
  uint32_t *NomFrequency
  );
