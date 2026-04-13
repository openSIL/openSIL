/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CcxClass-api.h
 * @brief openSIL-Host CCX IP interface
 *
 * @details The CCX API provides some controls for setting the number of and
 *  the organization of the processor cores. xPRF services are provided to
 *  extract domain information for use in creating SMBios and ACPI tables.
 *
 *  This file provides the structures details for the Host to configure the
 *  base CCX operations.
 *
 */
/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_CCX   Core Complex (CCX) api
 *
 *   The Processing Core CompleX (CCX) is an IP block containing one or more
 *   processor execution cores. 'Threads' are arranged into 'Cores'. 'Cores' are
 *   arranged into 'CCX'. 'CCX' are arranged into 'CCD' (Die); and 'CCD' are
 *   arranged into socket/SoC. Each Core operates independently from the others
 *   with some exceptions in regard to shared cache (L2 & L3) and Threads on a
 *   Core sharing some resources.
 *
 *   See the 'Files - @ref CcxClass-api.h' section of this document for
 *   further details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdint.h>   // needed for declarations later in this file
#include <stdbool.h>
#include <xSIM-api.h>

/** CCX Common External Data Block
 *  @details The major/minor version and instance of the CCX Data block.
 */
#define CCXCLASS_MAJOR_REV 0
#define CCXCLASS_MINOR_REV 1
#define CCXCLASS_INSTANCE  0

#pragma pack (push, 1)

/** CCX IP Revisions
 *  @details List the revisions of this IP device. These are used in the
 *  Data blocks to identify structural variations.
 */
typedef enum {
  CcxZen4 = 0x04,
} CcxRevisions;


/*  ---------------------------------------------
 *  Declare structures used within the Host visible data blocks.
 */
/**
 * Microcode Patch Equivalent Processor Id (MPB_REVISION)
 *
 * Classification(Public)
 * Disposition(TBD)
 */
typedef struct {
  uint16_t   ProcessorRevisionID;        ///< Equivalent ID
  uint8_t    Chipset1RevisionID;         ///< Revision level of 1st Chipset device to match
  uint8_t    Chipset2RevisionID;         ///< Revision level of 2nd Chipset device to match
} MPB_REVISION;

/**
 * Load Control
 *
 * Classification(Public)
 * Disposition(TBD)
 */
typedef union {
  struct {
    uint8_t    SerializedLoad:1;
    uint8_t    LoadOnBothThreads:1;
    uint8_t    Reserved:6;
  } Field;
  uint8_t Value;
} LOAD_CONTROL;

/// Reserved Data
typedef struct {
  uint8_t         Reserved[5536];
} PATCH_DATA;

/**
 * Microcode Patch Block (MPB)
 *
 * Classification(Public)
 * Disposition(TBD)
 */
typedef struct {
  uint32_t        MPB_DATE;
  uint32_t        PatchLevel;
  uint16_t        MPB_LOADER_ID;
  uint16_t        MPB_SIZE;
  uint32_t        Reserved1;
  uint32_t        MPB_NB_ID;
  uint32_t        MPB_SB_ID;
  MPB_REVISION    MPB_REVISION;
  uint8_t         MPB_BIOS_REVISION;
  LOAD_CONTROL    LoadControl;
  uint16_t        NEXT_MPB_OFFSET;
  PATCH_DATA      PatchData;
} MICROCODE_PATCH_BLOCK; // Microcode Patch Block

/** Ucode Patch Bios Entry Information
 *  @details These entries specify the location of the Micro-Code
 *  (uCode) data entries within the platform's memory zone. These
 *  uCode patches must be visible in the memory address space. If the
 *  platform uses a paged style BIOS ROM, then the Host must assure that
 *  the proper page is set to be visible when this data is referenced.
 */
typedef struct {

  uint64_t  UcodePatchEntryAddress;    ///< Entry Address of ucode patch files
                                       ///<  located in the SPIROM
  uint32_t  UcodePatchEntrySize;       ///< Size of ucode patch files located
                                       ///<  in the SPIROM
} UCODEPATCH_BIOSENTRYINFO;

