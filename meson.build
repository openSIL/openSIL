# Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Meson uses the compiler loaded in the environment. Therefore there are 4
# cross-files used to specify/select the target environment:
#    x86-amd64-linux-gnu    used for linux build Env targeting 64bit objs
#    x86-i386-linux-gnu     used for linux build Env targeting 32bit objs
#    x86-amd64-msvc         used for Win10 build Env targeting 64bit objs
#    x86-i386-msvc          used for Win10 build Env targeting 32bit objs
# The cross-file must be specified when performing the Meson setup cmd:
#   > meson setup build_32 --cross-file x86-i386-linux-gnu -Dunit_test=true
#   > meson setup build_w32 --cross-file x86-i386-msvc -Dunit_test=true
#
# During the CI tests, the Meson build is run 4 times, once in each Env;
# therefore the meson.build file below only needs to focus on the current
# build and not all four.
#
# Output will be found in the build dir set by the SETUP command, and each
# has its own build dir (meaning they can co-exist in the tree).
#
# The user can initiate a local build after the setup has completed by using:
#   > meson compile -C build_w32
# specifying the build dir and optional target name(s) appended.
#
# Current targets are:
#       AMDopensil32,           AMDopensil64,
#       AMD_xsim,               AMD_xusl,               AMD_xprf,
#       pseudo_host
#

project('opensil', 'c',
  version : '0.5',
  default_options : ['warning_level=3'])

message('openSIL:meson.build v0.3.01')

#----------------------------------------------------------
#  Import the Kconfig platform settings
#----------------------------------------------------------
#
# load the keyval module
PlatformConfig = import('keyval')

# Import the platform's Kconfig settings
# Note: The Platform Configuration filename is without extension
#       (e.g. the makefile output version from Kconfig)
PLATFORM_KCFG_FILE = get_option('PlatKcfg')
PLATFORM_KCFG_DIR = get_option('PlatKcfgDir')
message('Using Platform Configuration file: ', PLATFORM_KCFG_FILE)
PlatCfgList = PlatformConfig.load(join_paths(PLATFORM_KCFG_DIR, PLATFORM_KCFG_FILE))
conf = configuration_data(PlatCfgList)

# Place config data_set values for the meson options
conf.set('PlatFormKcfg', PLATFORM_KCFG_FILE)
conf.set('IS_COVERITY_BUILD', get_option('coverity'))   ## for use in code files
COVERITY_BUILD = get_option('coverity')              ## for local use in Meson

config_h = configure_file(
  input: ('configs' / 'SilCfg.Template'),
  output: 'config.h',            ## This file goes into the build_dir
  configuration: conf)


#----------------------------------------------------------
# Retrieve program options and set the build conditions
#   from the cross-file, use get_external_property()
#   from the meson.options.txt file, use get_option()
#   from the Kconfig, use conf.get()
#----------------------------------------------------------
#
BUILD_IS_32BIT = meson.get_external_property('is32bit', false, native: false)

OBJSZ = (BUILD_IS_32BIT ? '32' : '64')      # string used to form names

IS_COVERITY_BUILD = get_option('coverity')

UNIT_TEST = get_option('unit_test')         # sepecified on the meson build
                                            # comand line ("-Dunit_test=true")
                                            # Enables target(s) for CI testing.

#----------------------------------------------------------
#  Set up the tool chain and Compiler options
#----------------------------------------------------------
#
# Determine the compiler in use ( gcc or msvc )
CC_IS_GCC = meson.get_compiler('c').get_id() == 'gcc'
CC_IS_CLANG = meson.get_compiler('c').get_id() == 'clang'

cflags = []

# Set compiler options based on the environment
if CC_IS_GCC or CC_IS_CLANG
  compiler_name = CC_IS_GCC ? 'GCC' : 'CLANG'
  message('compiler found to be : ' + compiler_name)
  # Using the linker option --gc-sections unused data and functions will be stripped.
  cflags += ['-ffunction-sections', '-fdata-sections']

  # Enable all warnings and ignore some warnings
  cflags += ['-Wall', '-Werror',
             '-Wno-pedantic', '-Wno-unknown-pragmas',
             '-Wno-unused-parameter',
             '-Wno-missing-field-initializers']
  # Add some warnings
  cflags += ['-Wstrict-prototypes', '-Wmissing-prototypes']

  # force config.h to be included in all C file compilations
  cflags += ['-include', 'config.h']

  # set obj format for nasm outputs
  objfmt = '-f elf' + OBJSZ
  linkargs = ['-no-pie', '-Wl,--gc-sections']

else   # presume MSVC
  message('compiler found to be : MVSC')
  cflags += '/wd4214'  # non-INT bitfield warning
  cflags += '/wd4127'  # expression is constant warning

  # set obj format for nasm outputs
  objfmt = '-f win' + OBJSZ

  # force config.h to be included in all C file compilations
  cflags += ['/FI', 'config.h']
  linkargs = ''

endif

if CC_IS_GCC
  cflags += '-Wold-style-declaration'
endif
if CC_IS_CLANG
  cflags += '-Wno-self-assign'
endif

add_global_arguments(cflags, language : 'c')

nasm = find_program('nasm')
NasmInc = ('xUSL' / 'Include')
asm_gen = generator(nasm,
 output  : '@BASENAME@.o',
 arguments : [ '-s', '-I.',
            '-I', '@SOURCE_DIR@' / NasmInc,
            objfmt,
            '@INPUT@', '-o', '@OUTPUT@',
            '-l', '@BASENAME@.lst',
            '-Z', '@BASENAME@.err' ])

#
#----------------------------------------------------------
# Invoke the build files down the tree
#----------------------------------------------------------
#   These files will populate the file lists, and
#   the xSim, xPrf and xUsl control files will build
#   those individual libraries
#
#  Variables defined here and used down the tree are:
#       BUILD_IS_32BIT,
#       cpp,        asm_gen,
#       xusl,       xsim,       xprf,       incdir
#
incdir = [include_directories('.')]
incdir += include_directories( 'Include', PLATFORM_KCFG_DIR )
#note: The build_dir is automatically included in the build

#  Include the top library level for the internal API elements
#   Alternative would be to move the Sil Fcns (e.g. GetIpApi) to \Sil.h
incdir += include_directories( 'xSIM' )

xsim = []
xprf = []
xusl = []
subdir('xUSL')
subdir('xSIM')
subdir('xPRF')

#
#----------------------------------------------------------
# Send a list of included source files to a log file
#----------------------------------------------------------
#
print_list =  find_program('util' / 'ListFiles.py')
run_command( print_list, PLATFORM_KCFG_FILE, [xusl, xprf, xsim] )


#----------------------------------------------------------
# Combine the output libs into a single library
#----------------------------------------------------------
#
opensil_library = static_library(
  ('AMDopensil'+OBJSZ),
  link_whole: [xsim_library, xusl_library, xprf_library],
  install : true,
  include_directories : incdir
)

if not IS_COVERITY_BUILD
  # Standard build for a project, follow the file selections in the tree
    #----------------------------------------------------------
    # Unit Tests
    #----------------------------------------------------------
    #
    # If CI flags for doing unit tests, build executables and define tests
    #
    if UNIT_TEST

      pseudoHost = executable(
        'pseudo_host',
        join_paths(meson.source_root(), 'util', 'unitTests', 'linktest.c'),
        link_with : opensil_library,
        install : true,
        include_directories : incdir,
        link_args : linkargs
      )

    endif
  #else
endif
