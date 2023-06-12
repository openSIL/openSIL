/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSata.c
 * @brief FCH SATA controller functions
 *
 */

#include <SilCommon.h>
#include "FchSata.h"
#include "FchSataCmn2Rev.h"
#include "FchSataReg.h"
#include <FCH/Common/FchCommon.h>
#include <CommonLib/SmnAccess.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchReg.h>
#include <CommonLib/Io.h>

#define MAX_RETRY_NUM 200

/**
 * FchSataEnableWriteAccess
 *
 * @brief Enables write access on Sata controller
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataEnableWriteAccess (
  uint32_t DieBusNum,
  uint32_t Controller
  )
{
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + 0x00 + Controller * FCH_SMN_SATA_STEP,
    0xFFFFFFFE,
    BIT_32(0)
    );
}

/**
 * FchSataDisableWriteAccess
 *
 * @brief Disables write access to Sata controller
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataDisableWriteAccess (
  uint32_t DieBusNum,
  uint32_t Controller
  )
{
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + 0x00 + Controller * FCH_SMN_SATA_STEP,
    0xFFFFFFFE,
    0
    );
}

/**
 * FchSataInitDisableSata
 *
 * @brief Disables Sata Controller
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataInitDisableSata (
  uint32_t DieBusNum,
  uint32_t Controller
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
    0xfffffffe,
    BIT_32(0)
    );
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
    ~BIT_32(11),
    BIT_32(11)
    );
  // Hide Sata Pci header
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitPortOffline
 *
 * @brief Puts Sata Port in Offline mode
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortNum    Sata Port to put in offline.
 *
 */
static
void
FchSataInitPortOffline (
  uint32_t DieBusNum,
  uint32_t Controller,
  uint32_t PortNum
  )
{
  uint32_t Retry;
  uint32_t PortStatus;
  uint32_t SataRegAddr;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0289 + PortNum * 0x80;
  xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFFFFFFF0, 0x4);

  for (Retry = 0; Retry < MAX_RETRY_NUM; Retry++) {
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0288 + PortNum * 0x80;
    PortStatus = xUSLSmnRead(0, DieBusNum, SataRegAddr);
    if ((PortStatus & 0x0F) == 0x4) {
      break;
    }
    SilFchStall(10);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitPortClearFre
 *
 * @brief Clears PxCMD.FRE
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortNum    Sata Port to put in offline.
 *
 */
static
void
FchSataInitPortClearFre (
  uint32_t DieBusNum,
  uint32_t Controller,
  uint32_t PortNum
  )
{
  uint32_t Retry;
  uint32_t PortCmd;
  uint32_t SataRegAddr;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0287 + PortNum * 0x80;
  PortCmd = xUSLSmnRead(0, DieBusNum, SataRegAddr);
  if (PortCmd & BIT_32(4)) {                                                  // PxCMD.FRE == 1
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFFFFFFEF, 0x0);       // PxCMD.FRE == 0
    for (Retry = 0; Retry < MAX_RETRY_NUM; Retry++) {
      PortCmd = xUSLSmnRead(0, DieBusNum, SataRegAddr);
      if ((PortCmd & BIT_32(14)) == 0x0) {
        break;
      }
      SilFchStall(1);
    }
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataShutdownUnconnectedSataPortClock
 *
 * @brief Shutdowns unconnected Sata port clock
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataShutdownUnconnectedSataPortClock (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint8_t  PortNumByte;
  uint8_t  PortSataStatusByte;
  uint8_t  NumOfPorts;
  uint8_t  FchSataClkAutoOff;
  uint32_t PortStatusDword;
  uint32_t SmnAddr;

  NumOfPorts = 0;
  FchSataClkAutoOff = (uint8_t) FchSata[Controller].SataClkAutoOff;
  /*
   * Enable SATA auto clock control by default
   */
  if (FchSataClkAutoOff) {
    for (PortNumByte = 0; PortNumByte < SATA_PORT_NUM; PortNumByte++) {
      SmnAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0288 + PortNumByte * 0x80;
      PortStatusDword = xUSLSmnRead(0, DieBusNum, SmnAddr);
      PortSataStatusByte = (uint8_t) (PortStatusDword & 0xFF);
      /*
       * Shutdown the clock for the port and do the necessary port reporting changes.
       * Error port status should be 1 not 3
       */
      if (((PortSataStatusByte & 0x0F) != 0x03) && (!((FchSata[Controller].SataEspPort) & (1 << PortNumByte)))) {
        FchSataInitPortClearFre(DieBusNum, Controller, PortNumByte);
        FchSataInitPortOffline(DieBusNum, Controller, PortNumByte);
        xUSLSmnReadModifyWrite(0,
          DieBusNum,
          SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP
          + 0x00,
          ~(uint32_t) (BIT_32(16) << PortNumByte),
          (BIT_32(16) << PortNumByte)
          );
        xUSLSmnReadModifyWrite(0,
          DieBusNum,
          SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
          + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_PI,
          ~(uint32_t) (1 << PortNumByte),
          00
          );
      }
    }
  }

  PortStatusDword = xUSLSmnRead(0,
    DieBusNum,
    SIL_RESERVED_0274
    + Controller * FCH_SMN_SATA_STEP + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_PI
    );
  PortSataStatusByte = (uint8_t) (PortStatusDword & 0xFF);

  /*
   * if all ports are in disabled state, report at least one port
   */
  if ((PortSataStatusByte & 0xFF) == 0) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_PI,
      0xFFFFFF00,
      0x01
      );
  }

  PortStatusDword = xUSLSmnRead(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_PI
    );
  PortSataStatusByte = (uint8_t) (PortStatusDword & 0xFF);

  for (PortNumByte = 0; PortNumByte < SATA_PORT_NUM; PortNumByte++) {
    if (PortSataStatusByte & (1 << PortNumByte)) {
      NumOfPorts++;
    }
  }

  if ( NumOfPorts == 0) {
    NumOfPorts = 0x01;
  }

  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_CAP,
    0xFFFFFFE0,
    (uint32_t) (NumOfPorts - 1)
    );
}

