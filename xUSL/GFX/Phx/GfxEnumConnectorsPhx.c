/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxEnumConnectorsPhx.c
 *  @brief Phoenix GFX connector enumeration routines
 */

#include <string.h>
#include <SilCommon.h>
#include <SilSocLogicalId.h>
#include <GFX/Common/Gfx.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>
#include <GFX/GfxClass-api.h>
#include <Mpio/Common/MpioStructs.h>
#include <Mpio/MpioClass-api.h>
#include <Nbio/Common/GnbDxio.h>
#include <Nbio/NbioClass-api.h>
#include "GfxInitPhx.h"
#include <GFX/Common/GfxV4.h>

#define ATOM_ENCODER_CAP_RECORD_HBR2_DISABLE               0x0001
#define ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN             0x0002
#define ATOM_ENCODER_CAP_RECORD_EXT_CHIP_MASK              0x007C
#define ATOM_ENCODER_CAP_RECORD_HDMI20_PI3EQX1204          (0x01 << 2 )     //PI redriver chip
#define ATOM_ENCODER_CAP_RECORD_HDMI20_TISN65DP159RSBT     (0x02 << 2 )     //TI retimer chip
#define ATOM_ENCODER_CAP_RECORD_HDMI20_PARADE_PS175        (0x03 << 2 )     //Parade DP->HDMI recoverter chip
#define ATOM_ENCODER_CAP_RECORD_HBR3_DISABLE               0x0080
#define ATOM_ENCODER_CAP_RECORD_USB_C_TYPE                 0x100            // the DP connector is a USB-C type.
#define ATOM_ENCODER_CAP_RECORD_HDMI20_DISABLE             0x200
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

static PCIe_ENGINE_CONFIG DdiComplexDataPHX[] = {
  //Ddi0
  {
    {
      DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_ALLOCATED,
      0,
      0,
      0
    },
    {PcieDdiEngine, 16, 19},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {.Ddi={{0}}}                                    //PCIe_DDI_CONFIG
  },
  //Ddi1
  {
    {
      DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_ALLOCATED,
      0,
      0,
      0
    },
    {PcieDdiEngine, 20, 23},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {.Ddi={{0}}}                                    //PCIe_DDI_CONFIG
  },
  //Ddi2
  {
    {
      DESCRIPTOR_DDI_ENGINE,
      0,
      0,
      0
    },
    {PcieDdiEngine, 24, 27},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {.Ddi={{0}}}                                    //PCIe_DDI_CONFIG
  },
  //Ddi3
  {
    {
      DESCRIPTOR_DDI_ENGINE,
      0,
      0,
      0
    },
    {PcieDdiEngine, 28, 31},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {.Ddi={{0}}}                                    //PCIe_DDI_CONFIG
  },
  //Ddi4
  {
    {
      DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_TERMINATE_LIST,
      0,
      0,
      0
    },
    {PcieDdiEngine, 32, 35},
    0,                                              //Initialization Status
    0xFF,                                           //Scratch
    {.Ddi={{0}}}                                    //PCIe_DDI_CONFIG
  }
};

