;
; @file         CpuLib32.nasm
; @brief        Low level CPU operations
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

%include "Porting.h"

global ASM_TAG(xUslCpuSleep)

    SECTION .text
    bits 32

;------------------------------------------------------------------------------
; CommonLib/CpuLib.h: void xUslCpuSleep(void)
;
; @brief Places the CPU in a sleep state until an interrupt is received. If
; interrupts are disabled prior to calling this function, then the CPU will be
; placed in a sleep state indefinitely.
;
; @expected users: For openSIL internal use
;
;------------------------------------------------------------------------------
ASM_TAG(xUslCpuSleep):
    hlt
    ret
