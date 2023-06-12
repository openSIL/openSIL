; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file         PciExpressPciCfg32.nasm
; @brief        PCIe access functions executed in 32-bit mode
;

%include "Porting.h"

global ASM_TAG(xUslPciReadWrite)

SECTION .text

ASM_TAG(xUslPciReadWrite):
  cli

  push   ebp

  ; Prepare local variables
  push   dword 0
  push   dword 0
  push   dword 0

  ; ebp + 0x00 original gdt buffer
  ; ebp + 0x08 return value
  ; ebp + 0x0C original ebp
  ; ebp + 0x10 original eip
  ; ebp + 0x14 lower address
  ; ebp + 0x18 higher address
  ; ebp + 0x1C width
  ; ebp + 0x20 value
  ; ebp + 0x24 0: read other: write
  ; ebp + 0x28 new gdt
  mov    ebp, esp

  ; save original gdt and update new gdt
  lea    eax, [ebp + 0x00]
  sgdt   [eax]
  mov    eax, [ebp + 0x28]
  lgdt   [eax]

  ; Prepare the pointer for back to x86
  mov    eax, cs
  push   eax
  push   BackToX86

  ; Prepare the pointer for entering x64
  push   dword 0x38
  push   StartX64

  ; Enable PAE
  mov    eax, cr4
  bts    eax, 5
  mov    cr4, eax

  ; Set LME
  mov    ecx, 0xc0000080
  rdmsr
  bts    eax, 8
  wrmsr

  ; Set PG (enable paging)
  mov    eax, cr0
  bts    eax, 31
  mov    cr0, eax

  retf

StartX64:
BITS    64
  ; Set rdx to the target address
  xor   rax, rax
  mov   eax, [ebp + 0x18]
  shl   rax, 32
  xor   r9, r9
  mov   r9,  [ebp + 0x14]
  or    rax, r9
  mov   rdx, rax

  ; Read or write?
  mov   eax, [ebp + 0x24]
  cmp   eax, 0
  je    StartRead

StartWrite:
  ; Clear return value
  lea   rax, [ebp + 0x08]
  mov   [rax], dword 0

  ; Copy value to ebx
  mov   ebx, [ebp + 0x20]

  ; Check the width
  mov   eax, [ebp + 0x1C]
  cmp   al, 3
  je    Write32
  cmp   al, 2
  je    Write16

Write8:
  mov   [rdx], bl
  jmp   RWEnd

Write16:
  mov   [rdx], bx
  jmp   RWEnd

Write32:
  mov   [rdx], ebx
  jmp   RWEnd

StartRead:
  ; Check the width
  mov   eax, [ebp + 0x1C]
  cmp   al, 3
  je    Read32
  cmp   al, 2
  je    Read16

Read8:
  xor   ebx, ebx
  mov   bl, byte [rdx]
  lea   rax, [ebp + 0x08]
  mov   [rax], ebx
  jmp   RWEnd

Read16:
  xor   ebx, ebx
  mov   bx, word [rdx]
  lea   rax, [ebp + 0x08]
  mov   [rax], ebx
  jmp   RWEnd

Read32:
  xor   ebx, ebx
  mov   ebx, dword [rdx]
  lea   rax, [ebp + 0x08]
  mov   [rax], ebx
  jmp   RWEnd

RWEnd:
  ; Return to x86
  retf

BackToX86:
BITS    32
  ; Clear PG (disable paging)
  mov    eax, cr0
  btc    eax, 31
  mov    cr0, eax

  ; Clear LME
  mov    ecx, 0xc0000080
  rdmsr
  btc    eax, 8
  wrmsr

  ; Disable PAE
  mov    eax, cr4
  btc    eax, 5
  mov    cr4, eax

  ; Restore gdt
  lea   eax, [ebp + 0x00]
  lgdt  [eax]

  ; Pop local variables and set return value
  pop    eax
  pop    eax
  pop    eax

  pop    ebp

  ret
