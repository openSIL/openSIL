/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file Fch.h
 * @brief FCH Initialization data
 *
 */

#pragma once

#include <FCH/Common/FchCommonCfg.h>

/**********************************************************************************************************************
 * variable declaration
 *
 */

typedef struct FCH_PRIVATE FCH_PRIVATE;

#define FCH_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_FCH & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)


/**********************************************************************************************************************
 * Function prototypes
 *
 */

/// FCH INIT definition for XHCI
typedef
void
(*FP_FCH_XHCI_RECOVERY_PR) (
  FCH_PRIVATE *ptrFchPrivate,
  uint32_t XhciRomAddress
  );

/// FCH INIT interface definition for EHCI
typedef
void
(*FP_FCH_EHCI_RECOVERY_PR) (
  FCH_PRIVATE *ptrFchPrivate,
  uint32_t EhciTemporaryBarAddress
  );

/// FCH INIT interface definition for SATA
typedef
void
(*FP_FCH_SATA_RECOVERY_PR) (
  FCH_PRIVATE *ptrFchPrivate,
  uint32_t SataBar0,
  uint32_t SataBar5
  );

/// FCH INIT interface definition for GPP
typedef
void
(*FP_FCH_GPP_RECOVERY_PR) (
  FCH_PRIVATE *ptrFchPrivate,
  FCH_GPP_R *ptrFchGpp
  );

/// FCH INIT interface definition for eMMC
typedef
void
(*FP_FCH_EMMC_RECOVERY_PR) (
  FCH_PRIVATE *ptrFchPrivate
  );

FCHCLASS_INPUT_BLK*
GetFchDataBlock (void
  );
/*
void
FchInitXhciOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t           XhciRomAddress
  );

void
FchInitEhciOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t         EhciTemporaryBarAddress
  );

void
FchInitSataOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t         SataBar0,
  uint32_t         SataBar5
  );

void
FchInitGppOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  FCH_GPP_R        *ptrFchGpp
  );

void
FchInitEmmcOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate
  );

extern int
FchXhciOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t         XhciRomAddress
  );

extern int
FchEhciOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t         EhciTemporaryBarAddress
  );

extern int
FchSataOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  uint32_t         SataBar0,
  uint32_t         SataBar5
  );

extern int
FchGppOnRecovery (
  FCH_PRIVATE      *ptrFchPrivate,
  FCH_GPP_R        *ptrFchGpp
  );
*/
SIL_STATUS
FchUsbDisablePort (
  uint8_t Socket,
  uint32_t USB3DisableMap,
  uint32_t USB2DisableMap
  );

/**
 * Establish FCH input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
FchClassSetInputBlk (void);

/**
 * InitializeFchClassTp1
 *
 * @brief   Initialize the FCH silicon during time point 1.
 *
 * This function initializes the parent FCH silicon block then proceeds
 * to enable/disable the sub-ip components per config. Each sub-IP
 * component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeFchClassTp1 (void);

/**
 * InitializeFchClassTp2
 *
 * @brief   Initialize the FCH silicon during time point 2.
 *
 * This function initializes the parent FCH silicon block then proceeds
 * to enable/disable the sub-ip components per config. Each sub-IP
 * component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeFchClassTp2 (void);

/**
 * InitializeFchClassTp3
 *
 * @brief   Initialize the FCH silicon during time point 3.
 *
 * This function initializes the parent FCH silicon block then proceeds
 * to enable/disable the sub-ip components per config. Each sub-IP
 * component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeFchClassTp3 (void);

/**********************************************************************************************************************
 * @brief Module data structure
 *
 */
typedef struct FCH_PRIVATE {
  FP_FCH_XHCI_RECOVERY_PR    FpFchXhciRecovery;       ///< Xhci initial Fcuntion for recovery mode.
  FP_FCH_EHCI_RECOVERY_PR    FpFchEhciRecovery;       ///< Ehci Initial Function for recovery mode.
  FP_FCH_SATA_RECOVERY_PR    FpFchSataRecovery;       ///< Sata Initial Function for recovery mode.
  FP_FCH_GPP_RECOVERY_PR     FpFchGppRecovery;        ///< Gpp Initial Function for Recovery mode.
  FP_FCH_EMMC_RECOVERY_PR    FpFchEmmcRecovery;       ///< eMMC Initial Function for Recovery mode.
} FCH_PRIVATE;

/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define ACPI_S0       (0 << 10)
#define ACPI_S3       (3 << 10)
#define ACPI_S4       (4 << 10)
#define ACPI_S5       (5 << 10)
#define ACPI_BM_MASK  (7 << 10)

#define AMD_ACPI_S3 0x3

#define BOOT_WITH_FULL_CONFIGURATION                  0x00
#define BOOT_ON_S4_RESUME                             0x05
#define BOOT_ON_S5_RESUME                             0x06
#define BOOT_ON_S3_RESUME                             0x11
#define BOOT_ON_FLASH_UPDATE                          0x12
#define BOOT_IN_RECOVERY_MODE                         0x20

#define FCHCLASS_MAJOR_REV 0
#define FCHCLASS_MINOR_REV 1
#define FCHCLASS_INSTANCE  0

#define FEATURE_CPPC_MASK                    (1 << 22)
