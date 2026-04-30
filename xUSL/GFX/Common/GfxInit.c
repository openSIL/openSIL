/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  GfxInit.c
 * @brief GFX configuration routines
 */

#include <string.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>
#include <Nbio/NbioIp2Ip.h>
#include <GFX/GfxClass-api.h>
#include "Gfx.h"
#include "GfxDisplayPhySettings.h"
#include "GfxDisplayTypeSettings.h"

extern  ATOM_COMMON_TABLE_HEADER     table_header;
extern  ATOM_DISPLAY_PHY_TUNING_INFO display_phy_tuning_info;

extern  DDI_TABLE_HEADER             ddi_table_header;
extern  GFX_DDI_CONFIG_INFO          DdiConfig;


/**--------------------------------------------------------------------
 * SilDumpN6Table
 *
 * @brief Dump GFX N6 Table
 *
 * @details This function is dump the N6 table
 *
 * @param[in]  ATOM_DISPLAY_PHY_TUNING_INFO  Pointer to the M6 Table
 *
 * @returns Nothing
 * @retval Nothing
 */
void
SilDumpN6Table (
  ATOM_DISPLAY_PHY_TUNING_INFO  *GfxN6OpenSilInputData
  )
{
  uint8_t index;
  ATOM_N6_DISPLAY_PHY_TUNING_SET *disp_phy_tuningPtr = NULL;
  uint32_t  phySettingCounter;

  phySettingCounter = GfxN6OpenSilInputData->table_header.structuresize / sizeof (ATOM_N6_DISPLAY_PHY_TUNING_SET);
  GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL: phySettingCounter 0x%x \n", phySettingCounter);
  disp_phy_tuningPtr = GfxN6OpenSilInputData->disp_phy_tuning;


  GFX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL: table_header.structuresize 0x%x \n",
    GfxN6OpenSilInputData->table_header.structuresize
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL: table_header.format_revision 0x%x \n",
    GfxN6OpenSilInputData->table_header.format_revision
    );
  GFX_TRACEPOINT(SIL_TRACE_INFO,
    "SIL: table_header.content_revision 0x%x \n",
    GfxN6OpenSilInputData->table_header.content_revision
    );

  for (index = 0; index < phySettingCounter; index++) {
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.display_signal_type 0x%x \n",
      disp_phy_tuningPtr[index].display_signal_type
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.phy_sel 0x%x \n",
      disp_phy_tuningPtr[index].phy_sel
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.preset_level 0x%x \n",
      disp_phy_tuningPtr[index].preset_level
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.speed_upto 0x%x \n",
      disp_phy_tuningPtr[index].speed_upto
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_vboost_level 0x%x \n",
      disp_phy_tuningPtr[index].tx_vboost_level
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_vreg_v2i 0x%x \n",
      disp_phy_tuningPtr[index].tx_vreg_v2i
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_vregdrv_byp 0x%x \n",
      disp_phy_tuningPtr[index].tx_vregdrv_byp
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_term_cntl 0x%x \n",
      disp_phy_tuningPtr[index].tx_term_cntl
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_peak_level 0x%x \n",
      disp_phy_tuningPtr[index].tx_peak_level
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_slew_en 0x%x \n",
      disp_phy_tuningPtr[index].tx_slew_en
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_eq_pre 0x%x \n",
      disp_phy_tuningPtr[index].tx_eq_pre
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_eq_main 0x%x \n",
      disp_phy_tuningPtr[index].tx_eq_main
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_eq_post 0x%x \n",
      disp_phy_tuningPtr[index].tx_eq_post
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_en_inv_pre 0x%x \n",
      disp_phy_tuningPtr[index].tx_en_inv_pre
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_en_inv_post 0x%x \n",
      disp_phy_tuningPtr[index].tx_en_inv_post
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: disp_phy_tuning.tx_slew_ctrl_val 0x%x \n",
      disp_phy_tuningPtr[index].tx_slew_ctrl_val
      );
  }

}


