# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# List all C files to be generated in both 32 and 64 bit modes
xusl += files([ 'CpuOps.c',
                'IPC.c',
                'MmioOps.c',
                'PciOps.c',
                'Pstates.c',
                'SilServices.c',
                'SmnAccess.c',
                'Utils.c',
                'xUslCcxRoles.c'
                ])

#List all NASM files to be generated in 32bit mode
xusl_asm32 += files([ 'CpuLib32.nasm', 'CpuOps32.nasm', 'IoOps32.nasm',  ])

#List all NASM files to be generated in 64bit mode
xusl_asm64 += files([ 'CpuLib64.nasm', 'CpuOps64.nasm', 'IoOps64.nasm',  ])

incdir += include_directories( '.' )