/**
 * FchSataInitMpssMap
 *
 * @brief Configures mechanical presence switch state mapping
 *
 * @param[in] DieBusNum  Bus Number of current Die
 *
 */
void
FchSataInitMpssMap (
  uint32_t DieBusNum
  )
{
  uint32_t  PortNum;
  for (PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0275 + PortNum * 0x20 + 0x8C,
      0xFFFFF8FF,
      (uint32_t) (PortNum << 8)
      );
  }
}

/**
 * FchSataGpioInitial
 *
 * @brief Initializes Sata GPIO function procedure
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataGpioInitial (
  uint32_t DieBusNum,
  uint32_t Controller
  )
{
  uint32_t             FchSataBarRegDword;

  // 1.Set SGPIO PAD
  FchSataSetGpioPad(DieBusNum);

  /*
   * 2.Support 8 device on 1 SGPIO stream
   * set SPIO_TgtSpt8dev = 0x1, support 8 device on 1 SGPIO stream
   */
  xUSLSmnReadModifyWrite(0, DieBusNum, SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP, ~BIT_32(3), BIT_32(3));

  /*
   * 3.Enable SGPIO in Enclosure Management set
   * Program sata_ahci :: FCh[27] =1
   */
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SIL_RESERVED_0340,
    ~BIT_32(27),
    BIT_32(27)
    );

  /*
   * 4.Set SGPIO enable
   * 4.2.3 SGPIO Initialization for Initiator0
   * a. Program GPIO_AMD_0.ISEL=0
   * Program sata_ahci::506h = 0xC0
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0xC0
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x01
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x01
    );
  // Program ISEL=0, sata_ahci::50Ch = 0x00000020 (ISEL=0, GN_AT=1(default), BSY_BYPEN= 0)
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00,
    0x00000020
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);

  /* b. Configure GPIO_TX[0]
   * Program sata_ahci::506h = 0x03
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0x03
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x01
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x01
    );
  // Program sata_ahci::50Ch = 0xA0A0A0A0 (ACT=0x5 SOF, LOC=0x0, ERR=0x0)
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00,
    0xA0A0A0A0
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);

  /*
   * c. Configure GPIO_CFG[1:0] (enable SGPIO initiator0 and activity stretch settings)
   * Program sata_ahci::506h = 0x00
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0x00
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x02
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x02
    );
  // Program sata_ahci::50Ch = 0x00800000
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00000000,
    BIT_32(23)
    );
  // Program sata_ahci::510h = 0x0F0F3700
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x510,
    0x00000000,
    0x0F0F3700
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);

  /* 4.2.4 SGPIO Initialization for Initiator1
   * a. Program GPIO_AMD_0.ISEL=1
   * Program sata_ahci::506h = 0xC0
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0xC0
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x01
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x01
    );
  // Program ISEL=0, sata_ahci::50Ch = 0x00000021 (ISEL=1, GN_AT=1(default), BSY_BYPEN= 0)
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00,
    0x00000021
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);

  /*
   * b. Configure GPIO_TX[0]
   * Program sata_ahci::506h = 0x03
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0x03
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x01
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x01
    );
  // Program sata_ahci::50Ch = 0xA0A0A0A0 (ACT=0x5 SOF, LOC=0x0, ERR=0x0)
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00,
    0xA0A0A0A0
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);

  /*
   * c. Configure GPIO_CFG[1:0] (enable SGPIO initiator1 and activity stretch settings)
   * Program sata_ahci::506h = 0x00
   */
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x506,
    0x00,
    0x00
    );
  // Program sata_ahci::507h = 0x00
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x507,
    0x00,
    0x00
    );
  // Program sata_ahci::508h = 0x02
  xUSLSmnReadModifyWrite8(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x508,
    0x00,
    0x02
    );
  // Program sata_ahci::50Ch = 0x00800000
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x50C,
    0x00000000,
    BIT_32(23)
    );
  // Program sata_ahci::510h = 0x0F0F3700
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + 0x510,
    0x00000000,
    0x0F0F3700
    );
  // Program Sata_ahci::20h[8] = 1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
    + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL,
    ~(uint32_t) (BIT_32(8)),
    BIT_32(8)
    );
  do {
    FchSataBarRegDword = xUSLSmnRead(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
      + SYSHUB_SATA_AHCI_SATA_AHCI_GHC_EM_CTL
      );
  } while ( FchSataBarRegDword & BIT_32(8));
  SilFchStall(5000);
}

