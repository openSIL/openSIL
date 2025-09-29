; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;

;
; @file         CpuLib64.nasm
; @brief        Low level CPU operations
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUslCpuSleep
global xUslDisableCache
global xUslEnableCache

    SECTION .text
    bits 64

;------------------------------------------------------------------------------
; NASM_ABI void
; void xUslCpuSleep(void)
;
; @brief Places the CPU in a sleep state until an interrupt is received. If
; interrupts are disabled prior to calling this function, then the CPU will be
; placed in a sleep state indefinitely.
;
;------------------------------------------------------------------------------
xUslCpuSleep:
    hlt
    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslDisableCache (void);
;
;------------------------------------------------------------------------------
xUslDisableCache:
    mov     rax, cr0         ; Get CR0
    or      eax, 0x40000000  ; Set the CD bit (bit 30) to disable the cache
                             ; Note that the NW bit (bit 29) is ignored by AMD
    mov     cr0, rax         ; Move the modified value back into CR0
    wbinvd                   ; Write-back and invalidate the cache to sync it with
                             ; memory and prevent future cache accesses

    ret

;------------------------------------------------------------------------------
; NASM_ABI void
; xUslEnableCache (void);
;
;------------------------------------------------------------------------------
xUslEnableCache:
    wbinvd                   ; The state of the cache is unknown, so flush the
                             ; cache before enabling it.
    mov     rax, cr0         ; Get CR0
    and     eax, 0x9FFFFFFF  ; Enable cache by clearing CD (Cache Disable) bit
                             ; Clear the NW bit as shown in the Cache-Operating
                             ; table of the AMD Architecture spec: Volume 2
    mov     cr0, rax         ; Write CR0 with the new settings
    ret
