/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  RasClass-api.h
 * @brief OpenSIL-HOST  RAS IP interface
 *
 * @details The RAS API provides pieces of a much bigger system wide
 * application/service.
 *
 *  This file provides the structures details for the Host to interact with
 *  this segment of the RAS system application operations.
 */
/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_RAS RAS IP api
 *
 * The Reliability-Availability-Serviceability (RAS) IP module is more of an
 * application, or piece thereof. Some silicon level controls are provided to
 * set traps or responses to certain events, then collect silicon level data
 * for reporting to the Host.
 *
 *   See the 'Files - @ref RasClass-api.h' section of this document for
 *   further details.
 *
 *
 * @endcond
 */

#pragma once
#include <stddef.h>
#include <stdint.h>   // needed for declarations later in this file
#include <stdbool.h>
#include <RAS/Common/RasReg-api.h>

/**
 * @brief The maximum number of machine check banks visible to a logical core
 *
 * @details The number of visible error reporting banks may vary from core to
 *          core, but the maximum value of this number is 64.
 *
 */
#define XMCA_MAX_BANK_COUNT (64)
/*
 * xPRF RAS data structures
 */
/*
 * SIL_CPU_INFO: Contains information specific to each logical processor in the
 *               system (including threads). This structure is filled by openSIL
 *               at the request of Host firmware for RAS support.
 */
typedef struct {
  size_t        ProcessorNumber;
  uint8_t       SocketId;
  uint8_t       CcxId;
  uint8_t       DieId;
  uint8_t       CoreId;
  uint8_t       ThreadID;
} SIL_CPU_INFO;

/**
 * SIL_NBIO_RAS_POLICY
 *
 * @brief Configuration structure which should be populated by the Host-FW.
 *
 * @details  This structure contains policy configuration information for the
 *           platform that determine how the NBIO MCA Control Mask should be
 *           configured.
 *
 * MaskNbioSyncFlood       Input configuration flag to enable or disable NBIO sync flood.
 *                           true  - Prevent RAS SyncFlood (mask)
 *                           false - Allow SyncFlood errors (unmask)
 * PcieAerReportMechanism  Input host configuration that specifies the PCIE AER Error reporting mechanism.
 *                           0 - Hardware will report the error through firmware.
 *                           1 - "OS First" handling of the errors through generation of a system control
 *                               interrupt (SCI).
 *                           2 - "Firmware First" handling of errors through generation of a system management
 *                               interrupt (SMI).
 * NbioRasControlV2        Input host configuration that specifies the NBIO RAS error reporting mode
 *                           0 - Disabled
 *                           1 - MCA Reporting
 *                           2 - Legacy Reporting
 * NbioPoisonConsumption   Input host flag that specifies that specifies whether NBIO poison consumption is
 *                         enabled by the host.
 */
typedef struct {
  bool          MaskNbioSyncFlood;
  uint8_t       PcieAerReportMechanism;
  uint8_t       NbioRasControlV2;
  bool          NbioPoisonConsumption;
} SIL_NBIO_RAS_POLICY;

/**
 * SIL_IP_RAS_POLICY
 *
 * @brief Configuration structure which should be populated by the Host-FW.
 *
 * @details  This structure should be populated with any host policy information
 *           that is used to configure MCA_CTL_MASK for different IPs.
 *
 * @note    This is defined as a union to facilitate support of multiple IPs in
 *          the future. DummyValue is used until a second Policy type is added.
 *
 */
typedef union {
  SIL_NBIO_RAS_POLICY NbioRasPolicy;
  uint32_t            DummyValue;
} SIL_IP_RAS_POLICY;

/**
 * SIL_MCA_CTL_MASK_DATA
 *
 * @brief Configuration structure which should be populated by the Host-FW.
 *
 * @details  This structure contains the following:
 *
 *             HardwareId  - The hardware ID. This field is compared to the
 *                           MCA_IPID.HardwareID field for each MCA
 *                           bank.
 *             McaType     - The MCA Type value. This field is compared to the
 *                           MCA_IPID.McaType field for each MCA bank.
 *             IpRasPolicy - Platform pPolicy configuration information, for the
 *                           specified IP, that is used to conifgure MCA Control
 *                           Mask.
 *
 */
typedef struct {
  uint16_t          HardwareId;
  uint16_t          McaType;
  SIL_IP_RAS_POLICY *IpRasPolicy;
} SIL_MCA_CTL_MASK_DATA;

#pragma pack (push, 1)
/*
 * SIL_ADDR_DATA: Collection of UMC data related to each channel/dimm in the
 *                system.  This structure is filled by openSIL at the request of
 *                Host firmware for RAS support.
 */
