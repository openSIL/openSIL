/**
 * @file  FchXhciSn.c
 * @brief 9004 FCH XHCI Module functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <FCH/Common/FchHelper.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchCommonCfg.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include "FchXhciSn.h"
#include <CommonLib/SmnAccess.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/Fch.h>
#include <Utils.h>
#include <SMU/SmuIp2Ip.h>

/*
 * Compiler will generate warnings for unused formal parameters in function
 * declarations.  This is expected as some API function may not be implemented
 * for all versions of an IP.  Additionally, some version may have differing
 * implementations.  Suppress the warning here.
 */
#pragma warning(disable:4100)

static FCH_XHCI_XFER_TABLE mFchXhciXferSn = {
  .Header = {
    .Version                     = 1   // Version 1 = 9004 FCH
  },
  .FchInitPrePcieXhci            = FchInitPrePcieXhciSn,
  .FchInitPostPcieXhci           = FchInitPostPcieXhciSn,
  .FchInitPreOsXhci              = FchInitPreOsXhciSn,
  .FchGetSmcMsgIdUsbInit         = FchGetSmcMsgIdUsbInitSn,
  .FchGetSmcMsgIdUsbSxEntry      = FchGetSmcMsgIdUsbSxEntrySn,
  .FchGetSmcMsgIdUsbS3Exit       = FchGetSmcMsgIdUsbS3ExitSn,
  .FchGetSmcMsgIdUsbConfigUpdate = FchGetSmcMsgIdUsbConfigUpdateSn
};

/**
 * FchSNXhciSmuServiceUsbInit
 *
 * @brief Xhci SMU Service Request for UsbInit
 *
 * @details This routine is to send Usb INIT message to SMU to trigger USB
 * initizlization FSDL programming. It use parameter DW0 (SmuArg[0]) to tell
 * SMU/FSDL XHCI controller is enabled or not.
 *   DW0[0] - 0: XHCI0 is disabled; 1: XHCI0 is enabled.
 *   DW0[1] - 0: XHCI1 is disabled; 1: XHCI1 is enabled.
 *
 *
 * @param DieBusNum Bus Number on Current Die.
 * @param RequestId Request ID.
 *
 * @retval Status SMC_RESULT Bios-Smu Command response.
 *
 */
static SMC_RESULT
FchSNXhciSmuServiceUsbInit (
  uint32_t DieBusNum,
  FCH_USB  *FchUsbData
  )
{
  SMC_RESULT    Status;
  PCI_ADDR      PciAddress;
  uint32_t      SmuArg[6];
  SMU_IP2IP_API *SmuApi;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SMC_Result_Failed;

  if (SilGetIp2IpApi (SilId_SmuClass, (void **)&SmuApi) != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "Smu API not found!\n");
    assert (Status == SMC_Result_OK);
  }

  SmuApi->SmuServiceInitArguments (SmuArg);
  PciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);

  SmuArg[0] = 0;
  SmuArg[1] = 0;
  if (FchUsbData->Xhci0Enable) {
    SmuArg[0] |= BIT_32(0);
  }
  if (FchUsbData->Xhci1Enable) {
    SmuArg[0] |= BIT_32(1);
  }

  Status = SmuApi->SmuServiceRequest (PciAddress, BIOSSMC_MSG_USBINIT_SN, SmuArg, 0);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Status=%d.\n", Status);

  return Status;
}

