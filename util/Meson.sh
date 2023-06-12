#!/bin/bash
#
MsnHelp () {
    echo "*******************************************************************"
    echo "** Meson.sh                                                       *"
    echo " *    Provide a command set to simplify the Meson operations for  *"
    echo " *    the openSIL platform build tools  (meson)                   *"
    echo " *                                                                *"
    echo " *   %1 param - Action and target obj size                        *"
    echo " *        L32 32 or Set32 - establishes the 32bit build dir/env   *"
    echo " *        L64 64 or Set64 - established the 64bit build dir/env   *"
    echo " *        Build32 or Build64 - initiates a build for the 32/64 env*"
    echo " *                                                                *"
    echo " * Linux shells CANNOT set environment variables for the parent   *"
    echo " *   shell. You can use this script to establish the meson build  *"
    echo " *   directory; then use the echoed command to start the compile, *"
    echo " *   or again use this script with Buildnn parameter.             *"
    echo "*******************************************************************"
    return
};

#
#  Create the build directory and project config for Meson builder
#
shopt -s nocasematch        # ignore case
case $1 in
  ( L32 | 32 | Set32 )
    meson setup build_L32 --cross-file util/meson/x86-i386-linux-gnu
    echo "***** The Meson environment for Linux 32b is ready *****"
    echo "* Use this cmd for building:"
    echo "*  $   meson compile -C build_L32"
    echo "*"
    ;;

  (Build32 )
    meson compile -C build_L32
  ;;

  ( L64 | 64 | set64 )
    meson setup build_L64 --cross-file util/meson/x86-amd64-linux-gnu
    echo "**** The Meson environment for Linux 64b is ready ****"
    echo "* Use this cmd for building:"
    echo "*   $   meson compile -C build_L64"
    echo "*"
    ;;

  ( Build64 )
    meson compile -C build_L64
  ;;

  ( * )
    MsnHelp
    exit 1
esac