typedef struct {
  uint8_t       TOTAL_NUM_RANKS_PER_UMCCH_ADDR_TRANS[16*4];
  uint8_t       RANK_ENABLE_PER_UMCCH_ADDR_TRANS[16*4];
  uint64_t      RANK_SIZE_PER_UMCCH_ADDR_TRANS[16*4][8];
  uint32_t      ADDRHASHBANK0[16*4];
  uint32_t      ADDRHASHBANK1[16*4];
  uint32_t      ADDRHASHBANK2[16*4];
  uint32_t      ADDRHASHBANK3[16*4];
  uint32_t      ADDRHASHBANK4[16*4];
  uint32_t      ADDRHASHNORMADDR[16*4][3];
  uint32_t      ADDRHASHRMADDR[16*4][4];
  uint32_t      ADDRHASHPC[16*4];
  uint32_t      ADDRHASHPC2[16*4];
  uint8_t       EXT_ADDRHASHNORMADDR[16*4][3];
  uint8_t       EXT_ADDRHASHRMADDR[16*4][4];
  uint32_t      CSBASE[16*4][8];
  uint32_t      CSBASESEC[16*4][8];
  uint32_t      CSMASK[16*4][8];
  uint32_t      CSMASKSEC[16*4][8];
  uint32_t      EXT_CSBASE[16*4][8];
  uint32_t      EXT_CSBASESEC[16*4][8];
  uint32_t      EXT_CSMASK[16*4][8];
  uint32_t      EXT_CSMASKSEC[16*4][8];
  uint32_t      CONFIGDIMM[16*4][4];
  uint32_t      BANKSELDIMM[16*4][4];
  uint32_t      ROWSELDIMM[16*4][4];
  uint32_t      COL0SELDIMM[16*4][4];
  uint32_t      COL1SELDIMM[16*4][4];
  uint32_t      RMSELDIMM[16*4][4];
  uint32_t      CSSIZEDIMM[16*4][8];
  uint32_t      CHANSIZEDIMM[16*4];
  uint8_t       VALIDHI[16*4];
  uint8_t       CSPOSHI[16*4][4];
  uint8_t       CSPOSLO[16*4][4];
  uint32_t      CTRLREG[16*4][4];
  bool          addrhash[6];
} SIL_ADDR_DATA;

/*
 * SIL_MCA_INFO: Structure containing McaIpId, Thread ID, and Socket ID values
 *               for each CPU.  This structure is used to build the MCA Bank Map
 *               structure that can be used by Host for RAS support.
 */
typedef struct {
  SIL_MCA_IPID_MSR  McaIpid;    ///< MCA bank IPID.
  size_t            ThreadId;   ///< Processor thread Id per socket to the MCA
                                ///< bank.
  uint32_t          SocketId;   ///< Socket Id to the MCA bank.
} SIL_MCA_INFO;

/*
 * SIL_MCA_BANK_MAP: MCA Bank map structure created by Host.  It is a collection
 *                   of SIL_MCA_INFO structures created by openSIL. It is not
 *                   currently used in openSIL but is included to because it is
 *                   a member of the RAS policy structure that is used.
 */
typedef struct {
  uint32_t      InstanceCounter;  ///< Total McaInfo instances number
  SIL_MCA_INFO  McaInfo[];        ///< Place holder for MCA_INFO entries.
} SIL_MCA_BANK_MAP;
#pragma pack (pop)

/*
 * SIL_CPU_MCA_INFO: Structure containing the SIL_MCA_INFO buffer.
 */
typedef struct {
  SIL_MCA_INFO  BankMcaInfo[XMCA_MAX_BANK_COUNT]; ///< Place holder for MCA_INFO
                                                  ///< entries.
} SIL_CPU_MCA_INFO;

/*
 * SIL_CPU_MCA_INFO_BUFFER: This structure is used an in input to
 *                          xPrfCollectCoreMcaInfo.
 */
typedef struct {
  uint32_t          Size;     ///< The size of Buffer.  Used to verify Host
                              ///< buffer input is the correct size.
  SIL_CPU_MCA_INFO  *Buffer;  ///< A pointer to a buffer that contains a
                              ///< collection of SIL_MCA_INFO that should be
                              ///< populated by xPRF
} SIL_CPU_MCA_INFO_BUFFER;

///
/// Platform RAS configuration data structure
///
typedef struct {
    uint16_t          SwSmiCmdPortAddr;
    uint8_t           MceSwSmiData;
    bool              McaErrThreshEn;
    uint16_t          McaErrThreshCount;
    uint8_t           DdrEccErrorCounterEnable;
    bool              DdrEccErrorCounterIntEnable;
    uint8_t           DdrEccErrorCounterLeakRate;
    uint16_t          DdrEccErrorCounterLeakThreshold;
    size_t            TotalNumberOfProcessors;
    SIL_CPU_INFO      *RasCpuMap;
    SIL_ADDR_DATA     *AddrData;
    bool              AmdMcaFruTextEnable;
    SIL_MCA_BANK_MAP  *McaBankMap;
} SIL_AMD_RAS_POLICY;

/*******************************************************************************
 * Structures used in boottime/runtime error translation services
 *
 */

/*
 * SIL_NORMALIZED_ADDRESS:  This structure contains normalized address
 *                          that will be used in translation to dram address
 */
