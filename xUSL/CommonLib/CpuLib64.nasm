;
; @file         CpuLib64.nasm
; @brief        Low level CPU operations
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUslCpuSleep

    SECTION .text
    bits 64

;------------------------------------------------------------------------------
; CommonLib/CpuLib.h: void xUslCpuSleep(void)
;
; @brief Places the CPU in a sleep state until an interrupt is received. If
; interrupts are disabled prior to calling this function, then the CPU will be
; placed in a sleep state indefinitely.
;
; @expected users: For openSIL internal use
;------------------------------------------------------------------------------
xUslCpuSleep:
    hlt
    ret
