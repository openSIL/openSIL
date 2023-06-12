/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioHidePcieCore.c
 * @brief This file contains functions to Support wrapper removal
 *        from complex for harvesting or power gating
 *
 */

#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/NbioIp2Ip.h>


/**
 * FindLastPciePortOnPrevPcieCore
 *
 * @brief   Find the last PCIe port descriptor for a PCIe core (PrevPcieCore)
 * @details This function is called for each PCIe port of a PCIe core. By saving the
 *          address of each port descriptor, the last one saved is the last port
 *          descriptor for the PCIe core.
 *
 *
 * @param[in]     PciePort    PciePorter on this PcieCore
 * @param[in,out] Buffer      Save the hidden PcieCore's Info
 * @param[in]     PcieCore    PcieCore to be hidden
 */
static void FindLastPciePortOnPrevPcieCore (
  PCIe_ENGINE_CONFIG                *PciePort,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *PcieCore
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastPciePortOnPrev = PciePort;
}

/**
 * FindPrevPcieCore
 *
 * @brief   Locates the PCIe core descriptor that precedes the current one
 * @details For a given PCIe core descriptor in a linked list, this function locates
 *          the prior PCIe core descriptor in the list. This assumes that a PCIe core
 *          descriptor is the first one in a silicon descriptor and uses the last PCIe
 *          core descriptor of the prior silicon descriptor in the list.
 *
 * @param[in]     Buffer    void pointer to a HIDE_WRAPPER_INFO structure to save the hidden PcieCore's Info
 * @param[in,out] PcieCore  Pointer to PcieCore descriptor to be removed from the linked list
 */
static void FindPrevPcieCore (
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *PcieCore
  )
{
  PCIe_SILICON_CONFIG  *Silicon = NULL;
  PCIe_SILICON_CONFIG  *PrevSilicon = NULL;
  PCIe_WRAPPER_CONFIG  *PrevPcieCore = NULL;
  uint8_t               InstanceID = 0;
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  PCIe_PLATFORM_CONFIG  *Pcie = NULL;


  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  Pcie = (PCIe_PLATFORM_CONFIG *) PcieConfigGetParent(DESCRIPTOR_PLATFORM, &(PcieCore->Header));

  // Get parent Silicon of PcieCore
  Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetParent(DESCRIPTOR_SILICON, &(PcieCore->Header));
  InstanceID = Silicon->InstanceId;

  // Invalid. InstanceID should be greater than 0 for this function.
  if (InstanceID == 0)
    return;

  // Get the Child Silicon from Pcie
  PrevSilicon = (PCIe_SILICON_CONFIG *) PcieConfigGetChild(DESCRIPTOR_SILICON, &(Pcie->Header));

  while (PrevSilicon != NULL) {
    // Look for the previous Silicon
    if (PrevSilicon->InstanceId == (InstanceID - 1))
      break;
    PrevSilicon = (PCIe_SILICON_CONFIG *)PcieConfigGetNextTopologyDescriptor(PrevSilicon,
      DESCRIPTOR_TERMINATE_TOPOLOGY
      );
  }
  // Added for solving coverity error, technically PrevSilicon will never be NULL.
  if (PrevSilicon == NULL)
    return;
  PrevPcieCore = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(PrevSilicon->Header));

  while (PrevPcieCore) {
    // Eventually, the last PcieCore will be saved
    HideInfo->PrevPcieCore = PrevPcieCore;
    PrevPcieCore = PcieConfigGetNextDescriptor(PrevPcieCore);
  }
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * FindLastPciePortOfThisPcieCore
 *
 * @brief   Find the last PCIe port descriptor for a PCIe core (ThisPcieCore)
 * @details This function is called for each PCIe port of a PCIe core. By saving the
 *          address of each port descriptor, the last one saved is the last port
 *          descriptor for the PCIe core.
 * @param[in]     PciePort   PciePorter on this PcieCore
 * @param[in,out] Buffer     Save the hidden PcieCore's Info
 * @param[in]     PcieCore   PcieCore to be hidden
 */
static void FindLastPciePortOfThisPcieCore (
  PCIe_ENGINE_CONFIG                *PciePort,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *PcieCore
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastPciePortOnThis = PciePort;
}


/**
 * FindPcieCoreDesc
 *
 * @brief   Locates the PCIe core descriptor to be removed from a linked list
 * @details For a given PCIe core descriptor in a linked list, this function locates
 *          the PCIe core descriptor in the list that matches the PCIe core ID.
 *
 * @param[in]     PcieCore   PcieCore configuration info
 * @param[in,out] Buffer    Buffer pointer points to the hidden PcieCore's Info
 * @param[in]     GnbHandle Points to a silicon configuration structure data
 */
