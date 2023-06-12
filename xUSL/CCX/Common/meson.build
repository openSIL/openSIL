# SPDX-License-Identifier: MIT
# Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.

# List all C files to be generated in both 32 and 64 bit modes
xusl += files([
  'AmdTable.c',
  'Ccx.c',
  'CcxApLaunch.c',
  'CcxBrandString.c',
  'CcxC6.c',
  'CcxCacheInit.c',
  'CcxMicrocodePatch.c',
  'CcxMiscInit.c',
  'CcxSetMca.c',
  'SocServices.c' ])

incdir += include_directories( '.' )

subdir('x32')
subdir('x64')