static EXT_CONNECTOR_INFO ConnectorInfoTable[] = {
  {
    ConnectorTypeDP,
    DEVICE_DFP,
    CONNECTOR_DISPLAYPORT_ENUM,
    ENCODER_NOT_PRESENT,
    0,
  },
  {
    ConnectorTypeEDP,
    DEVICE_DFP,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeSingleLinkDVI,
    DEVICE_DFP,
    CONNECTOR_SINGLE_LINK_DVI_D_ENUM,
    ENCODER_NOT_PRESENT,
    2
  },
  {
    ConnectorTypeDualLinkDVI,
    DEVICE_DFP,
    CONNECTOR_DUAL_LINK_DVI_D_ENUM,
    ENCODER_NOT_PRESENT,
    3
  },
  {
    ConnectorTypeHDMI,
    DEVICE_DFP,
    CONNECTOR_HDMI_TYPE_A_ENUM,
    ENCODER_NOT_PRESENT,
    4
  },
  {
    ConnectorTypeDpToVga,
    DEVICE_CRT,
    CONNECTOR_VGA_ENUM,
    ENCODER_DP2VGA_ENUM_ID1,
    5
  },
  {
    ConnectorTypeDpToLvds,
    DEVICE_LCD,
    CONNECTOR_LVDS_ENUM,
    ENCODER_DP2LVDS_ENUM_ID2,
    6
  },
  {
    ConnectorTypeNutmegDpToVga,
    DEVICE_CRT,
    CONNECTOR_VGA_ENUM,
    ENCODER_ALMOND_ENUM_ID1,
    5
  },
  {
    ConnectorTypeSingleLinkDviI,
    DEVICE_DFP,
    CONNECTOR_SINGLE_LINK_DVI_I_ENUM,
    ENCODER_NOT_PRESENT,
    5
  },
  {
    ConnectorTypeDpWithTypeC,
    DEVICE_DFP,
    CONNECTOR_DISPLAYPORT_ENUM,
    ENCODER_NOT_PRESENT,
    0
  },
  {
    ConnectorTypeDpWithoutTypeC,
    DEVICE_DFP,
    CONNECTOR_DISPLAYPORT_ENUM,
    ENCODER_NOT_PRESENT,
    0
  },
  {
    ConnectorTypeEDPToLvds,
    DEVICE_LCD,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeEDPToLvdsSwInit,
    DEVICE_LCD,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeAutoDetect,
    DEVICE_LCD,
    CONNECTOR_LVDS_eDP_ENUM,
    ENCODER_DP2LVDS_ENUM_ID2,
    7
  },
  {
    UnusedType,
    0,
    0,
    0,
    0
  },
};


/**
 * GfxIntegratedExtConnectorInfo
 *
 * @brief Enumerate all display connectors for specific display device type.
 *
 *
 * @param ConnectorType   Connector type (see PCIe_DDI_DATA::ConnectorType).
 * @retval    Pointer to EXT_CONNECTOR_INFO, NULL if connector type unknown.
 */
static EXT_CONNECTOR_INFO*
GfxIntegratedExtConnectorInfo (
  uint8_t ConnectorType
  )
{
  size_t Index;

  for (Index = 0; Index < SIL_ARRAY_SIZE(ConnectorInfoTable); Index++) {
    if (ConnectorInfoTable[Index].ConnectorType == ConnectorType) {
      return &ConnectorInfoTable[Index];
    }
  }

  return NULL;
}

static EXT_DISPLAY_DEVICE_INFO DisplayDeviceInfoTable[] = {
  {
    DEVICE_CRT,
    1,
    ATOM_DEVICE_CRT1_SUPPORT,
    0x100,
  },
  {
    DEVICE_LCD,
    1,
    ATOM_DEVICE_LCD1_SUPPORT,
    0x110,
  },
  {
    DEVICE_DFP,
    1,
    ATOM_DEVICE_DFP1_SUPPORT,
    0x210,
  },
  {
    DEVICE_DFP,
    2,
    ATOM_DEVICE_DFP2_SUPPORT,
    0x220,
  },
  {
    DEVICE_DFP,
    3,
    ATOM_DEVICE_DFP3_SUPPORT,
    0x230,
  },
  {
    DEVICE_DFP,
    4,
    ATOM_DEVICE_DFP4_SUPPORT,
    0x240,
  },
  {
    DEVICE_DFP,
    5,
    ATOM_DEVICE_DFP5_SUPPORT,
    0x250,
  },
  {
    DEVICE_DFP,
    6,
    ATOM_DEVICE_DFP6_SUPPORT,
    0x260,
  }
};

