# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
"""Build native tests from provider sources and a generated openSIL config."""

import argparse
import json
import os
from pathlib import Path
import shlex
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[2]
SUITES = {
  "xsim-dispatch": {
    "sources": [
      "xSIM/xSIM.c",
      "tests/HostTest/XsimDispatch.c",
    ],
    "wrap": [],
  },
  "public-headers": {
    "headers": [
      "xPrfSmu.h",
      "xPrfFabricTypes.h",
      "xPrfFabric.h",
      "xPrfFabricAcpi.h",
      "xPrfCxl.h",
    ],
  },
  "provider-contracts": {
    "sources": [
      "xSIM/xSIM.c",
      "xUSL/CommonLib/SilServices.c",
      "xPRF/FCH/xPrfFch.c",
      "xUSL/FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.c",
      "xUSL/FCH/Common/FchCore/FchHwAcpi/FchHwAcpiDefaults.c",
      "xUSL/DF/DfX/DfXAcpiTables.c",
      "xUSL/DF/DfClassDflts.c",
      "xPRF/RAS/xPrfRasServices.c",
      "tests/HostTest/ProviderContracts.c",
    ],
    "wrap": [
      "FchI2cReleaseControl",
      "FchHwAcpiServiceSmiTimerStart",
      "FchHwAcpiServiceSmiTimerStop",
    ],
  },
  "multi-fch": {
    "sources": [
      "xUSL/FCH/Common/MultiFch/MultiFch.c",
      "xUSL/FCH/Kunlun/MultiFch/MultiFchCmn2Kl.c",
      "tests/HostTest/MultiFch.c",
    ],
    "wrap": [],
  },
  "fabric-domain": {
    "sources": [
      "xUSL/DF/DfX/DfXAcpiDomainInfo.c",
      "tests/HostTest/FabricDomain.c",
    ],
    "wrap": [],
  },
  "nbio-ioapic": {
    "sources": [
      "xUSL/Nbio/Brh/NbioIoApic.c",
      "xUSL/CommonLib/SmnAccess.c",
      "tests/HostTest/NbioIoApic.c",
    ],
    "wrap": [],
  },
  "nbio-non-pci-bar": {
    "sources": [
      "xUSL/Nbio/Common/Nbio.c",
      "xUSL/CommonLib/SmnAccess.c",
      "tests/HostTest/NbioNonPciBar.c",
    ],
    "wrap": [],
  },
  "cxl-device-info": {
    "sources": [
      "xUSL/Cxl/Common/CxlDeviceInfo.c",
      "xUSL/Cxl/Common/HostTest/CxlDeviceInfoHostTest.c",
    ],
    "wrap": [],
  },
}


def include_paths(database, config):
  """Keep provider/generated includes while excluding the host firmware libc."""
  commands = json.loads(database.read_text(encoding="utf-8"))
  allowed = (ROOT, database.parent, config.parent)
  paths = [config.parent]
  for command in commands:
    directory = Path(command["directory"])
    if not directory.is_absolute():
      directory = database.parent / directory
    arguments = command.get("arguments")
    if arguments is None:
      arguments = shlex.split(command["command"])
    arguments = iter(arguments)
    for argument in arguments:
      if argument in ("-I", "-isystem", "-iquote"):
        value = next(arguments)
      elif argument.startswith("-I"):
        value = argument[2:]
      else:
        continue
      path = (directory / value).resolve()
      if path == database.parent.parent or any(path == base or base in path.parents for base in allowed):
        if path not in paths:
          paths.append(path)
  if not any(ROOT == path or ROOT in path.parents for path in paths):
    raise ValueError("compile_commands.json must refer to this openSIL checkout")
  return ["-I" + str(path) for path in paths]


