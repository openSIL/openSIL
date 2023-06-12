;
; @file         IoOps32.nasm
; @brief        IO access functions executed in 32-bit mode
;
; Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: MIT
;

%include "Porting.h"

global ASM_TAG(xUSLIoWrite32)
global ASM_TAG(xUSLIoWrite16)
global ASM_TAG(xUSLIoWrite8)
global ASM_TAG(xUSLIoRead32)
global ASM_TAG(xUSLIoRead16)
global ASM_TAG(xUSLIoRead8)
global ASM_TAG(xUSLIoReadModifyWrite8)
global ASM_TAG(xUSLIoReadModifyWrite16)
global ASM_TAG(xUSLIoReadModifyWrite32)
global ASM_TAG(xUSLIoWriteCheck8)
;------------------------------------------------------------------------------
    SECTION .text
    bits 32

;------------------------------------------------------------------------------
; CommonLib/Io.h:  CommonLib/Io.h: void xUSLIoWrite32(uint16_t Port, uint32_t Value)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoWrite32):
    mov     dx, [esp +4] ; Port
    mov     eax, [esp + 8] ; Value
    out     dx, eax
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoWrite16(uint16_t Port, uint16_t Value)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoWrite16):
    mov     dx, [esp +4] ; Port
    mov     ax, [esp + 8] ; Value
    out     dx, ax
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoWrite8(uint16_t Port, uint8_t Value)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoWrite8):
    mov     dx, [esp +4] ; Port
    mov     al, [esp + 8] ; Value
    out     dx, al
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead32(uint16_t Port)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoRead32):
    mov     dx, [esp + 4] ; Port
    in      eax, dx
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead16(uint16_t Port)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoRead16):
    mov     dx, [esp + 4] ; Port
    in      ax, dx
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLRead8(uint16_t Port)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoRead8):
    mov     dx, [esp + 4] ; Port
    in      al, dx
    ret

;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite8(uint16_t Port, uint8_t AndMask, uint8_t OrMask)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoReadModifyWrite8):
    push    bx
    xor     eax, eax
    mov     dx, [esp + 4] ; Port
    in      al, dx
    mov     bl, [esp + 8] ; AndMask
    and     al, bl
    mov     bl, [esp + 12] ; OrMask
    or      al, bl
    out     dx, al
    pop     bx
    ret


;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite16(uint16_t Port, uint16_t AndMask, uint16_t OrMask)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoReadModifyWrite16):
    push    bx
    xor     eax, eax
    mov     dx, [esp + 4] ; Port
    in      ax, dx
    mov     bx, [esp + 8] ; AndMask
    and     ax, bx
    mov     bx, [esp + 12] ; OrMask
    or      ax, bx
    out     dx, ax
    pop     bx
    ret


;------------------------------------------------------------------------------
;  CommonLib/Io.h: void xUSLIoReadModifyWrite32(uint16_t Port, uint32_t AndMask, uint32_t OrMask)
;------------------------------------------------------------------------------
ASM_TAG(xUSLIoReadModifyWrite32):
    push    ebx
    xor     eax, eax
    mov     dx, [esp + 4] ; Port
    in      eax, dx
    mov     ebx, [esp + 8] ; AndMask
    and     eax, ebx
    mov     ebx, [esp + 12] ; OrMask
    or      eax, ebx
    out     dx, eax
    pop     ebx
    ret
