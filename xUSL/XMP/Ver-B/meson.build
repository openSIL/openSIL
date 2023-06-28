# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Check the platform config to see if this IP is to be included
#
# Convert the Kconfig (y,n) to (1,0)
Meson_HAVE_XMP_VER_B = (conf.get_unquoted('CONFIG_HAVE_XMP_VER_B', 'n') == 'y') ? 1 : 0
#

if (Meson_HAVE_XMP_VER_B == 1)  or COVERITY_BUILD
  xusl += files([ 'XmpEntry-B.c', 'XmpCmn2RevB.c', 'XmpIp2IpB.c', 'XmpDflts-B.c' ])

  incdir += include_directories( '.' )

  message('IP: XMP Ver-B is INCLUDED in the platform build')
endif