/**
 * @brief FchSNUsbPortForceGen1  -  Port Force Gen1
 *
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNUsbPortForceGen1 (
  uint32_t DieBusNum,
  FCH_USB *FchUsbData
  )
{
  uint8_t    UsbPortForceGen1;

  UsbPortForceGen1  = FchUsbData->Usb3PortForceGen1;
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_INFO, "Overall UsbPortForceGen1 Parameter = 0x%x\n", UsbPortForceGen1);

  // Controller 0
  FchUsb3PortForceGen1 (DieBusNum,
    0,
    FCH_SN_XHCI_USB3_PORT_NUM,
    UsbPortForceGen1 & 0x3,
    FCH_SN_USB0_SMN_BASE
    );

  // Controller 1
  UsbPortForceGen1 = UsbPortForceGen1 >> 2;
  FchUsb3PortForceGen1 (DieBusNum,
    1,
    FCH_SN_XHCI_USB3_PORT_NUM,
    UsbPortForceGen1 & 0x3,
    FCH_SN_USB1_SMN_BASE
    );

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSNUsbOemUsb20PhyConfigure
 *
 * @brief USB 2.0 PHY Platform Configurationb
 *
 * @details This routine is to update USB 2.0 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. This SN program routine would check if
 * Customer wants to enable USB 2.0 PHY update and then call to PHY update
 * common routine to program each port.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static void
FchSNUsbOemUsb20PhyConfigure (
  uint32_t DieBusNum,
  void *PlatformConfigureTable
  )
{
  uint8_t  Port;
  FCH_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;

  FchUsbOemPlatformTable = (FCH_USB_OEM_PLATFORM_TABLE *)PlatformConfigureTable;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchUsbOemPlatformTable->Usb20PhyEnable == 0x00) {
    return;
  }

  // Controller 0
  for (Port = 0; Port < 2; Port++) {
    FchUsbOemUsb20PhyConfigure (
      DieBusNum,
      0,
      Port,
      &(FchUsbOemPlatformTable->Usb20PhyPort[Port]),
      FCH_SN_USB0_SMN_BASE
      );
  }

  // Controller 1
  for (Port = 2; Port < 4; Port++) {
    FchUsbOemUsb20PhyConfigure (
      DieBusNum,
      1,
      Port - 2,
      &(FchUsbOemPlatformTable->Usb20PhyPort[Port]),
      FCH_SN_USB1_SMN_BASE
      );
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchSNUsbOemUsb31PhyConfigure  -  USB 3.1 PHY Platform Configuration
 *
 * @details This routine is to update USB 3.1 PHY settings. Soc has default PHY
 * setting value. Customer may choose to modify the default PHY value according
 * to different board layout design. This SN program routine would check if
 * Customer wants to enable USB 3.1 PHY update and then call to PHY update
 * common routine to program each port.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static void FchSNUsbOemUsb31PhyConfigure (uint32_t DieBusNum, void *PlatformConfigureTable)
{
  uint8_t  Controller, Port;
  FCH_USB_OEM_PLATFORM_TABLE *FchUsbOemPlatformTable;

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FchUsbOemPlatformTable  = (FCH_USB_OEM_PLATFORM_TABLE *)PlatformConfigureTable;

  if (FchUsbOemPlatformTable->Usb31PhyEnable == 0x00) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "[FCH] Usb3.1 PHY override is not enabled\n");
    return;
  }

  for (Controller = 0; Controller < 2; Controller++) {
    for (Port = 0; Port < 2; Port++) {
      FchUsbOemUsb31PhyConfigurePortD10 (
        DieBusNum,
        Controller,
        Port,
        &(FchUsbOemPlatformTable->Usb31PhyPort[Controller * 2 + Port]),
        FCH_SN_USB0_SMN_BASE + (FCH_SN_USB_CNTL_STEP * Controller)
        );
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief FchSNXhciOemConfigure  -  Xhci OEM Platform Configuration
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] ptrPlatformConfigureTable Usb Oem Platform table pointer passed as input params.
 *
 */
static void
FchSNXhciOemConfigure (
  uint32_t DieBusNum,
  void *PlatformConfigureTable
  )
{
  FchSNUsbOemUsb20PhyConfigure (DieBusNum, PlatformConfigureTable);
  FchSNUsbOemUsb31PhyConfigure (DieBusNum, PlatformConfigureTable);
}