/**--------------------------------------------------------------------
 * GetGfxN6Config
 *
 * @brief Interface to configure GFX on each PCIe controller
 *
 * @details This function is called once for each socket
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle            Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 */
void *
GetGfxN6Config (
  SIL_CONTEXT  *SilContext,
  uint32_t     *InfoN6BlockDataSize
  )
{
  ATOM_DISPLAY_PHY_TUNING_INFO  *GfxN6InputData;

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  GfxN6InputData = (ATOM_DISPLAY_PHY_TUNING_INFO *)SilCreateInfoBlock(SilContext,
    SilId_GfxClass,
    table_header.structuresize,
    GFXCLASS_N6_INSTANCE,
    GFXCLASS_MAJOR_REV,
    GFXCLASS_MINOR_REV
    );
  if (GfxN6InputData == NULL) {
    GFX_TRACEPOINT(SIL_TRACE_ERROR, "SIL Failed to create GFX N6 Data Block\n");
    assert(GfxN6InputData != NULL);
    // return SilAborted;
  } else {
    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL GFX N6 Data Block at: 0x%x \n", GfxN6InputData);

    memcpy((void *)(&GfxN6InputData->table_header), &table_header, sizeof (table_header));
    memcpy((void *)(&GfxN6InputData->disp_phy_tuning),
      &display_phy_tuning_info,
      table_header.structuresize - sizeof (table_header)
      );

    SilDumpN6Table(GfxN6InputData);
  }

  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (void *)GfxN6InputData;
}

/**--------------------------------------------------------------------
 * GetGfxDdiConfig
 *
 * @brief Interface to configure GFX on each PCIe controller
 *
 * @details This function is called once for each socket
 *
 * @param  SilContext            A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param  InfoDdiBlockDataSize  Pointer to the Ddi Data Size
 *
 * @returns Nothing
 * @retval Nothing
 */
void *
GetGfxDdiConfig (
  SIL_CONTEXT  *SilContext,
  uint32_t     *InfoDdiBlockDataSize
  )
{
  GFX_DDI_CONFIG_INFO   *GfxDdiInputData;

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  GfxDdiInputData = (GFX_DDI_CONFIG_INFO *)SilCreateInfoBlock(SilContext,
    SilId_GfxClass,
    ddi_table_header.structuresize,
    GFXCLASS_DDI_INSTANCE,
    GFXCLASS_MAJOR_REV,
    GFXCLASS_MINOR_REV
    );
  if (GfxDdiInputData == NULL) {
    GFX_TRACEPOINT(SIL_TRACE_ERROR, "SIL Failed to create GFX DDI Data Block\n");
    assert(GfxDdiInputData != NULL);
    // return SilAborted;
  } else {
    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL GFX DDI Data Block at: 0x%x \n", GfxDdiInputData);
    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL ddi_table_header.structuresize: 0x%x \n", ddi_table_header.structuresize);

    memcpy((void *)(&GfxDdiInputData->table_header), &ddi_table_header, sizeof (ddi_table_header));
    memcpy((void *)(&GfxDdiInputData->ddi_descriptor),
      &DdiConfig,
      ddi_table_header.structuresize - sizeof (ddi_table_header)
      );

    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL After \n");

    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL SilDdiConfig \n");
    SilDumpDdiTable(GfxDdiInputData->ddi_descriptor);
  }

  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (void *)GfxDdiInputData;
}


SIL_STATUS
GetUmaInformation (
  SIL_CONTEXT *SilContext,
  MEMORY_HOLE_DESCRIPTOR *UmaRange
  )
{
  SIL_STATUS                          Status;
  uint8_t                             MemRangeIndex;
  APOB_SYSTEM_MEMORY_MAP_TYPE_STRUCT  *ApobEntry;
  MEMORY_HOLE_DESCRIPTOR              *HoleMapPtr;
  APOB_IP2IP_API                      *ApobIp2IpApi;

  if (SilContext == NULL || UmaRange == NULL) {
    return SilInvalidParameter;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
    APOB_FABRIC,
    APOB_SYS_MAP_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );
  if (Status != SilPass) {
    return Status;
  }

  /* Scan through all mem ranges to find the base address of UMA range. */
  for (MemRangeIndex = 0; MemRangeIndex < ApobEntry->ApobSystemMap.NumberOfHoles; MemRangeIndex++) {
    HoleMapPtr = &ApobEntry->ApobSystemMap.HoleInfo[MemRangeIndex];
    if (HoleMapPtr->Type == UMA) {
      memcpy(UmaRange, HoleMapPtr, sizeof(MEMORY_HOLE_DESCRIPTOR));
      return SilPass;
    }
  }

  return SilNotFound;
}
