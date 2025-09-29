/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  CcxApLaunch.c
 * @brief 32 bit AP launch code.
 *
 */

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include <Ccx.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

extern ASSEMBLY_PATCH_LABEL  gApStartupCode;
extern ASSEMBLY_PATCH_LABEL  gApStartupCodeEnd;

static const uint64_t mGdtEntries[] =
{
  0x0000000000000000,  // [00h] Null descriptor
  0x00CF92000000FFFF,  // [08h] Linear data segment descriptor
  0x00CF9A000000FFFF,  // [10h] Linear code segment descriptor
  0x00CF92000000FFFF,  // [18h] System data segment descriptor
  0x00CF9A000000FFFF,  // [20h] System code segment descriptor
  0x0000000000000000,  // [28h] Spare segment descriptor
  0x00CF93000000FFFF,  // [30h] System data segment descriptor
  0x00AF9B000000FFFF,  // [38h] System code segment descriptor
  0x0000000000000000   // [40h] Spare segment descriptor
};

static const uint8_t mAsmNearJump[] =
{
  //[00]
  0x90,         // nop
  //[01]
  0xE9,         // near jmp
  //[02], [03]
  0x00, 0x00    // 0x10000 + (0xFFF0 - Ap startup code offset) - 0xFFF4
};

void
PatchAssemblyInstruction (
  ASSEMBLY_PATCH_LABEL  *InstructionEnd,
  uint64_t               PatchValue,
  uintptr_t              ValueSize
  )
{
  switch (ValueSize) {
  case 1:
    assert(PatchValue <= UINT8_MAX);
    *((uint8_t *)(uintptr_t)InstructionEnd - 1) = (uint8_t)PatchValue;
    break;

  case 2:
    assert(PatchValue <= UINT16_MAX);
    *((uint16_t *)(uintptr_t)InstructionEnd - 1) = (uint16_t)PatchValue;
    break;

  case 4:
    assert(PatchValue <= UINT32_MAX);
    *((uint32_t *)(uintptr_t)InstructionEnd - 1) = (uint32_t)PatchValue;
    break;

  case 8:
    *((uint64_t *)(uintptr_t)InstructionEnd - 1) = (uint64_t)PatchValue;
    break;

  default:
    assert(false);
  }
}



/**
 * SetupApStartupRegion
 * @brief This routine sets up the necessary code and data to launch APs.
 *
 * @details First, saves data in the memory region that will be altered by copying it all
 *          to MemoryContentCopy. Then, does some fixup on the ApStartupCode byte code by
 *          putting in the correct bytes for JMP instructions and the correct pointers for
 *          ApLaunchGlobalData and ApEntryInCOffset. Then, copies everything to memory as
 *          detailed in Ccx.h. Finally, restores the memory region.
 *
 * @param ApLaunchGlobalData  AP launch global data
 * @param ApStartupVector     Pointer AP startup code
 * @param MemoryContentCopy   This is temporary buffer to store data at reset
 *                            vector to allow AP launch
 * @param CcxDataBlock        Pointer to CCX input data block
 *
 */
