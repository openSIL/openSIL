/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxMicrocodePatch.c
 * @brief OpenSIL CCX Microcode patch specific initialization functions.
 *
 */

#include <SilCommon.h>
#include "Ccx.h"
#include <CcxMicrocodePatch.h>
#include <xUslCcxRoles.h>
#include <Pci.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <CCX/CcxClass-api.h>

/**
 * GetMicrocodeVersion
 * @brief Return the version of the currently loaded microcode patch, if any.
 *    Read from the patch level MSR, return the value in eax. If no patch
 *    has been loaded, 0 will be returned.
 *
 * @return  uint32_t MicrocodeVersion
 *
 */
static uint32_t GetMicrocodeVersion (void)
{
  return (uint32_t) xUslRdMsr(MSR_PATCH_LEVEL);
}

/**
 * ValidateMicrocode
 *
 * @brief Determine if the microcode patch block, currently pointed to
 * is valid, and is appropriate for the current processor

 * @param[in]       MicrocodePatchPtr      - Pointer to Microcode Patch.
 * @param[in]       ProcessorId            - Processor ID.
 *
 * @retval          true  - Patch Found.
 * @retval          false - Patch Not Found.
 *
 */
static bool ValidateMicrocode (MICROCODE_PATCH_BLOCK *MicrocodePatchPtr, uint16_t ProcessorId)
{
  bool      Chipset1Matched;
  bool      Chipset2Matched;
  PCI_ADDR  PciAddress;
  uint32_t  PciDeviceVidDid;
  uint8_t   PciDeviceRevision;
  uint8_t   DevCount;
  uint8_t   FunCount;
  uint32_t  Chipset1DeviceID;
  uint32_t  Chipset2DeviceID;
  uint8_t   MulitFunction;

  Chipset1Matched = false;
  Chipset2Matched = false;
  PciDeviceVidDid = 0;
  PciDeviceRevision = 0;
  Chipset1DeviceID = MicrocodePatchPtr->MPB_NB_ID;
  Chipset2DeviceID = MicrocodePatchPtr->MPB_SB_ID;
  MulitFunction = 0;

  //
  // parse the supplied microcode to see if it is compatible with the processor
  //
  if (MicrocodePatchPtr->MPB_REVISION.ProcessorRevisionID != ProcessorId) {
    return false;
  }

  if (Chipset1DeviceID == 0) {
    Chipset1Matched = true;
  }

  if (Chipset2DeviceID == 0) {
    Chipset2Matched = true;
  }

  if ((!Chipset1Matched) || (!Chipset2Matched)) {
    //
    // Scan all PCI devices in Bus 0, try to find out matched case.
    //
    for (DevCount = 0; DevCount < 32; DevCount++) {
      for (FunCount = 0; FunCount < 8; FunCount++) {
        PciAddress.AddressValue = PCI_LIB_ADDRESS(0, DevCount, FunCount, 0);
        PciDeviceVidDid = xUSLPciRead32(PciAddress.AddressValue);
        if (PciDeviceVidDid == 0xFFFFFFFF) {
          if (FunCount == 0) {
            break;
          } else {
            continue;
          }
        }
        PciAddress.Address.Register = PCI_REVISION_ID_REG;
        PciDeviceRevision = xUSLPciRead8(PciAddress.AddressValue);
        if ((!Chipset1Matched) && (PciDeviceVidDid == Chipset1DeviceID)) {
          if (PciDeviceRevision == MicrocodePatchPtr->MPB_REVISION.Chipset1RevisionID) {
            Chipset1Matched = true;
          }
        }
        if ((!Chipset2Matched) && (PciDeviceVidDid == Chipset2DeviceID)) {
          if (PciDeviceRevision == MicrocodePatchPtr->MPB_REVISION.Chipset2RevisionID) {
            Chipset2Matched = true;
          }
        }
        if (Chipset1Matched && Chipset2Matched) {
          break;
        }
        //
        // Check multi-function. If it doesn't exist, we don't have to loop functions to 7.
        //
        if (FunCount == 0) {
          MulitFunction = 0;
          PciAddress.Address.Register = PCI_HEADER_TYPE_REG;
          MulitFunction = xUSLPciRead8(PciAddress.AddressValue);
          if ((MulitFunction & MULTI_FUNC_DEVICE_MASK) == 0) {
            break;
          }
        }
      } // end FunCount for loop.

      if (Chipset1Matched && Chipset2Matched) {
        break;
      }
    } // end DevCount for loop.
  }

  return (Chipset1Matched && Chipset2Matched);
}