/**
 * FchSataControllerSetPortGenMode
 *
 * @brief Sets Sata port mode (each) for Gen1/Gen2/Gen3
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortMode   Sata Port Mode (Gen1/2/3) per port.
 * @param[in] FchSata    Fch SATA configuration structure pointer.
 *
 */
void
FchSataControllerSetPortGenMode (
  uint32_t DieBusNum,
  uint32_t Controller,
  uint16_t PortMode
  )
{
  uint16_t PortNumByte;
  uint16_t PortModeByte;
  uint16_t SataPortMode;

  SataPortMode = PortMode;
  PortNumByte = 0;

  while ( PortNumByte < SATA_PORT_NUM ) {
    PortModeByte = (uint16_t) (SataPortMode & 3);
    if ((PortModeByte == BIT_32(0)) || (PortModeByte == BIT_32(1))) {
      if ( PortModeByte == BIT_32(0)) {
        xUSLSmnReadModifyWrite(0,
          DieBusNum,
          SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
          + SIL_RESERVED_0289 + PortNumByte * 0x80,
          0xFFFFFF0F,
          0x10
          );
      }
      if ( PortModeByte == BIT_32(1)) {
        xUSLSmnReadModifyWrite(0,
          DieBusNum,
          SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
          + SIL_RESERVED_0289 + PortNumByte * 0x80,
          0xFFFFFF0F,
          0x20
          );
      }
      xUSLSmnReadModifyWrite(0,
        DieBusNum,
        SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
        + SIL_RESERVED_0289 + PortNumByte * 0x80,
        0xFFFFFFFF,
        0x01
        );
    }
    SataPortMode >>= 2;
    PortNumByte++;
  }

  SilFchStall(1000);
  SataPortMode = PortMode;
  PortNumByte = 0;

  while ( PortNumByte < SATA_PORT_NUM ) {
    PortModeByte = (uint16_t) (SataPortMode & 3);
    if ((PortModeByte == BIT_32(0)) || (PortModeByte == BIT_32(1))) {
      xUSLSmnReadModifyWrite(0,
        DieBusNum,
        SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP
        + SIL_RESERVED_0289 + PortNumByte * 0x80,
        0xFFFFFFFE,
        0x00
        );
    }
    PortNumByte++;
    SataPortMode >>= 2;
  }
}

