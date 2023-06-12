# Kconfig Intro        {#Kcfg-top}
@tableofcontents
Kconfig is the build time configuration tool used by the linux kernel, coreboot and a few other popular projects. openSIL used the Kconfiglib project from GitHub, which is a Kconfig implementation in Python 3.
It supports a GUI menuconfig interface for interactive editing of the project values. Find it at [Kconfiglib](https://github.com/ulfalizer/Kconfiglib).
## Kconfig Files
There are ```kconfig``` files (no extension) in nearly every folder in the openSIL project tree. This is a distributed configuration system that keeps the menu items (config values) close to the source using them.
There are a few top level files that provide the overview set of menu pages for selecting the Market Segment, Socket and SoC's for the target platform.

## config Files
The platform configuration files are kept in a separate folder ('openSIL\configs').
The Kconfig tool sub-system expects to run from the project root; so this means that the target platform config file must be copied to the root while being edited, then returned to the config folder. This can be easily messed up by humans, so a script file is provided to organize this process and keep things in order.

## Script Support
The openSIL project provides some script files for the most common command sequences. Use of these are optional but **highly** recommended. They are located in the 'openSIL\util'  folder.

### Linux: Kcfg.sh
Provides a command set to simplify the Kconfig operations for the openSIL platform  configuration process.

#### Expectations:
- Run this tool from the openSIL root directory ```> util\Kcfg menu MyPlat```
- A new platform config file can only be created using 'Menu' command.
- A base config file must already exist for the 'Update' and 'AllYes' command operations.

#### prototype:
```Kcfg  {Command}  {PlatName}```

* Command -  action to perform. Use one of the following:
  - 'menu'    - run the interactive menu system to select options for a specific platform.
                   This produces the [platName] file
  - 'update'  - After creating a [PlatName] file with 'menu',
                   create an updated platform configuration
                   for use in the code base (this creates
                   the [platName].h file)

* PlatName - This is the name used to identify the config.
              This name will be used as the filename for the
              output config file. Some commands will also use
              this name to load previous config selections.

Both parameters are required.
### Win10: Kcfg.cmd
This is a windows version of the above linux shell script. It has the same parameters, expectations and outputs.