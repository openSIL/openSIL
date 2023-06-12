# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

xprf += files( 'xPRF.c' )

subdir('Mem')
subdir('RAS')
subdir('CCX')
subdir('NBIO')

  xprf_library = static_library(
    'AMD_xprf',
    xprf,
    install : true,
    include_directories : incdir
  )
