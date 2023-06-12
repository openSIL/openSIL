;
; @file ApAsm32.nasm
; @brief 32-bit AP startup code.
; @details
;

;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

%include "Porting.h"

SECTION .text

bits 32

BspMsrLocationOffset                    EQU 0
AllowToLaunchNextThreadLocationOffset   EQU 4
ApStackBasePtrOffset                    EQU 8
ApGdtDescriptorOffset                   EQU 10h

AP_STACK_SIZE                           EQU 200h

extern ASM_TAG(RegSettingBeforeLaunchingNextThread)
extern ASM_TAG(ApEntryPointInC)

global ASM_TAG(ApAsmCode) ; ApAsmCode Address is updated at offset 0x53 in ApStartupCode,
                          ; and ApStartupCode is copied temporarily into reset vector.

;------------------------------------------------------------------------------
; ApAsmCode
;
; @brief    AP startup code
;
; @details  ASM code executed by APs, called at end of ApStartupCode,
;           which syncs MSRs to BSP values, sets up GDT, calls
;           ApEntryPointInC, then allows the next AP to launch
;
;------------------------------------------------------------------------------
ASM_TAG(ApAsmCode):
  ; NOTE: EDI points to ApLaunchGlobalData
  mov ax, 18h
  mov ds, ax
  mov es, ax
  mov ss, ax

  ; Reset ESP
  ; Use only 1 AP stack, later increment of AllowToLaunchNextThreadLocationOffset
  ; needs to be done after finishing stack usage of current AP thread
  mov eax, AP_STACK_SIZE

  mov esi, [edi + ApStackBasePtrOffset]
  add eax, esi
  mov esp, eax

  ; Enable Fixed MTRR modification
  mov ecx, 0C0010010h
  rdmsr
  or  eax, 00080000h
  wrmsr

  ; Setup MSRs to BSP values
  mov esi, [edi + BspMsrLocationOffset]
MsrStart:
  mov ecx, [esi]
  cmp ecx, 0FFFFFFFFh
  jz MsrDone
  add esi, 4
  mov eax, [esi]
  add esi, 4
  mov edx, [esi]
  wrmsr
  add esi, 4
  jmp MsrStart

MsrDone:
  ; Disable Fixed MTRR modification and enable MTRRs
  mov ecx, 0C0010010h
  rdmsr
  and eax, 0FFF7FFFFh
  or  eax, 00140000h
  bt  eax, 21           ;SYS_CFG_MTRR_TOM2_EN
  jnc Tom2Disabled
  bts eax, 22           ;SYS_CFG_TOM2_FORCE_MEM_TYPE_WB
Tom2Disabled:
  wrmsr

  ; Enable caching
  mov eax, cr0
  btr eax, 30
  btr eax, 29
  mov cr0, eax

  ; Call into C code before next thread is launched
  push edi
  call ASM_TAG(RegSettingBeforeLaunchingNextThread)
  pop edi

  ; Call into C code
  push edi
  call ASM_TAG(ApEntryPointInC)
  pop edi

  ; Set up resident GDT
  mov esi, ApGdtDescriptorOffset
  add esi, edi
  lgdt [esi]
  ; Use stack base as a long jump pointer buffer
  mov esi, [edi + ApStackBasePtrOffset]
  ; Update selector
  mov WORD [esi + 4], 0010h
  mov ebx, NewGdtAddress
  mov [esi], ebx
  jmp far [esi]
NewGdtAddress:
  ; Increment call count to allow to launch next thread, after stack usage is done
  mov esi, [edi + AllowToLaunchNextThreadLocationOffset]
  lock inc WORD [esi]

  ; Hlt
Hlt_loop:
  cli
  hlt
  jmp Hlt_loop
