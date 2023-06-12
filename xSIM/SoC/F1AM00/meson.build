# SPDX-License-Identifier: MIT
# Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
# build control for: xSIM/SoC/F1AM00

# Convert the Kconfig (y,n) to (1,0)
Meson_SOC_F1AM00 = (conf.get_unquoted('CONFIG_SOC_F1AM00', 'n') == 'y') ? 1 : 0
#
if ((Meson_SOC_F1AM00 == 1) or COVERITY_BUILD)

    ##TEMP: While moving the IPs from xSIM to xUSL the Inc list is broken
    ## since we build each Lib separately. Here the IpBlkList file in xUSL
    ## needs the IP incs from xSIM, but since xUSL is done first, the xSIM
    ## list has not yet been scanned. So, list them specifically.
##    incdir += include_directories( '../../../xSIM/SMU/Common',
##            '../../../xSIM' )

    xsim += files([ 'IpBlkListF1AM00Tp1.c',
                    'IpBlkListF1AM00Tp2.c',
                    'IpBlkListF1AM00Tp3.c'
                    ])

    message('Including: FA1 M00 Turin')
endif
