# AMD openSIL: Proof-of-Concept (POC)


## About the project:

The AMD open Silicon Initialization Library (openSIL) is a collection of C libraries which can be integrated into an x86 host firmware, by directly compiling source or by linking with static libraries.

AMD openSIL consists of three statically linked libraries; xSIM (x86 Silicon Initialization Libraries), xPRF (x86 Platform Reference Library), and xUSL (x86 Utilities & Services Library).  These libraries can be statically linked to a host firmware during compile/link time.

Source for the libraries resides under [xSIM](xSIM), [xPRF](xPRF), and [xUSL](xUSL).

Formal documentation is in final review and will be uploaded to the [GitHub project page](https://github.com/openSIL/openSIL/tree/master/Documentation) when available.<br>
The AMD blog "[Empowering The Industry with Open System Firmware - AMD openSIL](https://community.amd.com/t5/business/empowering-the-industry-with-open-system-firmware-amd-opensil/ba-p/599644)" provides a starting point to grasp the openSIL design goals.<br><br>

## AMD openSIL open-source projected roadmap: 
   ### Evaluation Only Phases (no support for production implementations):
   1. Phase I   - Internal POC (complete).
   2. Phase II  - AMD openSIL POC open-sourced for evaluation on AMD 4th Gen EPYC&trade; based CRB (complete: this source).
   3. Phase III - POC openSIL POC open-sourced, trending Q4 2024.
   ### Production Phase:
   4. Phase IV: - AMD openSIL POR with UEFI Host FW trending 2026.<br><br>
## Getting Started:

1. Clone Repository:
   1. Establish your GitHub user account and your SSH keys (details are beyond this doc)
   2. Open a command/terminal window
   3. Run git to obtain the project:
       ```> git clone git@github.com:openSIL/openSIL.git```<br><br>

2. Establish the project environment variables.

   * You will find a shell/cmd file (SetSilEnv) in the 'util' directory for this purpose.<br><br>

3. Configure your project
   * This release of the AMD openSIL libraries supports AMD 4th Gen EPYC&trade; on the Onyx CRB only.

   * The openSIL project uses the python version of the Kconfig tool for this purpose. (See GitHub Kconfiglib).

   * Run the interactive configuration UI:
      ```> python %PYTHONPATH%\menuconfig.py Kconfig```<br><br>

4. Build openSIL Libraries:
   *  The AMD openSIL library build is performed using 'Meson build', an open source python tool (see GitHub).  Several targets allow a focused build for xUSL, xSIM, xPRF or openSIL(xUSL + xSIM + XPRF) static libraries. AMD openSIL specific Meson build documentation is not yet available.
   * The project can be built for 32bit and/or 64bit compilation and static libraries.
   * The project supports both the GNU/GCC or LLVM/clang tool chain and the Microsoft Visual C tool chain. Generally it is recommended to use the latest versions of these tool chains.<br />
   Specific versions being used today (June 2023) are:
     *  GCC - v10.2.0
     *  llvm/clang - v10.0
     *  MSVC v19.00.24210  (Visual Studio 2015)<br>

5. Integrate with Host Firmware:

   * AMD has a separate repository ("opensil-uefi-interface" ) for helping customers integrate openSIL with previous UEFI-AGESA installations.  Please contact your AMD representative for more information.<br><br>

6. Test Host Firmware on reference platform

   * All AMD openSIL testing has been performed on AMD an AMD reference platform (Onyx CRB).
   * Present list of supported reference platforms is shown in the following table.

     | Market<br>Segment | AMD Processor<br>Family Model | Firmware | Reference Platform<br>Name |
     | ------------------- | -------------------------------- | -------- | ----------------------------- |
     | Server              | F19M10                           | UEFI     | Onyx                          |

## Forthcoming items:
   * Formal documentation to be published to this repository.
   * Continuous integration (CI) tools will be implemented as a pre-requisite to merging pull requests.

## License:

The MIT License (MIT): https://opensource.org/license/mit/
