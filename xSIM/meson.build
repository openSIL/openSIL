# SPDX-License-Identifier: MIT
# Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.


xsim += files([ 'SocList.c',
                'SocUtil.c',
                'xSIM.c'
                ])

subdir('SoC')

# Use the library to cause source compilation
  xsim_library = static_library(
    'AMD_xsim',
    xsim,
    install : true,
    include_directories : incdir
  )
