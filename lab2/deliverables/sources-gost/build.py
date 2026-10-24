"""Standalone student-lab build; requires Python 3, Apple clang and separately supplied Eco SDK."""
from pathlib import Path
import argparse
import hashlib
import json
import platform
import shutil
import subprocess
import sys


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True, help="Root of separately supplied Eco.Framework")
    parser.add_argument("--repeat", type=int, choices=[1, 2, 3], default=1)
    parser.add_argument("--profile", choices=["A", "C"], default="A")
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--benchmark", choices=["calls", "sort"])
    args = parser.parse_args()
    if sys.platform != "darwin" or platform.machine() != "arm64":
        parser.error("This supplied static SDK build requires macOS Apple Silicon")
    root = Path(__file__).resolve().parent
    manifest = json.loads((root / "source-manifest.json").read_text())
    for relative, expected in manifest.items():
        source = root / relative
        if not source.is_file() or digest(source) != expected:
            raise ValueError("Source integrity mismatch: " + relative)
    workspace = root / ".build" / ("repeat-%d-%s" % (args.repeat, args.profile))
    workspace.mkdir(parents=True, exist_ok=True)
    shutil.copytree(root / "common", workspace, dirs_exist_ok=True)
    shutil.copyfile(root / ("variants/repeat-%d/Reuse.c" % args.repeat), workspace / "SourceFiles/Reuse.c")
    receipt_file = root / "provenance/sdk-receipt.json"
    receipt = json.loads(receipt_file.read_text())
    sdk = args.sdk.expanduser().resolve()
    for relative, expected in receipt["files"].items():
        source = sdk / relative
        if not source.is_file() or digest(source) != expected:
            raise ValueError("Expected pinned Eco SDK file is missing or differs: " + str(source))
        destination = workspace / ".sdk" / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, destination)
    shutil.copyfile(receipt_file, workspace / ".sdk/arpo-sdk.json")
    tools = {}
    for name in ["clang", "ar", "nm"]:
        tools[name] = subprocess.check_output(["xcrun", "--find", name], text=True).strip()
    tools["sdkroot"] = subprocess.check_output(["xcrun", "--show-sdk-path"], text=True).strip()
    (workspace / ".arpo-toolchain.json").write_text(json.dumps(tools, indent=2) + "\n")
    command = [sys.executable, str(workspace / ".arpo-build.py"), "--project", str(workspace),
               "--profile", args.profile, "--run", "--output", "BuildFiles/Standalone"]
    if args.check:
        command += ["--client", str(root / "checks/acom.c"), "--sanitize"]
    elif args.benchmark:
        command += ["--client", str(root / ("checks/benchmark.c" if args.benchmark == "calls" else "checks/benchmark-sort.c"))]
    result = subprocess.run(command)
    if result.returncode == 0:
        print("Build artifacts:", workspace / "BuildFiles/Standalone")
    return result.returncode


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print("BUILD_ERROR", error, file=sys.stderr)
        raise SystemExit(1)
