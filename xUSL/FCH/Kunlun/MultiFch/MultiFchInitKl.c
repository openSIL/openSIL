/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchInitKl.c
 * @brief Kunlun FCH multi-die silicon init functions
 *
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchReg.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/MultiFch/MultiFch.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include <FCH/Common/FchCore/FchAb/FchAb.h>
#include <FCH/Kunlun/MultiFch/MultiFchCmn2Kl.h>
#include <FCH/FchSata-api.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <FCH/Common/FchCore/FchSata/FchSata.h>

#define SIL_RESERVED_0375  7

/**
 * FchMultiFchGetFchRbIndexOnDieKl
 * @brief Returns FCH root bridge index on IOD
 *
 * @param[out]  RbIndex 32-bit pointer to return FCH root bridge index on IOD
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchGetFchRbIndexOnDieKl (
  uint32_t *RbIndex
  )
{
  SIL_STATUS  Status;
  Status = FchMultiFchGetFchRbIndexOnDieCmn(RbIndex);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }
  return SilPass;
}

/**
 * FchMultiFchInitAbKl
 * @brief Initiates AB component on secondary FCH modules
 *
 * @param[in]  SocketNum        Socket number
 * @param[in]  DieNum           IOD number
 * @param[in]  RbNum            FCH root bridge number on the IOD
 * @param[in]  AcpiMmioBaseAddr ACPI MMIO base address for the input Socket, Die, and Rb combination
 * @param[in]  IohcBusNumber    Iohc bus number of the target root bridge
 * @param[in]  HasFchModule     Indicates whether the root bridge has FCH module or not
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchInitAbKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  )
{
  SIL_STATUS          Status;
  uint32_t            Socket0IoBaseAddr;
  uint32_t            Socket0IoBaseAddrLimit;
  FCHAB_INPUT_BLK    *FchAbDataBlk;
  FCHCLASS_INPUT_BLK *FchDataBlk;

  FchDataBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilId_FchClass, 0);
  if (FchDataBlk == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH data block not found.\n");
    return SilNotFound;
  }
  FchAbDataBlk = (FCHAB_INPUT_BLK *) xUslFindStructure(SilId_FchAb, 0);
  if (FchAbDataBlk == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH AB data block not found.\n");
    return SilNotFound;
  }
  if ((SocketNum == 1) && (DieNum == 0)) {

    // Read current IoBaseAddr/IoBaseAddrLimit on socket0
    Status = FchMultiFchReadIoBaseAddress(0, 0, &Socket0IoBaseAddr, &Socket0IoBaseAddrLimit);
    if (Status != SilPass) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchReadIoBaseAddress failed (Status=0x%x).\n", Status);
      assert(Status == SilPass);
      return Status;
    }

    // Note: We are redirecting IO cycles issued by socket0 to the FCH module on socket 1. Hence, the
    // FabricId change on the IoBaseAddrLimit register.
    Status = FchMultiFchWriteIoBaseAddress(0,
      0,
      0x03,
      Socket0IoBaseAddrLimit | (1 << SIL_RESERVED_0375)
      );
    if (Status != SilPass) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchWriteIoBaseAddress failed (Status=0x%x).\n", Status);
      assert(Status == SilPass);
      return Status;
    }
    FchSecondaryFchInitAB(IohcBusNumber, AcpiMmioBaseAddr, FchAbDataBlk);

    // Set socket0 IoBaseAddr/IoBaseAddrLimit back to its original values
    Status = FchMultiFchWriteIoBaseAddress(0, 0, Socket0IoBaseAddr, Socket0IoBaseAddrLimit);
    if (Status != SilPass) {
      assert(Status == SilPass);
      return Status;
    }
  }
  return SilPass;
}

/**
 * FchMultiFchInitUsbKl
 * @brief Initiates USB3/USB2 components on secondary FCH modules
 *
 * @param[in]  SocketNum        Socket number
 * @param[in]  DieNum           IOD number
 * @param[in]  RbNum            FCH root bridge number on the IOD
 * @param[in]  AcpiMmioBaseAddr ACPI MMIO base address for the input Socket, Die, and Rb combination
 * @param[in]  IohcBusNumber    Iohc bus number of the target root bridge
 * @param[in]  HasFchModule     Indicates whether the root bridge has FCH module or not
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchInitUsbKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  )
{
  if ((SocketNum == 1) && (DieNum == 0)) {
    FCHUSB_INPUT_BLK *FchUsbData;
    FchUsbData = (FCHUSB_INPUT_BLK *) xUslFindStructure(SilId_FchUsb, 0);
    if (FchUsbData == NULL) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH Usb data block not found.\n");
      return SilNotFound;
    }
    if (!(FchUsbData->Xhci2Enable)) {
      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchSecondaryFchInitUsb() XHCI2 disabled.\n");
    }
    if (!(FchUsbData->Xhci3Enable)) {
      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchSecondaryFchInitUsb() XHCI3 disabled.\n");
    }
    // Set Usb parameters for secondary FCH init.
    FchUsbData->DieBusNum = IohcBusNumber;
    FchUsbData->Xhci0Enable = FchUsbData->Xhci2Enable;
    FchUsbData->Xhci1Enable = FchUsbData->Xhci3Enable;
    FchUsbData->Xhci0DevRemovable = (uint32_t) (FchUsbData->Xhci0DevRemovable >> 4);
    FchUsbData->XhciUsb3PortDisable = (uint32_t) (FchUsbData->XhciUsb3PortDisable >> 4);
    FchUsbData->XhciUsb2PortDisable = (uint32_t) (FchUsbData->XhciUsb2PortDisable >> 4);
    FchUsbData->XhciOCpinSelect[0].Usb20OcPin = FchUsbData->XhciOCpinSelect[2].Usb20OcPin;
    FchUsbData->XhciOCpinSelect[0].Usb31OcPin = FchUsbData->XhciOCpinSelect[2].Usb31OcPin;
    FchUsbData->XhciOCpinSelect[1].Usb20OcPin = FchUsbData->XhciOCpinSelect[3].Usb20OcPin;
    FchUsbData->XhciOCpinSelect[1].Usb31OcPin = FchUsbData->XhciOCpinSelect[3].Usb31OcPin;
    FchXhciPrePcieInit(FchUsbData);
  }
  return SilPass;
}

/**
 * FchMultiFchInitSdKl
 * @brief Initiates SD component on secondary FCH modules
 *
 * @param[in]  SocketNum        Socket number
 * @param[in]  DieNum           IOD number
 * @param[in]  RbNum            FCH root bridge number on the IOD
 * @param[in]  AcpiMmioBaseAddr ACPI MMIO base address for the input Socket, Die, and Rb combination
 * @param[in]  IohcBusNumber    Iohc bus number of the target root bridge
 * @param[in]  HasFchModule     Indicates whether the root bridge has FCH module or not
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchInitSdKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  )
{
  uint32_t AcpiMmioBaseAddr32;
  if ((SocketNum == 1) && (DieNum == 0)) {
    AcpiMmioBaseAddr32 = (uint32_t)(AcpiMmioBaseAddr & 0xFFFFFFFF);
    xUSLMemReadModifyWrite8((void *) (size_t)(AcpiMmioBaseAddr32 + PMIO_BASE + SIL_RESERVED_0331), 0xFE, 0x00);
    xUSLMemReadModifyWrite8((void *) (size_t)(AcpiMmioBaseAddr32 + AOAC_BASE + FCH_AOAC_DEVCTRL_25), 0xF7, 0x00);
  }
  return SilPass;
}
