# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


xsim += files([ 'xSIM.c'
                ])

incdir += include_directories( '.' )

subdir('Common/SoC')
subdir('SoC')

# Use the library to cause source compilation
  xsim_library = static_library(
    'AMD_xsim',
    xsim,
    install : true,
    include_directories : incdir
  )