static uint32_t
GfxMappingUserConfigPhx (
  DDI_DESCRIPTOR         *DdiConfig,
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t                *PriorityConnectorType
  )
{
  bool     LocalFlag;
  uint32_t Index;
  uint32_t DfpCounter;
  uint8_t  DpCounter;
  uint32_t NumbersOfDdi;
  uint8_t  eDpCounter;

  NumbersOfDdi = 0;
  Index = 0;
  DfpCounter = 0;
  DpCounter = 0;
  LocalFlag = false;
  eDpCounter = 0;

  do {
    Engine[Index].Type.Ddi.DdiData.ConnectorType = DdiConfig[Index].Ddi.ConnectorType;
    Engine[Index].Type.Ddi.DdiData.AuxIndex = DdiConfig[Index].Ddi.AuxIndex;
    Engine[Index].Type.Ddi.DdiData.HdpIndex = DdiConfig[Index].Ddi.HdpIndex;
    Engine[Index].Type.Ddi.DdiData.LanePnInversionMask = DdiConfig[Index].Ddi.LanePnInversionMask;
    Engine[Index].Type.Ddi.DdiData.Flags = DdiConfig[Index].Ddi.Flags;

    if (DdiConfig[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
      LocalFlag = true;
    }
    //Init Priority Array
    if (DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeEDP) {
      eDpCounter++;
      PriorityConnectorType[Index] = eDpCounter;
    }
    if (DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDP ||
           DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeHDMI ||
           DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeSingleLinkDVI
      || DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDpWithTypeC ||
         DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDpWithoutTypeC) {
      DfpCounter++;
    }
    NumbersOfDdi++;
    Index++;
  } while (LocalFlag != true);

  // Assign Tag
  while (DfpCounter != 0) {
    for (Index = 0; Index < NumbersOfDdi; Index++) {
      // Treate All Usb type C the same as DP
      if (DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDP ||
          DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDpWithTypeC ||
          DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeDpWithoutTypeC) {
        DpCounter++;
        DfpCounter--;
        PriorityConnectorType[Index] = DpCounter;
      }
    }
    for (Index = 0; Index < NumbersOfDdi; Index++) {
      if (DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeHDMI) {
        DpCounter++;
        DfpCounter--;
        PriorityConnectorType[Index] = DpCounter;
      }
    }
    for (Index = 0; Index < NumbersOfDdi; Index++) {
      if (DdiConfig[Index].Ddi.ConnectorType == ConnectorTypeSingleLinkDVI) {
        DpCounter++;
        DfpCounter--;
        PriorityConnectorType[Index] = DpCounter;
      }
    }
  }

  return NumbersOfDdi;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors for specific display device type.
 *
 *
 *
 * @param[in] DisplayDeviceEnum   Display device enum
 * @param[in] DisplayDeviceIndex  Display device index
 * @retval    Pointer to EXT_DISPLAY_DEVICE_INFO
 * @retval    NULL if can not get display device info
 */
static EXT_DISPLAY_DEVICE_INFO*
GfxIntegratedExtDisplayDeviceInfo (
  uint8_t DisplayDeviceEnum,
  uint8_t DisplayDeviceIndex
  )
{
  uint8_t Index;
  uint8_t LastIndex;

  LastIndex = 0xff;

  for (Index = 0; Index < SIL_ARRAY_SIZE(DisplayDeviceInfoTable); Index++) {
    if (DisplayDeviceInfoTable[Index].DisplayDeviceEnum == DisplayDeviceEnum) {
      LastIndex = Index;
      if (DisplayDeviceInfoTable[Index].DeviceIndex == DisplayDeviceIndex) {
        return &DisplayDeviceInfoTable[Index];
      }
    }
  }
  if (DisplayDeviceEnum == DEVICE_LCD && LastIndex != 0xff) {
    return &DisplayDeviceInfoTable[LastIndex];
  }
  return NULL;
}

/**
 * GfxIntegratedEnumerateAllConnectorsPhx
 *
 * @brief Enumerate all display connectors
 *
 * @param    SilContext        openSIL context
 * @param    InputBlk          GFX IP block configuration input data pointer
 * @param    DisplayPathList   Display path list
 */
SIL_STATUS
GfxIntegratedEnumerateAllConnectorsPhx (
  SIL_CONTEXT                 *SilContext,
  GFXCLASS_INPUT_BLK          *InputBlk,
  EXT_DISPLAY_PATH            *DisplayPathList
  )
{
  SIL_STATUS                        Status;
  PCIe_ENGINE_CONFIG                DdiComplexData[NUM_DDI_PORTS];
  MPIO_COMPLEX_DESCRIPTOR           *PcieTopologyData;
  GFX_DDI_CONFIG_INFO               *GfxDdiInputData;
  MPIOCLASS_COMMON_INPUT_BLK        *MpioData;
  EXT_CONNECTOR_INFO                *ExtConnectorInfo;
  EXT_DISPLAY_DEVICE_INFO           *ExtDisplayDeviceInfo;
  uint32_t                          DdiCounter;
  EXT_DISPLAY_PATH                  *StoreDisplayPathList;
  uint8_t                           DisplayDeviceIndex;
  SOC_LOGICAL_ID                    LogicalId;
  size_t                            NumbersOfDdi;
  uint8_t                           PriorityConnectorType[NUM_DDI_PORTS] = {0};

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  ExtDisplayDeviceInfo = NULL;
  LogicalId.Revision = AMD_REVISION_UNKNOWN;
  LogicalId.Family = AMD_FAMILY_UNKNOWN;
  NumbersOfDdi = 0;

  MpioData = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );

  if (MpioData == NULL) {
    return SilNotFound;
  }

  memcpy(DdiComplexData, DdiComplexDataPHX, sizeof (PCIe_ENGINE_CONFIG) * NUM_DDI_PORTS);
  PcieTopologyData = MpioData->PcieTopologyData.PlatformData;
  GfxDdiInputData = (GFX_DDI_CONFIG_INFO *)xUslFindStructure(SilContext,
    SilId_GfxClass,
    GFXCLASS_DDI_INSTANCE
    );

  if ((PcieTopologyData->DdiLinkList == NULL) && (GfxDdiInputData != NULL)) {
    PcieTopologyData->DdiLinkList = (MPIO_DDI_DESCRIPTOR *)GfxDdiInputData->ddi_descriptor;
  }

  if (PcieTopologyData->DdiLinkList != NULL) {
    NumbersOfDdi = GfxMappingUserConfigPhx (
                    (DDI_DESCRIPTOR *)PcieTopologyData->DdiLinkList,
                    &DdiComplexData[0],
                    &PriorityConnectorType[0]
                    );
  }

  GFX_TRACEPOINT(SIL_TRACE_INFO, "NumbersOfDdi %x\n", NumbersOfDdi);

  StoreDisplayPathList = DisplayPathList;
  DisplayDeviceIndex = 1;

  Status = GetSocLogicalIdOnCurrentCore (&LogicalId);
  if (Status != SilPass) {
    return Status;
  }

  for (DdiCounter = 0; DdiCounter < NumbersOfDdi; DdiCounter++) {
    ExtConnectorInfo = GfxIntegratedExtConnectorInfo(
                         DdiComplexData[DdiCounter].Type.Ddi.DdiData.ConnectorType
                         );
    GFX_TRACEPOINT(SIL_TRACE_INFO, "Ddi[%d]\n", DdiCounter);
    GFX_TRACEPOINT(SIL_TRACE_INFO, "ExtConnectorInfo %x\n", ExtConnectorInfo);
    GFX_TRACEPOINT(SIL_TRACE_INFO, "ExtConnectorInfo->ConnectorType %x\n",
                   ExtConnectorInfo->ConnectorType);

    StoreDisplayPathList->usDeviceConnector = ExtConnectorInfo->ConnectorEnum |
                                              (PriorityConnectorType[DdiCounter] << 8);

    GFX_TRACEPOINT(SIL_TRACE_INFO, "DisplayPathList->usDeviceConnector %x\n",
                   DisplayPathList->usDeviceConnector);
    ExtDisplayDeviceInfo = GfxIntegratedExtDisplayDeviceInfo(
                             ExtConnectorInfo->DisplayDeviceEnum,
                             PriorityConnectorType[DdiCounter]
                             );
    if (ExtDisplayDeviceInfo != NULL) {
      StoreDisplayPathList->usDeviceTag = ExtDisplayDeviceInfo->DeviceTag;
      StoreDisplayPathList->usDeviceACPIEnum = ExtDisplayDeviceInfo->DeviceAcpiEnum;
    } else {
      GFX_TRACEPOINT(SIL_TRACE_INFO, "Returned NULL ExtDisplayDeviceInfo\n");
      StoreDisplayPathList->usDeviceTag = 0;
      StoreDisplayPathList->usDeviceACPIEnum = 0;
    }

    //eDP Case
    if (ExtConnectorInfo->ConnectorType == ConnectorTypeEDP) {
      GFX_TRACEPOINT(SIL_TRACE_INFO, "StoreDisplayPathList->usDeviceConnector %x\n", \
                                   StoreDisplayPathList->usDeviceConnector);
      if (StoreDisplayPathList->usDeviceConnector == (CONNECTOR_eDP_ENUM + (1 << 8))) {
        StoreDisplayPathList->usDeviceTag = ATOM_DEVICE_LCD1_SUPPORT;
        StoreDisplayPathList->usDeviceACPIEnum = 0x110;
      } else {
        StoreDisplayPathList->usDeviceTag = ATOM_DEVICE_LCD2_SUPPORT;
        StoreDisplayPathList->usDeviceACPIEnum = 0x120;
      }
    }

    StoreDisplayPathList->ucExtAUXDDCLutIndex = DdiComplexData[DdiCounter].Type.Ddi.DdiData.AuxIndex;
    StoreDisplayPathList->ucExtHPDPINLutIndex = DdiComplexData[DdiCounter].Type.Ddi.DdiData.HdpIndex;
    StoreDisplayPathList->ucChannelMapping = 0xE4;
    StoreDisplayPathList->ucChPNInvert = 0;
    StoreDisplayPathList->usCaps = DdiComplexData[DdiCounter].Type.Ddi.DdiData.Flags;

    switch (DdiCounter) {
      case 0:
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDP) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeHDMI) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL);
        }
        if (InputBlk->DisplayCapDdi0 != 0) {
          StoreDisplayPathList->usCaps = InputBlk->DisplayCapDdi0;
        }
        break;
      case 1:
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDP) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeHDMI) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL);
        }
        if (InputBlk->DisplayCapDdi1 != 0) {
          StoreDisplayPathList->usCaps = InputBlk->DisplayCapDdi1;
        }
        break;
      case 2:
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDP) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeHDMI) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithTypeC) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE |
                                           ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN);

          StoreDisplayPathList->usCaps |= (ATOM_ENCODER_CAP_RECORD_USB_C_TYPE |
                                           EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithoutTypeC) {
          StoreDisplayPathList->usCaps &= (~ATOM_ENCODER_CAP_RECORD_USB_C_TYPE);
          StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (InputBlk->DisplayCapDdi2 != 0) {
          StoreDisplayPathList->usCaps = InputBlk->DisplayCapDdi2;
        }
        break;
      case 3:
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDP) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeHDMI) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithTypeC) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE |
                                           ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN);

          StoreDisplayPathList->usCaps |= (ATOM_ENCODER_CAP_RECORD_USB_C_TYPE |
                                           EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithoutTypeC) {
          StoreDisplayPathList->usCaps &= (~ATOM_ENCODER_CAP_RECORD_USB_C_TYPE);
          StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (InputBlk->DisplayCapDdi3 != 0) {
          StoreDisplayPathList->usCaps = InputBlk->DisplayCapDdi3;
        }
        break;
      case 4:
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDP) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeHDMI) {
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
                                           EXT_DISPLAY_PATH_CAPS__HDMI_FRL);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithTypeC ||
            ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithoutTypeC) {
          if (LogicalId.Family == AMD_FAMILY_PHX) {
            // Check if it is PHX2 AM5
            if (ISSOCPHX2AM5) {
              GFX_TRACEPOINT(SIL_TRACE_INFO, "PHX2 AM5 GFX\n");
              StoreDisplayPathList->usCaps |= ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN;
            }
            if (ISSOCPHX1AM5) {
              GFX_TRACEPOINT(SIL_TRACE_INFO, "PHX AM5 GFX\n");
              StoreDisplayPathList->usCaps |= ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN;
            }
          }
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithTypeC) {
          StoreDisplayPathList->usCaps |= ATOM_ENCODER_CAP_RECORD_USB_C_TYPE;
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (ExtConnectorInfo->ConnectorType == ConnectorTypeDpWithoutTypeC) {
          StoreDisplayPathList->usCaps &= (~ATOM_ENCODER_CAP_RECORD_USB_C_TYPE);
          StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
          StoreDisplayPathList->usCaps |= (EXT_DISPLAY_PATH_CAPS__DP2 |
                                           EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
        }
        if (InputBlk->DisplayCapDdi4 != 0) {
          StoreDisplayPathList->usCaps = InputBlk->DisplayCapDdi4;
        }
        break;
      default:
        break;
    }

    if (!InputBlk->Usb4Rt0En || !InputBlk->Usb4Rt0DpTnlEn) {
      if (DdiCounter == 2) {
        StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
      }
    }

    if (!InputBlk->Usb4Rt1En || !InputBlk->Usb4Rt1DpTnlEn) {
      if (DdiCounter == 3) {
        StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
      }
    }

    if (InputBlk->Usb4DpiaDisable){
      StoreDisplayPathList->usCaps &= (~EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE);
    }

    if (ExtConnectorInfo->ConnectorType == UnusedType) {
      StoreDisplayPathList->usDeviceTag = 0;
      StoreDisplayPathList->usDeviceACPIEnum = 0;
      StoreDisplayPathList->usDeviceConnector = 0;
      StoreDisplayPathList->ucExtAUXDDCLutIndex = 0;
      StoreDisplayPathList->ucExtHPDPINLutIndex = 0;
      StoreDisplayPathList->ucChannelMapping = 0;
      StoreDisplayPathList->ucChPNInvert = 0;
      StoreDisplayPathList->usCaps = 0;
      StoreDisplayPathList->usExtEncoderObjId = 0;
    }

    GFX_TRACEPOINT(SIL_TRACE_INFO, "Address of StoreDisplayPathList %x\n",
                   StoreDisplayPathList);
    GFX_TRACEPOINT(SIL_TRACE_INFO, "Address of ExtConnectorInfo %x\n",
                   ExtConnectorInfo);
    GFX_TRACEPOINT(SIL_TRACE_INFO, "StoreDisplayPathList usCaps: %x\n",
                   StoreDisplayPathList->usCaps);

    StoreDisplayPathList++;
    if(ExtDisplayDeviceInfo != NULL) {
       ExtDisplayDeviceInfo++;
    }
    DisplayDeviceIndex++;
  }

  GFX_TRACEPOINT(SIL_TRACE_INFO, "AmdBitMapDisaplyOnlyController  [0x%x]\n",
                 InputBlk->AmdBitMapDisaplyOnlyController);

  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/**
 * Dump  display path settings
 *
 * @param[in] DisplayPath            Display path
 */

void
GfxIntegratedDebugDumpDisplayPath (
  EXT_DISPLAY_PATH *DisplayPath
  )
{
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     usDeviceConnector = 0x%x\n",
    DisplayPath->usDeviceConnector
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     usDeviceTag = 0x%x\n",
    DisplayPath->usDeviceTag
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     usDeviceACPIEnum = 0x%x\n",
    DisplayPath->usDeviceACPIEnum
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     ucExtAUXDDCLutIndex = 0x%x\n",
    DisplayPath->ucExtAUXDDCLutIndex
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     ucExtHPDPINLutIndex = 0x%x\n",
    DisplayPath->ucExtHPDPINLutIndex
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     usExtEncoderObjId = 0x%x\n",
    DisplayPath->usExtEncoderObjId
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     ucChannelMapping = 0x%x\n",
    DisplayPath->ucChannelMapping
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     ucChPNInvert = 0x%x\n",
    DisplayPath->ucChPNInvert
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO, "     usCaps = 0x%x\n",
    DisplayPath->usCaps
    );
}