static void
FindPcieCoreDesc (
  PCIe_WRAPPER_CONFIG               *PcieCore,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  HIDE_WRAPPER_INFO   *HideInfo;
  HideInfo = (HIDE_WRAPPER_INFO *) Buffer;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (PcieCore->WrapId == HideInfo->PcieCoreNum) {
    // PcieCore matched. Save it to ThisPcieCore.
    HideInfo->ThisPcieCore = PcieCore;
  } else {
    // If ThisPcieCore has not been found, always set PrevPcieCore with the current PcieCore
    if (HideInfo->ThisPcieCore == NULL) {
      HideInfo->PrevPcieCore = PcieCore;
    } else {
      // Only set NextPcieCore once.
      if (HideInfo->NextPcieCore == NULL) {
        HideInfo->NextPcieCore = PcieCore;
      }
    }
  }
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * PcieComplexHidePcieCore
 *
 * @brief   Removes a PCIe core descriptor from the SOC Topology Complex.
 * @details In the event of PCIe core power gating or harvesting, we may need to remove a
 * PCIe core descriptor from the SOC Topology Complex to prevent any access to the missing IP.
 *
 *  @param  [in] GnbHandle  Points to a silicon configuration structure data
 *  @param  [in] PcieCoreNum WrapId to be hidden
 *  @retval SilPass         Hiding PcieCore is successful
 *
 *  @details
 */
SIL_STATUS
PcieComplexHidePcieCore (
  GNB_HANDLE     *GnbHandle,
  uint32_t       PcieCoreNum
  )
{
  HIDE_WRAPPER_INFO     HideInfo;
  PCIe_SILICON_CONFIG   *Silicon = (PCIe_SILICON_CONFIG *)GnbHandle;
  uint8_t               DeleteFirstPcieCore = 0;
  uint8_t               DeleteLastPcieCore = 0;

  HideInfo.PcieCoreNum = PcieCoreNum;
  HideInfo.PrevPcieCore = NULL;
  HideInfo.ThisPcieCore = NULL;
  HideInfo.NextPcieCore = NULL;
  HideInfo.FirstPciePortOnNext = NULL;
  HideInfo.LastPciePortOnThis = NULL;
  HideInfo.LastPciePortOnPrev = NULL;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  // Update HideInfo with ThisPcieCore, PrevPcieCore, and NextPcieCore
  PcieConfigRunProcForAllWrappersInNbio(DESCRIPTOR_ALL_WRAPPERS,
    FindPcieCoreDesc,
    &HideInfo,
    GnbHandle
    );

  // For the case of hiding "PcieCore 0 from NBIO 1, 2, 3, etc. Find the PrevPcieCore from the last GnbHandle - 1
  if (HideInfo.PrevPcieCore == NULL) {
    DeleteFirstPcieCore = 1;
    if (Silicon->InstanceId != 0)
      FindPrevPcieCore(&HideInfo, HideInfo.ThisPcieCore);
  }

  // For the case of hiding the Last PcieCore, need to set NextPcieCore to the first PcieCore from GnbHandle + 1
  if (HideInfo.NextPcieCore == NULL && HideInfo.ThisPcieCore != NULL) {
    HideInfo.NextPcieCore = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor(HideInfo.ThisPcieCore,
      DESCRIPTOR_TERMINATE_TOPOLOGY
      );
    DeleteLastPcieCore = 1;
  }

  // Find the 1st PciePort on NextPcieCore
  if (HideInfo.NextPcieCore != NULL) {
    HideInfo.FirstPciePortOnNext = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES,
      &((HideInfo.NextPcieCore)->Header)
      );
  } else {
    HideInfo.FirstPciePortOnNext = NULL;
  }
  // Find the Last PciePort on PrevPcieCore
  PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_PCIE_ENGINE,
    FindLastPciePortOnPrevPcieCore,
    &HideInfo,
    HideInfo.PrevPcieCore
    );

  // Find the Last PciePort on ThisPcieCore
  PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_PCIE_ENGINE,
    FindLastPciePortOfThisPcieCore,
    &HideInfo,
    HideInfo.ThisPcieCore
    );

  // First wrapper (PcieCore0) in GnbHandle
  if (DeleteFirstPcieCore) {
    // If it's NOT on NBIO0, need to set the PEER
    if (Silicon->InstanceId != 0) {
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_WRAPPER,
        &HideInfo.PrevPcieCore->Header,
        &HideInfo.NextPcieCore->Header
        );
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_ENGINE,
        &HideInfo.LastPciePortOnPrev->Header,
        &HideInfo.FirstPciePortOnNext->Header
        );
    }
    // Set Silicon's Child to PcieCore1
    PcieConfigAttachChild(&Silicon->Header, &HideInfo.NextPcieCore->Header);
  }
  // Last wrapper in GnbHandle
  else if (DeleteLastPcieCore) {
    PcieConfigSetDescriptorFlags(HideInfo.PrevPcieCore, HideInfo.ThisPcieCore->Header.DescriptorFlags);
    PcieConfigSetDescriptorFlags(HideInfo.LastPciePortOnPrev, HideInfo.LastPciePortOnThis->Header.DescriptorFlags);

    // Note: Must have this condition. Otherwise, the DescriptorFlags
    // set above will be overwritten when NextPcieCore = NULL.
    if (HideInfo.NextPcieCore) {
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_WRAPPER,
        &HideInfo.PrevPcieCore->Header,
        &HideInfo.NextPcieCore->Header
        );
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_ENGINE,
        &HideInfo.LastPciePortOnPrev->Header,
        &HideInfo.FirstPciePortOnNext->Header
        );
    }
  } else {
    // wrapper that is neither the first nor the last on the GnbHandle
    // The existing PcieConfigAttachDescriptors() will not work. so, trying this way.
    HideInfo.PrevPcieCore->Header.Peer = (uint16_t) ((uint8_t *) &HideInfo.NextPcieCore->Header -
      (uint8_t *) &HideInfo.PrevPcieCore->Header);
    HideInfo.LastPciePortOnPrev->Header.Peer = (uint16_t) ((uint8_t *) &HideInfo.FirstPciePortOnNext->Header -
      (uint8_t *) &HideInfo.LastPciePortOnPrev->Header);
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
