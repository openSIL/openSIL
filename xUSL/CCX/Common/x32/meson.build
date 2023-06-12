# SPDX-License-Identifier: MIT
# Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.

# For coverity build, force all files into the build
if (BUILD_IS_32BIT or COVERITY_BUILD)
  xusl += files([
    'ApStartupFixups.c'
    ])
endif

#List all NASM files to be generated in 32bit mode
xusl_asm32 += files([
  'ApAsm.nasm',
  'ApStartup.nasm'
  ])