/**
 * FchSataSetPortGenMode
 *
 * @brief Sets Sata port mode (each) for Gen1/Gen2/Gen3
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataSetPortGenMode (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint16_t SataPortMode;
  SataPortMode = (uint16_t) FchSata[Controller].SataPortMd;
  FchSataControllerSetPortGenMode(DieBusNum, Controller, SataPortMode);
}

/**
 * FchSataInitEnableErr
 *
 * @brief Enables error reporting
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitEnableErr (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t PortNum;
  uint32_t SataRegAddr;
  bool     FchD2HFifoParityErr;
  bool     FchH2DFifoParityErr;
  bool     FchContextMemoryParityErr;
  bool     FchSataRasSupport;

  FchSataRasSupport = FchSata[Controller].SataRasSupport;
  FchD2HFifoParityErr = true;          //Optional
  FchH2DFifoParityErr = true;          //Optional
  FchContextMemoryParityErr = true;    //Optional

  if (FchSataRasSupport) {
    // (Optional) Enable Report D2H FIFO Parity Error to System for each port.
    if (FchD2HFifoParityErr) {
      for (PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNum * 0x20 + 0x88;
        xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~BIT_32(4), BIT_32(4));
      }
    }
    // (Optional) Enable Report H2D FIFO Parity Error to System for each port
    if (FchH2DFifoParityErr) {
      for ( PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNum * 0x20 + 0x88;
        xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~BIT_32(5), BIT_32(5));
      }
    }
    // (Optional) Enable Report Context Memory Parity Error to System for each port
    if (FchContextMemoryParityErr) {
      for ( PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + PortNum * 0x20 + 0x88;
        xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~BIT_32(6), BIT_32(6));
      }
    }
  }

  // Clear error status
  for (PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++) {
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0290 + PortNum * 0x80;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFFFFFFFF, 0xFFFFFFFF);
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0286 + PortNum * 0x80;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFFFFFFFF, 0x00);
  }
}

/**
 * FchSataInitEsata
 *
 * @brief Enables eSATA port
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitEsata (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t SataRegAddr;

  /*
   * SATA ESP port setting
   * These config bits are set for SATA driver to identify which ports are external SATA ports and need to
   * support hotplug. If a port is set as an external SATA port and need to support hotplug, then driver will
   * not enable power management (HIPM & DIPM) for these ports.
   */
  if (FchSata[Controller].SataEspPort != 0) {
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0341;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~(FchSata[Controller].SataEspPort), 0);
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFF00FFFF, (FchSata[Controller].SataEspPort << 16));
    /*
     * External SATA Port Indication Registers
     * If any of the ports was programmed as an external port, HCAP.SXS should also be set
     */
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0340;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(20)), BIT_32(20));
    // Enable  eSATA PHY power-saving when no device presented.
    SataRegAddr = SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x10;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(29)), BIT_32(29));
  } else {
    /*
     * External SATA Port Indication Registers
     * If any of the ports was programmed as an external port, HCAP.SXS should also be set (Clear for no ESP port)
     */
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0341;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, 0xFF00FF00, 0x00);
    SataRegAddr = SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0340;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(20)), 0x00);
    SataRegAddr = SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x10;
    xUSLSmnReadModifyWrite(0, DieBusNum, SataRegAddr, ~(uint32_t) (BIT_32(29)), 0);
  }
}

