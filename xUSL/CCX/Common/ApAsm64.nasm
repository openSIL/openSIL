;
; @file ApAsm64.nasm
; @brief 64-bit AP startup code.
; @details
;

;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

SECTION .text

bits 64

BspMsrLocationOffset                    EQU 0
AllowToLaunchNextThreadLocationOffset   EQU 4
ApStackBasePtrOffset                    EQU 8
ApGdtDescriptorOffset                   EQU 10h

AP_STACK_SIZE                           EQU 200h

extern RegSettingBeforeLaunchingNextThread
extern ApEntryPointInC

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global ApAsmCode

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
ApAsmCode:
  ; NOTE: EDI points to ApLaunchGlobalData
  mov ax, 30h
  mov ds, ax
  mov es, ax
  mov ss, ax

  ; Reset RSP
  ; Use only 1 AP stack, later increment of AllowToLaunchNextThreadLocationOffset
  ; needs to be done after finishing stack usage of current AP thread
  xor rax, rax
  mov eax, AP_STACK_SIZE

  mov rsi, [edi + ApStackBasePtrOffset]
  add rax, rsi
  mov rsp, rax

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
  mov rax, cr0
  btr eax, 30
  btr eax, 29
  mov cr0, rax

  ; Call into C code before next thread is launched
  call RegSettingBeforeLaunchingNextThread

  ; Call into C code
  mov rcx, rdi
  call ApEntryPointInC

  ; Set up resident GDT
  mov rsi, ApGdtDescriptorOffset
  add rsi, rdi
  lgdt [rsi]
  ; Use stack base as a long jump pointer buffer
  mov esi, [edi + ApStackBasePtrOffset]
  ; Update selector
  mov WORD [rsi + 8], 0038h
  mov rbx, NewGdtAddress
  mov [rsi], ebx
  jmp far [rsi]
NewGdtAddress:
  ; Increment call count to allow to launch next thread, after stack usage is done
  mov esi, [edi + AllowToLaunchNextThreadLocationOffset]
  lock inc WORD [esi]

  ; Hlt
Hlt_loop:
  cli
  hlt
  jmp Hlt_loop
