/**
 * @file  FchSataCmn2Sn.c
 * @brief 9004 FCH SATA controller  revision specific transfer functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CommonLib/SmnAccess.h>
#include <Fch.h>
#include <FCH/Common/FchCore/FchSata/FchSataCmn2Rev.h>
#include <FCH/Common/FchCore/FchSata/FchSataDef.h>
#include <FCH/Common/FchCore/FchSata/FchSataReg.h>
#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include "FchSataSn.h"
#include "FchSataCmn2Sn.h"

FCH_SATA_XFER_TABLE mFchSataXferSn = {
  .Header = {
    .Version    = 0   // Version 0
  },
  .FchInitResetSataProgram        = FchInitResetSataProgramSn,
  .FchSataInitHideSataPci         = FchSataInitHideSataPciSn,
  .FchSataInitMsi                 = FchSataInitMsiSn,
  .FchSgpioToMpio                 = FchSgpioToMpioSn,
  .FchInitEnvSata                 = FchInitEnvSataSn
};

/**
 * FchSataInitHideNbifDev1PciSn
 *
 * @brief Disable Nbio Sata Controller PCI configuration space.
 *
 * @param DieBusNum IOCH bus number on current Die.
 * @param NbioSata  NbioSata controller number.
 *
 */
static
void
FchSataInitHideNbifDev1PciSn (
  uint32_t DieBusNum,
  uint32_t NbioSata
  )
{
  uint32_t SataBridgeAddress;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  SataBridgeAddress = FCH_SN_NBIF1DEVINDCFG1_IOHC_Bridge_CNTL;
  SataBridgeAddress += FCH_SN_NBIF1DEVINDCFG1_IOHC_Bridge_CNTL_NBIO_STEP * NbioSata;
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "Hide Nbif Dev1 Pci Bridge. DieBusNum 0x%x, \
      nBIF Sata %d, SMN address 0x%x set 0x7\n",
    DieBusNum,
    NbioSata,
    SataBridgeAddress
    );
  xUSLSmnReadModifyWrite (
    0,
    DieBusNum,
    SataBridgeAddress,
    ~(uint32_t) (0x7),
    0x7
    );
}

/**
 * FchInitResetSataProgramSn
 *
 * @brief Config Sata controller during Power-On
 *
 * @param DieBusNum  IOCH bus number on current Die.
 * @param FchSata    Fch SATA configuration structure pointer.
 *
 */
void
FchInitResetSataProgramSn (
  uint32_t            DieBusNum,
  FCHSATA_INPUT_BLK   *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // check if Sata0 and Sata1 are both disabled
  if ((!FchSata[0].SataEnable) && (!FchSata[1].SataEnable)) {
    FchSataInitHideNbifDev1PciSn (DieBusNum, 0);
  }
  // check if Sata2 and Sata3 are both disabled
  if ((!FchSata[2].SataEnable) && (!FchSata[3].SataEnable)) {
    FchSataInitHideNbifDev1PciSn (DieBusNum, 2);
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*************************** ENV INIT ***************************************/

/**
 * FchSataInitHideSataPciSn
 *
 * @brief Disable Sata Controller PCI configuration space.
 *
 * @param DieBusNum  IOCH bus number on current Die.
 * @param Controller Sata controller number.
 *
 */
void
FchSataInitHideSataPciSn (
  uint32_t  DieBusNum,
  uint32_t  Controller
  )
{
  uint32_t  SataBridgeAddress;

  SataBridgeAddress = 0;

  if ((Controller != 0) && (Controller != 2)) {
    SataBridgeAddress = SIL_RESERVED_44 + FCH_SMN_SATA_STEP *
      (Controller - 1);

    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Hide SATA PCI agent. DieBusNum 0x%x, nBIF SATA %d, SMN 0x%x set BIT28\n",
      DieBusNum,
      Controller,
      SataBridgeAddress
      );

    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      SataBridgeAddress,
      ~(BIT_32(28)),
      0
      );
  }
}

/**
 * FchInitEnvSataRaid
 *
 * @brief Shutdown unconnected Sata port clock
 *
 * @param DieBusNum  Bus Number of current Die.
 * @param Controller Sata controller number.
 * @param FchSata Fch Sata configuration structure pointer.
 *
 */
static
void
FchInitEnvSataRaid (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t    Strap0;
  uint32_t    Strap13;
  uint32_t    PcicfgBase;

  Strap0  = SIL_RESERVED_47;
  Strap13 = SIL_RESERVED_46;
  PcicfgBase = FCH_SN_SATA0_SMN_PCICFG;

  switch (Controller) {
  case 0:
    break;
  case 1:
    Strap0  = SIL_RESERVED_44;
    Strap13 = SIL_RESERVED_43;
    PcicfgBase = FCH_SN_SATA1_SMN_PCICFG;
    break;
  case 2:
    Strap0  = SIL_RESERVED_41;
    Strap13 = SIL_RESERVED_40;
    PcicfgBase = FCH_SN_SATA2_SMN_PCICFG;
    break;
  case 3:
    Strap0  = SIL_RESERVED_38;
    Strap13 = SIL_RESERVED_37;
    PcicfgBase = FCH_SN_SATA3_SMN_PCICFG;
    break;
  default:
    break;
  }

  //
  // Class code
  //
  xUSLSmnReadModifyWrite (0, DieBusNum, Strap13, 0x00, 0x00010400);
  //
  // Device ID
  //
  xUSLSmnReadModifyWrite (0, DieBusNum, Strap0, 0xFFFF0000, FCH_SATA_RAID_DID);

  //
  // SSID
  //
  if (FchSata[Controller].SataRaidSsid != 0x0 ) {
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      PcicfgBase + 0x4C,
      0x00,
      FchSata[Controller].SataRaidSsid
      );
  }
}

