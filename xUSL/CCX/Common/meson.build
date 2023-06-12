# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# List all C files to be generated in both 32 and 64 bit modes
xusl += files([ 'AmdTable.c', 'Ccx.c', 'CcxBrandString.c',
                'CcxC6.c', 'CcxCacheInit.c', 'CcxDownCoreInit.c',
                'CcxMicrocodePatch.c', 'CcxMiscInit.c', 'CcxSetMca.c',
                'SocServices.c' ])

# For coverity build, force all files into the build
if (BUILD_IS_32BIT or COVERITY_BUILD)
    xusl += files([ 'CcxApLaunch32.c' ])
endif
if ((not BUILD_IS_32BIT) or COVERITY_BUILD)
    xusl += files([ 'CcxApLaunch64.c' ])
endif

incdir += include_directories( '.' )

#List all NASM files to be generated in 32bit mode
xusl_asm32 += files( 'ApAsm32.nasm' )

#List all NASM files to be generated in 64bit mode
xusl_asm64 += files( 'ApAsm64.nasm' )
