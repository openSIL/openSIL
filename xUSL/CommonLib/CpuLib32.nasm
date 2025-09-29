; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;

;
; @file         CpuLib32.nasm
; @brief        Low level CPU operations
;

%include "Porting.h"

global ASM_TAG(xUslCpuSleep)
global ASM_TAG(xUslDisableCache)
global ASM_TAG(xUslEnableCache)

    SECTION .text
    bits 32

;------------------------------------------------------------------------------
; NASM_ABI void
; void xUslCpuSleep(void)
;
; @brief Places the CPU in a sleep state until an interrupt is received. If
; interrupts are disabled prior to calling this function, then the CPU will be
; placed in a sleep state indefinitely.
;
;------------------------------------------------------------------------------
ASM_TAG(xUslCpuSleep):
    hlt
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslDisableCache  (void);
;
;------------------------------------------------------------------------------
ASM_TAG(xUslDisableCache):
    mov     eax, cr0         ; Get CR0
    or      eax, 0x40000000  ; Set the CD bit (bit 30) to disable the cache
                             ; Note that the NW bit (bit 29) is ignored by AMD
    mov     cr0, eax         ; Move the modified value back into CR0
    wbinvd                   ; Write-back and invalidate the cache to sync it with
                             ; memory and prevent future cache accesses
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslEnableCache (void);
;
;------------------------------------------------------------------------------
ASM_TAG(xUslEnableCache):
    wbinvd                   ; The state of the cache is unknown, so flush the
                             ; cache before enabling it.
    mov     eax, cr0         ; Get CR0
    and     eax, 0x9FFFFFFF  ; Enable cache by clearing CD (Cache Disable) bit
                             ; Clear the NW bit as shown in the Cache-Operating
                             ; table of the AMD Architecture spec: Volume 2
    mov     cr0, eax         ; Write CR0 with the new settings
    ret
