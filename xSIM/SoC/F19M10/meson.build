# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# build control for: xUSL/SoC/F19M10

# Convert the Kconfig (y,n) to (1,0)
Meson_SOC_F19M10 = (conf.get_unquoted('CONFIG_SOC_F19M10', 'n') == 'y') ? 1 : 0
#
if ((Meson_SOC_F19M10 == 1) or COVERITY_BUILD)

    xsim += files([ 'IpBlkListF19M10Tp1.c',
                    'IpBlkListF19M10Tp2.c',
                    'IpBlkListF19M10Tp3.c'
                    ])

    message('Including: F19 M10 Genoa')
endif

