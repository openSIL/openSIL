AMD openSIL project (Host API)
==============================

 @tableofcontents
This document details the actions the Host firmware needs to take in preparation to build and operate the AMD silicon Initialization module; and to highlight the Application Interface (API) to be used by the Host firmware to access the openSIL initialization services.

## Host Side Preparations

There are several topics the Host firmware needs to understand in order to incorporate the openSIL project into their firmware structure. Not just coding programs, but also the mechanics of file placement, supplying platform details and creating a build environment.

### Logistics

Details the Host needs to know about the openSIL module and the expectations placed upon the Host firmware.

#### Include Path

The Host program files will need to reference C include files for the API function prototypes, structures and constants needed to operate the API. All of the header, or include, files needed are available in one sub directory ( '/Include').

#### Multi-platform Support

Be sure to organize your Host firmware source code tree to handle building two or more platforms in the same tree and being under active development at the same time. Code specific to the platform containing values or settings special to the platform should be separated into platform directories. The openSIL can be shared, but needs the config file passed as a parameter during the build process.

### Configuration

The openSIL libraries contain code support for multiple AMD processors (SoCs) pertaining to several market segments and socket infrastructures. Each platform is required to establish a configuration file that selects the support, features and options to be used on the platform.

The Host build environment is responsible for passing the target platform name to the openSIL build so as to incorporate the proper platform needs.

#### The Config Files

An openSIL config file contains the settings to select the SoC(s) to be supported, details about the platform (number of sockets) and more. These files are stored in the ( '/configs') directory. Each filename is comprised of the &lt;Platform_Name&gt;, '_SilCfg' and the type of '.h'.

##### Connection to the Build

The platform config file is included into every openSIL C file compiled for the platform.

- regardless of the Host environment, the platform owner must ensure the openSIL config file for the platform exists in the configs directory. See the [Kconfig tool](KcfgTool) section.

##### UEFI Example

For a UEFI Host, the platform name may already exist in a variable for use elsewhere; so, it can be used also for openSIL. The platform name string, appended with '_SilCfg.h' will be the config file name used in the build.

```c
[BuildOptions]
# Set the compiler to include the platform openSIL config file
   DEBUG_*_IA32_CC_FLAGS        =  /FIOpenSIL/configs/$(SIL_PLATFORM_NAME)_SilCfg.h
   RELEASE_*_IA32_CC_FLAGS      =  /FIOpenSIL/configs/$(SIL_PLATFORM_NAME)_SilCfg.h
   DEBUG_*_X64_CC_FLAGS         =  /FIOpenSIL/configs/$(SIL_PLATFORM_NAME)_SilCfg.h
   RELEASE_*_X64_CC_FLAGS       =  /FIOpenSIL/configs/$(SIL_PLATFORM_NAME)_SilCfg.h

```

This control block is added to the &lt;openSIL-pkg&gt;.inf file to instruct the UEFI build system to add this config file to the include list for all of the openSIL files to be compiled.

## Tools

### Meson Build

The openSIL project uses the Mesonbuild python tool set for performing local project builds. The Host environment may choose to use these build files or incorporate their own system.

Prior to initiating a meson build, the user must  set the meson variable "PlatKcfg" to the string representing the platform name:

```c
>  meson configure build_w32 -DPlatKcfg="MyPlat"
```

For more information about the Meson build options,

* see @subpage meson-top "Project Builds"

### Kconfig Tool

The Kconfig tool is provided for creating and managing the platform configuration files.

More information about the configuration sub-system,

* see @subpage Kcfg-top "Configurating a project"

### Compiler Tool Chain

openSIL code may be compiled using one of the following tool chains. Latest versions are preferable. The minimum tested are listed.

- GNU C - GCC v9.4.0 as delivered in Ubuntu 20.04
- MSVC  - Visual Studio 2019
  Both of these are used in the Continuous Improvement (CI) Quality Assurance (QA) testing for the project.

## Host Coding

The Host firmware will need to create some program files to properly interface the Host firmware structure with the openSIL project code. These programs deal with setting up the execution environment, providing user option selections to the openSIL code, processing output information for use in the platform and handling any error conditions that may occur.

### Establishing the Environment

When first starting, the Host must establish the operating environment for the openSIL module. Certain requirements must be set by the Host before the process of initialization can begin.

#### Working Memory

The openSIL system needs some dedicated memory for processing the silicon initialization. Many of the algorithms used need this working space to process the complex details of the SoC.  The Host API includes a few routines used to handle openSIL infrastructure. These are routines that are not connected to any particular SoC or IP, but instead provide housekeeping services.

The Host owns all memory, so the openSIL system relies on the Host to allocate a chunk of memory to the openSIL system. The Host uses the call to xSilQueryMemoryRequirements to find out how much memory is needed, then uses the call xSilAssignMemory to inform the openSIL system of the location and size.