def check_headers(args, output, headers):
  """Compile each public header alone, then all together, with native libc."""
  groups = [[header] for header in headers] + [headers]
  failed = False
  for index, group in enumerate(groups):
    source = output / f"public-headers-{index}.c"
    source.write_text("".join(f"#include <{header}>\n" for header in group), encoding="utf-8")
    command = [args.cc, "-std=c17", "-Wall", "-Wextra", "-Werror", "-fsyntax-only",
               "-I" + str(ROOT / "Include"), str(source)]
    (output / f"public-headers-{index}-command.txt").write_text(shlex.join(command) + "\n", encoding="utf-8")
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (output / f"public-headers-{index}-build.log").write_text(result.stdout, encoding="utf-8")
    if result.returncode:
      print(result.stdout, end="")
      failed = True
  print(f"Public headers: {len(groups)} translation units, {'FAIL' if failed else 'PASS'}", flush=True)
  return failed


def run_suites(args, output):
  includes = include_paths(args.compile_commands, args.config)
  command_base = [
    args.cc, "-std=c17", "-O0", "-g", "-Wall", "-Wextra",
    "-Wno-unused-parameter", "-Wno-unused-but-set-variable", "-Wno-unknown-pragmas",
    "-ffunction-sections", "-fdata-sections", "-fno-pie", "-no-pie",
    "-fno-omit-frame-pointer", "-DSIL_DEBUG_ENABLE=false",
    "-include", str(args.config), *includes,
  ]
  if not args.no_sanitizers:
    command_base.append("-fsanitize=address,undefined")
  environment = os.environ.copy()
  # No fixture owns heap allocations. LSan may be unavailable under ptrace.
  environment.setdefault("ASAN_OPTIONS", "detect_leaks=0")
  environment.setdefault("UBSAN_OPTIONS", "halt_on_error=1")
  failed = False
  for name, suite in SUITES.items():
    if args.suite and name not in args.suite:
      continue
    if "headers" in suite:
      failed = check_headers(args, output, suite["headers"]) or failed
      continue
    binary = output / name
    command = [
      *command_base, *(str(ROOT / source) for source in suite["sources"]),
      "-Wl,--gc-sections", *("-Wl,--wrap=" + name for name in suite["wrap"]),
      "-o", str(binary),
    ]
    (output / (name + "-command.txt")).write_text(shlex.join(command) + "\n", encoding="utf-8")
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (output / (name + "-build.log")).write_text(result.stdout, encoding="utf-8")
    if result.returncode:
      print(result.stdout, end="")
      print(f"{name}: compilation failed ({result.returncode})", flush=True)
      failed = True
      continue
    result = subprocess.run([str(binary)], text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, env=environment)
    (output / (name + "-results.txt")).write_text(result.stdout, encoding="utf-8")
    print(result.stdout, end="", flush=True)
    if result.returncode:
      print(f"{name}: test failed ({result.returncode})", flush=True)
      failed = True
  return int(failed)


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument("--compile-commands", type=Path, required=True,
                      help="compile_commands.json from a configured openSIL Meson build")
  parser.add_argument("--config", type=Path, required=True,
                      help="generated platform configuration header (for example opensil_config.h)")
  parser.add_argument("--output-dir", type=Path, help="keep binaries, build commands and logs here")
  parser.add_argument("--cc", default="cc", help="native GCC-compatible C compiler, default: cc")
  parser.add_argument("--no-sanitizers", action="store_true", help="disable ASan and UBSan")
  parser.add_argument("--suite", action="append", choices=SUITES,
                      help="run only this suite; may be repeated")
  args = parser.parse_args()
  args.compile_commands = args.compile_commands.resolve()
  args.config = args.config.resolve()
  for path in (args.compile_commands, args.config):
    if not path.is_file():
      parser.error(f"missing input file: {path}")
  try:
    if args.output_dir:
      output = args.output_dir.resolve()
      output.mkdir(parents=True, exist_ok=True)
      return run_suites(args, output)
    with tempfile.TemporaryDirectory(prefix="opensil-host-test-") as directory:
      return run_suites(args, Path(directory))
  except (OSError, ValueError, KeyError, StopIteration) as error:
    parser.error(str(error))


if __name__ == "__main__":
  raise SystemExit(main())