/**
 * FchSNXhciSparseMode
 *
 * @brief Enable Sparse Mode for each controller on Genoa
 *
 * @details Some devices are slow in responding to Control transfers.
 * Scheduling multiple transactions in one microframe/frame can cause these
 * devices to misbehave. If enabled, the host controller schedules each phase
 * of a Control transfer in different microframes/frames.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNXhciSparseMode (
    uint32_t DieBusNum,
    FCH_USB *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < 2; UsbController++) {
    FchXhciSparseMode(
      DieBusNum,
      UsbController,
      FchUsbData,
      FCH_SN_USB0_SMN_BASE + (FCH_SN_USB_CNTL_STEP * UsbController)
    );
  }

}

/**
 * FchSNXhciOCPolarity
 *
 * @brief Set Over-Current Pin input polarity
 *
 * @details This routine is to call common routine to program Over-current
 * input polarity configuration for each controller. There're 2 XHCI controller
 * on each dies of Genoa.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNXhciOCPolarity (
    uint32_t DieBusNum,
    FCH_USB *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < 2; UsbController++) {
    FchXhciOCPolarity(
      DieBusNum,
      UsbController,
      FchUsbData,
      FCH_SN_USB0_SMN_BASE + (FCH_SN_USB_CNTL_STEP * UsbController)
    );
  }

}

/**
 * FchSNXhciRasFeature
 *
 * @brief Xhci RAS Control
 *
 * @details This routine is to ecommon routine to enable USB ECC error report
 * for each controller on Genoa.
 *
 * @param[in] DieBusNum Bus Number on Current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNXhciRasFeature (
    uint32_t DieBusNum,
    FCH_USB *FchUsbData
  )
{
  uint32_t UsbController;

  for (UsbController = 0; UsbController < 2; UsbController++) {
    FchXhciRasFeature(
      DieBusNum,
      UsbController,
      FchUsbData,
      FCH_SN_USB0_SMN_BASE + (FCH_SN_USB_CNTL_STEP * UsbController)
    );
  }

}

/**
 * FchSNXhciDeviceRemovable
 *
 * @brief Xhci Device Removable Control
 *
 * @details This routine is to call common routine to program Device Removable
 * register for each controller and port on Genoa. This is to tell
 * driver if the device attached to a downstream port (SS port or 2.0 port) is
 * permanently attached or not.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNXhciDeviceRemovable (
  uint32_t DieBusNum,
  FCH_USB *FchUsbData
  )
{
  uint32_t DeviceRemovable;

  // Port0-1 : Controller0
  DeviceRemovable = FchUsbData->Xhci0DevRemovable;
  DeviceRemovable = DeviceRemovable & 0x00030003;
  FchXhciDeviceRemovable (
    DieBusNum,
    0,
    DeviceRemovable,
    FCH_SN_USB0_SMN_BASE
  );

  // Port2-3 : Controller1
  DeviceRemovable = FchUsbData->Xhci0DevRemovable;
  DeviceRemovable = DeviceRemovable >> 2;
  DeviceRemovable = DeviceRemovable & 0x00030003;
  FchXhciDeviceRemovable (
    DieBusNum,
    1,
    DeviceRemovable,
    FCH_SN_USB1_SMN_BASE
  );
}

/**
 * FchSNXhciOverCurrent
 * @brief Update Xhci OC Pin Mapping
 *
 * @details This routine is to call common routine to program which
 * Over-Current Pin is mapping to each USB port of Genoa.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNXhciOverCurrent (
  uint32_t DieBusNum,
  FCH_USB *FchUsbData
  )
{
  uint32_t OverCurrentMap;

  // Port0-1 : Controller0
  OverCurrentMap = (FchUsbData->XhciOCpinSelect[0].Usb20OcPin & 0xFF);
  OverCurrentMap |= ((FchUsbData->XhciOCpinSelect[0].Usb31OcPin & 0xFF) << 8);
  FchXhciOverCurrent (
    DieBusNum,
    0,
    4,
    OverCurrentMap,
    FCH_SN_USB0_SMN_BASE
  );

  // Port2-3 : Controller1
  OverCurrentMap = (FchUsbData->XhciOCpinSelect[1].Usb20OcPin & 0xFF);
  OverCurrentMap |= ((FchUsbData->XhciOCpinSelect[1].Usb31OcPin & 0xFF) << 8);
  FchXhciOverCurrent (
    DieBusNum,
    1,
    4,
    OverCurrentMap,
    FCH_SN_USB1_SMN_BASE
  );
}

/**
 * FchSNXhciPassParameter
 *
 * @brief Xhci Pass Parameters
 *
 * @details This routine is to call Genoa/SN specific functions to send
 * register update messages to SMU for USB initializaion.
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void FchSNXhciPassParameter (uint32_t DieBusNum, FCH_USB *FchUsbData)
{
  FCH_USB_OEM_PLATFORM_TABLE *PlatformUsbConfigureTable;
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  PlatformUsbConfigureTable = &(FchUsbData->OemUsbConfigurationTable);

  FchSNXhciOverCurrent (DieBusNum, FchUsbData);
  FchSNXhciDeviceRemovable (DieBusNum, FchUsbData);
  FchSNXhciRasFeature (DieBusNum, FchUsbData);
  FchSNXhciOCPolarity (DieBusNum, FchUsbData);
  FchSNXhciSparseMode (DieBusNum, FchUsbData);

  if ((PlatformUsbConfigureTable->Version_Major == 0x0D) &&
    (PlatformUsbConfigureTable->Version_Minor == 0x10)) { //Genoa USB D.10
    FCH_TRACEPOINT (SIL_TRACE_INFO, "[FCH]PlatformUsbConfigureTable (D.2) found!\n");
    FchSNXhciOemConfigure (DieBusNum, PlatformUsbConfigureTable);
  }

  FchSNUsbPortForceGen1 (DieBusNum, FchUsbData);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchSNXhciInitS3ExitProgram
 *
 * @brief Config Xhci controller during S3 Exit
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void FchSNXhciInitS3ExitProgram (uint32_t DieBusNum, FCH_USB *FchUsbData)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);
  FchSNXhciPassParameter (DieBusNum, FchUsbData);
  FchXhciSmuService (DieBusNum, BIOSSMC_MSG_USBS3EXIT_SN);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}

/**
 * FchSNXhciInitBootProgram
 *
 * @brief Config Xhci controller during Power-On
 *
 *
 * @param[in] DieBusNum  IOHC bus number on current Die.
 *
 */