/**
 * FchInitEnvSataRaidSn
 *
 * @brief Config SATA Raid controller before PCI emulation
 *
 * @param DieBusNum   IOCH bus number on current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
static
void
FchInitEnvSataRaidSn  (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchInitEnvSataRaid (DieBusNum, Controller, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitEnvSataAhci
 *
 * @brief Config SATA AHCI controller before PCI emulation
 *
 * @param DieBusNum   IOCH bus number on current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
static
void
FchInitEnvSataAhci (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t    Strap0;
  uint32_t    Strap13;
  uint32_t    PcicfgBase;

  Strap0  = SIL_RESERVED_47;
  Strap13 = SIL_RESERVED_46;
  PcicfgBase = FCH_SN_SATA0_SMN_PCICFG;

  switch (Controller) {
  case 0:
    break;
  case 1:
    Strap0  = SIL_RESERVED_44;
    Strap13 = SIL_RESERVED_43;
    PcicfgBase = FCH_SN_SATA1_SMN_PCICFG;
    break;
  case 2:
    Strap0  = SIL_RESERVED_41;
    Strap13 = SIL_RESERVED_40;
    PcicfgBase = FCH_SN_SATA2_SMN_PCICFG;
    break;
  case 3:
    Strap0  = SIL_RESERVED_38;
    Strap13 = SIL_RESERVED_37;
    PcicfgBase = FCH_SN_SATA3_SMN_PCICFG;
    break;
  default:
    break;
  }

  //
  // Class code
  //
  xUSLSmnReadModifyWrite (0, DieBusNum, Strap13, 0x00, 0x00010601);
  //
  // Device ID
  //
  if (FchSata[Controller].SataClass == SataAhci7804) {
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      Strap0,
      0xFFFF0000,
      FCH_SATA_AMDAHCI_DID
      );
  } else {
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      Strap0,
      0xFFFF0000,
      FCH_SATA_AHCI_DID
      );
  }

  //
  // SSID
  //
  if (FchSata[Controller].SataAhciSsid != 0x0 ) {
    xUSLSmnReadModifyWrite (
      0,
      DieBusNum,
      PcicfgBase + 0x4C,
      0x00,
      FchSata[Controller].SataAhciSsid
      );
  }
}

/**
 * FchInitEnvSataAhciSn
 *
 * @brief Config SATA Ahci controller before PCI emulation
 *
 * @param DieBusNum   IOCH bus number on current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
static
void
FchInitEnvSataAhciSn (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchInitEnvSataAhci (DieBusNum, Controller, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitMsiSn
 *
 * @brief Enable/Disable Sata Msi capabilities
 *
 * @param DieBusNum   Bus Number of current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitMsiSn (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  bool FchSataMsiEnable;
  FchSataMsiEnable = FchSata[Controller].SataMsiEnable;

  if (FchSataMsiEnable) {
    switch (Controller) {
    case 0:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_45,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 1:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_42,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 2:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_39,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    case 3:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_36,
        ~BIT_32(18),
        BIT_32(18)
        );
      break;
    default:
      break;
    }
  } else {
    switch (Controller) {
    case 0:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_45,
        ~BIT_32(18),
        0x00
        );
      break;
    case 1:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_42,
        ~BIT_32(18),
        0x00
        );
      break;
    case 2:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_39,
        ~BIT_32(18),
        0x00
        );
      break;
    case 3:
      xUSLSmnReadModifyWrite (
        0,
        DieBusNum,
        SIL_RESERVED_36,
        ~BIT_32(18),
        0x00
        );
      break;
    default:
      break;
    }
  }
}

/**
 * FchSgpioToMpioSn
 *
 * @brief Set to MPIO mode
 *
 * @param DieBusNum  Bus Number of current Die.
 * @param Controller Sata controller number.
 *
 */
void
FchSgpioToMpioSn (
  uint32_t  DieBusNum,
  uint32_t  Controller
  )
{
  xUSLSmnReadModifyWrite (
    0,
    DieBusNum,
    SIL_RESERVED_48 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_33,
    ~(BIT_32(28)),
    BIT_32(28)
    );
}

/**
 * FchInitEnvSataSn
 *
 * @brief Config SATA controller before PCI emulation
 *
 * @param FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchInitEnvSataSn (
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t SataController;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  for (SataController = 0; SataController < SATA_CONTROLLER_NUM;
    SataController++) {
    if (FchSata[SataController].SataEnable) {
      FchInitEnvProgramSata (0, SataController, FchSata);

      //
      // Call Sub-function for each Sata mode
      //
      if ((FchSata[SataController].SataClass == SataAhci7804) ||
        (FchSata[SataController].SataClass == SataAhci)) {
        FchInitEnvSataAhciSn (0, SataController, FchSata);
      }

      if (FchSata[SataController].SataClass == SataRaid) {
        FchInitEnvSataRaidSn (0, SataController, FchSata);
      }

    } else {
      continue;
    }
  }

  // check if Sata0 and Sata1 are both disabled
  if ((!FchSata[0].SataEnable) && (!FchSata[1].SataEnable)) {
    FchSataInitHideNbifDev1PciSn (0, 0);
  }

  // check if Sata2 and Sata3 are both disabled
  if ((!FchSata[2].SataEnable) && (!FchSata[3].SataEnable)) {
    FchSataInitHideNbifDev1PciSn (0, 2);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
