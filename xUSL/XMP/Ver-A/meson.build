# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Check the platform config to see if this IP is to be included
#
# Convert the Kconfig (y,n) to (1,0)
Meson_HAVE_XMP_VER_A = (conf.get_unquoted('CONFIG_HAVE_XMP_VER_A', 'n') == 'y') ? 1 : 0
#

if (Meson_HAVE_XMP_VER_A == 1)  or COVERITY_BUILD
  xusl += files([ 'XmpEntry-A.c', 'XmpCmn2RevA.c', 'XmpIp2IpA.c', 'XmpDflts-A.c' ])

  incdir += include_directories( '.' )

  message('IP: XMP Ver-A is INCLUDED in the platform build')
endif