/**
 * FchSataInitRsmuCtrl
 *
 * @brief Configures Sata RSMU control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitRsmuCtrl (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  // SATA controller operates in maximum Gen2
  if (FchSata[Controller].SataSetMaxGen2) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(2),
      BIT_32(2)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(2),
      0x00
      );
  }

  // Sata Target Support 8 devices function
  if (FchSata[Controller].SataTargetSupport8Device) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(3),
      BIT_32(3)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(3),
      0x00
      );
  }

  // Sata Generic Mode setting
  if (FchSata[Controller].SataDisableGenericMode) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(1),
      BIT_32(1)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(1),
      0x00
      );
  }

  // OOB Detection Enhancement
  if (FchSata[Controller].SataOobDetectionEnh) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(24),
      BIT_32(24)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(24),
      0x00
      );
  }
}

/**
 * FchSataInitCtrlReg
 *
 * @brief Configures Sata control register setting
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitCtrlReg (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t            AndMaskDword;
  uint32_t            OrMaskDword;
  uint16_t            FchSataAggrLinkPmCap;
  uint16_t            FchSataPortMultCap;
  uint16_t            FchSataPscCap;
  uint16_t            FchSataSscCap;
  uint16_t            FchSataFisBasedSwitching;
  uint16_t            FchSataCccSupport;
  uint16_t            FchSataAhciEnclosureManagement;
  FCH_SATA_XFER_TABLE *FchSataXfer;

  if (SilGetCommon2RevXferTable(SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }

  FchSataAggrLinkPmCap = (uint16_t) FchSata[Controller].SataAggrLinkPmCap;
  FchSataPortMultCap = (uint16_t) FchSata[Controller].SataPortMultCap;
  FchSataPscCap = (uint16_t) FchSata[Controller].SataPscCap;
  FchSataSscCap = (uint16_t) FchSata[Controller].SataSscCap;
  FchSataFisBasedSwitching = (uint16_t) FchSata[Controller].SataFisBasedSwitching;
  FchSataCccSupport = (uint16_t) FchSata[Controller].SataCccSupport;
  FchSataAhciEnclosureManagement = (uint16_t) FchSata[Controller].SataAhciEnclosureManagement;

  AndMaskDword = 0;
  OrMaskDword = 0;

  if (!FchSataPortMultCap) {
    AndMaskDword |= BIT_32(12);
  }

  if (FchSataFisBasedSwitching) {
    OrMaskDword |= BIT_32(10);
  } else {
    AndMaskDword |= BIT_32(10);
  }

  if (FchSataAggrLinkPmCap) {
    OrMaskDword |= BIT_32(11);
  } else {
    AndMaskDword |= BIT_32(11);
  }

  if (FchSataPscCap) {
    OrMaskDword |= BIT_32(1);
  } else {
    AndMaskDword |= BIT_32(1);
  }

  if ( FchSataSscCap ) {
    OrMaskDword |= BIT_32(26);
  } else {
    AndMaskDword |= BIT_32(26);
  }

  // Disabling CCC (Command Completion Coalescing) support.
  if (FchSataCccSupport) {
    OrMaskDword |= BIT_32(19);
  } else {
    AndMaskDword |= BIT_32(19);
  }

  if (FchSataAhciEnclosureManagement) {
    OrMaskDword |= BIT_32(27);
  } else {
    AndMaskDword |= BIT_32(27);
  }

  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0274 + SIL_RESERVED_0340
    + Controller * FCH_SMN_SATA_STEP,
    ~AndMaskDword,
    OrMaskDword
    );

  if (FchSataFisBasedSwitching) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274 + SIL_RESERVED_0341
      + Controller * FCH_SMN_SATA_STEP,
      0x00FFFFFF,
      0xFF000000
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274 + SIL_RESERVED_0341
      + Controller * FCH_SMN_SATA_STEP,
      0x00FFFFFF,
      0x00
      );
  }

  if (FchSata[Controller].BiosOsHandOff == 1) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274
      + SYSHUB_SATA_AHCI_SATA_AHCI_HBA_CAP_EXT + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(0),
      BIT_32(0)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274
      + SYSHUB_SATA_AHCI_SATA_AHCI_HBA_CAP_EXT + Controller * FCH_SMN_SATA_STEP,
      ~BIT_32(0),
      0x00
      );
  }

  /*
   * BIT_32(2) : Enable dynamic blink clock gating
   * BIT_32(14): Enable BlcClkGated gating
   * BIT_32(15): Enable PciClkGated gating
   * BIT_32(8) & BIT_32(9) Disable performance enhancement
   * BIT_32(25)
   */
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x10,
    ~(uint32_t) (BIT_32(2) + BIT_32(8) + BIT_32(9) + BIT_32(14) + BIT_32(15) + BIT_32(25)),
    BIT_32(2) + BIT_32(14) + BIT_32(15) + BIT_32(25)
    );

  /*
   * Program sata_slor :: 0x14[8:5] = 0xF
   * Program sata_slor :: 0x14[30] = 1
   */
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x14,
    ~(uint32_t) (BIT_32(5) + BIT_32(6) + BIT_32(7) + BIT_32(8) + BIT_32(30)),
    BIT_32(5) + BIT_32(6) + BIT_32(7) + BIT_32(8) + BIT_32(30)
    );
  // Disable Prefetch In Ahci Mode
  if (FchSata[Controller].SataAhciDisPrefetchFunction) {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x00,
      ~BIT_32(13),
      BIT_32(13)
      );
  } else {
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x00,
      ~BIT_32(13),
      0x00
      );
  }

  FchSataXfer->FchSataInitMsi(DieBusNum, Controller, FchSata);
  // Shutdown ports
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x00,
    0xFF00FFFF,
    (FchSata[Controller].SataPortPower << 16)
    );

  // UBTS610116 - Program sata_slor :: 0x04[22] = 0x1
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP + 0x04,
    ~(uint32_t) (BIT_32(22)),
    BIT_32(22)
    );
}

/**
 * FchSataRegsInitLateProgram
 *
 * @brief Initializes Sata Control register setting (Late)
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataRegsInitLateProgram (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchSataInitMmc(DieBusNum, Controller, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitLate
 *
 * @brief Prepares SATA controller for OS boot
 *
 * @param[in] FchSata    Fch Sata configuration structure pointer.
 *
 */
