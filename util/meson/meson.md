@page mesonbuild-top Meson Build Tool
@tableofcontents

# Meson Build Intro      {#meson-top}
 Meson build is a python based replacement for make files or cmake. It is multi-platform, multi-language and is very user friendly ([meson build on the web](https://mesonbuild.com)).

The openSIL project uses meson build for all of its internal test builds.

# Cross compile files

Cross-compile files are used to handle a 64b  Build machine compiling code for a 32b Target machine. The combinations support are as follows (all expect the Build machine to be 64b):

 - x86-amd64-linux-gnu  -  64b linux Build machine compiling for a 64b Target
 - x86-i386-linux-gnu          - 64b linux Build machine compiling for a 32b Target
 - x86-amd64-msvc             - 64b Win10 Build machine compiling for 64b Target
 - x86-i386-msvc                  - 64b Win10 Build machine compiling for 32b Target

Not that the output (Target machine) OS is not specified as the openSIL code is independent of any OS.

# Options file

The Meson_options.txt file describes project option controls which can be used to control the types and output of the project builds. The following options are defined:

- "PlatCfg" - Platform Name. This is used to locate the platform config file in the '\configs' folder. You should set this when you open the build environment to select your platform's config file. The default is the config file used for internal project builds and CI testing.
- "coverity" - Flag to indicate the build is targeted for a Coverity test. This is used by our Continuous Improvement (CI) testing group. Platform builds should never have this enabled.
- "unit_test" - Flag to indicate the build should also include some special code builds for executing unit tests. Platform builds should never have this enabled.

# Build Directories

Meson build can run multiple builds in the same directory since it places all build related files into a build output folder. Due to the cross compile combinations, there are four predefined folders in the script files;
- build_w32    - Win10 32b Target build
- build_w64    - Win10 64b Target build
- build_l32    - Linux 32b Target build
- build_l64    - Linux 64b Target build

# Script support

The meson web pages provide excellent reference for the available commands. The openSIL project provides some script files for the most common command sequences. Use of these are optional. They are located in the openSIL\util folder.

There are two phases to running meson; the build folder setup and the compilation of the project.

<b>*Setup*</b>

Setting up the build folder uses the meson 'setup' command:

    ``` meson setup {folder} --cross-file {filename} {options} ```

This is the most frequently erroneously typed command, so the scripts are provided as an aid to the user.

<b>*Project Compilation*</b>

Building the project uses the meson 'compile' command:

    ``` meson compile -C {folder} {options} ```

This is not as difficult to remember, but using the scripts is advised.

## Script Invocation
The scripts provide a command set to simplify the Meson operations for the openSIL platform build tools. The script parameters are the same for both the  linux environment and the Windows environment.

for the Linux environment: ``` util/Meson.sh   {Command} {options} ```

for the Windows environment: ``` util/Meson.cmd  {Command} {options} ```
      * command - Action and target obj size
          - 32 or Set32 - establishes the 32bit build dir/env
          - 64 or Set64 - established the 64bit build dir/env
          - Build32 or Build64 - initiates a build for the 32/64 target
      * options - These are meson command parameters that may become needed
          - 'wipe' this option can be used with the 'Buildnn' commands. It directs meson to clean out and re-build the project build folder. This may be needed when the meson command files within the code folders have changed and you want the old content removed and the new content scanned for the build.
          - 'clean' this option can be used to clean the build folder of any previously compiled project code files.

Note: using an option with the wrong command will produce a meson program execution error.


# Roll-your-own

The meson.build files are available for use by the Host firmware but it is not required that it use them.

For those firmware systems needing/wanting a build system specific to their Host environment, they will need a list of files to include for the configured platform. The Meson build tools can provide a list of files needed for the configured platform.

See these output files from a build run.
    * ``` filelistC-<platname>.txt ``` contains list of C files for the project
    * ``` filelistH-<platname>.txt ``` contains list of H files for the project
    * ``` filelistI-<platname>.txt ``` contains list of Inc Dirs for the project

The target platform config file must exist prior to this run. Also, you must specify to the Meson system which config file is for the target. Do this by using the meson command to specify the platform config file parameter:

``` meson configure -D PlatKcfg=MyPlatCfg ```

Example:  "meson configure -D PlatKcfg=Mayan_SilCfg"

Note: the default config file is "SilCfg-AllYes" which is used by the CI tests to include all files in the tree.

The filelist files will appear in the build folder. These files have a TXT format (one file per line) suitable for editing import or processing by a script into your Host build system control files.