typedef struct {
  uint64_t  NormalizedAddr;       ///< Error Address that needs to be translated
                                  ///< to dimm identification.
  uint8_t   NormalizedSocketId;   ///< The socket on which the targeted address
                                  ///< locates.
  uint8_t   NormalizedDieId;      ///< The die on which the targeted address
                                  ///< locates.
  uint8_t   NormalizedChannelId;  ///< The die on which the targeted address
                                  ///< locates.
  uint8_t   Reserved;             ///< Reserved
} SIL_NORMALIZED_ADDRESS;

/**
 * @brief This structure stores DIMM information.
 *
 * @details When an ECC error occurs in a DIMM, the RAS feature code converts the Normalized Address
 *          in MCA into a DRAM address and stores it in DIMM_INFO structure.
 */
typedef struct {
  uint8_t     ChipSelect; ///< The chip select on which the targeted address
                          ///< locates.
  uint8_t     Bank;       ///< The Bank for which the error address resides
  uint32_t    Row;        ///< The Row for which the error address resides
  uint16_t    Column;     ///< The Column for which the error address resides
  uint8_t     Rankmul;    ///< The Rank Multiplex for which the error address
                          ///< resides
  uint8_t     DimmId;     ///< Reserved
  uint8_t     SubChan;    ///< The subchannel for which the error address
                          ///< resides
} SIL_DIMM_INFO;

#pragma pack (push, 1)

/**
 * @brief This structure stores the MCA registers of the block where the error occurred.
 *
 * @details A processor contains multiple blocks. When an error occurs in the block,
 *          RAS feature code stores the MCA registers in the block into MCA_BANK_ERROR_INFO structure
 *          for reference by the error handler.
 *          the register order in the structure need match to the actul register MSR address offset
 *          the counting start from MCA_STATUS at offset 0x1
 */
typedef struct {
  size_t      McaBankNumber;
  uint64_t    McaStatusMsr;
  uint64_t    McaAddrMsr;
  uint64_t    McaMisc0Msr;
  uint64_t    McaConfigMsr;
  uint64_t    McaIpidMsr;
  uint64_t    McaSyndMsr;
  uint64_t    McaDeStatMsr;
  uint64_t    McaDeAddrMsr;
  uint64_t    McaMisc1Msr;
  uint64_t    Reserved1;
  uint64_t    Reserved2;
  uint64_t    Reserved3;
  uint64_t    Reserved4;
  uint64_t    McaSynd1Msr;
  uint64_t    McaSynd2Msr;
} SIL_MCA_BANK_ERROR_INFO;

/**
 * @brief This structure stores the MCA register of the block where the error
 *        occurred and the bank number and processor information associated with
 *        it.
 *
 * @details A processor contains multiple blocks. When an error occurs in the
 *          block, RAS feature code stores the MCA registers in the block into
 *          MCA_BANK_ERROR_INFO structure, stores the bank number in
 *          McaBankCount, and stores the processor information in the CPU_INFO
 *          structure for reference by the error handler.
 */
typedef struct {
  SIL_CPU_INFO            CpuInfo;      ///< Logical processor to Physical
                                        ///< processor map.
  size_t                  McaBankCount; ///< For Genoa this is the Error bank
                                        ///< count from the processor.
  SIL_MCA_BANK_ERROR_INFO McaBankErrorInfo[XMCA_MAX_BANK_COUNT]; ///< MCA bank regster dump.
} SIL_RAS_MCA_ERROR_INFO_V2;

/**
 * @brief BSP/AP MSR Access
 */

/**
 * @brief Platform DIMM FRU Text structures
 *
 */
typedef struct {
  uint32_t McaFruTextDW0; ///< Value for UMC::McaFruTextDW0
  uint32_t McaFruTextDW1; ///< Value for UMC::McaFruTextDW1
  uint32_t McaFruTextDW2; ///< Value for UMC::McaFruTextDW2
  uint32_t McaFruTextDW3; ///< Value for UMC::McaFruTextDW3
} SIL_FRUTEXT_STR;

typedef struct {
  uint16_t        Socket;        ///< Socket ID
  uint16_t        Die;           ///< Die ID
  uint16_t        Channel;       ///< Channel ID
  uint16_t        Module;        ///< Module 0 (CS=0,1):Module 1 (CS=2,3)
  SIL_FRUTEXT_STR AmdFrutextStr; ///< DIMM Fru text string
} SIL_FRUTEXT_ENTRY;

typedef struct {
  uint32_t          TableSize;     ///< Table Entries total size, not
                                   ///< include header size
  uint32_t          TableEntryNum; ///< Table Entries number
  SIL_FRUTEXT_ENTRY *FrutextEntry; ///< Oem Dimm Frutext entry.
} SIL_DIMM_FRUTEXT_TABLE;

/**********************************************************************************************************************
 * Declare Module data structure
 *
 */

typedef struct {
  uint8_t   Temp;
} RASCLASS_INPUT_BLK;

typedef struct {
  uint8_t   Temp;
} RASCLASS_OUTPUT_BLK;

/**
 * @brief RAS class data block for SilId_RasClass
 *
 */
typedef struct {
  RASCLASS_INPUT_BLK  RasInputBlock;
  RASCLASS_OUTPUT_BLK RasOutputBlock;
} RASCLASS_DATA_BLK;

#pragma pack (pop)
