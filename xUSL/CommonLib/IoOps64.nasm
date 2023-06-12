;
; @file         IoOps64.nasm
; @brief        IO access functions executed in 64-bit mode
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUSLIoWrite32
global xUSLIoWrite16
global xUSLIoWrite8
global xUSLIoRead32
global xUSLIoRead16
global xUSLIoRead8
global xUSLIoReadModifyWrite8
global xUSLIoReadModifyWrite16
global xUSLIoReadModifyWrite32
global xUSLIoWriteCheck8

;------------------------------------------------------------------------------
    SECTION .text
    bits 64

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoWrite32(uint16_t Port, uint32_t Value)
;------------------------------------------------------------------------------
xUSLIoWrite32:
    mov     eax, edx ; Value
    mov     dx, cx  ; Port
    out     dx, eax
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoWrite16(uint16_t Port, uint16_t Value)
;------------------------------------------------------------------------------
xUSLIoWrite16:
    mov     ax, dx ; Value
    mov     dx, cx ; Port
    out     dx, ax
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoWrite8(uint16_t Port, uint8_t Value)
;------------------------------------------------------------------------------
xUSLIoWrite8:
    mov     al, dl ; Value
    mov     dx, cx ; Port
    out     dx, al
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead32(uint16_t Port)
;------------------------------------------------------------------------------
xUSLIoRead32:
    mov     dx, cx ; Port
    in      eax, dx
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead16(uint16_t Port)
;------------------------------------------------------------------------------
xUSLIoRead16:
    mov     dx, cx ; Port
    in      ax, dx
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead8(uint16_t Port)
;------------------------------------------------------------------------------
xUSLIoRead8:
    mov     dx, cx ; Port
    in      al, dx
    ret

 ;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite8(uint16_t Port, uint8_t AndMask, uint8_t OrMask)
;------------------------------------------------------------------------------
xUSLIoReadModifyWrite8:
    push    bx
    xor     eax, eax
    push    rdx	; Save AndMask
    mov     dx, cx ;  Port
    in      al, dx
    pop     rdx ; Retreive AndMask
    mov     bl, dl
    and     al, bl
    or      al, r8b
    mov     dx, cx
    out     dx, al
    pop     bx
    ret


;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite16(uint16_t Port, uint16_t AndMask, uint16_t OrMask)
;------------------------------------------------------------------------------
xUSLIoReadModifyWrite16:

    push    bx
    xor     eax, eax
    push    rdx	; Save AndMask
    mov     dx, cx ;  Port
    in      ax, dx
    pop     rdx ; Retreive AndMask
    mov     bx, dx
    and     ax, bx
    or      ax, r8w
    mov     dx, cx
    out     dx, ax
    pop     bx
    ret


;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite32(uint16_t Port, uint32_t AndMask, uint32_t OrMask)
;------------------------------------------------------------------------------
xUSLIoReadModifyWrite32:

    push    rbx
    xor     eax, eax
    push    rdx	; Save AndMask
    mov     dx, cx ;  Port
    in      eax, dx
    pop     rdx ; Retreive AndMask
    mov     ebx, edx
    and     eax, ebx
    or      eax, r8d
    mov     dx, cx
    out     dx, eax
    pop     rbx
    ret
