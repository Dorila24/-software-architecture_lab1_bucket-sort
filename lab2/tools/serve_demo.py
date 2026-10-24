"""Local screencast UI for the real Eco clients; Python standard library only."""

import argparse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
from pathlib import Path
import re
import subprocess
import sys
import threading
import time


ROOT = Path(__file__).resolve().parents[1]
BUILD_LOCK = threading.Lock()
VARIANTS = ("short", "gost", "ordinary-control")


def execute(sdk, settings):
    """Run only a fixed published build/check command, never browser-supplied code."""
    variant = settings.get("variant", "gost")
    repeat = settings.get("repeat", 2)
    profile = settings.get("profile", "A")
    action = settings.get("action", "demo")
    if (variant not in VARIANTS or type(repeat) is not int or repeat not in (1, 2, 3)
            or profile not in ("A", "C") or action not in ("demo", "verify")):
        raise ValueError("Unknown variant, repeat, profile or action")
    if action == "demo":
        relative = "deliverables/sources-%s/build.py" % variant
        command = [sys.executable, "-I", str(ROOT / relative), "--sdk", str(sdk),
                   "--repeat", str(repeat), "--profile", profile]
    else:
        relative = "tools/run_checks.py"
        command = [sys.executable, "-I", str(ROOT / relative), "--sdk", str(sdk),
                   "--variant", variant, "--repeat", str(repeat),
                   "--profile", profile, "--extended"]
    started = time.monotonic()
    result = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, timeout=120)
    output = result.stdout
    if action == "demo":
        expected = ("Addition 3+5=8; subtraction 7-2=5; multiplication 3*5=15; division 7/2=3",
                    "Bucket sort via aggregated Lab65: -100 -4 0 2 7 7")
        passed = result.returncode == 0 and all(line in output for line in expected)
    else:
        passed = result.returncode == 0 and bool(re.search(
            r"EXTENDED_ACOM cases=39436 extra=10500 failures=0 mock_live=0 "
            r"environment_refs=1 memory_refs=1", output))
    return {"passed": passed, "returncode": result.returncode, "output": output,
            "seconds": round(time.monotonic() - started, 3), "variant": variant,
            "repeat": repeat, "profile": profile, "action": action,
            "command": "python3 " + relative + " --sdk <Eco.Framework> --repeat "
                       + str(repeat) + " --profile " + profile
                       + (" --variant " + variant + " --extended" if action == "verify" else "")}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True)
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args()
    sdk = args.sdk.expanduser().resolve()
    if not sdk.is_dir():
        parser.error("--sdk must point to the separately supplied Eco.Framework directory")

    class Handler(BaseHTTPRequestHandler):
        def send(self, status, content, content_type):
            self.send_response(status)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(len(content)))
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(content)

        def reply(self, status, data):
            self.send(status, json.dumps(data, ensure_ascii=False).encode(),
                      "application/json; charset=utf-8")

        def do_GET(self):
            if self.path == "/":
                self.send(200, (ROOT / "docs/demo.html").read_bytes(), "text/html; charset=utf-8")
            elif self.path == "/architecture.png":
                self.send(200, (ROOT / "docs/figures/acom-architecture.png").read_bytes(), "image/png")
            else:
                self.reply(404, {"error": "Not found"})

        def do_POST(self):
            if self.path != "/api/run":
                self.reply(404, {"error": "Not found"})
                return
            origin = self.headers.get("Origin")
            address = "http://127.0.0.1:%d" % self.server.server_port
            if origin is not None and origin != address:
                self.reply(403, {"error": "Open the local 127.0.0.1 page"})
                return
            try:
                length = int(self.headers.get("Content-Length", "0"))
                if not 0 < length <= 1024:
                    raise ValueError("Invalid request size")
                settings = json.loads(self.rfile.read(length))
                if not isinstance(settings, dict):
                    raise ValueError("Expected an object")
            except (ValueError, json.JSONDecodeError) as error:
                self.reply(400, {"error": str(error)})
                return
            if not BUILD_LOCK.acquire(blocking=False):
                self.reply(409, {"error": "A build is already running"})
                return
            try:
                self.reply(200, execute(sdk, settings))
            except ValueError as error:
                self.reply(400, {"error": str(error)})
            except (OSError, subprocess.TimeoutExpired) as error:
                self.reply(500, {"error": str(error)})
            finally:
                BUILD_LOCK.release()

    server = ThreadingHTTPServer(("127.0.0.1", args.port), Handler)
    print("Eco Lab 2 demo: http://127.0.0.1:%d" % server.server_port, flush=True)
    print("Ctrl+C stops the local server. Native sources and reports are unchanged.", flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