- [SilQueryMemoryRequirements](#HostSIL_Mem) - Gather Memory Requirements
- [SilAssignMemoryTp1](#HostSIL_Assign1) -  Assign The Host Supplied Memory for timepoint 1
- [SilAssignMemoryTp2](#HostSIL_Assign2) -  Assign The Host Supplied Memory for timepoint 2
- [SilAssignMemoryTp3](#HostSIL_Assign3) -  Assign The Host Supplied Memory for timepoint 3

#### Output Messages and Debug Services

Output and debug Services in OpenSIL is controlled by **two** major elements:

1. `SIL_DEBUG_ENABLE` Constant
2. `SilTracePoint` Host-FW Function

*SIL_DEBUG_ENABLE*

The Host may define this value and pass it into the build. This definition sets the program default if the Host does not provide a value for this constant.

> This global variable can be seen used in `SilCommon.h`

*SilTracePoint*

`SilTracePoint` is a function added by the Host-FW into the build as a definition that can be used by OpenSIL *to output log messages*.

> The Host-FW is not *required* to provide an implementation for SilTracePoint BUT still must provide the declaration for `SilTracePoint`. If `SilTracePoint` is not declared, there will be **linker error**

SilTracePoint is referenced in `SilCommon.h` if `SIL_DEBUG_ENABLE` is true and will enable OpenSIL to output log messages *iff* the Host-FW also provides a definition for the function.

**Description**

This function will process the Message, SilPrefix, and VarArgs parameters then output the resulting string to the debug destination. The Host is responsible for any formatting required by the destination device. This function will be called by the openSIL layer via the XSIM_TRACEPOINT, XUSL_TRACEPOINT, and XPRF_TRACEPOINT macros whenever the Host has enabled the openSIL debug service by setting the master enable switch.

```
void
SilTracePoint (
  size_t      SilMsgLevel,
  const char  *SilPrefix,
  const char  *Message,
  const char  *Function,
  size_t      Line,
  ...
  );
```

`SilMsgLevel` - Value from the defined enum that sets the message level of importance. This parameter indicates to which level this message belongs. The Host may filter messages by using level filtering; If the message level meets the threshold of the Host filter value then the Host will send this message to the log output.

- `SIL_TRACE_ERROR` - Unexpected/unwanted (problematic) conditions
- `SIL_TRACE_WARNING` - Result is unexpected but not necessarily a problem
- `SIL_TRACE_ENTRY` - Messages showing entry flow
- `SIL_TRACE_EXIT` - Messages showing exit flow
- `SIL_TRACE_INFO` - Discovery/Configuration information that is useful for debugging

`SilPrefix` - String containing the openSIL prefix message with placeholders for function name, line number, and message level to print.

> xSIM's prefix is `SIL:xSIM:` , xUSL's prefix is `SIL:xUSL:` and xPRF's prefix is `SIL:xPRF:`

`Message` - String containing the message to log. See notes on the format below. This string may contain VarArg style references similar to the standard C printf() functions.

`Function` - name of the function that called SilTracePoint

`Line` - line number of the caller in code

`...` - This is the list of VarArgs to be applied into the message string. The Host environment is responsible for the processing of the VarArgs.

> i.e. an OpenSIL xUSL output message in `CcxZen5.c`
>
> The `...` in SilTracePoint is a parameter for all the arguments, `*Frequency, CpuFid, CpuVid, CpuDfsId` used in the tracepoint below:
>
> ```
> XUSL_TRACEPOINT (SIL_TRACE_INFO,
>     "    FrequencyInMHz=%d, CpuFid=%d, CpuVid=%d, CpuDfsId=%d\n", *Frequency,
>     CpuFid, CpuVid, CpuDfsId);
> ```

**Returns and outputs:**

* None

#### SoCs and IPs

  An SoC (System on Chip) is comprised of several IP (Intellectual Property)
  blocks. Things like the CPU core, memory controller, PCIe Root Bridge,
  SATA controller are each an IP block in the SoC. Many times, the
  same IP block is used in multiple SoCs. The openSIL system is organized
  around the IP blocks. Each SoC has a list of IP blocks contained within.

  The platform may use a socket that supports a group of AMD processors. In this
  case, the platform configuration must indicate which SoCs it plans to support.
  This makes the SoC a variant entity determined at boot time which then loads
  the IP list for that SoC.

### Setting Options

Before the call to start silicon initialization, the Host must set the openSIL IP option values. These are located in the Input blocks of the various IPs.

#### Input Blocks

Each IP has an Input Block of data (assigned from the Host pool provided). While the Kconfig configurator will set many of the IP option selections, the Host must interact with each IP that needs to have dynamic (e.g. SETUP controls) or otherwise dynamically determined settings. The Host must call the FindStructure to locate the IP's  Input block.

- [SilFindStructure](#HostSIL_Find) - Find a structure

Once the IP's Input block is located, the Host can use the structure definitions provided in the IP module api to reference/set specific controls.

- See the 'Modules - @ref MODULES_IP' section of this document for details.

#### Output Blocks

Each IP also has the option to create an output block. After the initialization, the Host must use the  SilFindStructure routine to locate the output block and retrieve the data created during the initialization. There also may be error indicators within these blocks for the Host to interpret.

### Silicon Initialization

The silicon initialization is segregated into two groups. First, the Host makes the call to initialize the base silicon operation. This is needed to bring the silicon to a point where it can process further settings.

- See the 'Pages - @ref TopXsim' section of this document for details.

Next, is the platform customization -  actions needed to customize the SoC
features for the platform and also provide services to collect Silicon related data.

- See the 'Pages - @ref TopXprf' section of this document for details.

Both operations reference the IP module supports listed in the @ref MODULES_IP group of functions.
