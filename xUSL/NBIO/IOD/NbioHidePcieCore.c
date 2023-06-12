/**
 * @file  NbioHidePcieCore.c
 * @brief This file contains functions to Support wrapper removal
 *        from complex for harvesting or power gating
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <NbioSmnTable.h>
#include "GnbRegisters.h"
#include "NbioData.h"
#include "NbioCommon.h"
#include <NBIO/GnbDxio.h>
#include "NbioPcieTopologyHelper.h"
#include "NbioPcie.h"

typedef struct {
  uint32_t             PcieCoreNum;
  PCIe_WRAPPER_CONFIG  *ThisPcieCore;
  PCIe_WRAPPER_CONFIG  *PrevPcieCore;
  PCIe_WRAPPER_CONFIG  *NextPcieCore;
  PCIe_ENGINE_CONFIG   *FirstPciePortOnNext; // First PciePort on NextPcieCore
  PCIe_ENGINE_CONFIG   *LastPciePortOnThis;  // Last PciePort on ThisPcieCore
  PCIe_ENGINE_CONFIG   *LastPciePortOnPrev;  // Last PciePort on PrevPcieCore
} HIDE_WRAPPER_INFO;

/**
 * PcieConfigAttachDescriptors
 *
 * @brief   Attach descriptors of a linked list
 * @details This funciton is used to remove a descriptor of a specific "Type" from a linked
 *          list of descriptors, adjusting the previous descriptor to link to the next
 *          descriptor of the same type. This removes that IP from the SOC Topology Descriptor.
 *
 * @param[in]      Type       Descriptor type
 * @param[in,out]  Base       Base descriptor
 * @param[in,out]  New        New  descriptor
 */
