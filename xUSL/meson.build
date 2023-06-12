# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# ASM files are specific to the target environment.
# Collect their names in separate lists.
xusl_asm32 = []
xusl_asm64 = []

xusl += files([ 'AmdFeatures.c',
                'Apob.c',
                'BaseSocLogicalIdXlat.c',
                'CoreTopologyService.c',
                'SocLogicalId.c',
                'SilGbls.c' ])

incdir += include_directories( '.' )
incdir += include_directories( 'Include' )

subdir('CommonLib')
subdir('CCX')
subdir('DF')
subdir('FCH')
subdir('MEM')
subdir('Mpio')
subdir('Sdci')
subdir('Cxl')
subdir('NBIO')
subdir('RAS')
subdir('SMU')
subdir('RcMgr')
subdir('XMP')     # the Example Device


# Select the proper group of ASM files for this build
if BUILD_IS_32BIT
    asm_files = xusl_asm32
else
    asm_files = xusl_asm64
endif

if not COVERITY_BUILD
    # Generate the xUSL library (32b or 64b)
    message('Lib: xUSL built for Project')
    xusl_library = static_library(
        'AMD_xusl',
        asm_gen.process(asm_files),
        xusl,
        include_directories : incdir
    )
else
    # coverity does not process NASM files
    # Use the library to cause source compilation
    message('Lib: xUSL built for Coverity')
    xusl_library = static_library(
        'AMD_xusl',
        xusl,
        include_directories : incdir
    )
endif
