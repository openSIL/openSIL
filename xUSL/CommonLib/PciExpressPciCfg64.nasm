; SPDX-License-Identifier: MIT
; Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
;
; @file         PciExpressPciCfg64.nasm
; @brief        PCIe access functions executed in 64-bit mode
;

; NOTE: ASM_TAG() is not used for 64 bit assembly functions because the C compiler
; does not decorate function names with a leading underscore in 64 bit mode.

global xUslPciReadWrite

SECTION .text
bits 64

; dummy function
xUslPciReadWrite:
ret
