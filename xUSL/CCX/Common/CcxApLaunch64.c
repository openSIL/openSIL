/**
 * @file  CcxApLaunch32.c
 * @brief 32 bit AP launch code.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include <Ccx.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

// This needs to be global.
uint8_t ApGdt[0x400];
uint8_t ApStack[AP_STACK_SIZE] = {0,};

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
void
SetupApStartupRegion (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  uint64_t                               *ApStartupVector,
  uint8_t                                *MemoryContentCopy,
  CCXCLASS_DATA_BLK                      *CcxDataBlock
  )
{
  uint8_t             i;
  CCX_GDT_DESCRIPTOR  BspGdtr;
  uint32_t            EntrySize;
  uint32_t            Segment;
  uint32_t            FarJmpOffset;
  uint16_t            NearJmpOffset;
  uint64_t            ApEntryInCOffset;
  uint64_t            Lcl64bData;       // used to convert pointers to 64b data var
  uint64_t            EntryDest;
  uint64_t            C3Value;

  uint8_t ApStartupCode[] =
  {
  //[000] [001] [002]
    0xBE, 0xF4, 0xFF,                     // mov si, 0FFF4h
  //[003]
    0x66,
  //[004] [005] [006] [007]
    0x2E, 0x0F, 0x01, 0x14,               // lgdt       fword ptr cs:[si]
  //[008] [009] [010]
    0x0F, 0x20, 0xC0,                     // mov        eax, cr0                    ; Get control register 0
  //[011] [012] [013] [014]
    0x66, 0x83, 0xC8, 0x03,               // or         eax, 000000003h             ; Set PE bit (bit #0)
  //[015] [016] [017]
    0x0F, 0x22, 0xC0,                     // mov        cr0, eax
  //[018] [019] [020]
    0x0F, 0x20, 0xE0,                     // mov        eax, cr4
  //[021] [022] [023] [024] [025] [026]
    0x66, 0x0D, 0x00, 0x06, 0x00, 0x00,   // or         eax, 00000600h
  //[027] [028] [029]
    0x0F, 0x22, 0xE0,                     // mov        cr4, eax
    // Protected Mode Start
  //[030] [031] [032]
    0xB8, 0x18, 0x00,                     // mov        ax,  18h
  //[033] [034]
    0x8E, 0xD8,                           // mov        ds,  ax
  //[035] [036]
    0x8E, 0xC0,                           // mov        es,  ax
  //[037] [038]
    0x8E, 0xE0,                           // mov        fs,  ax
  //[039] [040]
    0x8E, 0xE8,                           // mov        gs,  ax
  //[041] [042]
    0x8E, 0xD0,                           // mov        ss,  ax
  //[043] [044] [045]
    0x66, 0x67, 0xEA,                     // Far jump
  //[046] [047] [048] [049]
    0x00, 0x00, 0x00, 0x00,               // Byte [48], [49] will be replaced with segment from Host Firmware Directory
                                          // Byte [46], [47] will be replaced with offset of [52]
  //[050] [051]
    0x10, 0x00,

    // Long Mode setup
  //[052] [053] [054]
    0x0F, 0x20, 0xE0,                     // mov eax, cr4
  //[055] [056] [057] [058]
    0x0F, 0xBA, 0xE8, 0x05,               // bts eax, 5                             ; Set PAE (bit #5)
  //[059] [060] [061]
    0x0F, 0x22, 0xE0,                     // mov cr4, eax
  //[062] [063] [064] [065] [066]
    0xB9, 0x80, 0x00, 0x00, 0xC0,         // mov ecx, 0C0000080h                    ; Read EFER MSR
  //[067] [068]
    0x0F, 0x32,                           // rdmsr
  //[069] [070] [071] [072]
    0x0F, 0xBA, 0xE8, 0x08,               // bts eax, 8                             ; Set LME (bit #8)
  //[073] [074]
    0x0F, 0x30,                           // wrmsr

  //[075] [076] [077] [078] [079]
    0xBE, 0xE8, 0xFF, 0x00, 0x00,         // mov esi, 0000ffE8
                                          // Byte [78] [79] will be replaced with segment from Host Firmware Directory
  //[080] [081]
    0x8B, 0x0E,                           // mov ecx, [esi]
  //[082] [083] [084]
    0x0F, 0x22, 0xD9,                     // mov cr3, ecx                           ; Load CR3 with value from BSP

  //[085] [086] [087]
    0x0F, 0x20, 0xC0,                     // mov eax, cr0
  //[088] [089] [090] [091]
    0x0F, 0xBA, 0xE8, 0x1F,               // bts eax, 31                            ; Set PG bit (bit #31)
  //[092] [093] [094]
    0x0F, 0x22, 0xC0,                     // mov cr0, eax

  //[095] [096]
    0x67, 0xEA,                           // Far jump to long mode
  //[097] [098] [099] [100]
    0x00, 0x00, 0x00, 0x00,               // Bytes [99], [100] will be replaced with segment from Host Firmware Directory
                                          // Bytes [97], [98] will be replaced with offset [103]
  //[101] [102]
    0x38, 0x00,
  //[103] [104]
    0x48, 0xB8,                           // mov rax, offset ApEntryInCOffset
  //[105] [106] [107] [108]               // Bytes [105] - [112] will be replaced with offset to ApEntryInCOffset
    0xFF, 0xFF, 0xFF, 0xFF,
  //[109] [110] [111] [112]
    0xFF, 0xFF, 0xFF, 0xFF,
  //[113] [114]
    0x48, 0xBF,                           // mov rdi, offset ApLaunchGlobalData
  //[115] [116] [117] [118]               // Bytes [115] - [122] will be replaced with offset to ApLaunchGlobalData
    0xFF, 0xFF, 0xFF, 0xFF,
  //[119] [120] [121] [122]
    0xFF, 0xFF, 0xFF, 0xFF,
  //[123] [124]
    0xFF, 0xE0                            // jmp rax
  };

  uint64_t GdtEntries[] =
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

  uint8_t AsmNearJump[] =
  {
    //[00]
    0x90,         // nop
    //[01]
    0xE9,         // near jmp
    //[02], [03]
    0x00, 0x00    // 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4
  };

  if ((ApLaunchGlobalData == NULL) ||
    (ApStartupVector == NULL) ||
    (MemoryContentCopy == NULL) ||
    (CcxDataBlock == NULL)) {
    CCX_TRACEPOINT (SIL_TRACE_ERROR, "NULL argument passed to SetupApStartupRegion.\n");
    assert (false);
  }

  C3Value = xUslReadCr3();
  // Page tables need to be accessible in 32-bit address space
  assert (C3Value < 0x100000000);

  EntryDest = CONFIG_PSP_BIOS_BIN_BASE;                 //0x76CD0000, 0x75CD0000
  EntrySize = CONFIG_PSP_BIOS_BIN_SIZE;

  Segment = ((uint32_t) EntryDest + EntrySize - 0x10000);
  // Need to copy upper 16 bits into ApStartupCode
  Segment = Segment >> 16;

  /* ApStartupVector is where the AP will begin executing instructions, at the very
     end of the startup region with the last 16 bits being FFF0 (hence - 0x10).*/
  *ApStartupVector = (uint64_t) (EntryDest + EntrySize - 0x10);

  CCX_TRACEPOINT (SIL_TRACE_INFO, "ApStartupVector = 0x%x\n", *ApStartupVector);

  // Fixup ApStartupCode
  // Copy upper 16 bits of segment to locations needed in ApStartupCode
  memcpy (&ApStartupCode[48], &Segment, sizeof(uint16_t));
  memcpy (&ApStartupCode[78], &Segment, sizeof(uint16_t));
  memcpy (&ApStartupCode[99], &Segment, sizeof(uint16_t));

  // Compute and copy in the jmp offsets
  FarJmpOffset = 0xFFF0 - AP_STARTUP_CODE_OFFSET + 52; // [46], [47] should be replaced with offset of [52]
  memcpy (&ApStartupCode[46], &FarJmpOffset, sizeof(uint16_t));

  FarJmpOffset = 0xFFF0 - AP_STARTUP_CODE_OFFSET + 103; // [97], [98] should be replaced with offset of [103]
  memcpy (&ApStartupCode[97], &FarJmpOffset, sizeof(uint16_t));

  // Places the addresses to ApAsmCode and ApLaunchGlobalData into ApStartupCode
  ApEntryInCOffset = (uintptr_t) &ApAsmCode;

  memcpy (&ApStartupCode[105], &ApEntryInCOffset, sizeof(uint64_t));
  // Need to cast ApLaunchGlobalData to avoid volatile quantifier warning (C4090)
  Lcl64bData = (uintptr_t) &ApLaunchGlobalData;  // translate ptr to uint64 data
  memcpy (&ApStartupCode[115],
           &Lcl64bData,
           sizeof(uint64_t)
         );

  // Use host allocated space for APs to use as stack space
  ApLaunchGlobalData->ApStackBasePtr = (uintptr_t) ApStack;

  // MemoryContentCopy is used to store data at reset vector
  memset (
    MemoryContentCopy,
    0,
    AP_TEMP_BUFFER_SIZE
    );

  // Copy data at reset vector to temporary buffer so we
  // can temporarily replace it with AP start up code.
  memcpy (
    MemoryContentCopy,
    /* coverity[cert_int36_c_violation] */
    (void *) ((uintptr_t)( (*ApStartupVector) - AP_STARTUP_CODE_OFFSET)),
    AP_TEMP_BUFFER_SIZE
    );
  memset (
    (void *) ((uintptr_t)( (*ApStartupVector) - AP_STARTUP_CODE_OFFSET)),
    0,
    AP_TEMP_BUFFER_SIZE
    );

  // Copy AP start up code to Segment + 0xFFF0 - AP_STARTUP_CODE_OFFSET
  memcpy (
    (void *) ((uintptr_t)( (*ApStartupVector) - (AP_STARTUP_CODE_OFFSET) )),
    &ApStartupCode,
    sizeof (ApStartupCode)
    );

  ApLaunchGlobalData->AllowToLaunchNextThreadLocation = (uint32_t) ((*ApStartupVector) + 0xE);

  // Fixup AsmNearJump so that it jumps to the beginning of ApStartupCode
  // 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4
  NearJmpOffset = 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4;
  memcpy(&AsmNearJump[2], &NearJmpOffset, sizeof(uint16_t));

  // Copy the near jump to AP startup code to reset vector. The near jump
  // forces execution to start from CS:FFF0 - AP_STARTUP_CODE_OFFSET
  memcpy ((void *) ((uintptr_t)(*ApStartupVector)), AsmNearJump, sizeof (AsmNearJump));

  // Copy GDT Entries to Segment + 0xFFF0 - BSP_GDT_OFFSET
  memcpy (
    (void *) ((uintptr_t)( (*ApStartupVector) - BSP_GDT_OFFSET)),
    &GdtEntries,
    sizeof (GdtEntries)
    );

  // Load Fixed-MTRRs list with values from BSP.
  xUslMsrOr (MSR_SYS_CFG, BIT_64(19));

  for (i = 0; ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    ApLaunchGlobalData->ApMtrrSyncList[i].MsrData =
        xUslRdMsr (ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr);
  }

  // Some Fixed-MTRRs should be set according to input arguments
  UpdateApMtrrSettings (ApLaunchGlobalData->ApMtrrSyncList, &(CcxDataBlock->CcxInputBlock));

  xUslMsrAnd (MSR_SYS_CFG, ~((uint64_t) SYS_CFG_MTRR_FIX_DRAM_MOD_EN));

  // Copy MTRRs setting to Segment + 0xFFF0 - BSP_MSR_OFFSET
  ApLaunchGlobalData->BspMsrLocation = (uint32_t) ((*ApStartupVector) - (BSP_MSR_OFFSET));

  /* coverity[cert_exp44_c_violation] */
  /* coverity[cert_pre31_c_violation] */
  assert ((ApLaunchGlobalData->SizeOfApMtrr) <= BSP_MSR_SIZE);

  // Need to cast ApLaunchGlobalData to avoid volatile quantifier warning (C4090)
  Lcl64bData = (uintptr_t) ApLaunchGlobalData->ApMtrrSyncList;
  memcpy (
      (void *) ((uintptr_t)( (*ApStartupVector) - BSP_MSR_OFFSET)),
      (void *) &Lcl64bData,
      ApLaunchGlobalData->SizeOfApMtrr
      );

  /*  coverity[cert_pre31_c_violation] */
  CCX_TRACEPOINT (
      SIL_TRACE_INFO,
      "ApLaunchGlobalData->AllowToLaunchNextThreadLocation = 0x%x\n",
      ApLaunchGlobalData->AllowToLaunchNextThreadLocation
      );

  /*  coverity[cert_pre31_c_violation] */
  CCX_TRACEPOINT (
      SIL_TRACE_INFO,
      "ApLaunchGlobalData->BspMsrLocation = 0x%x\n",
      ApLaunchGlobalData->BspMsrLocation
      );

  CCX_TRACEPOINT (
      SIL_TRACE_INFO,
      "ApLaunchGlobalData->ApMtrrSyncList = 0x%x\n",
      (uint32_t)( (*ApStartupVector) - BSP_MSR_OFFSET)
      );

  BspGdtr.Limit = (uint16_t) sizeof (GdtEntries) - 1;
  BspGdtr.Base = (uint64_t) ((*ApStartupVector) - BSP_GDT_OFFSET);

  // Copy pointer to GDT entries to Segment + 0xFFF4
  memcpy (
    (void *) ((uintptr_t)( (*ApStartupVector) + sizeof (AsmNearJump) )),
    &BspGdtr,
    sizeof (BspGdtr)
    );

  memcpy ((void *) ((uintptr_t)( (*ApStartupVector) - 0x08)),
          &C3Value,
          sizeof (C3Value)
    );

  // Save BSP's patch level so that AP can use it to determine whether microcode patch
  // loading should be skipped
  ApLaunchGlobalData->BspPatchLevel = xUslRdMsr (MSR_PATCH_LEVEL);

  memcpy (ApGdt, &GdtEntries[0], sizeof (GdtEntries));

  ApLaunchGlobalData->ApGdtDescriptor.Limit = (sizeof (GdtEntries)) - 1;
  ApLaunchGlobalData->ApGdtDescriptor.Base = (uint64_t)((uintptr_t) ApGdt);

  // Force content into memory, out of cache, so that AP can have access.
  xUslWbinvd ();
}
