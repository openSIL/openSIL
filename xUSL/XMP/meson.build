# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Check the platform config to see if this IP is to be included
#
# Convert the Kconfig (y,n) to (1,0)
Meson_HAVE_XMP_VER_A = (conf.get_unquoted('CONFIG_HAVE_XMP_VER_A', 'n') == 'y') ? 1 : 0
Meson_HAVE_XMP_VER_B = (conf.get_unquoted('CONFIG_HAVE_XMP_VER_B', 'n') == 'y') ? 1 : 0

if (Meson_HAVE_XMP_VER_A == 1) or (Meson_HAVE_XMP_VER_B == 1) or COVERITY_BUILD

    #xusl += files(['.c' ])   # No C files at XMP root
    incdir += include_directories( '.' )

    subdir('Common')
    subdir('Ver-A')
    subdir('Ver-B')

endif