/** Bios Firmware Bios Entry Information
 *  @details The entry specifies the location of the Bios Firmware data entry
 *  within the platform's memory zone. The entry must be visible in the memory
 *  address space.
 */
typedef struct {
  uint64_t  PspBiosBinBase;            ///< Psp Bios Binary Base Address
  uint32_t  PspBiosBinSize;            ///< Psp Bios Binary Size
} PSPBIOSBIN_BIOSENTRYINFO;

/**********************************************************************************************************************
 * @brief  CCX Common data Input block
 * @details  Declare CCX Common data block for configuration
 * Define the IP device data elements visible to the Host.
 *  This is comprised of areas for
 *  1) shared variables to the Host, 2) section for revision
 *  specific data elements.
 *
 * If the Host is not interested in a feature, then it would/should not
 *  touch the var. The Kconfig configurator will assign default values.
 *  The Host may override them here after IP block is established.
 */
typedef struct {
  uint8_t  AmdApicMode;         ///< Operating mode for the APIC
  uint8_t  AmdDownCoreMode;     ///< Down core mode specifies the desired
                                ///< number of cores to enable
  uint8_t  AmdCcdMode;          ///< Ccd mode specifies the desired number of
                                ///< Ccds to enable
  uint8_t  AmdSmtMode;          ///< Simultaneous Multi-Threading
                                ///< 0 - SMT disabled, 1 - SMT Auto
  uint32_t ApSyncFlag;
  uint16_t AmdCoreDisCcd[0x20];     ///< Core disable bitmap buffer.  Current
                                    ///< allocation is for max 32 CCDs. Setting
                                    ///< AmdCoreDisCcd[Ccd#].bit[x] = 1 could
                                    ///< down logical core x.
  bool     AmdIbrsEn;               ///< IBRS (Indirect Branch Prediction
                                    ///< Speculation) setting
  bool     AmdBranchSampling;       ///< Branch Sampling enabled/disabled flag
  bool     AmdGameMode;             ///< Game mode.
  bool     AmdVmplEnable;           ///< Enable VMPL
  bool     L1BurstPrefetch;         ///< Enable L1 Burst Prefetch Mode
  bool     AmdL1StreamPrefetcher;   ///< Enable L1 Stream HW Prefetcher
  bool     AmdL1StridePrefetcher;   ///< Enable L1 Stride Prefetcher
  bool     AmdL1RegionPrefetcher;   ///< Enable L1 Region Prefetcher
  bool     AmdL2StreamPrefetcher;   ///< Enable L2 Stream HW Prefetcher
  bool     AmdL2UpDownPrefetcher;   ///< Enable L2 Up/Down Prefetcher
  bool     AmdCpuWdtEn;             ///< Enable CPU WDT
  uint16_t AmdCpuWdtTimeout;        ///< CPU WDT Timeout value
  uint8_t  AmdCpuWdtSeverity;       ///< Control MSRC001_0074[CpuWdTmrCfgSeverity], 0xFF means leave it at reset value
  uint8_t  AmdCStateMode;           ///< This element specifies the C State operational mode.
                                    ///< This can be used with processors which support deep sleep C states such as CC6.
  uint8_t  AmdCc6Ctrl;              ///< This value controls whether openSIL will disable CC6 support in the CPU Cores
  uint16_t AmdCStateIoBaseAddress;  ///< This item specifies a free block of 8 consecutive bytes of I/O ports
                                    ///< that can be used to allow the CPU to enter C-States.
  uint8_t  AmdCpbMode;              ///< This item allows Core Performance Boost (CPB) to be forced to disabled,
                                    ///< even if the hardware provides this feature.
  uint8_t  AmdHardwarePrefetchMode; ///< This value provides for advanced performance tuning by controlling
                                    ///< the hardware prefetcher setting.
  uint8_t  AmdSoftwarePrefetchMode; ///< This value provides for advanced performance tuning by controlling
                                    ///< the software prefetch instructions.
  bool     AmdSmee;                 ///< Secure Memory Encryption Extension (SMEE).
  bool     AmdHmkee;                ///< Host Multi-Key Encryption Enable
  uint8_t  RedirectForReturnDis;
  uint8_t  OpCacheCtrl;
  uint8_t  StreamingStoresCtrl;
  bool     EnSpecStFill;         ///< This control adjusts the performance of the speculative operations and changes
                                 ///< the behavior of memory write operations.
  bool     EnableRMSS;           ///< This selects the state of the REP-MOV/STOS Streaming instruction
                                 ///< capability in the processor.
  bool     EnableSvmAVIC;        ///<
                                 ///< instruction capability in the processor.
  bool     IbsHardwareEn;        ///< Work around for errata 1285, this control will disable a portion
                                 ///< of the speculative read process to avoid the condition noted in the errata.
  uint8_t  EnableAvx512;         ///< Enable AVX512
  bool     EnableSvmX2AVIC;      ///< Enable SVM x2AVIC.
  uint8_t  MonMwaitDis;          ///< The MONITOR, MWAIT, MONITORX, and MWAITX opcodes become invalid,when Enabled.
  uint64_t AmdFixedMtrr250;
  uint64_t AmdFixedMtrr258;
  uint64_t AmdFixedMtrr259;
  uint64_t AmdFixedMtrr268;
  uint64_t AmdFixedMtrr269;
  uint64_t AmdFixedMtrr26A;
  uint64_t AmdFixedMtrr26B;
  uint64_t AmdFixedMtrr26C;
  uint64_t AmdFixedMtrr26D;
  uint64_t AmdFixedMtrr26E;
  uint64_t AmdFixedMtrr26F;
  uint8_t  AmdSnpMemCover;                 ///< SNP Memory status
  uint32_t AmdSnpMemSize;                  ///< SNP Memory size
  UCODEPATCH_BIOSENTRYINFO UcodePatchEntryInfo;
  PSPBIOSBIN_BIOSENTRYINFO PspBiosBinEntryInfo;
  uint8_t  DisableWcSpecConfig;              ///< WC Spec Configuration
  bool     IommuSupport;
  uint8_t  AmdPstatePolicy;
  uint8_t  AmdSplitRmpTable;               ///< This control selects if the RMP Table is allocated at
                                           ///< the End of Physical DRAM, or split in Memory across Sockets.
  uint8_t  AmdCpuSpeculativeStoreMode;     ///< CPU Speculative Store Modes
  uint8_t  AmdCpuPauseDelay;               ///< Control number of cycles a thread will be idle
                                           ///< after PAUSE instruction.
  bool     AmdEnableFSRM;
  bool     AmdEnableERMS;
  bool     AmdStatisticalCorrectPredictor;
  bool     CapsuleUpdateDetected;          ///< Flag indicating capsule update detected
  bool     SvmEnable;                      ///< Secure Virtual Machine mode enable
  bool     SvmLock;                        ///< Secure Virtual Machine mode lock
  uint32_t P0Setting;                      ///< Pstate 0 Setting: 1- Custom; 2- Auto (For all)
  uint32_t P0Freq;                         ///< Custom P0 Freq
  uint32_t PxAutoFreq;                     ///< Auto Default Frequency
  uint32_t P0Vid32;                        ///< VID (Extend to 32bit, SVI3 use 9bit VID)
  uint32_t PxAutoVid;                      ///< Auto Default VID
  bool     CcxLoadUcodePatch;              ///< Load uCode Patch
} CCXCLASS_INPUT_BLK;


