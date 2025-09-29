#!/bin/bash
#
# SPDX-License-Identifier: MIT
# Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.

MenuHelp () {
    echo "*******************************************************************"
    echo "** Meson.sh   {command}  {option}                                 *"
    echo " *    Provide a command set to simplify the Meson operations for  *"
    echo " *    the openSIL platform build tools  (meson)                   *"
    echo " *                                                                *"
    echo " *   command - Action and target obj size                         *"
    echo " *        L32 32 or Set32 - establishes the 32bit build dir/env   *"
    echo " *        L64 64 or Set64 - established the 64bit build dir/env   *"
    echo " *        Build32 or Build64 - initiates a build for the 32/64 env*"
    echo " *        Show32 or Show64 - Shows the config for 32/64 env       *"
    echo " *                                                                *"
    echo " *   option - Option for command                                  *"
    echo " *        'wipe' may be used with the setup command to rebuild  *"
    echo " *                the make system control files                   *"
    echo " *        'clean' may be used with the compile command to clear *"
    echo " *                previous compilation reasults.                  *"
    echo "*******************************************************************"
    return
};

#
#  Create the build directory and project config for Meson builder
#
shopt -s nocasematch        # ignore case

CMD=$1
OPT=$2
shift 2

# Verify that the meson version is 1.1.0 or newer
verify_meson_version() {
  IFS='.' read -r -a version <<< "$(meson --version)"

  if [[ "${version[0]}" -gt 1 ]]; then
    return
  elif  [[ "${version[0]}" -eq 1 && "${version[1]}" -ge 1 ]]; then
    return
  fi

  echo "Error: Your meson version is below 1.1.0 and needs to be updated."
  exit 1
}

verify_meson_version

case ${CMD} in
  ( L32 | 32 | Set32 )
    if [[ ${OPT} = wipe ]]; then
        meson setup build_L32 --cross-file util/meson/x86-i386-linux-gnu --wipe "$@"
    else
        echo "meson setup build_L32 --cross-file util/meson/x86-i386-linux-gnu $*"
        meson setup build_L32 --cross-file util/meson/x86-i386-linux-gnu "$@"
    fi
    echo "***** The Meson environment for Linux 32b is ready *****"
    ;;

  ( Build32 )
    if [[ ${OPT} = clean ]]; then
        meson compile -C build_L32 --clean "$@"
    else
        echo "meson compile -C build_L32 $*"
        meson compile -C build_L32 "$@"
    fi
  ;;

  ( Show32 )
    (
      cd build_L32 || exit 1
      meson configure --no-pager
    )
  ;;

  ( L64 | 64 | set64 )
    if [[ ${OPT} = wipe ]]; then
        meson setup build_L64 --cross-file util/meson/x86-amd64-linux-gnu --wipe "$@"
    else
        echo "meson setup build_L64 --cross-file util/meson/x86-amd64-linux-gnu $*"
        meson setup build_L64 --cross-file util/meson/x86-amd64-linux-gnu "$@"
    fi
    echo "**** The Meson environment for Linux 64b is ready ****"
    ;;

  ( Build64 )
    if [[ ${OPT} = clean ]]; then
        meson compile -C build_L64 --clean "$@"
    else
        echo "meson compile -C build_L64 $*"
        meson compile -C build_L64 "$@"
    fi
  ;;

  ( Show64 )
    (
      cd build_L64 || exit 1
      meson configure --no-pager
    )
  ;;

  ( * )
    MenuHelp
    exit 1
esac
