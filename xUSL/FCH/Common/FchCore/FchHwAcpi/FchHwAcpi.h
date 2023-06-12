/**
 * @file  FchHwAcpi.h
 * @brief FCH HW ACPI header
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpiReg.h>

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

/*
 *
 * FCH Silicon IP code
 *
 */
void FchInitLateHwAcpi (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi);
void FchGetAcpiMmioBase (uint32_t *AcpiMmioBase);
void ProgramFchAcpiMmioTbl (ACPI_REG_WRITE *AcpiTbl);
void FchInitEnableBootTimer (FCHHWACPI_INPUT_BLK *FchHwAcpi);
bool IsExternalClockMode (void);
void ProgramCpuRstBTmr (void);
void ProgramFchEnvSpreadSpectrum (FCHHWACPI_INPUT_BLK *FchHwAcpi);

//
// FCH 8.0
//
void FchInitTogglePwrGdOnCf9 (FCHHWACPI_INPUT_BLK *FchHwAcpi);
void FchInitPspIntr0 (void);
void FchI2cReleaseControl (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi);
void FchInitEnableBootTimerV80 (FCHHWACPI_INPUT_BLK *FchHwAcpi);
void ProgramFchEnvSpreadSpectrumV80 (FCHHWACPI_INPUT_BLK *FchHwAcpi);
void FchUartInitV80 (FCHCLASS_INPUT_BLK *FchDataPtr);


FCHHWACPI_INPUT_BLK* GetFchHwAcpiData (void);
SIL_STATUS InitializeFchHwAcpiTp1 (void);
SIL_STATUS InitializeFchHwAcpiTp2 (void);
SIL_STATUS InitializeFchHwAcpiTp3 (void);
SIL_STATUS InitializeFchHwAcpiPreliminaryTp1 (void);
SIL_STATUS InitializeFchHwAcpiPreliminaryTp2 (void);
SIL_STATUS InitializeFchHwAcpiPreliminaryTp3 (void);
SIL_STATUS FchHwAcpiPreliminarySetInputBlk (void);

void FchInitResetHwAcpiP (FCHCLASS_INPUT_BLK *FchData);
void FchInitEnvHwAcpiP (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi);
void FchInitResetHwAcpi (FCHCLASS_INPUT_BLK *FchDataPtr);
void FchInitEnvHwAcpi (FCHCLASS_INPUT_BLK *FchDataPtr, FCHHWACPI_INPUT_BLK *FchHwAcpi);
/**
 * FCH_INIT_PREPCIE_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREPCIE_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_POSTPCIE_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_POSTPCIE_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREOS_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module before OS booting
 *
 */
typedef void (*FCH_INIT_PREOS_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREOS_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module before OS bootiny
 *
 */
typedef void (*FCH_INIT_PREOS_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

typedef struct {
  uint8_t   Version;
} FCH_HWACPI_INT_API_HEADER;
/**
 * Internal IP APIs
 *
 * This structure contains an API for internal IP specific functions that need
 * to run from IP common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  FCH_HWACPI_INT_API_HEADER   Header;
  FCH_INIT_PREPCIE_HWACPI_P   FchInitPrePcieHwAcpiP;
  FCH_INIT_PREPCIE_HWACPI     FchInitPrePcieHwAcpi;
  FCH_INIT_POSTPCIE_HWACPI_P  FchInitPostPcieHwAcpiP;
  FCH_INIT_POSTPCIE_HWACPI    FchInitPostPcieHwAcpi;
  FCH_INIT_PREOS_HWACPI_P     FchInitPreOsHwAcpiP;
  FCH_INIT_PREOS_HWACPI       FchInitPreOsHwAcpi;
} FCH_HWACPI_XFER_TABLE;

/**********************************************************************************************************************
 * @brief Module data structure
 *
 */

///
/// DESCRIPTION_HEADER - ACPI common table header
///
typedef struct _DESCRIPTION_HEADER {
  uint32_t               Signature;                      ///< ACPI signature (4 ASCII characters)
  uint32_t               Length;                         ///< Length of table, in bytes, including header
  uint8_t                Revision;                       ///< ACPI Specification minor version #
  uint8_t                CheckSum;                       ///< To make sum of entire table == 0
  uint8_t                OemId[6];                       ///< OEM identification
  uint8_t                OemTableId[8];                  ///< OEM table identification
  uint32_t               OemRevision;                    ///< OEM revision number
  uint32_t               CreatorId;                      ///< ASL compiler vendor ID
  uint32_t               CreatorRevision;                ///< ASL compiler revision number
} DESCRIPTION_HEADER;

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define FCHHWACPI_MAJOR_REV 0
#define FCHHWACPI_MINOR_REV 1
#define FCHHWACPI_INSTANCE  0