static void
PcieConfigAttachDescriptors (
      uint32_t                       Type,
      PCIe_DESCRIPTOR_HEADER         *Base,
      PCIe_DESCRIPTOR_HEADER         *New
  )
{
  PCIe_DESCRIPTOR_HEADER  *Left;
  PCIe_DESCRIPTOR_HEADER  *Right;

  Left = PcieConfigGetPeer (DESCRIPTOR_TERMINATE_GNB, PcieConfigGetChild (Type, Base));
  assert (Left != NULL);
  Right = PcieConfigGetChild (Type, New);

  Left->Peer = (uint16_t) ((uint8_t *) Right - (uint8_t *) Left);
  PcieConfigResetDescriptorFlags (Left, DESCRIPTOR_TERMINATE_TOPOLOGY);
}

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
 * @brief   Locates the PCIe core desciptor that precedes the current one
 * @details For a given PCIe core descriptor in a linked list, this function locates
 *          the prior PCIe core descriptor in the list. This assumes that a PCIe core
 *          descriptor is the first one in a siicon descriptor and uses the last PCIe
 *          core descriptor of the prior silicon descriptor in the list.
 *
 * @param[in]     Buffer    void pointer to a HIDE_WRAPPER_INFO structure to save the hidden PcieCore's Info
 * @param[in,out] PcieCore  Pointer to PcieCore descitport to be removed from the linked list
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


  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  Pcie = (PCIe_PLATFORM_CONFIG*) PcieConfigGetParent (DESCRIPTOR_PLATFORM, &(PcieCore->Header));

  // Get parent Silicon of PcieCore
  Silicon = (PCIe_SILICON_CONFIG*) PcieConfigGetParent (DESCRIPTOR_SILICON, &(PcieCore->Header));
  InstanceID = Silicon->InstanceId;

  // NBIO_TRACEPOINT (SIL_TRACE_INFO,
  //                  "Silicon = 0x%08X, Silicon->Header.DescriptorFlags = 0x%08X, InstanceID = 0x%08X\n",
  //                  Silicon,
  //                  Silicon->Header.DescriptorFlags,
  //                  InstanceID
  //                  );

  // Invalid. InstanceID should be greater than 0 for this function.
  if (InstanceID == 0)
    return;

  // Get the Child Silicon from Pcie
  PrevSilicon = (PCIe_SILICON_CONFIG*) PcieConfigGetChild (DESCRIPTOR_SILICON, &(Pcie->Header));

  // NBIO_TRACEPOINT (SIL_TRACE_INFO, "Pcie = 0x%08X, Pcie->Header.DescriptorFlags = 0x%08X, PrevSilicon = 0x%08X\n",
  //  Pcie, Pcie->Header.DescriptorFlags, PrevSilicon);

  while (PrevSilicon != NULL) {
    // Look for the previous Silicon
    if (PrevSilicon->InstanceId == (InstanceID - 1))
      break;
    // NBIO_TRACEPOINT (SIL_TRACE_INFO,
    //                  "PrevSi = 0x%08X, PrevSi_DescriptorFlags = 0x%08X, PrevSilicon->InstanceID = %d\n",
    //                  PrevSilicon,
    //                  PrevSilicon->Header.DescriptorFlags,
    //                  PrevSilicon->InstanceId
    //                  );

    // PrevSilicon = PcieConfigGetNextDescriptor(PrevSilicon); Not working since DESCRIPTOR_TERMINATE_LIST is set
    PrevSilicon = (PCIe_SILICON_CONFIG *)PcieConfigGetNextTopologyDescriptor (PrevSilicon,
                                                                              DESCRIPTOR_TERMINATE_TOPOLOGY
                                                                              );
  }
  // Added for solving coverity error, technically PrevSilicon will never be NULL.
  if (PrevSilicon == NULL)
    return;
  PrevPcieCore = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(PrevSilicon->Header));

  // NBIO_TRACEPOINT (SIL_TRACE_INFO, "PrevPcieCore = 0x%08X\n", PrevPcieCore);

  while (PrevPcieCore) {
    // Eventually, the last PcieCore will be saved
    HideInfo->PrevPcieCore = PrevPcieCore;
    PrevPcieCore = PcieConfigGetNextDescriptor(PrevPcieCore);
  }

  // NBIO_TRACEPOINT (SIL_TRACE_INFO, "HideInfo->PrevPcieCore = 0x%08X\n", HideInfo->PrevPcieCore);
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
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

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  // NBIO_TRACEPOINT (SIL_TRACE_INFO, "PcieCore->WrapId = %d, HideInfo->PcieCoreNum = %d, PcieCore = 0x%08X\n",
  // PcieCore->WrapId, HideInfo->PcieCoreNum, PcieCore);

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
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**
 * PcieComplexHidePcieCore
 *
 * @brief   Remvoes a PCIe core descriptor from the SOC Topology Complex.
 * @details In the event of PCIe core power gating or harvesting, we may need to remove a
 * PCIe core desciptor from the SOC Topology Complex to prevent any access to the missing IP.
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

  HideInfo.PcieCoreNum  = PcieCoreNum;
  HideInfo.PrevPcieCore = NULL;
  HideInfo.ThisPcieCore = NULL;
  HideInfo.NextPcieCore = NULL;
  HideInfo.FirstPciePortOnNext = NULL;
  HideInfo.LastPciePortOnThis  = NULL;
  HideInfo.LastPciePortOnPrev  = NULL;

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  // NBIO_TRACEPOINT (SIL_TRACE_INFO, "% GnbHandle = 0x%08X\n", GnbHandle);

  // Update HideInfo with ThisPcieCore, PrevPcieCore, and NextPcieCore
  PcieConfigRunProcForAllWrappersInNbio (
    DESCRIPTOR_ALL_WRAPPERS,
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
    HideInfo.NextPcieCore = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor (HideInfo.ThisPcieCore,
                                                                                        DESCRIPTOR_TERMINATE_TOPOLOGY);
    DeleteLastPcieCore = 1;
    // NBIO_TRACEPOINT (SIL_TRACE_INFO, "HideInfo.NextPcieCore = 0x%08Xd, WrapId = 0x%08X, DescriptorFlags = 0x%08X\n",
    // HideInfo.NextPcieCore, HideInfo.NextPcieCore->WrapId, HideInfo.NextPcieCore->Header.DescriptorFlags);
  }

  // Find the 1st PciePort on NextPcieCore
  if (HideInfo.NextPcieCore != NULL){
    HideInfo.FirstPciePortOnNext = (PCIe_ENGINE_CONFIG *) PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &((HideInfo.NextPcieCore)->Header));
  } else {
    HideInfo.FirstPciePortOnNext=NULL;
  }
  // Find the Last PciePort on PrevPcieCore
  PcieConfigRunProcForAllEnginesInWrapper (
    DESCRIPTOR_PCIE_ENGINE,
    FindLastPciePortOnPrevPcieCore,
    &HideInfo,
    HideInfo.PrevPcieCore
    );

  // Find the Last PciePort on ThisPcieCore
  PcieConfigRunProcForAllEnginesInWrapper (
    DESCRIPTOR_PCIE_ENGINE,
    FindLastPciePortOfThisPcieCore,
    &HideInfo,
    HideInfo.ThisPcieCore
    );

  // NBIO_TRACEPOINT (SIL_TRACE_INFO,
  //                  "PcieCoreNum = %d, PrevPcieCore = 0x%08X, ThisPcieCore = 0x%08X, NextPcieCore = 0x%08X\n,
  //                  FirstPciePortOnNext = 0x%08X, LastPciePortOnThis = 0x%08X, LastPciePortOnPrev 0x%08X\n",
  //                  HideInfo.PcieCoreNum,
  //                  HideInfo.PrevPcieCore,
  //                  HideInfo.ThisPcieCore,
  //                  HideInfo.NextPcieCore,
  //                  HideInfo.FirstPciePortOnNext,
  //                  HideInfo.LastPciePortOnThis,
  //                  HideInfo.LastPciePortOnPrev
  //                  );

  // First wrapper (PcieCore0) in GnbHandle
  if (DeleteFirstPcieCore) {
    // If it's NOT on NBIO0, need to set the PEER
    // NBIO_TRACEPOINT (SIL_TRACE_INFO, "Silicon->InstanceId = %d\n", Silicon->InstanceId);
    if (Silicon->InstanceId != 0) {
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER,
                                   &HideInfo.PrevPcieCore->Header,
                                   &HideInfo.NextPcieCore->Header
                                   );
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE,
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
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER,
                                   &HideInfo.PrevPcieCore->Header,
                                   &HideInfo.NextPcieCore->Header
                                   );
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE,
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

  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}