SIL_STATUS
SetupApStartupRegion (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  void                                   **ApStartupBuffer,
  void                                   *ApStack,
  void                                   *MemoryContentCopy,
  uint32_t                               *MemoryContentCopySize,
  CCXCLASS_DATA_BLK                      *CcxDataBlock,
  CCX_DATA_BLOCK                         *CcxDataBlk
  )
{
  uint8_t               i;
  CCX_GDT32_DESCRIPTOR  BspGdtr;
  uint32_t              EntrySize;
  uint32_t              EntryDest;
  uint32_t              ApBufferSize;

  uint32_t              StartupCodeSize;
  AP_STARTUP_MEMORY_MAP ApStartupMemoryMap;
  uint32_t              ApStartupVector;

  uint32_t Cr3Value = (uint32_t) xUslReadCr3 ();

  if ((ApLaunchGlobalData == NULL) ||
    (ApStartupBuffer == NULL) ||
    (MemoryContentCopy == NULL) ||
    (MemoryContentCopySize == NULL) ||
    (CcxDataBlock == NULL)) {
    CCX_TRACEPOINT(SIL_TRACE_ERROR, "NULL argument(s) passed to SetupApStartupRegion.\n");
    assert(false);
    return SilInvalidParameter;
  }

  if (CcxDataBlock->CcxInputBlock.PspBiosBinEntryInfo.PspBiosBinBase != 0) {
    EntryDest = (uint32_t)CcxDataBlock->CcxInputBlock.PspBiosBinEntryInfo.PspBiosBinBase;
    EntrySize = CcxDataBlock->CcxInputBlock.PspBiosBinEntryInfo.PspBiosBinSize;
  } else {
    // Rom Boot is enabled. Set default value for it.
    EntryDest = 0x10000000;
    EntrySize = 0x100000;
  }

  ApStartupVector = (uint32_t)((uint64_t) EntryDest + EntrySize - 0x10000 + 0xFFF0);

  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupVector = 0x%x\n", ApStartupVector);

  // Use host allocated space for APs to use as stack space
  ApLaunchGlobalData->ApStackBasePtr = (uint32_t)(uintptr_t)ApStack;

  //
  // Allocate space to store data at reset vector
  //

  StartupCodeSize = (uint32_t)((uintptr_t)(gApStartupCodeEnd) - (uintptr_t)(gApStartupCode));

  ApStartupMemoryMap.TopOfApRegion = ApStartupVector;
  ApStartupMemoryMap.PageTable = ApStartupVector - CR3_SAVE_SIZE;            // Should always be at ApStartup Vector - 8
  ApStartupMemoryMap.GdtTable = ApStartupMemoryMap.PageTable - sizeof (mGdtEntries);
  ApStartupMemoryMap.ApMttrSyncList = ApStartupMemoryMap.GdtTable - ApLaunchGlobalData->SizeOfApMtrr;
  ApStartupMemoryMap.ApStartupCodeBase = ApStartupMemoryMap.ApMttrSyncList - StartupCodeSize;

  *ApStartupBuffer = (void *)(uintptr_t)ApStartupMemoryMap.ApStartupCodeBase;
  ApBufferSize = CR3_SAVE_SIZE + sizeof (mGdtEntries) + ApLaunchGlobalData->SizeOfApMtrr + StartupCodeSize + 0x10;


  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupMemoryMap.TopOfApRegion = 0x%x\n", ApStartupMemoryMap.TopOfApRegion);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupMemoryMap.PageTable = 0x%x\n", ApStartupMemoryMap.PageTable);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupMemoryMap.GdtTable = 0x%x\n", ApStartupMemoryMap.GdtTable);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupMemoryMap.ApMttrSyncList = 0x%x\n", ApStartupMemoryMap.ApMttrSyncList);
  CCX_TRACEPOINT(SIL_TRACE_INFO, "ApStartupMemoryMap.ApStartupCodeBase = 0x%x\n", ApStartupMemoryMap.ApStartupCodeBase);


  if (*MemoryContentCopySize < ApBufferSize) {

    assert(false);

    //Trace buffer size too small.
    return SilOutOfResources;
  }

  *MemoryContentCopySize = ApBufferSize;

  // Copy data at reset vector to temporary buffer so we
  // can temporarily replace it with AP start up code.
  memcpy(MemoryContentCopy, *ApStartupBuffer, ApBufferSize);

  // Copy AP start up code to Segment + 0xFFF0 - AP_STARTUP_CODE_OFFSET
  memcpy((void *)(uintptr_t)ApStartupMemoryMap.ApStartupCodeBase,
    (void *)(uintptr_t)&gApStartupCode,
    (uintptr_t)StartupCodeSize
    );

  ApStartupFixups(&ApStartupMemoryMap, (void *)ApLaunchGlobalData);

  ApLaunchGlobalData->ApSyncCount = 0;
  ApLaunchGlobalData->AllowToLaunchNextThreadLocation = ApStartupVector + 0xa;
  *(uint32_t *)(uintptr_t)ApLaunchGlobalData->AllowToLaunchNextThreadLocation = 0;

  // Copy the near jump to AP startup code to reset vector. The near jump
  // forces execution to start from CS:FFF0 - AP_STARTUP_CODE_OFFSET
  memcpy((void *)(uintptr_t)ApStartupVector, (void *)mAsmNearJump, sizeof (mAsmNearJump));

  // Fixup AsmNearJump
  *(uint16_t *)((uintptr_t)ApStartupVector + 2)
    = (uint16_t)(0x10000 + (0xFFF0 - ((uintptr_t)ApStartupVector - ApStartupMemoryMap.ApStartupCodeBase)) - 0xFFF4);

  memcpy((void *)(uintptr_t) ApStartupMemoryMap.GdtTable, (void *) &mGdtEntries, sizeof (mGdtEntries));

  // Load Fixed-MTRRs list with values from BSP.
  xUslMsrOr(MSR_SYS_CFG, BIT_64(19));

  for (i = 0; ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    ApLaunchGlobalData->ApMtrrSyncList[i].MsrData =
      xUslRdMsr(ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr);
  }

  // Some Fixed-MTRRs should be set according to input arguments
  UpdateApMtrrSettings(ApLaunchGlobalData->ApMtrrSyncList, &(CcxDataBlock->CcxInputBlock));

  xUslMsrAnd(MSR_SYS_CFG, ~((uint64_t) SYS_CFG_MTRR_FIX_DRAM_MOD_EN));

  ApLaunchGlobalData->BspMsrLocation = (uint32_t)ApStartupMemoryMap.ApMttrSyncList;
  memcpy((void *)(uintptr_t)ApStartupMemoryMap.ApMttrSyncList,
    (void *)(uintptr_t)ApLaunchGlobalData->ApMtrrSyncList,
    ApLaunchGlobalData->SizeOfApMtrr
    );

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "ApLaunchGlobalData->AllowToLaunchNextThreadLocation = 0x%x\n",
    ApLaunchGlobalData->AllowToLaunchNextThreadLocation
    );

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "ApLaunchGlobalData->BspMsrLocation = 0x%x\n",
    ApLaunchGlobalData->BspMsrLocation
    );

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "ApLaunchGlobalData->ApMtrrSyncList = 0x%x\n",
    ApLaunchGlobalData->ApMtrrSyncList
    );

  BspGdtr.Limit = sizeof (mGdtEntries) - 1;
  BspGdtr.Base = (uint32_t)ApStartupMemoryMap.GdtTable;

  // Copy pointer to GDT entries to Segment + 0xFFF4
  memcpy((void *) ((uint32_t)ApStartupVector + sizeof (mAsmNearJump)),
    &BspGdtr,
    sizeof (BspGdtr)
    );

  // Copy the value of C3 to Segment + 0xFFE8
  *(uint32_t *)(uintptr_t)ApStartupMemoryMap.PageTable = Cr3Value;

  // Save BSP's patch level so that AP can use it to determine whether microcode patch
  // loading should be skipped
  ApLaunchGlobalData->BspPatchLevel = xUslRdMsr(MSR_PATCH_LEVEL);

  memcpy((void *)&CcxDataBlk->ApGdt[0], (void *) &mGdtEntries, sizeof (mGdtEntries));
  ApLaunchGlobalData->ApGdtDescriptor.Limit = (sizeof (mGdtEntries)) - 1;
  ApLaunchGlobalData->ApGdtDescriptor.Base = (uintptr_t) CcxDataBlk->ApGdt;

  // Force content into memory, out of cache, so that AP can have access.
  xUslWbinvd ();

  return SilPass;
}

/**
 * RestoreResetVector
 * @brief This routine restores the code in the AP reset vector once all the
 * APs that were launched are done running AP code.
 *
 * @param[in] ApLaunchGlobalData Global data for AP launch
 * @param[in] TotalCoresLaunched The number of cores that were launched by the BSC
 * @param[in] ApStartupVector pointer to AP startup code
 * @param[in] MemoryContentCopy Temporarily buffer to hold reset vector to start AP.
 *
 */

void
RestoreResetVector (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  uint32_t TotalApCoresLaunched,
  void     *ApStartupBuffer,
  void     *MemoryContentCopy,
  uint32_t  MemoryContentCopySize
  )
{
  if ((ApLaunchGlobalData == NULL) || (ApStartupBuffer == NULL) || (MemoryContentCopy == NULL)) {
    return;
  }

  while (ApLaunchGlobalData->ApSyncCount != TotalApCoresLaunched) {;
  }

  memcpy(ApStartupBuffer, MemoryContentCopy, MemoryContentCopySize);
}
