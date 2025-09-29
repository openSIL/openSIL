/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file GfxDisplayTypeSettings.c
 * @brief This file contain the Gfx Display Phy Setting
 */

#include <stdint.h>
#include <stdbool.h>
#include <Nbio/Common/PciStructs.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>

#pragma pack(push,1)
DDI_DESCRIPTOR         DdiConfig[] = {
  //
  // Refer DxioDdiConnectorTypeList[] for final default setting.
  //
  { // DDI0 - DP
    0,
    PCIE_DDI_DATA_INITIALIZER(ConnectorTypeEDP, Aux1, Hdp1)
  },
  { // DDI1 - DP
    0,
    PCIE_DDI_DATA_INITIALIZER(ConnectorTypeDP, Aux2, Hdp2)
  },
  { // DDI2 - DP (USB TypeC)
    0,
    PCIE_DDI_DATA_INITIALIZER(ConnectorTypeDpWithTypeC, Aux3, Hdp3)
  },
  { // DDI2 - DP (USB TypeC)
    0,
    PCIE_DDI_DATA_INITIALIZER(ConnectorTypeDpWithTypeC, Aux4, Hdp4)
  },
  { // DDI3 - DP (USB TypeC)
    DESCRIPTOR_TERMINATE_LIST,
    PCIE_DDI_DATA_INITIALIZER(ConnectorTypeDpWithTypeC, Aux5, Hdp5)
  }
};

DDI_TABLE_HEADER ddi_table_header = {
  sizeof (DdiConfig) + sizeof (DDI_TABLE_HEADER), //uint16_t structure size;
  DDI_TABLE_FORMAT_REVISION,                      //uint8_t  format_revision;
  DDI_TABLE_CONTENT_REVISION,                     //uint8_t  content_revision;
};


#pragma pack(pop)
