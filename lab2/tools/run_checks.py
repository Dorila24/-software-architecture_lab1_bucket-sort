"""Build and check the published C implementations without an AI runtime."""

import argparse
import json
from pathlib import Path
import re
import subprocess
import sys


def run(command, log):
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    log.write_text(result.stdout, encoding="utf-8")
    print(result.stdout, end="")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True)
    parser.add_argument("--variant", choices=["short", "gost", "ordinary-control"],
                        default="short")
    parser.add_argument("--repeat", type=int, choices=[1, 2, 3], default=1)
    parser.add_argument("--profile", choices=["A", "C"], default="A")
    parser.add_argument("--all", action="store_true")
    parser.add_argument("--extended", action="store_true")
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    logs = root / ".check-logs"
    logs.mkdir(exist_ok=True)
    targets = [(v, r, p) for v in ["short", "gost", "ordinary-control"]
               for r in [1, 2, 3] for p in ["A", "C"]] if args.all else [
                   (args.variant, args.repeat, args.profile)]
    rows = []
    for variant, repeat, profile in targets:
        package = root / "deliverables" / ("sources-" + variant)
        name = "%s-%d-%s" % (variant, repeat, profile)
        command = [sys.executable, "-I", str(package / "build.py"),
                   "--sdk", str(args.sdk.expanduser().resolve()),
                   "--repeat", str(repeat), "--profile", profile, "--check"]
        result = run(command, logs / (name + "-baseline.log"))
        passed = result.returncode == 0 and bool(re.search(
            r"REUSE_CHECK cases=28936 failures=0 mock_live=0 "
            r"environment_refs=1 memory_refs=1", result.stdout))
        if passed and args.extended:
            workspace = package / ".build" / ("repeat-%d-%s" % (repeat, profile))
            command = [sys.executable, "-I", str(workspace / ".arpo-build.py"),
                       "--project", str(workspace), "--profile", profile,
                       "--client", str(root / "docs/audit-20261024/extended-acom.c"),
                       "--sanitize", "--run", "--output", "BuildFiles/ExtendedAudit"]
            result = run(command, logs / (name + "-extended.log"))
            passed = result.returncode == 0 and bool(re.search(
                r"EXTENDED_ACOM cases=39436 extra=10500 failures=0 mock_live=0 "
                r"environment_refs=1 memory_refs=1", result.stdout))
        rows.append({"variant": variant, "repeat": repeat,
                     "profile": profile, "passed": passed})
        (logs / "results.json").write_text(json.dumps(rows, indent=2) + "\n")
        print(name, "PASS" if passed else "FAIL")
        if not passed:
            return 1
    print("ALL_CHECKS_PASSED %d configurations" % len(rows))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
