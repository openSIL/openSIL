# Meson Build		{#meson-top}
@tableofcontents
 Meson build is a python based replacement for make files or cmake. It is multi-platform, multi-language and is very user friendly ([meson build on the web](https://mesonbuild.com)).

The openSIL project uses meson build for all of its internal test builds.

## Cross compile files

Cross-compile files are used to handle a 64b  Build machine compiling code for a 32b Target machine. The combinations support are as follows (all expect the Build machine to be 64b):

 - x86-amd64-linux-gnu 	-  64b linux Build machine compiling for a 64b Target
 - x86-i386-linux-gnu          - 64b linux Build machine compiling for a 32b Target
 - x86-amd64-msvc             - 64b Win10 Build machine compiling for 64b Target
 - x86-i386-msvc                  - 64b Win10 Build machine compiling for 32b Target

Not that the output (Target machine) OS is not specified as the openSIL code is independent of any OS.

## Options file

The Meson_options.txt file describes project option controls which can be used to control the types and output of the project builds. The following options are defined:

- "PlatCfg" - Platform Name. This is used to locate the platform config file in the '\configs' folder. You should set this when you open the build environment to select your platform's config file. The default is the config file used for internal project builds and CI testing.
- "coverity" - Flag to indicate the build is targeted for a Coverity test. This is used by our Continuous Improvement (CI) testing group. Platform builds should never have this enabled.
- "unit_test" - Flag to indicate the build should also include some special code builds for executing unit tests. Platform builds should never have this enabled.

## Build Directories

Meson build can run multiple builds in the same directory since it places all build related files into a build output folder. Due to the cross compile combinations, there are four predefined folders in the script files;
- build_w32	- Win10 32b Target build
- build_w64    - Win10 64b Target build
- build_l32      - Linux 32b Target build
- build_l64      - Linux 64b Target build

## Script support

The meson web pages provide excellent reference for the available commands. The openSIL project provides some script files for the most common command sequences. Use of these are optional. They are located in the openSIL\util folder.

### Setup

Setting up the build folder is the most common and frequently erroneously typed commands, so these scripts are provided as an aid to the user.

#### Linux: Meson.sh
This script provides a command set to simplify the Meson operations for the openSIL platform build tools  (meson) in a linux environment.
      * %1 param - Action and target obj size
          - L32 32 or Set32 - establishes the 32bit build dir/env
          - L64 64 or Set64 - established the 64bit build dir/env
          - Build32 or Build64 - initiates a build for the 32/64 target

Linux shells CANNOT set environment variables for the parent shell. You can use this script to establish the meson build directory; then use the echoed command to start the compile, or again use this script with Build__<u>nn</u>__ parameter.

#### Windows: SetSilEnv.cmd

SetSilEnv.cmd

Establish the environment control variable for the openSIL

build tools  (meson & Kconfig)

* %1 param - indicator flag for obj size desired ( 'W32' or 'W64')



## **Roll-your-own**

The meson.build files are available for use by the Host firmware but it is not required that it use them.

For those firmware sytems needing/wanting a build system specific to their Host environment, you will need a list of files to include for the configured platform. The Meson build tools can provide a list of files needed for the configured platform.

See the ```filelist-<platname>.txt``` output file from a build run. The platform config file must exist prior to this run. This file will appear in the root {option soon: in the build} folder. This file is in a format (one file per line) suitable for editing import or processing by a script.