/**********************************************************************************************************************
 * @brief  CCX Common output block
 * @details Declare CCX Common data block for output.
 * Define the IP device data elements visible to the Host.
 *  These elements are generated or altered by the IP during silicon
 *  initialization.
 */
typedef struct {
  uint8_t   AmdApicMode;
  uint8_t   AmdDownCoreMode;
  uint8_t   AmdCcdMode;
  bool      AmdAcpiS3Support;
  uint32_t  AmdCcxCoreCount;    ///< Count of enabled cores
  bool      AmdIsSnpSupported;  ///< SNP Supported status  0 - SNP Not Support   1 - SNP Support
  uint64_t  AmdRmpTableSize;    ///< RMP Table size
  uint64_t  AmdRmpTableBase;    ///< RMP Table Base
  uint16_t  ProcessorId;
} CCXCLASS_OUTPUT_BLK;

typedef struct {
  CCXCLASS_INPUT_BLK CcxInputBlock;
  CCXCLASS_OUTPUT_BLK CcxOutputBlock;
} CCXCLASS_DATA_BLK;

/***************************************************************
 * Pstate Data API
 **************************************************************/
#define MAX_THREAD_NUMBER_PER_SOCKET  512

/// P-state structure for each state
typedef struct {
  uint32_t               PStateEnable;                ///< Pstate enable
  uint32_t               CoreFreq;                    ///< MHz
  uint32_t               Power;                       ///< milliWatts
  uint32_t               SwPstateNumber;              ///< Software P-state number
} SIL_PSTATE_VALUES;

