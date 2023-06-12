# SPDX-License-Identifier: MIT
# Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.

xprf += files( 'xPRF.c' )

subdir('Mem')
subdir('RAS')
subdir('CCX')
subdir('NBIO')
subdir('FCH')

  xprf_library = static_library(
    'AMD_xprf',
    xprf,
    install : true,
    include_directories : incdir
  )