void FchSataInitLate (
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t SataController;
  for (SataController = 0; SataController < SATA_CONTROLLER_NUM; SataController++) {
    if (FchSata[SataController].SataEnable) {
      FchSataEnableWriteAccess(0, SataController);
      if ((FchSata[SataController].SataClass == SataAhci7804) || (FchSata[SataController].SataClass == SataAhci )) {
        FchSataAhciInitLate(0, SataController, FchSata);
      }
      if ( FchSata[SataController].SataClass == SataRaid) {
        FchSataRaidInitLate(0, SataController);
      }
      FchSataRegsInitLateProgram(0, SataController, FchSata);
      FchSataDisableWriteAccess(0, SataController);
    }
  }
}

/**
 * FchSataRaidInitLate
 *
 * @brief Prepares SATA Raid controller to boot to OS
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataRaidInitLate (
  uint32_t DieBusNum,
  uint32_t Controller
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataAhciInitLate
 * @brief Prepare SATA AHCI controller to boot to OS.
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 *
 */
void
FchSataAhciInitLate (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchSataShutdownUnconnectedSataPortClock(DieBusNum, Controller, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataPrePcieInit
 *
 * @brief Configures Sata controller during power-on
 *
 * @param FchSata FCH Sata configuration structure pointer.
 *
 */
void
FchSataPrePcieInit (
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchSataInitReset(0, FchSata);
  FchSataInitEnv(FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataSetIrqIntResource
 *
 * @brief Configures SATA IRQ/INT# resources
 *
 *
 */
void
FchSataSetIrqIntResource (
  void
  )
{
  uint8_t   ValueByte;
  // IRQ14/IRQ15 come from IDE or SATA
  ValueByte = 0x08;
  xUSLIoWrite8(FCH_IO_PCI_INTR_INDEX, ValueByte);
  ValueByte = xUSLIoRead8(FCH_IO_PCI_INTR_DATA);
  ValueByte = ValueByte & 0x0F;
  ValueByte = ValueByte | 0xF0;
  xUSLIoWrite8(FCH_IO_PCI_INTR_DATA, ValueByte);
}

/**
 * FchSataEnableSataMac
 *
 * @brief Enables Sata Controller
 *
 * @param DieBusNum  IOHC bus number on current Die.
 * @param Controller Sata controller number.
 *
 */
void
FchSataEnableSataMac (
  uint32_t           DieBusNum,
  uint32_t           Controller,
  FCHSATA_INPUT_BLK  *FchSata
  )
{
  uint8_t Port;
  uint8_t RxPolarity;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  RxPolarity = FchSata[Controller].SataRxPolarity;

  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0292 + Controller * FCH_SMN_SATA_STEP,
    0xFFFFFFFE,
    0
    );

  // RxPolarity
  for (Port = 0; Port < SATA_PORT_NUM; Port++) {
    if (RxPolarity & (1 << Port)) {
      xUSLSmnReadModifyWrite(0,
        DieBusNum,
        SIL_RESERVED_0275 + Controller * FCH_SMN_SATA_STEP +
        Port * 0x20 + 0x80,
        0xFFFFFFDF,
        BIT_32(5)
        );
    }
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataEnableSataStaggeredSpinup
 *
 * @brief Enables Sata Staggered Spin-up feature
 *
 * @param DieBusNum   IOHC bus number on current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch SATA configuration structure pointer.
 *
 */
void
FchSataEnableSataStaggeredSpinup (
  uint32_t            DieBusNum,
  uint32_t            Controller,
  FCHSATA_INPUT_BLK   *FchSata
  )
{
  uint32_t     PortNum;
  PortNum = 0;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  UNUSED(FchSata);

  if (FchSata->SataStaggeredSpinupEnable) {
    // 1. Program cfg_cap_sss bit (Sata rsmu0x00[7]) to 1b
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0292 + (Controller * FCH_SMN_SATA_STEP),
      ~(BIT_32(7)),
      BIT_32(7)
      );

    // 2. Write 0b to PxCmd.SUD for each port
    for (PortNum = 0; PortNum < SATA_PORT_NUM; PortNum++) {
      xUSLSmnReadModifyWrite(0,
        DieBusNum,
        SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP +
        SIL_RESERVED_0287 + PortNum * 0x80,
        ~(BIT_32(1)),
        0
        );
    }
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitEnableSata
 *
 * @brief Enables Sata Controller
 *
 * @param DieBusNum   IOHC bus number on current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch SATA configuration structure pointer.
 *
 */
void
FchSataInitEnableSata (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FchSataEnableSataMac(DieBusNum, Controller, FchSata);
  FchSataEnableSataStaggeredSpinup(DieBusNum, Controller, FchSata);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitMmc
 *
 * @brief Configures Sata MSI MMC setting
 *
 * @param DieBusNum   Bus Number of current Die.
 * @param Controller  Sata controller number.
 * @param FchSata     Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitMmc (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  uint32_t  SataPciMmc;
  // Sata MMC programming
  SataPciMmc = 0x04000000;
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SIL_RESERVED_0291 + Controller * FCH_SMN_SATA_STEP,
    0xF8FFFFFF,
    SataPciMmc
    );
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitHideNbifDev1Pci
 *
 * @brief Disables Nbio Sata Controller PCI configuration space
 *
 * @param DieBusNum IOHC bus number on current Die.
 * @param NbioSata  NbioSata controller number.
 *
 */
void
FchSataInitHideNbifDev1Pci (
  uint32_t DieBusNum,
  uint32_t NbioSata
  )
{
  uint32_t SataBridgeAddress;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  SataBridgeAddress = SIL_RESERVED_0284;
  SataBridgeAddress += SIL_RESERVED_0285 * NbioSata;
  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "Hide Nbif Dev1 Pci Bridge. DieBusNum 0x%x, \
      nBIF Sata %d, SMN address 0x%x set 0x7\n",
    DieBusNum,
    NbioSata,
    SataBridgeAddress
    );
  xUSLSmnReadModifyWrite(0,
    DieBusNum,
    SataBridgeAddress,
    ~(uint32_t) (0x7),
    0x7
    );
}

/**
 * FchSataInitReset
 *
 * @brief Configures Sata controller during Power-On
 *
 * @param DieBusNum  IOHC bus number on current Die.
 * @param FchSata    Fch SATA configuration structure pointer.
 *
 */
void
FchSataInitReset (
  uint32_t            DieBusNum,
  FCHSATA_INPUT_BLK   *FchSata
  )
{
  uint32_t SataController;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  // Enable the SATA controller.
  for (SataController = 0; SataController < SATA_CONTROLLER_NUM; SataController++) {
    if (FchSata[SataController].SataEnable) {
      FchSataInitEnableSata(DieBusNum, SataController, FchSata);
    } else {
      FchSataInitDisableSata(DieBusNum, SataController);
    }
  }
  // check if Sata0 and Sata1 are both disabled
  if ((!FchSata[0].SataEnable) && (!FchSata[1].SataEnable)) {
    FchSataInitHideNbifDev1Pci(DieBusNum, 0);
  }
  // check if Sata2 and Sata3 are both disabled
  if ((!FchSata[2].SataEnable) && (!FchSata[3].SataEnable)) {
    FchSataInitHideNbifDev1Pci(DieBusNum, 2);
  }
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataInitEnv
 *
 * @brief Configures SATA controller
 *
 * @param FchSata Fch Sata configuration structure pointer.
 *
 */
void
FchSataInitEnv (
  FCHSATA_INPUT_BLK *FchSata
  )
{
  uint32_t SataController;

  FCH_SATA_XFER_TABLE  *FchSataXfer;
  if (SilGetCommon2RevXferTable(SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }

  for (SataController = 0; SataController < SATA_CONTROLLER_NUM; SataController++) {
    if (FchSata[SataController].SataEnable) {
      FchSataEnableWriteAccess(0, SataController);
      FchSataInitEnvProgram(0, SataController, FchSata);
      // Call Sub-function for each Sata mode
      if ((FchSata[SataController].SataClass == SataAhci7804) ||
        (FchSata[SataController].SataClass == SataAhci)) {
        FchSataXfer->FchSataInitEnvAhci(0, SataController, FchSata);
      }
      if (FchSata[SataController].SataClass == SataRaid) {
        FchSataXfer->FchSataInitEnvRaid(0, SataController, FchSata);
      }
      FchSataDisableWriteAccess(0, SataController);
      FchSataXfer->FchSataAutoShutdown(0, SataController, FchSata);
    }
  }

  // check if Sata0 and Sata1 are both disabled
  if ((!FchSata[0].SataEnable) && (!FchSata[1].SataEnable)) {
    FchSataInitHideNbifDev1Pci(0, 0);
  }

  // check if Sata2 and Sata3 are both disabled
  if ((!FchSata[2].SataEnable) && (!FchSata[3].SataEnable)) {
    FchSataInitHideNbifDev1Pci(0, 3);
  }
  FchSataSetIrqIntResource ();
}

/**
 * FchSataInitEnvProgram
 *
 * @brief Initialized Sata controllers before PCI scan
 *
 * @param[in] DieBusNum - Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchSata - Fch Sata configuration structure pointer
 *
 */
void
FchSataInitEnvProgram (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FCH_SATA_XFER_TABLE  *FchSataXfer;
  if (SilGetCommon2RevXferTable(SilId_FchSata, (void **)(&FchSataXfer)) != SilPass) {
    return;
  }

  FchSataInitRsmuCtrl(DieBusNum, Controller, FchSata);
  FchSataInitCtrlReg(DieBusNum, Controller, FchSata);
  FchSataInitEsata(DieBusNum, Controller, FchSata);
  FchSataInitEnableErr(DieBusNum, Controller, FchSata);
  FchSataSetPortGenMode(DieBusNum, Controller, FchSata);

  FchSataXfer->FchSataSetBistLComplianceMode(DieBusNum, Controller, FchSata);// Need to be checked

  if (FchSata[Controller].SataSgpio0) {
    FchSataGpioInitial(DieBusNum, Controller);
  } else {
    // If Sgpio is not enable, set to MPIO mode
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RESERVED_0274 + Controller * FCH_SMN_SATA_STEP + SIL_RESERVED_0343,
      ~(BIT_32(28)),
      BIT_32(28)
      );
  }

  FchSataXfer->FchSataInitDevSlp(DieBusNum, Controller, FchSata);

  if (Controller == 0) {
    FchSataInitMpssMap(DieBusNum);
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSataSetGpioPad
 *
 * @brief Sets SATA GPIO Setting
 *
 * @param[in] DieBusNum - Bus Number of current Die.
 *
 */
void
FchSataSetGpioPad (
  uint32_t DieBusNum
  )
{
  // Set it via MMIO when access it on Master FCH.
  if ( DieBusNum == 0 ) {
    // Set to SGPIO Pin
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX0_GPIO256),
      0x03,
      0x01
      );  // Set IOMUXx100 to SGPIO0_CLK
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX1_GPIO257),
      0x03,
      0x01
      );  // Set IOMUXx101 to SGPIO1_CLK
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX2_GPIO258),
      0x03,
      0x01
      ); // Set IOMUXx102 to SGPIO2_CLK
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX3_GPIO259),
      0x03,
      0x01
      );  // Set IOMUXx103 to SGPIO3_CLK
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX4_GPIO260),
      0x03,
      0x00
      );  // Set IOMUXx104 to SGPIO_DATAOUT
    xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_IOMUX5_GPIO261),
      0x03,
      0x00
      );  // Set IOMUXx105 to SGPIO_LOAD
    // Make SPIO pins pull up enable
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_AGPIO256_SGPIO0_CLK),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO0_CLK
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE +
      FCH_RMTGPIO_AGPIO257_SGPIO1_CLK_CLK_REQ01_L),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO1_CLK
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE +
      FCH_RMTGPIO_AGPIO258_SGPIO2_CLK_CLK_REQ02_L),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO2_CLK
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_AGPIO259_SGPIO3_CLK),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO3_CLK
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_SGPIO_DATAOUT_AGPIO260),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO_DATAOUT
    xUSLMemReadModifyWrite32((void *)(size_t)(ACPI_MMIO_BASE + REMOTE_GPIO_BASE + FCH_RMTGPIO_SGPIO_LOAD_AGPIO261),
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO_LOAD
  } else {
    // Set to SGPIO Pin
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX0_GPIO256,
      0x03,
      0x01
      );  // Set IOMUXx100 to SGPIO0_CLK
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX1_GPIO257,
      0x03,
      0x01
      );  // Set IOMUXx101 to SGPIO1_CLK
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX2_GPIO258,
      0x03,
      0x01
      );  // Set IOMUXx102 to SGPIO2_CLK
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX3_GPIO259,
      0x03,
      0x01
      );  // Set IOMUXx103 to SGPIO3_CLK
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX4_GPIO260,
      0x03,
      0x00
      );  // Set IOMUXx104 to SGPIO_DATAOUT
    xUSLSmnReadModifyWrite8(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_IOMUX5_GPIO261,
      0x03,
      0x00
      );  // Set IOMUXx105 to SGPIO_LOAD

    // Make SPIO pins pull up enable
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_AGPIO256_SGPIO0_CLK,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO0_CLK
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_AGPIO257_SGPIO1_CLK_CLK_REQ01_L,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO1_CLK
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_AGPIO258_SGPIO2_CLK_CLK_REQ02_L,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO2_CLK
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_AGPIO259_SGPIO3_CLK,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO3_CLK
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_SGPIO_DATAOUT_AGPIO260,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO_DATAOUT
    xUSLSmnReadModifyWrite(0,
      DieBusNum,
      SIL_RSVD_ADDR_02D02200 + FCH_RMTGPIO_SGPIO_LOAD_AGPIO261,
      ~BIT_32(21),
      BIT_32(20)
      );  // Set SGPIO_LOAD
  }
}
