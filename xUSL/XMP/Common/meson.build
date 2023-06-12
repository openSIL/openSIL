# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Check the platform config to see if this IP is to be included
#
#

if (Meson_HAVE_XMP_VER_A == 1) or (Meson_HAVE_XMP_VER_B == 1) or COVERITY_BUILD

  xusl += files([ 'XmpCommon.c', 'XmpCmnDflts.c' ])

  incdir += include_directories( '.' )

endif