static void FchSNXhciInitBootProgram (uint32_t DieBusNum, FCH_USB *FchUsbData)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "Bus 0x%x\n", DieBusNum);
  FchSNXhciPassParameter (DieBusNum, FchUsbData);
  FchSNXhciSmuServiceUsbInit (DieBusNum, FchUsbData);
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "Bus 0x%x\n", DieBusNum);
}

/************************************************************************
 * FchSNInitResetXhci
 *
 * @brief Initlize Xhci controller during early Power-On
 *
 * @param[in] FchDataBlock FCHCLASS_INPUT_BLK configuration structure pointer.
 * @param[in] FchUsbData Fch Usb configuration structure pointer.
 *
 */
static void
FchSNInitResetXhci (
  FCH_USB *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (FchUsbData->Xhci0Enable || FchUsbData->Xhci1Enable) {
    if (SilFchReadSleepType () == ACPI_SLPTYP_S3) {
         FchSNXhciInitS3ExitProgram (FchUsbData->DieBusNum, FchUsbData);
    } else {
      FchSNXhciInitBootProgram (FchUsbData->DieBusNum, FchUsbData);
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * FchSNXhciIohcPmeDisable
 *
 * @brief Enable or disable IOHC PME for USB function
 *
 *
 *
 * @param[in] DieBusNum Fch configuration structure pointer.
 * @param[in] PMEDis Fch configuration structure pointer.
 *
 */
static
void
FchSNXhciIohcPmeDisable (
  uint32_t DieBusNum,
  bool PMEDis
  )
{
  if (PMEDis) {
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SN_NBIO0_IOHC_NB_PCI_CTRL, ~BIT_32(4), BIT_32(4));
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SN_NBIO1_IOHC_NB_PCI_CTRL, ~BIT_32(4), BIT_32(4));
  } else {
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SN_NBIO0_IOHC_NB_PCI_CTRL, ~BIT_32(4), 0);
    xUSLSmnReadModifyWrite (0, DieBusNum, FCH_SN_NBIO1_IOHC_NB_PCI_CTRL, ~BIT_32(4), 0);
  }
}

/**
 * FchSNInitEnvUsbXhci
 * @brief Config XHCI controller before PCI enumeration
 *
 *
 * @param[in] Usb Fch Usb configuration structure pointer.
 *
 */
static void FchSNInitEnvUsbXhci(FCH_USB *FchUsbData)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  FchSNXhciIohcPmeDisable (FchUsbData->DieBusNum, true);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/************************************* LATE INIT ****************************/

/**
 * FchInitPrePcieXhciSn
 *
 * @brief Config FCH XHCI Module before PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPrePcieXhciSn (
  FCH_USB *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //
  FchSNInitResetXhci (FchUsbData);
  FchSNInitEnvUsbXhci (FchUsbData);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPostPcieXhciSn
 *
 * @brief Config FCH XHCI Module after PCI enumeration.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPostPcieXhciSn (
  FCH_USB *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchInitPreOsXhciSn
 *
 * @brief Config FCH XHCI Module before OS boot.
 *
 * @param FchHwAcpi Fch XHCI configuration structure pointer.
 *
 */
void
FchInitPreOsXhciSn (
  FCH_USB *FchUsbData
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  //
  // Common IP Init
  //

  //
  // Program specific Init
  //

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * FchGetSmcMsgIdUsbInitSn
 *
 * @brief Gey SMC Message ID for UsbInit Message
 *
 *
 */
uint32_t
FchGetSmcMsgIdUsbInitSn (void)
{
  return BIOSSMC_MSG_USBINIT_SN;
}

/**
 * FchGetSmcMsgIdUsbSxEntrySn
 *
 * @brief Gey SMC Message ID for UsbSxEntry Message
 *
 *
 */
uint32_t
FchGetSmcMsgIdUsbSxEntrySn (void)
{
  return BIOSSMC_MSG_USBSXENTRY_SN;
}

/**
 * FchGetSmcMsgIdUsbS3ExitSn
 *
 * @brief Gey SMC Message ID for UsbS3Exit Message
 *
 *
 */
uint32_t
FchGetSmcMsgIdUsbS3ExitSn (void)
{
  return BIOSSMC_MSG_USBS3EXIT_SN;
}

/**
 * FchGetSmcMsgIdUsbConfigUpdateSn
 *
 * @brief Gey SMC Message ID for UsbConfigUpdate Message
 *
 *
 */
uint32_t
FchGetSmcMsgIdUsbConfigUpdateSn (void)
{
  return BIOSSMC_MSG_USBCONFIGUPDATE_SN;
}


/**
 * InitializeApiFchXhciSn
 *
 * @brief   Initialize internal APIs for FCH Xhci
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchXhciSn (void)
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable (SilId_FchUsb, &mFchXhciXferSn);
}