/**
 * LoadMicrocode
 *
 * @brief Update microcode patch in current processor, then reads the
 * patch id, and compare it to the expected, in the Microprocessor
 * patch block.
 *
 * @param[in]       MicrocodePatchPtr  - Pointer to Microcode Patch.
 *
 * @retval          true  - Patch Loaded Successfully.
 * @retval          false - Patch Did Not Get Loaded.
 *
 */
static bool LoadMicrocode (MICROCODE_PATCH_BLOCK *MicrocodePatchPtr)
{
  uint32_t     MicrocodeVersion;
  PATCH_LOADER PatchLoaderMsr;

  // Load microcode patch into CPU
  PatchLoaderMsr.RawData = (uint64_t)(uintptr_t) MicrocodePatchPtr;
  PatchLoaderMsr.BitFields.Sbz = 0;
  xUslWrMsr(MSR_PATCH_LOADER, PatchLoaderMsr.RawData);

  // Do ucode patch Authentication
  // Read microcode version back from CPU, determine if
  // it is the same patch level as contained in the source
  // microprocessor patch block passed in
  MicrocodeVersion = GetMicrocodeVersion ();
  if (MicrocodeVersion == MicrocodePatchPtr->PatchLevel) {
    if (xUslIsBsp ()) {
      CCX_TRACEPOINT(SIL_TRACE_INFO, "Microcode Patch 0x%X is loaded.\n", MicrocodeVersion);
    }
    return true;
  }

  return false;
}

/**
 * LoadMicrocodePatch
 *
 * @brief Update microcode patch in current processor.
 * Then reads the patch id, and compare it to the expected, in the Microprocessor
 * patch block.
 *
 * @param SilContext      A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param UcodePatchAddr  The selected UcodePatch address output. Return 0 if not found.
 *
 * @retval    true   - Patch Loaded Successfully.
 * @retval    false  - Patch Did Not Get Loaded.
 *
 */
bool
LoadMicrocodePatch (
  SIL_CONTEXT         *SilContext,
  uint64_t            *UcodePatchAddr
  )
{
  uint16_t           ProcessorId;
  uint64_t           EntryAddress;
  bool               Status;
  CCXCLASS_INPUT_BLK *CcxData;

  Status = false;
  *UcodePatchAddr = 0;

  CcxData = (CCXCLASS_INPUT_BLK *)xUslFindStructure(SilContext, SilId_CcxClass, 0);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "LoadMicrocodePatch Ccx memory found at: 0x%x\n", CcxData);
  if (CcxData == NULL) {
    return Status; // Could not find the IP input block
  }

  xUslWrMsr(MSRxC001100C, 0);
  // Get the processor ID
  ProcessorId = (uint16_t)xUslGetProcessorId ();
  CCX_TRACEPOINT(SIL_TRACE_INFO, "ProcessorId:0x%x\n", ProcessorId);
  EntryAddress = (uint32_t)CcxData->UcodePatchEntryInfo.UcodePatchEntryAddress;
  if (ValidateMicrocode((MICROCODE_PATCH_BLOCK *) (size_t) EntryAddress, ProcessorId)) {
    // Save address of ucode to scratch register
    assert((EntryAddress & 0xFFFFFFFF00000000) == 0);
    xUslWrMsr(MSRxC001100C, EntryAddress);
    *UcodePatchAddr = EntryAddress;
    if (LoadMicrocode((MICROCODE_PATCH_BLOCK *) (size_t) EntryAddress)) {
      Status = true;
    }
  }

  return Status;
}