/// P-state structure for socket
typedef struct {
  uint8_t           SocketNumber;                              ///< Physical socket number of this socket
  uint16_t          TotalLogicalCores;                         ///< Logical core number in this socket
  bool              CreateAcpiTables;                          ///< Create table flag
  uint32_t          LocalApicId[MAX_THREAD_NUMBER_PER_SOCKET]; ///< Local Apic Id
  uint8_t           PStateMaxValue;                            ///< Max p-state number in this core
  uint32_t          TransitionLatency;                         ///< Transition latency
  bool              IsPsdDependent;                            ///< Dependent or Independent PSD
  SIL_PSTATE_VALUES PStateStruct[1];                           ///< P state structure
} SIL_PSTATE_SOCKET_INFO;

/// P-state structure for whole system
typedef struct {
  uint8_t                 TotalSocketInSystem;       ///< Total node number in system
  uint32_t                SizeOfBytes;               ///< Structure size
  SIL_PSTATE_SOCKET_INFO  PStateSocketStruct[1];     ///< P state structure for socket
} SIL_PSTATE_SYS_INFO;

/// Format for Processor Local APIC/SAPIC Affinity Structure
typedef struct {
  uint32_t  Enabled:1;               ///< Enabled
  uint32_t  :31;                     ///< Reserved
} SIL_SRAT_APIC_FLAG;

/// Format for Processor Local APIC/SAPIC Affinity Structure
typedef struct {
  uint8_t   Type;                      ///< 0 Processor Local APIC/SAPIC Affinity Structure
  uint8_t   Length;                    ///< 16
  uint8_t   ProximityDomain_7_0;       ///< Bit [7:0] of the proximity domain to which the processor belongs
  uint8_t   ApicId;                    ///< The processor local APIC ID
  SIL_SRAT_APIC_FLAG Flags;            ///< Flags
  uint8_t   LocalSapicEid;             ///< The processor local SAPIC EID
  uint8_t   ProximityDomain_31_8[3];   ///< Bit [31:8] of the proximity domain to which the processor belongs
  uint32_t  ClockDomain;               ///< The clock domain to which the processor belongs
} SIL_SRAT_APIC;

/// Format for Processor Local x2APIC Affinity Structure
typedef struct {
  uint8_t   Type;                      ///< 2 Processor Local APIC/SAPIC Affinity Structure
  uint8_t   Length;                    ///< 24
  uint8_t   Reserved[2];               ///< Reserved - Must be zero
  uint32_t  ProximityDomain;           ///< The proximity domain to which the logical processor belongs
  uint32_t  x2ApicId;                  ///< The processor local x2APIC ID
  SIL_SRAT_APIC_FLAG Flags;            ///< Flags
  uint32_t  ClockDomain;               ///< The clock domain to which the processor belongs
  uint8_t   Reserved2[4];              ///< Reserved
} SIL_SRAT_x2APIC;

#pragma pack (pop)
